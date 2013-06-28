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

#include <xc.h>
/** Define to SSPCON1 from the pic.*/
#define I2C_SSPCON1             SSPCON1
/** Define to SSPCON2 from the pic.*/
#define I2C_SSPCON2             SSPCON2
/** Define to I2C buffer.*/
#define I2C_BUFFER              SSPBUF
/** Define to I2C register that configures the speed or addres*/
#define I2C_CLOCK_OR_ADDRESS    SSPADD
/**The bitwise define for the I2C control register 1 (i.e. _______bits)*/
#define I2CCON1bits             SSPCON1bits
/**The bitwise define for the I2C control register 2 (i.e. _______bits)*/
#define I2CCON2bits             SSPCON2bits
/**The bitwise define for the I2C status register (i.e. _______bits)*/
#define I2CSTATbits             SSPSTATbits
/** Define to the I2C interrupt enable.*/
#define I2C_INTERRUPT_ENABLE    PIE1bits.SSPIE
/** Define to the I2C interrupt priority.*/
#define I2C_INTERRUPT_PRIORITY  IPR1bits.SSPIP
/** Define to the I2C interrupt flag.*/
#define I2C_INTERRUPT_FLAG      PIR1bits.SSPIF

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

typedef struct
{
    unsigned char AddrFlag : 1;
    unsigned char DataFlag : 1;
} FlagType;

extern FlagType Flag;

void ConfigureI2CModule(DEVICE_TYPE role, I2C_SPEED speed, unsigned char address,
                        unsigned char *RXBuffer, unsigned int RXBSize,
                        unsigned char *TXBuffer, unsigned int TXBSize);
void I2CSlaveParseInterrupt(void);


#define MSSP_DA_DATA        1
#define MSSP_DA_ADDRESS     0
#define MSSP_RW_READ        1
#define MSSP_RW_WRITE       0

enum
{
    I2C_ADDRESS_IN,
    I2C_DATA_IN,
    I2C_DATA_OUT,
    I2C_DATA_DISCARD,
    I2C_DUMMY_OUT
};



#endif
