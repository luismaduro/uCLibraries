/**
 *  @file       SPIDevice.h
 *  @brief      Main SPI device functions header
 *  @details    Abstracts bit and byte SPI R/W functions
 *  @author     Luis Maduro
 *  @version    1.0
 *  @date       September 2012
 */
#include "SPIDevice.h"

/**
 * Hardware dependent funtion to write to SPI module. The module must be
 * configured by the user.
 * @param data Byte to be sent by the SPI module.
 */
void SPIWrite(unsigned char data)
{
    unsigned char TempVar;

    TempVar = SSP1BUF; // Clears BF

    PIR1bits.SSPIF = 0; // Clear interrupt flag

    SSP1CON1bits.WCOL = 0; //Clear any previous write collision

    SSP1BUF = data; // write byte to SSP1BUF register

    if (SSP1CON1 & 0x80) // test if write collision occurred
        return; // if WCOL bit is set return negative #
    else
        while (!PIR1bits.SSP1IF); // wait until bus cycle complete

    return; // if WCOL bit is not set return non-negative#
}

/**
 * Hardware dependent funtion to read from SPI module. The module must be
 * configured by the user.
 * @return Byte read from the SPI bus.
 */
unsigned char SPIRead(void)
{
    unsigned char TempVar;

    TempVar = SSP1BUF; //Clear BF

    PIR1bits.SSP1IF = 0; //Clear interrupt flag

    SSP1BUF = 0x00; // initiate bus cycle

    while (!PIR1bits.SSP1IF); //wait until cycle complete

    return (SSP1BUF); // return with byte read
}


///**
// * Read multiple bytes from a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address First register address to read from
// * @param length Number of bytes to read
// * @param data Buffer to store read data in
// */
//void SPIDeviceReadBytes(unsigned char deviceAddress,
//                        unsigned char address,
//                        unsigned char length,
//                        unsigned char *data)
//{
//    unsigned char i = 0;
//
//    StartSPI1();
//    WriteSPI1(deviceAddress & 0xFE);
//    WriteSPI1(address);
//    RestartSPI1();
//
//    WriteSPI1(deviceAddress | 0x01);
//
//    for (i = 0; i < length; i++)
//    {
//        data[i] = ReadSPI1();
//
//        if (i == (length - 1))
//        {
//            NotAckSPI1();
//        }
//        else
//        {
//            AckSPI1();
//        }
//    }
//
//    StopSPI1();
//}
//
///**
// * Write multiple bytes to a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address First register address to write to
// * @param length Number of bytes to write
// * @param data Buffer to copy new data from
// */
//void SPIDeviceWriteBytes(unsigned char deviceAddress,
//                         unsigned char address,
//                         unsigned char length,
//                         unsigned char *data)
//{
//    unsigned char i;
//
//    StartSPI1();
//    WriteSPI1(deviceAddress  & 0xFE);
//    WriteSPI1(address);
//
//    for (i = 0; i < length; i++)
//    {
//        WriteSPI1(data[i]);
//    }
//    StopSPI1();
//}
//
//
///**
// * Read a single bit from a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address Register address to read from
// * @param _bit Bit in register position to read (0-7)
// * @return Single bit value
// */
//unsigned char SPIDeviceReadBit(unsigned char deviceAddress,
//                               unsigned char address,
//                               unsigned char _bit)
//{
//    return (SPIDeviceReadByte(deviceAddress, address) & (1 << _bit));
//}
//
///**
// * Read multiple bits from a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address Register address to read from
// * @param bitStart First bit position to read (0-7)
// * @param length Number of bits to read (not more than 8)
// * @return Right-aligned value (i.e. '101' read from any bitStart position will
// *                              equal 0x05)
// */
//unsigned char SPIDeviceReadBits(unsigned char deviceAddress,
//                                unsigned char address,
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
//    b = SPIDeviceReadByte(deviceAddress, address);
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
// * @param deviceAddress Address of the device in SPI bus
// * @param address Register address to read from
// * @return Byte value read from device
// */
//unsigned char SPIDeviceReadByte(unsigned char deviceAddress,
//                                unsigned char address)
//{
//    unsigned char b = 0;
//    SPIDeviceReadBytes(deviceAddress, address, 1, &b);
//    return b;
//}
//
///**
// * Write a single bit to a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address Register address to write to
// * @param _bit Bit position to write (0-7)
// * @param value New bit value to write
// */
//void SPIDeviceWriteBit(unsigned char deviceAddress,
//                       unsigned char address,
//                       unsigned char _bit,
//                       unsigned char value)
//{
//    unsigned char b;
//
//    b = SPIDeviceReadByte(deviceAddress, address);
//
//    b = value ? (b | (1 << _bit)) : (b & ~(1 << _bit));
//
//    SPIDeviceWriteByte(deviceAddress, address, b);
//}
//
///**
// * Write multiple bits to a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address Register address to write to
// * @param bitStart First bit position to write (0-7)
// * @param length Number of bits to write (not more than 8)
// * @param value Right-aligned value to write
// */
//void SPIDeviceWriteBits(unsigned char deviceAddress,
//                        unsigned char address,
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
//    b = SPIDeviceReadByte(deviceAddress, address);
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
//    SPIDeviceWriteByte(deviceAddress, address, b);
//}
//
///**
// * Write single byte to a device register.
// * @param deviceAddress Address of the device in SPI bus
// * @param address Register address to write to
// * @param value New byte value write
// */
//void SPIDeviceWriteByte(unsigned char deviceAddress,
//                        unsigned char address,
//                        unsigned char value)
//{
//    SPIDeviceWriteBytes(deviceAddress, address, 1, &value);
//}
