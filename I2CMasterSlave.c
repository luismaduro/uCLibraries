/**
 *  @file       I2CMasterSlave.c
 *  @author     Based on the work done by Micael Santos on EduBot L
 *  @author     Modified by Luis Maduro
 *  @version    1.00
 *  @date       May 2012
 *  @brief      Library to interface I2C Master - Slave devices.
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

#ifdef __18CXX
#include <p18cxxx.h>
#elif defined __dsPIC33F__
#include <p33Fxxxx.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <i2c.h>
#include "I2CMasterSlave.h"

//xpto[ROWS][COL]
unsigned char RXDataBuffer[10][4];
unsigned char TXDataBuffer[10][4];
unsigned char *RXDataPtr;
unsigned char *TXDataPtr;
struct FlagType Flag;
unsigned char x = 0;

/**
 * This funtion configures the I2C module to the specified parameters.
 * @param role Microcontroller role, defined by the DEVICE_TYPE enumerator.
 * @param speed I2C bus speed, defined by the I2C_SPEED enumerator.
 * @param address This is only used for the slave mode, to set the device address.
 *                If the device is configured in master mode this value is a "don't
 *                care".
 */
void Configure_I2C_Module(DEVICE_TYPE role, I2C_SPEED speed, unsigned char address)
{
#ifdef __18CXX
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
        if (speed == I2C_SPEED_100KHZ) // and speed is 100kHz then configure
        {
            OpenI2C(MASTER,
                    SLEW_OFF);

            I2C_CLOCK_OR_ADDRESS = (unsigned char) ((SYSTEM_CLOCK / (4 * 100000)) - 4);
        }
        else if (speed == I2C_SPEED_400KHZ) // and speed is 400kHz then configure
        {
            OpenI2C(MASTER,
                    SLEW_ON);
            I2C_CLOCK_OR_ADDRESS = (unsigned char) ((SYSTEM_CLOCK / (4 * 400000)) - 4);
        }
        else if (speed == I2C_SPEED_1MHZ) // and speed is 1MHz then configure
        {
            OpenI2C(MASTER,
                    SLEW_OFF);

            I2C_CLOCK_OR_ADDRESS = (unsigned char) ((SYSTEM_CLOCK / (4 * 1000000)) - 4);
        }
    }
    else if (role == DEVICE_SLAVE) //if the device role is slave
    {
        if (speed == I2C_SPEED_100KHZ) // and speed is 400kHz then configure
        {
            OpenI2C(SLAVE_7,
                    SLEW_OFF);
        }
        else if (speed == I2C_SPEED_400KHZ) // and speed is 400kHz then configure
        {
            OpenI2C(SLAVE_7,
                    SLEW_ON);
        }
        else if (speed == I2C_SPEED_1MHZ) // and speed is 1MHz then configure
        {
            OpenI2C(SLAVE_7,
                    SLEW_OFF);
        }

        I2C_CLOCK_POLARITY = 1; //SCK Release Control bit, realeases the clock line.

        I2C_CLOCK_OR_ADDRESS = (address << 1); // Sets the slave address to the specified address.

        I2C_INTERRUPT_FLAG = 0; //Clear the I2C interrut flag.
        I2C_INTERRUPT_PRIORITY = 1; //Set the I2C interrupt priority to high.
        I2C_INTERRUPT_ENABLE = 1; //Enable the I2C interrupt.
    }
#endif

