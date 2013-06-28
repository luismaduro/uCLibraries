/**
 *  @file       I2CMasterSlave.c
 *  @author     Luis Maduro
 *  @version    2.00
 *  @date       June 2013
 *  @brief      Library to interface I2C Master - Slave devices. Based on the the
 *              AN734 from Microchip.
 *
 *  Copyright (C) 2012  Luis Maduro
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2CMasterSlave.h"

unsigned char *RXDataPtr;
unsigned char *TXDataPtr;
unsigned char *RXDataPtrOriginal;
unsigned char *TXDataPtrOriginal;
unsigned int RXBufferSize;
unsigned int TXBufferSize;
unsigned int RXBufferIndex;
unsigned int TXBufferIndex;

FlagType Flag;

/**
 * This funtion configures the I2C module to the specified parameters.
 * @param role Microcontroller role, defined by the DEVICE_TYPE enumerator.
 * @param speed I2C bus speed, defined by the I2C_SPEED enumerator.
 * @param address This is only used for the slave mode, to set the device address.
 *                If the device is configured in master mode this value is a "don't
 *                care".
 */
void ConfigureI2CModule(DEVICE_TYPE role, I2C_SPEED speed, unsigned char address,
                        unsigned char *RXBuffer, unsigned int RXBSize,
                        unsigned char *TXBuffer, unsigned int TXBSize)
{
    I2CCON1bits.SSPEN = 0;
    /**
     * The formula used to calculate the I2C clock based on the crystal frequency
     * is present on the page 172 on the PIC18F4520 datasheet.
     * The value of the crystal used is DEFINED on the "I2CMasterSlave.h"
     * header file.
     *
     * In slave mode the LSB must be zero, like: 0b 1011 0110
     *                                                       ^
     *                                                       |
     *
     * Also in slave mode, the default priority of the interrupt is high, and
     * will be activated by default. The apropriate code must be written on the
     * interrupts funtions to deal with the interrupts of I2C. You can change
     * the interrupt priotity bellow in the respective bit.
     */

    /** The formula is:
     *                              FOSC
     *                  SSPADD = ---------- - 4
     *                            4*speed
     */


    if (role == DEVICE_MASTER) //If the device role is master
    {
        I2CSTATbits.CKE = 0;

        I2CCON1bits.WCOL = 0;
        I2CCON1bits.SSPOV = 0;
        I2CCON1bits.CKP = 0;
        I2CCON1bits.SSPM = 0b1000;
        I2C_CLOCK_OR_ADDRESS = 8;

        if (speed == I2C_SPEED_100KHZ) // and speed is 100kHz then configure
        {
            I2CSTATbits.SMP = 0;
        }
        else if (speed == I2C_SPEED_400KHZ) // and speed is 400kHz then configure
        {
            I2CSTATbits.SMP = 1;
        }
        else if (speed == I2C_SPEED_1MHZ) // and speed is 1MHz then configure
        {
            I2CSTATbits.SMP = 0;
        }

        I2C_INTERRUPT_FLAG = 0; //Clear the I2C interrut flag.
        I2C_INTERRUPT_PRIORITY = 0; //Set the I2C interrupt priority to high.
        I2C_INTERRUPT_ENABLE = 0; //Enable the I2C interrupt.
        I2CCON1bits.SSPEN = 1;
    }
    else if (role == DEVICE_SLAVE) //if the device role is slave
    {
        I2CSTATbits.CKE = 0;

        I2CCON1bits.WCOL = 0;
        I2CCON1bits.SSPOV = 0;
        I2CCON1bits.CKP = 0;
        I2CCON1bits.SSPM = 0b0110;
        I2CCON2bits.SEN = 1;

        if (speed == I2C_SPEED_100KHZ) // and speed is 100kHz then configure
        {
            I2CSTATbits.SMP = 0;
        }
        else if (speed == I2C_SPEED_400KHZ) // and speed is 400kHz then configure
        {
            I2CSTATbits.SMP = 1;
        }
        else if (speed == I2C_SPEED_1MHZ) // and speed is 1MHz then configure
        {
            I2CSTATbits.SMP = 0;
        }

        I2CCON1bits.CKP = 1; //SCK Release Control bit, realeases the clock line.

        I2C_CLOCK_OR_ADDRESS = (address << 1); // Sets the slave address to the specified address.

        I2C_INTERRUPT_FLAG = 0; //Clear the I2C interrut flag.
        I2C_INTERRUPT_PRIORITY = 1; //Set the I2C interrupt priority to high.
        I2C_INTERRUPT_ENABLE = 1; //Enable the I2C interrupt.
        I2CCON1bits.SSPEN = 1;
    }

    RXDataPtrOriginal = RXBuffer;
    TXDataPtrOriginal = TXBuffer;
    RXBufferSize = RXBSize;
    TXBufferSize = TXBSize;
}

