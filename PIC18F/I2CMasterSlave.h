/**
 *  @file       I2CMasterSlave.h
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
 
#ifndef I2CMASTERSLAVE_H
#define I2CMASTERSLAVE_H

#if defined(__18CXX)
#include <p18cxxx.h>
/** Define to SSPCON1 from the pic.*/
#define I2C_SSPCON              SSPCON1
/** Define to SSPCON2 from the pic.*/
#define I2C_SSPCON2             SSPCON2
/** Define to I2C buffer.*/
#define I2C_BUFFER              SSPBUF
/** Define to I2C register that configures the speed or addres*/
#define I2C_CLOCK_OR_ADDRESS    SSPADD
/** Define to the I2C interrupt enable.*/
#define I2C_INTERRUPT_ENABLE    PIE1bits.SSPIE
/** Define to the I2C interrupt priority.*/
#define I2C_INTERRUPT_PRIORITY  IPR1bits.SSPIP
/** Define to the I2C interrupt flag.*/
#define I2C_INTERRUPT_FLAG      PIR1bits.SSPIF
/** Define to the I2C data or address information register.*/
#define I2C_DATA_NOT_ADDRESS    SSPSTATbits.D_A
/** Define to the I2C read or write information register.*/
#define I2C_READ_NOT_WRITE      SSPSTATbits.R_W
/** Define to the I2C buffer full flag..*/
#define I2C_BUFFER_FULL         SSPSTATbits.BF
/** Define to the I2C clock stretch bit.*/
#define I2C_CLOCK_POLARITY      SSPCON1bits.CKP
/** Define to the I2C Receive Overflow Indicator bit*/
#define I2C_RECEIVE_OVERFLOW    SSPCON1bits.SSPOV
/** Define to set the crystal / oscillator frequency in Hz. Change to your system
    value.*/
#define SYSTEM_CLOCK 40000000L
/** Define to get the Instructions clock for PIC18*/
#define INSTRUCTIONS_CLOCK (SYSTEM_CLOCK / 4)
#endif

#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"

/** Define to SSPCON1 from the pic.*/
#define I2C_SSPCON              I2C1CON
/** Define to I2C rx buffer.*/
#define I2C_RX_BUFFER           I2C1RCV
/** Define to I2C tx buffer.*/
#define I2C_TX_BUFFER           I2C1TRN
/** Define to I2C register that configures the speed or addres*/
#define I2C_ADDRESS    I2C1ADD
/** Define to the I2C interrupt enable.*/
#define I2C_INTERRUPT_ENABLE    IEC1bits.SI2C1IE
/** Define to the I2C interrupt priority.*/
#define I2C_INTERRUPT_PRIORITY  IFS1bits.SI2C1IF
/** Define to the I2C interrupt flag.*/
#define I2C_INTERRUPT_FLAG      IFS1bits.SI2C1IF
/** Define to the I2C data or address information register.*/
#define I2C_DATA_NOT_ADDRESS    I2C1STATbits.D_A
/** Define to the I2C read or write information register.*/
#define I2C_READ_NOT_WRITE      I2C1STATbits.R_W
/** Define to the I2C RX buffer full flag..*/
#define I2C_RX_BUFFER_FULL      I2C1STATbits.RBF
/** Define to the I2C TX buffer full flag..*/
#define I2C_TX_BUFFER_FULL      I2C1STATbits.TBF
/** Define to the I2C clock stretch bit.*/
#define I2C_CLOCK_POLARITY      I2C1CONbits.SCLREL
/** Define to the I2C Receive Overflow Indicator bit*/
#define I2C_RECEIVE_OVERFLOW    I2C1STATbits.I2COV
/** Define to the I2C SLAVE MODE ADDRESS MASK REGISTER */
#define I2C_ADDRESS_MASKING     I2C1MSK

/** Define to set the crystal / oscillator frequency in Hz. Change to your system
    value.*/
#define SYSTEM_CLOCK 40000000L
/** Define to get the Instructions clock for PIC18*/
#define INSTRUCTIONS_CLOCK (SYSTEM_CLOCK / 2)

#endif

/** Define to set the communication to write mode.*/
#define I2C_WRITE   0b00000000

/** Define to set the communication to read mode.*/
#define I2C_READ    0b00000001

/** DO NOT MODIFY THE CODE BELLOW UNLESS YOU NOW WHAT YOU ARE DOING!*/

/**
 * @enum DEVICE_TYPE
 * @brief typedef to define the microcontroller role.
 */
typedef enum
{
    /** Value to set the microcontroller role to MASTER. */
    DEVICE_MASTER,
    /** Value to set the microcontroller role to SLAVE. */
    DEVICE_SLAVE

} DEVICE_TYPE;

/**
 * @enum I2C_SPEED
 * @brief Typedef to set the I2C speed.
 */
typedef enum
{
    /** I2C speed set to 100 kHz*/
    I2C_SPEED_100KHZ,
    /** I2C speed set to 400 kHz*/
    I2C_SPEED_400KHZ,
    /** I2C speed set to 1 MHz*/
    I2C_SPEED_1MHZ

} I2C_SPEED;

struct FlagType
{
	unsigned char AddrFlag:1;
	unsigned char DataFlag:1;
};

extern struct FlagType Flag;
extern unsigned char RXDataBuffer[10][4];
extern unsigned char TXDataBuffer[10][4];

void Configure_I2C_Module(DEVICE_TYPE role, I2C_SPEED speed, unsigned char address);
void I2C_Slave_Parse_Interrupt(void);


#endif