#if defined(__dsPIC33F__)

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
        /*
        if (speed == I2C_SPEED_100KHZ) // and speed is 100kHz then configure
        {
            I2C_SSPCON = 0x8040; //Enable I2C1 module, enable clock stretching
            I2C_ADDRESS_MASKING = 0x0000;
            I2C_CLOCK_OR_ADDRESS = (unsigned int) ((((1 / 100000L) - 130E-9) *
                    INSTRUCTIONS_CLOCK) - 2);
        }
        else if (speed == I2C_SPEED_400KHZ) // and speed is 400kHz then configure
        {
            I2C_SSPCON = 0x8040; //Enable I2C1 module, enable clock stretching
            I2C_ADDRESS_MASKING = 0x0000;
            I2C_CLOCK_OR_ADDRESS = (unsigned int) ((((1 / 100000L) - 130E-9) *
                    INSTRUCTIONS_CLOCK) - 2);
        }
        else if (speed == I2C_SPEED_1MHZ) // and speed is 1MHz then configure
        {
            I2C_SSPCON = 0x8040; //Enable I2C1 module, enable clock stretching
            I2C_ADDRESS_MASKING = 0x0000;
            I2C_CLOCK_OR_ADDRESS = (unsigned int) ((((1 / 100000L) - 130E-9) *
                    INSTRUCTIONS_CLOCK) - 2);
        }
         */
    }
    else if (role == DEVICE_SLAVE) //if the device role is slave
    {
        if (speed == I2C_SPEED_100KHZ) // and speed is 400kHz then configure
        {
            OpenI2C1(I2C1_ON &
                     I2C1_IDLE_CON &
                     I2C1_CLK_REL &
                     I2C1_IPMI_DIS &
                     I2C1_7BIT_ADD &
                     I2C1_SLW_DIS &
                     I2C1_SM_DIS &
                     I2C1_GCALL_EN &
                     I2C1_STR_EN &
                     I2C1_ACK &
                     I2C1_ACK_EN &
                     I2C1_RCV_EN &
                     I2C1_STOP_EN &
                     I2C1_RESTART_EN &
                     I2C1_START_EN,
                     0);
            I2C_ADDRESS_MASKING = 0x0000;
            I2C_ADDRESS = (address);
        }
        else if (speed == I2C_SPEED_400KHZ) // and speed is 400kHz then configure
        {
            OpenI2C1(I2C1_ON &
                     I2C1_IDLE_CON &
                     I2C1_CLK_REL &
                     I2C1_IPMI_DIS &
                     I2C1_7BIT_ADD &
                     I2C1_SLW_EN &
                     I2C1_SM_DIS &
                     I2C1_GCALL_EN &
                     I2C1_STR_EN &
                     I2C1_ACK &
                     I2C1_ACK_EN &
                     I2C1_RCV_EN &
                     I2C1_STOP_EN &
                     I2C1_RESTART_EN &
                     I2C1_START_EN,
                     0);
            I2C_ADDRESS_MASKING = 0x0000;
            I2C_ADDRESS = (address);
        }
        else if (speed == I2C_SPEED_1MHZ) // and speed is 1MHz then configure
        {
            OpenI2C1(I2C1_ON &
                     I2C1_IDLE_CON &
                     I2C1_CLK_HLD &
                     I2C1_IPMI_DIS &
                     I2C1_7BIT_ADD &
                     I2C1_SLW_DIS &
                     I2C1_SM_DIS &
                     I2C1_GCALL_EN &
                     I2C1_STR_EN &
                     I2C1_ACK &
                     I2C1_ACK_EN &
                     I2C1_RCV_EN &
                     I2C1_STOP_EN &
                     I2C1_RESTART_EN &
                     I2C1_START_EN,
                     0);
            I2C_ADDRESS_MASKING = 0x0000;
            I2C_ADDRESS = (address);
        }

        ConfigIntI2C1(MI2C1_INT_OFF & SI2C1_INT_ON & SI2C1_INT_PRI_7);
        IEC1bits.SI2C1IE = 1;
        IFS1bits.SI2C1IF = 0;
    }

#endif
}

/**
 * This funtion is intended to be put in the interrupt funtion so that the data
 * arrived from the I2C bus can be processed.
 * @remarks This funtion clears all the necessary flags and ajusts the
 * configurations as needed.
 */
