/**
 *  @file      MCP6S2X.h
 *  @brief     Library to interface with the MCP6S2X Programable Gain Amplifier.
 *  @author    Luis Maduro
 *  @version   1.0
 *  @date      September 2012
 */

#ifndef _MCP6S2X_H_
#define _MCP6S2X_H_

#include "SPIDevice.h"

#define MCP6S21
//#define MCP6S22
//#define MCP6S26
//#define MCP6S28

#define MCP6S2X_SHUTDOWN                    0b00100000
#define MCP6S2X_WRITE_TO_CHANNEL_REGISTER   0b01000001
#define MCP6S2X_WRITE_TO_GAIN_REGISTER      0b01000000

#define MCP6S2X_SET_GAIN_TO_1               0b00000000
#define MCP6S2X_SET_GAIN_TO_2               0b00000001
#define MCP6S2X_SET_GAIN_TO_4               0b00000010
#define MCP6S2X_SET_GAIN_TO_5               0b00000011
#define MCP6S2X_SET_GAIN_TO_8               0b00000100
#define MCP6S2X_SET_GAIN_TO_10              0b00000101
#define MCP6S2X_SET_GAIN_TO_16              0b00000110
#define MCP6S2X_SET_GAIN_TO_32              0b00000111

#ifdef MCP6S21
#define MCP6S2X_SET_CHANNEL_0               0b00000000

#elif defined MCP6S22
#define MCP6S2X_SET_CHANNEL_0               0b00000000
#define MCP6S2X_SET_CHANNEL_1               0b00000001

#elif defined MCP6S26
#define MCP6S2X_SET_CHANNEL_0               0b00000000
#define MCP6S2X_SET_CHANNEL_1               0b00000001
#define MCP6S2X_SET_CHANNEL_2               0b00000010
#define MCP6S2X_SET_CHANNEL_3               0b00000011
#define MCP6S2X_SET_CHANNEL_4               0b00000100
#define MCP6S2X_SET_CHANNEL_5               0b00000101

#elif defined MCP6S28
#define MCP6S2X_SET_CHANNEL_0               0b00000000
#define MCP6S2X_SET_CHANNEL_1               0b00000001
#define MCP6S2X_SET_CHANNEL_2               0b00000010
#define MCP6S2X_SET_CHANNEL_3               0b00000011
#define MCP6S2X_SET_CHANNEL_4               0b00000100
#define MCP6S2X_SET_CHANNEL_5               0b00000101
#define MCP6S2X_SET_CHANNEL_6               0b00000110
#define MCP6S2X_SET_CHANNEL_7               0b00000111
#endif

void MCP6S2XShutdown(volatile unsigned char *port,
                     unsigned char portBit);
void MCP6S2XSetGain(volatile unsigned char *port,
                    unsigned char portBit,
                    unsigned char gainValue);
void MCP6S2XSelectChannel(volatile unsigned char *port,
                           unsigned char portBit,
                           unsigned char channel);


#endif
