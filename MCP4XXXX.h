/**
 *  @file      MCP4XXXX.h
 *  @brief     Library to interface with the MCP4XXXX digital potenciometers.
 *  @author    Luis Maduro
 *  @version   1.0
 *  @date      September 2012
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