void I2C_Slave_Parse_Interrupt(void)
{
    unsigned char dummy = 0;

#if defined(__18CXX)

    if (I2C_RECEIVE_OVERFLOW == 1)
    {
        I2C_RECEIVE_OVERFLOW = 0;
        dummy = I2C_BUFFER; //dummy read
        return;
    }
        //Address matched
    else if ((I2C_READ_NOT_WRITE == 0) && (I2C_DATA_NOT_ADDRESS == 0))
    {
        dummy = I2C_BUFFER; //dummy read
        Flag.AddrFlag = 1; //next byte will the address to the data
        Flag.DataFlag = 0;
    }
//check for data
    else if ((I2C_READ_NOT_WRITE == 0) && (I2C_DATA_NOT_ADDRESS == 1)) 
    {
        if (Flag.AddrFlag)
        {
            x = 0;
            Flag.AddrFlag = 0;
            Flag.DataFlag = 1; //next byte is data
            RXDataPtr = &RXDataBuffer[I2C_BUFFER][x];
            TXDataPtr = &TXDataBuffer[I2C_BUFFER][x];

            I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        }
        else if (Flag.DataFlag)
        {
            *RXDataPtr = (unsigned char) I2C_BUFFER; // store data into RAM

            if (x < 4)
            {
                x++;
                RXDataPtr++;
                Flag.AddrFlag = 0; //next byte
                Flag.DataFlag = 1;
            }
            else
            {
                x = 0;
                RXDataPtr = &RXDataBuffer[0][0];
                Flag.AddrFlag = 0; //end of tx
                Flag.DataFlag = 0;
            }

            I2C_CLOCK_POLARITY = 1; //Release SCL1 line

        }
    }
    else if ((I2C_READ_NOT_WRITE == 1) && (I2C_DATA_NOT_ADDRESS == 0))
    {
        dummy = I2C_BUFFER;
        I2C_BUFFER = *TXDataPtr++; //Read data from RAM & send data to I2C master device
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_BUFFER_FULL); //Wait till all
        I2C_BUFFER = *TXDataPtr++;
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_BUFFER_FULL); //Wait till all
        I2C_BUFFER = *TXDataPtr++;
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_BUFFER_FULL); //Wait till all
        I2C_BUFFER = *TXDataPtr;
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_BUFFER_FULL); //Wait till all
    }
    I2C_INTERRUPT_FLAG = 0; //clear I2C1 Slave interrupt flag
#endif

#if defined(__dsPIC33F__)

    if (I2C_RECEIVE_OVERFLOW == 1)
    {
        I2C_RECEIVE_OVERFLOW = 0;
        dummy = I2C_RX_BUFFER; //dummy read
        return;
    }

    else if ((I2C_READ_NOT_WRITE == 0) && (I2C_DATA_NOT_ADDRESS == 0)) //Address matched
    {
        dummy = I2C_RX_BUFFER; //dummy read
        Flag.AddrFlag = 1; //next byte will the address to the data
        Flag.DataFlag = 0;
    }

    else if ((I2C_READ_NOT_WRITE == 0) && (I2C_DATA_NOT_ADDRESS == 1)) //check for data
    {
        if (Flag.AddrFlag)
        {
            x = 0;
            Flag.AddrFlag = 0;
            Flag.DataFlag = 1; //next byte is data
            RXDataPtr = &RXDataBuffer[I2C_RX_BUFFER][x];
            TXDataPtr = &TXDataBuffer[I2C_RX_BUFFER][x];

            I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        }
        else if (Flag.DataFlag)
        {
            *RXDataPtr = (unsigned char) I2C_RX_BUFFER; // store data into RAM

            if (x < 4)
            {
                x++;
                RXDataPtr++;
                Flag.AddrFlag = 0; //next byte
                Flag.DataFlag = 1;
            }
            else
            {
                x = 0;
                RXDataPtr = &RXDataBuffer[0][0];
                Flag.AddrFlag = 0; //end of tx
                Flag.DataFlag = 0;
            }

            I2C_CLOCK_POLARITY = 1; //Release SCL1 line

        }
    }
    else if ((I2C_READ_NOT_WRITE == 1) && (I2C_DATA_NOT_ADDRESS == 0))
    {
        dummy = I2C_RX_BUFFER;
        I2C_RX_BUFFER = *TXDataPtr++; //Read data from RAM & send data to I2C master device
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_RX_BUFFER_FULL); //Wait till all
        I2C_RX_BUFFER = *TXDataPtr++;
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_RX_BUFFER_FULL); //Wait till all
        I2C_RX_BUFFER = *TXDataPtr++;
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_RX_BUFFER_FULL); //Wait till all
        I2C_RX_BUFFER = *TXDataPtr;
        I2C_CLOCK_POLARITY = 1; //Release SCL1 line
        while (I2C_RX_BUFFER_FULL); //Wait till all
    }
    I2C_INTERRUPT_FLAG = 0; //clear I2C1 Slave interrupt flag
#endif
}
