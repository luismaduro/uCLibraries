/**
 *  @file       SPIDevice.h
 *  @brief      Main SPI device functions header
 *  @details    Abstracts bit and byte SPI R/W functions
 *  @author     Luis Maduro
 *  @version    1.0
 *  @date       September 2012
 */

#ifndef _SPIDEV_H_
#define _SPIDEV_H_

#include <p18cxxx.h>

/**The bitwise define for the I2C control register 1 (i.e. _______bits)*/
#define SPICON1bits         SSPCON1bits
/**The bitwise define for the I2C control register 2 (i.e. _______bits)*/
#define SPICON2bits         SSPCON2bits
/**The bitwise define for the I2C status register (i.e. _______bits)*/
#define SPISTATbits         SSPSTATbits
/**The buffer of the I2C module*/
#define SPIBUF              SSPBUF

void SPIWrite(unsigned char data);
unsigned char SPIRead(void);

//#include "stdbool.h"

//unsigned char SPIDeviceReadBit(unsigned char deviceAddress,
//                               unsigned char address,
//                               unsigned char _bit);
//unsigned char SPIDeviceReadBits(unsigned char deviceAddress,
//                                unsigned char address,
//                                unsigned char bitStart,
//                                unsigned char length);
//unsigned char SPIDeviceReadByte(unsigned char deviceAddress,
//                                unsigned char address);
//void SPIDeviceReadBytes(unsigned char deviceAddress,
//                        unsigned char address,
//                        unsigned char length,
//                        unsigned char *data);
//void SPIDeviceWriteBit(unsigned char deviceAddress,
//                       unsigned char address,
//                       unsigned char _bit,
//                       unsigned char value);
//void SPIDeviceWriteBits(unsigned char deviceAddress,
//                        unsigned char address,
//                        unsigned char bitStart,
//                        unsigned char length,
//                        unsigned char value);
//void SPIDeviceWriteByte(unsigned char deviceAddress,
//                        unsigned char address,
//                        unsigned char value);
//void SPIDeviceWriteBytes(unsigned char deviceAddress,
//                         unsigned char address,
//                         unsigned char length,
//                         unsigned char *data);

#endif /* _SPIDEV_H_ */