/**
 * This funtion is intended to be put in the interrupt funtion so that the data
 * arrived from the I2C bus can be processed.
 * @remarks This funtion clears all the necessary flags and ajusts the
 * configurations as needed.
 */
void I2CSlaveParseInterrupt(void)
{
    volatile unsigned char dummy = 0;
    volatile unsigned char I2cState;

    /* have we just received a valid slave address? */
    if (I2CSTATbits.DA == MSSP_DA_ADDRESS)
    {
        /* reset I2C state */
        I2cState = I2C_ADDRESS_IN;
    }

    switch (I2cState)
    {
    case I2C_ADDRESS_IN:
        /* first clear the buffer */
        dummy = I2C_BUFFER;

        /* check what the master wants to do */
        if (I2CSTATbits.RW == MSSP_RW_READ)
        {
            /* master wants to read data so get byte from
               circular buffer and write it to SSPBUF */
            I2CCON1bits.WCOL = 0;
            do
            {
                I2C_BUFFER = *TXDataPtr; //Read data from RAM & send data to I2C master device
            }
            while (I2CCON1bits.WCOL);

            if (TXBufferIndex < TXBufferSize)
            {
                TXBufferIndex++;
                TXDataPtr++;
                I2cState = I2C_DATA_OUT;
            }
            else
            {
                /* oops, nothing to send */
                I2cState = I2C_DUMMY_OUT;
                /* put 0xFF into SSPBUF to prevent SDA getting stuck low */
                I2C_BUFFER = 0xFF;
            }

            /* release SCL */
            I2CCON1bits.CKP = 1;

        }
        else
        {
            /* master wants to write data */
            I2cState = I2C_DATA_IN;
            Flag.AddrFlag = 1; //next byte will the address to the data
            Flag.DataFlag = 0;
            /* release SCL */
            I2CCON1bits.CKP = 1;
        }
        break;

    case I2C_DATA_IN:
        /* check SSPBUFF has valid data */
        if (I2CSTATbits.BF)
        {
            if (Flag.AddrFlag)
            {
                Flag.AddrFlag = 0;
                Flag.DataFlag = 1; //next byte is data
                dummy = I2C_BUFFER;
                RXDataPtr = RXDataPtrOriginal + dummy;
                TXDataPtr = TXDataPtrOriginal + dummy;
                RXBufferIndex = dummy;
                TXBufferIndex = dummy;
            }
            else if (Flag.DataFlag)
            {
                *RXDataPtr = (unsigned char) I2C_BUFFER; // store data into RAM

                if (RXBufferIndex < RXBufferSize)
                {
                    RXBufferIndex++;
                    RXDataPtr++;
                    Flag.AddrFlag = 0; //next byte
                    Flag.DataFlag = 1;
                }
                else
                {
                    /* oh dear, an overflow. Ignore rest of incomming data */
                    I2cState = I2C_DATA_DISCARD;
                }
            }
            /* release SCL */
            I2CCON1bits.CKP = 1;
        }
        break;

    case I2C_DATA_OUT:
        /* Ensure buffer is empty and SCL is disabled */
        if (!I2CSTATbits.BF && !I2CCON1bits.CKP)
        {
            I2CCON1bits.WCOL = 0;
            do
            {
                I2C_BUFFER = *TXDataPtr; //Read data from RAM & send data to I2C master device
            }
            while (I2CCON1bits.WCOL);

            if (TXBufferIndex < TXBufferSize)
            {
                TXBufferIndex++;
                TXDataPtr++;
                I2cState = I2C_DATA_OUT;
            }
            else
            {
                /* oops, nothing to send */
                I2cState = I2C_DUMMY_OUT;
                /* put 0xFF into SSPBUF to prevent SDA getting stuck low */
                I2C_BUFFER = 0xFF;
            }
            /* release SCL */
            I2CCON1bits.CKP = 1;
        }
        break;

    case I2C_DATA_DISCARD:
        /* just read SSPBUF to clear the interrupt flag and discard data */
        dummy = I2C_BUFFER;
        /* release SCL */
        I2CCON1bits.CKP = 1;
        break;

    case I2C_DUMMY_OUT:
        /* Ensure buffer is empty and SCL is disabled */
        if (!I2CSTATbits.BF && !I2CCON1bits.CKP)
        {
            /* put 0xFF into SSPBUF to prevent SDA getting stuck low */
            I2C_BUFFER = 0xFF;
            /* release SCL */
            I2CCON1bits.CKP = 1;
        }
        break;

    }

    I2C_INTERRUPT_FLAG = 0;
}
