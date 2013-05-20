/**
 *  @file       SPIDevice.c
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
#include "SPIDevice.h"

/**
 * Initiates the MSSP module to work as an SPI master.
 */
void SPIInit(void)
{

}

/**
 * Hardware dependent funtion to write to SPI module. The module must be
 * configured by the user.
 * @param data Byte to be sent by the SPI module.
 * @return Returns 0 if all went well, !=0 otherwise.
 */
void SPIWrite(unsigned char data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    SPI_I2S_SendData(SPI1, (uint16_t) data);
}

/**
 * Hardware dependent funtion to read from SPI module. The module must be
 * configured by the user.
 * @return Byte read from the SPI bus.
 */
unsigned char SPIRead(void)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, (uint16_t) 0x00);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return (SPI_I2S_ReceiveData(SPI1));
}

/**
 * Read multiple bytes from a device register.
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 */
void SPIDeviceReadBytes(unsigned char length,
                        unsigned char *data)
{
    unsigned char i = 0;

    for (i = 0; i < length; i++)
    {
        data[i] = SPIRead();
    }
}

/**
 * Write multiple bytes to a device register.
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 */
void SPIDeviceWriteBytes(unsigned char length,
                         unsigned char *data)
{
    unsigned char i;

    for (i = 0; i < length; i++)
    {
        SPIWrite(data[i]);
    }
}

///**
// * Read a single bit from a device register.
// * @param address Register address to read from
// * @param _bit Bit in register position to read (0-7)
// * @return Single bit value
// */
//unsigned char SPIDeviceReadBit(unsigned char address,
//                               unsigned char _bit)
//{
//    return (SPIDeviceReadByte(address) & (1 << _bit));
//}
//
///**
// * Read multiple bits from a device register.
// * @param address Register address to read from
// * @param bitStart First bit position to read (0-7)
// * @param length Number of bits to read (not more than 8)
// * @return Right-aligned value (i.e. '101' read from any bitStart position will
// *                              equal 0x05)
// */
//unsigned char SPIDeviceReadBits(unsigned char address,
//                                unsigned char bitStart,
//                                unsigned char length)
//{
//    // 01101001 read byte
//    // 76543210 bit numbers
//    //    xxx   args: bitStart=4, length=3
//    //    010   masked
//    //   -> 010 shifted
//    unsigned char i;
//    unsigned char b;
//    unsigned char r = 0;
//
//    b = SPIDeviceReadByte(address);
//
//    for (i = bitStart; i > bitStart - length; i--)
//    {
//        r |= (b & (1 << i));
//    }
//    r >>= (bitStart - length + 1);
//
//    return r;
//}
//
///**
// * Read single byte from a device register.
// * @param address Register address to read from
// * @return Byte value read from device
// */
//unsigned char SPIDeviceReadByte(unsigned char address)
//{
//    unsigned char b = 0;
//    SPIDeviceReadBytes(address, 1, &b);
//    return b;
//}
//
///**
// * Write a single bit to a device register.
// * @param address Register address to write to
// * @param _bit Bit position to write (0-7)
// * @param value New bit value to write
// */
//void SPIDeviceWriteBit(unsigned char address,
//                       unsigned char _bit,
//                       unsigned char value)
//{
//    unsigned char b;
//
//    b = SPIDeviceReadByte(address);
//
//    b = value ? (b | (1 << _bit)) : (b & ~(1 << _bit));
//
//    SPIDeviceWriteByte(address, b);
//}
//
///**
// * Write multiple bits to a device register.
// * @param address Register address to write to
// * @param bitStart First bit position to write (0-7)
// * @param length Number of bits to write (not more than 8)
// * @param value Right-aligned value to write
// */
//void SPIDeviceWriteBits(unsigned char address,
//                        unsigned char bitStart,
//                        unsigned char length,
//                        unsigned char value)
//{
//    //      010 value to write
//    // 76543210 bit numbers
//    //    xxx   args: bitStart=4, length=3
//    // 01000000 shift left (8 - length)    ]
//    // 00001000 shift right (7 - bitStart) ] ---    two shifts ensure all
//    //                                              non-important bits are 0
//    // 11100011 mask byte
//    // 10101111 original value (sample)
//    // 10100011 original & mask
//    // 10101011 masked | value
//    unsigned char b;
//    unsigned char mask;
//
//    b = SPIDeviceReadByte(address);
//
//    mask = (0xFF << (8 - length)) | (0xFF >> (bitStart + length - 1));
//
//    value <<= (8 - length);
//
//    value >>= (7 - bitStart);
//
//    b &= mask;
//
//    b |= value;
//
//    SPIDeviceWriteByte(address, b);
//}
//
///**
// * Write single byte to a device register.
// * @param address Register address to write to
// * @param value New byte value write
// */
//void SPIDeviceWriteByte(unsigned char address,
//                        unsigned char value)
//{
//    SPIDeviceWriteBytes(address, 1, &value);
//}
//
///**
// * Sends data contained in a buffer over the SPI bus.
// *
// * \param[in] data A pointer to the buffer which contains the data to send.
// * \param[in] data_len The number of bytes to send.
// */
//void SPIDeviceSendData(const unsigned char *data, unsigned int data_len)
//{
//    do
//    {
//        unsigned char b = *data++;
//
//        SPIWrite(b);
//
//    }
//    while (--data_len);
//}
//
///**
// * Receives multiple bytes from the SPI bus and writes them to a buffer.
// *
// * \param[out] buffer A pointer to the buffer into which the data gets written.
// * \param[in] buffer_len The number of bytes to read.
// */
//void SPIDeviceReceiveData(unsigned char *buffer, unsigned int buffer_len)
//{
//    --buffer;
//
//    do
//    {
//        ++buffer;
//        *buffer = SPIRead();
//
//    }
//    while (--buffer_len);
//}
