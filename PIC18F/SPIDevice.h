/**
 *  @file       SPIDevice.h
 *  @brief      SPI device library
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       9 de Outubro de 2012, 15:10
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
#ifndef _SPIDEV_H_
#define _SPIDEV_H_

#include <xc.h>

//------------MUST UNCOMENT ONE OF THE FOLLOWING--------//

/**Defines the SPI clock speed to Fosc/64.*/
//#define SPI_CLOCK_FOSC_64

/**Defines the SPI clock speed to Fosc/16*/
//#define SPI_CLOCK_FOSC_16

/**Defines the SPI clock speed to Fosc/4*/
#define SPI_CLOCK_FOSC_4
//------------------------------------------------------//

#ifdef SPI_CLOCK_FOSC_64
#define SPI_CLOCK 0b0010
#elif defined SPI_CLOCK_FOSC_16
#define SPI_CLOCK 0b0001
#elif defined SPI_CLOCK_FOSC_4
#define SPI_CLOCK 0b0000
#endif

///**Configure the clock pin associated with the SPI*/
//#define SPISCKPIN           TRISCbits.TRISC3
///**Configure the data input pin associated with the SPI*/
//#define SPISDIPIN           TRISCbits.TRISC4
///**Configure the data output pin associated with the SPI*/
//#define SPISDOPIN           TRISCbits.TRISC5
/**The bitwise define for the I2C control register 1 (i.e. _______bits)*/
#define SPICON1bits         SSP1CON1bits
/**The bitwise define for the I2C control register 2 (i.e. _______bits)*/
#define SPICON2bits         SSP1CON2bits
/**The bitwise define for the I2C status register (i.e. _______bits)*/
#define SPISTATbits         SSP1STATbits
/**The buffer of the I2C module*/
#define SPIBUF              SSP1BUF

#define SPIINTFLAG          PIR1bits.SSP1IF

void SPIInit(void);
unsigned char SPIWrite(unsigned char data);
unsigned char SPIRead(void);

unsigned char SPIDeviceReadBit(unsigned char address,
                               unsigned char _bit);
unsigned char SPIDeviceReadBits(unsigned char address,
                                unsigned char bitStart,
                                unsigned char length);
unsigned char SPIDeviceReadByte(unsigned char address);

void SPIDeviceReadBytes(unsigned char length,
                        unsigned char *data);
void SPIDeviceWriteBit(unsigned char address,
                       unsigned char _bit,
                       unsigned char value);
void SPIDeviceWriteBits(unsigned char address,
                        unsigned char bitStart,
                        unsigned char length,
                        unsigned char value);
void SPIDeviceWriteByte(unsigned char address,
                        unsigned char value);
void SPIDeviceWriteBytes(unsigned char length,
                         unsigned char *data);
void SPIDeviceSendData(const unsigned char *data, unsigned int data_len);
void SPIDeviceReceiveData(unsigned char *buffer, unsigned int buffer_len);
#endif /* _SPIDEV_H_ */
