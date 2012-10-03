 /**
 *  @file       MCP4XXXX.c
 *  @brief      Library to interface with the MCP4XXXX digital potenciometers.
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       4 de Outubro de 2012, 0:39
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

#include <p18cxxx.h>
#include "MCP4XXXX.h"

/**
 * Sends an shutdown command to the Potenciometer 0 of the MCP4XXXX chip.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP4XXXXShutdownPotenciometer0(volatile unsigned char *port,
                                    unsigned char portBit)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_SHUTDOWN_POT_0);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
}

/**
 * Sends an shutdown command to the Potenciometer 1 of the MCP4XXXX chip. Not
 * valid for MCP41XXX devices.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP4XXXXShutdownPotenciometer1(volatile unsigned char *port,
                                    unsigned char portBit)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_SHUTDOWN_POT_1);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
}

/**
 * Sends an shutdown command to all potenciometers of the MCP4XXXX chip.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP4XXXXShutdownBothPotenciometers(volatile unsigned char *port,
                                        unsigned char portBit)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_SHUTDOWN_BOTH_POTS);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
}

/**
 * Sends the byte with the value to be write to the potenciometer 0 of the
 * MCP4XXXX chip.
 *
 * @param port          PORT address of the CS pin.
 *                      E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit       PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1,
 *                      etc.
 *
 * @param resistance    Byte value to be sent to the potenciometer.
 */
void MCP4XXXXSetPotenciometer0Value(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_WRITE_DATA_POT_0);
    SPIWrite(resistance);

    *port |= (0x01 << portBit);
}

/**
 * Sends the byte with the value to be write to the potenciometer 1 of the
 * MCP4XXXX chip. Not valid for MCP41XXX devices.
 *
 * @param port          PORT address of the CS pin.
 *                      E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit       PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1,
 *                      etc.
 *
 * @param resistance    Byte value to be sent to the potenciometer.
 */
void MCP4XXXXSetPotenciometer1Value(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_WRITE_DATA_POT_1);
    SPIWrite(resistance);

    *port |= (0x01 << portBit);
}

/**
 * Sends the byte with the value to be write to all potenciometers on the
 * MCP4XXXX chip.
 *
 * @param port          PORT address of the CS pin.
 *                      E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit       PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1,
 *                      etc.
 *
 * @param resistance    Byte value to be sent to the potenciometer.
 */
void MCP4XXXXSetPotenciometersValue(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_WRITE_DATA_BOTH_POTS);
    SPIWrite(resistance);

    *port |= (0x01 << portBit);
}