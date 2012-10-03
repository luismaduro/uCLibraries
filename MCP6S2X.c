/**
 *  @file       MCP6S2X.c
 *  @brief      Library to interface with the MCP6S2X Programable Gain Amplifier.
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
#include "MCP6S2X.h"

/**
 * Sends an shutdown command.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP6S2XShutdown(volatile unsigned char *port,
                     unsigned char portBit)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP6S2X_SHUTDOWN);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
}

/**
 * Sets the gain for the PGA. The gain value are defined in the include file.
 *
 * @condition Must select the correct define in the MCP6S2X.h file to select the
 * device used. The value of several registers change.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 *
 * @param gainValue Gain value to be writen to the PGA
 *
 * @see MCP6S2X.h
 */
void MCP6S2XSetGain(volatile unsigned char *port,
                    unsigned char portBit,
                    unsigned char gainValue)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP6S2X_WRITE_TO_GAIN_REGISTER);
    SPIWrite(gainValue);

    *port |= (0x01 << portBit);
}

/**
 * Selects the channel to perform the convertions. Channel values are defined in
 * the include file.
 *
 * @condition Must select the correct define in the MCP6S2X.h file to select the
 * device used. The value of several registers change.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 *
 * @see MCP6S2X.h
 */
void MCP6S2XSelectChannel(volatile unsigned char *port,
                           unsigned char portBit,
                           unsigned char channel)
{
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP6S2X_WRITE_TO_CHANNEL_REGISTER);
    SPIWrite(channel);

    *port |= (0x01 << portBit);
}