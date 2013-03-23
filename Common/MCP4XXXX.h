 /**
 *  @file       MCP4XXXX.h
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

#ifndef _MCP41XXXX_H_
#define _MCP41XXXX_H_

#include "SPIDevice.h"

#define MCP4XXXX_WRITE_DATA_POT_0           0b00010001
#define MCP4XXXX_WRITE_DATA_POT_1           0b00010010
#define MCP4XXXX_WRITE_DATA_BOTH_POTS       0b00010011

#define MCP4XXXX_SHUTDOWN_POT_0             0b00100001
#define MCP4XXXX_SHUTDOWN_POT_1             0b00100010
#define MCP4XXXX_SHUTDOWN_BOTH_POTS         0b00100011




void MCP4XXXXShutdownPotenciometer0(volatile unsigned char *port,
                                    unsigned char portBit);
void MCP4XXXXShutdownPotenciometer1(volatile unsigned char *port,
                                    unsigned char portBit);
void MCP4XXXXShutdownBothPotenciometers(volatile unsigned char *port,
                                        unsigned char portBit);
void MCP4XXXXSetPotenciometer0Value(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance);
void MCP4XXXXSetPotenciometer1Value(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance);
void MCP4XXXXSetPotenciometersValue(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance);
#endif /* _SPIDEV_H_ */
