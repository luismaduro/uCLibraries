/**
 *  @file 	   I2CDevice.h	 
 *  @brief     Main I2C device functions header
 *  @details   Abstracts bit and byte I2C R/W functions
 *  @author    Jeff Rowberg
 *  @author    Luis Maduro (Port from C++ to C and adapted to PIC18)
 *  @version   1.0
 *  @date      June 2012
 *  @copyright MIT License.
 * 
 * I2CDevice library code is placed under the MIT license
 * Copyright (c) 2011 Jeff Rowberg
 * Copyright (c) 2012 Luis Maduro
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 */

#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include <p18cxxx.h>

extern unsigned char deviceAddressRead;
extern unsigned char deviceAddressWrite;

#define SYSTEM_OSCILATOR    64000000UL
/**Selects the desired I2C clock speed.*/
#define I2C_SPEED           400000
/**Calculates the value to put on the baudrate register based on the desired speed.
 SYSTEM_OSCILATOR must contain the value of the oscilator. Check the formula for
 different microcontrollers.*/
#define I2CBAUDVALUE        (((SYSTEM_OSCILATOR/4)/I2C_SPEED)-1)
/**Configure the clock pin associated with the I2C*/
#define I2CSCLPIN           TRISCbits.TRISC3
/**Configure the data pin associated with the I2C*/
#define I2CSDAPIN           TRISCbits.TRISC4
/**The bitwise define for the I2C control register 1 (i.e. _______bits)*/
#define I2CCON1bits         SSPCON1bits
/**The bitwise define for the I2C control register 2 (i.e. _______bits)*/
#define I2CCON2bits         SSPCON2bits
/**The bitwise define for the I2C status register (i.e. _______bits)*/
#define I2CSTATbits         SSPSTATbits
/**The buffer of the I2C module*/
#define I2CBUF              SSPBUF
/**The register that controls the clock speed*/
#define I2CBAUDREGISTER     SSPADD

void I2CInit(void);
void I2CStart(void);
void I2CRestart(void);
void I2CStop(void);
void I2CAck(void);
void I2CNotAck(void);
unsigned char I2CWrite(unsigned char data_out);
unsigned char I2CRead(void);
void I2CDeviceSetDeviceAddress(unsigned char address);
unsigned char I2CDeviceReadBit(unsigned char address,
                               unsigned char _bit);
unsigned char I2CDeviceReadBits(unsigned char address,
                                unsigned char bitStart,
                                unsigned char length);
unsigned char I2CDeviceReadByte(unsigned char address);
void I2CDeviceReadBytes(unsigned char address,
                        unsigned char length,
                        unsigned char *data);
void I2CDeviceWriteBit(unsigned char address,
                       unsigned char _bit,
                       unsigned char value);
void I2CDeviceWriteBits(unsigned char address,
                        unsigned char bitStart,
                        unsigned char length,
                        unsigned char value);
void I2CDeviceWriteByte(unsigned char address,
                        unsigned char value);
void I2CDeviceWriteBytes(unsigned char address,
                         unsigned char length,
                         unsigned char *data);

#endif /* _I2CDEV_H_ */
