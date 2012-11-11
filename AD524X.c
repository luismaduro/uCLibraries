/**
 *  @file       AD524X.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       4 de Outubro de 2012, 0:39
 *  @brief      Source file for AD524X digital potenciometer.
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
 /**
 * \addtogroup I2C Devices
 *
 * @{
 */
#include "AD524X.h"

unsigned char *instructionByte;
unsigned char instructionByteA0;
unsigned char instructionByteA1;
unsigned char instructionByteA2;
unsigned char instructionByteA3;
unsigned char ad524xAddress;

/**
 * Set the device address that we want to communicate. It should be called every
 * time that we want to change the device to communicate.
 * @param address
 * @warning One of the following must be selected:
 * #AD524X_DEVICE_A0,
 * #AD524X_DEVICE_A1,
 * #AD524X_DEVICE_A2 and
 * #AD524X_DEVICE_A3
 * @see AD524X.h
 */
void AD524XSetDeviceAddress(unsigned char address)
{
    switch (address)
    {
    case AD524X_DEVICE_A0:
        instructionByte = &instructionByteA0;
        ad524xAddress = AD524X_DEVICE_ADDRESS | AD524X_DEVICE_A0;
        break;

    case AD524X_DEVICE_A1:
        instructionByte = &instructionByteA1;
        ad524xAddress = AD524X_DEVICE_ADDRESS | AD524X_DEVICE_A1;
        break;

    case AD524X_DEVICE_A2:
        instructionByte = &instructionByteA2;
        ad524xAddress = AD524X_DEVICE_ADDRESS | AD524X_DEVICE_A2;
        break;

    case AD524X_DEVICE_A3:
        instructionByte = &instructionByteA3;
        ad524xAddress = AD524X_DEVICE_ADDRESS | AD524X_DEVICE_A3;
        break;

    default:
        instructionByte = &instructionByteA0;
        ad524xAddress = AD524X_DEVICE_ADDRESS | AD524X_DEVICE_A0;
        break;
    }

}

/**
 * Set the value of the RDAC 1 of the chip.
 * @param value Value to be write on the RDAC 1 register. Must be between 0...255.
 */
void AD524XSetRDAC1Value(unsigned char value)
{
    *instructionByte &= 0b01111111;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteByte(*instructionByte, value);
}

/**
 * Set the value of the RDAC 2 of the chip.
 * @param value Value to be write on the RDAC 2 register. Must be between 0...255.
 * @warning This funtion as no effect on the AD5241 since the chip doesn't have
 * the second RDAC.
 */
void AD524XSetRDAC2Value(unsigned char value)
{
    *instructionByte |= 0b10000000;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteByte(*instructionByte, value);
}

/**
 * Sets the value of the output 1 to logic high.
 */
void AD524XSetOutput1(void)
{
    *instructionByte |= 0b00010000;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteBytes(*instructionByte, 0, 0x00);
}

/**
 * Sets the value of the output 2 to logic high.
 */
void AD524XSetOutput2(void)
{
    *instructionByte |= 0b00001000;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteBytes(*instructionByte, 0, 0x00);
}

/**
 * Sets the value of the output 1 to logic low.
 */
void AD524XClearOutput1(void)
{
    *instructionByte &= 0b11101111;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteBytes(*instructionByte, 0, 0x00);
}

/**
 * Sets the value of the output 2 to logic low.
 */
void AD524XClearOutput2(void)
{
    *instructionByte &= 0b11110111;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteBytes(*instructionByte, 0, 0x00);
}

/**
 * Sets the RDAC 1 to midscale.
 */
void AD524XSetRDAC1Midscale(void)
{
    *instructionByte |= 0b01000000;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteBytes(*instructionByte, 0, 0x00);

    *instructionByte &= 0b10111111;
}

/**
 * Sets the RDAC 2 to midscale.
 */
void AD524XSetRDAC2Midscale(void)
{
    *instructionByte |= 0b11000000;

    I2CDeviceSetDeviceAddress(ad524xAddress);
    I2CDeviceWriteBytes(*instructionByte, 0, 0x00);

    *instructionByte &= 0b10111111;
}

/**
 * @}
 */