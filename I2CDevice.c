#include <i2c.h>
#include "imu/IMUDevice.h"

/**
 * Read a single bit from a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address Register address to read from
 * @param _bit Bit in register position to read (0-7)
 * @return Single bit value
 */
unsigned char I2CDeviceReadBit(unsigned char deviceAddress,
                               unsigned char address,
                               unsigned char _bit)
{
    return (I2CDeviceReadByte(deviceAddress, address) & (1 << _bit));
}

/**
 * Read multiple bits from a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address Register address to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @return Right-aligned value (i.e. '101' read from any bitStart position will
 *                              equal 0x05)
 */
unsigned char I2CDeviceReadBits(unsigned char deviceAddress,
                                unsigned char address,
                                unsigned char bitStart,
                                unsigned char length)
{
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    //    010   masked
    //   -> 010 shifted
    unsigned char i;
    unsigned char b;
    unsigned char r = 0;

    b = I2CDeviceReadByte(deviceAddress, address);

    for (i = bitStart; i > bitStart - length; i--)
    {
        r |= (b & (1 << i));
    }
    r >>= (bitStart - length + 1);

    return r;
}

/**
 * Read single byte from a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address Register address to read from
 * @return Byte value read from device
 */
unsigned char I2CDeviceReadByte(unsigned char deviceAddress,
                                unsigned char address)
{
    unsigned char b = 0;
    I2CDeviceReadBytes(deviceAddress, address, 1, &b);
    return b;
}

/**
 * Read multiple bytes from a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address First register address to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 */
void I2CDeviceReadBytes(unsigned char deviceAddress,
                        unsigned char address,
                        unsigned char length,
                        unsigned char *data)
{
    unsigned char i = 0;

    StartI2C1();
    WriteI2C1(deviceAddress & 0xFE);
    WriteI2C1(address);
    RestartI2C1();

    WriteI2C1(deviceAddress | 0x01);

    for (i = 0; i < length; i++)
    {
        data[i] = ReadI2C1();

        if (i == (length - 1))
        {
            NotAckI2C1();
        }
        else
        {
            AckI2C1();
        }
    }

    StopI2C1();
}

/**
 * Write a single bit to a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address Register address to write to
 * @param _bit Bit position to write (0-7)
 * @param value New bit value to write
 */
void I2CDeviceWriteBit(unsigned char deviceAddress,
                       unsigned char address,
                       unsigned char _bit,
                       unsigned char value)
{
    unsigned char b;

    b = I2CDeviceReadByte(deviceAddress, address);

    b = value ? (b | (1 << _bit)) : (b & ~(1 << _bit));

    I2CDeviceWriteByte(deviceAddress, address, b);
}

/**
 * Write multiple bits to a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address Register address to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param value Right-aligned value to write
 */
void I2CDeviceWriteBits(unsigned char deviceAddress,
                        unsigned char address,
                        unsigned char bitStart,
                        unsigned char length,
                        unsigned char value)
{
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bitStart=4, length=3
    // 01000000 shift left (8 - length)    ]
    // 00001000 shift right (7 - bitStart) ] ---    two shifts ensure all
    //                                              non-important bits are 0
    // 11100011 mask byte
    // 10101111 original value (sample)
    // 10100011 original & mask
    // 10101011 masked | value
    unsigned char b;
    unsigned char mask;

    b = I2CDeviceReadByte(deviceAddress, address);

    mask = (0xFF << (8 - length)) | (0xFF >> (bitStart + length - 1));

    value <<= (8 - length);

    value >>= (7 - bitStart);

    b &= mask;

    b |= value;

    I2CDeviceWriteByte(deviceAddress, address, b);
}

/**
 * Write single byte to a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address Register address to write to
 * @param value New byte value write
 */
void I2CDeviceWriteByte(unsigned char deviceAddress,
                        unsigned char address,
                        unsigned char value)
{
    I2CDeviceWriteBytes(deviceAddress, address, 1, &value);
}

/**
 * Write multiple bytes to a device register.
 * @param deviceAddress Address of the device in I2C bus
 * @param address First register address to write to
 * @param length Number of bytes to write
 * @param data Buffer to copy new data from
 */
void I2CDeviceWriteBytes(unsigned char deviceAddress,
                         unsigned char address,
                         unsigned char length,
                         unsigned char* data)
{
    unsigned char i;

    StartI2C1();
    WriteI2C1(deviceAddress  & 0xFE);
    WriteI2C1(address);

    for (i = 0; i < length; i++)
    {
        WriteI2C1(data[i]);
    }
    StopI2C1();
}
