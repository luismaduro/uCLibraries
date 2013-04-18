/**
 *  @file       DS2482.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       18/04/2013
 *  @brief      Source file for DS2482 One Wire to I2C converter.
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

#ifndef __DS2482_H
#define __DS2482_H

#include <stdbool.h>
#include "../STM32F1/I2CDevice.h"

// constants/macros/typedefs
#define DS2482_I2C_ADDR		0x1B	//< Base I2C address of DS2482 devices
#define POLL_LIMIT				0x30	// 0x30 is the minimum poll limit
//1-wire EEPROM and silicon serial number commands
#define READ_ROM_COMMAND        0x33
#define MATCH_ROM_COMMAND       0x55
#define SKIP_ROM_COMMAND        0xCC
#define WRITE_SCRATCHPAD        0x4E
#define READ_SCRATCHPAD         0xBE
#define COPY_SCRATCHPAD         0x48
#define RECALL_E_E              0xB8

// DS2482 command defines
#define DS2482_CMD_DRST		0xF0	//< DS2482 Device Reset
#define DS2482_CMD_SRP		0xE1	//< DS2482 Set Read Pointer
#define DS2482_CMD_WCFG		0xD2	//< DS2482 Write Configuration
#define DS2482_CMD_CHSL		0xC3	//< DS2482 Channel Select
#define DS2482_CMD_1WRS		0xB4	//< DS2482 1-Wire Reset
#define DS2482_CMD_1WWB		0xA5	//< DS2482 1-Wire Write Byte
#define DS2482_CMD_1WRB		0x96	//< DS2482 1-Wire Read Byte
#define DS2482_CMD_1WSB		0x87	//< DS2482 1-Wire Single Bit
#define DS2482_CMD_1WT		0x78	//< DS2482 1-Wire Triplet
// DS2482 status register bit defines
#define DS2482_STATUS_1WB	0x01	//< DS2482 Status 1-Wire Busy
#define DS2482_STATUS_PPD	0x02	//< DS2482 Status Presence Pulse Detect
#define DS2482_STATUS_SD	0x04	//< DS2482 Status Short Detected
#define DS2482_STATUS_LL	0x08	//< DS2482 Status 1-Wire Logic Level
#define DS2482_STATUS_RST	0x10	//< DS2482 Status Device Reset
#define DS2482_STATUS_SBR	0x20	//< DS2482 Status Single Bit Result
#define DS2482_STATUS_TSB	0x40	//< DS2482 Status Triplet Second Bit
#define DS2482_STATUS_DIR	0x80	//< DS2482 Status Branch Direction Taken
// DS2482 configuration register bit defines
#define DS2482_CFG_APU		0x01	//< DS2482 Config Active Pull-Up
#define DS2482_CFG_PPM		0x02	//< DS2482 Config Presence Pulse Masking
#define DS2482_CFG_SPU		0x04	//< DS2482 Config Strong Pull-Up
#define DS2482_CFG_1WS		0x08	//< DS2482 Config 1-Wire Speed
// DS2482 channel selection code for defines
#define DS2482_CH_IO0		0xF0	//< DS2482 Select Channel IO0
#define DS2482_CH_IO1		0xE1	//< DS2482 Select Channel IO1
#define DS2482_CH_IO2		0xD2	//< DS2482 Select Channel IO2
#define DS2482_CH_IO3		0xC3	//< DS2482 Select Channel IO3
#define DS2482_CH_IO4		0xB4	//< DS2482 Select Channel IO4
#define DS2482_CH_IO5		0xA5	//< DS2482 Select Channel IO5
#define DS2482_CH_IO6		0x96	//< DS2482 Select Channel IO6
#define DS2482_CH_IO7		0x87	//< DS2482 Select Channel IO7
// DS2482 channel selection read back code for defines
#define DS2482_RCH_IO0		0xB8	//< DS2482 Select Channel IO0
#define DS2482_RCH_IO1		0xB1	//< DS2482 Select Channel IO1
#define DS2482_RCH_IO2		0xAA	//< DS2482 Select Channel IO2
#define DS2482_RCH_IO3		0xA3	//< DS2482 Select Channel IO3
#define DS2482_RCH_IO4		0x9C	//< DS2482 Select Channel IO4
#define DS2482_RCH_IO5		0x95	//< DS2482 Select Channel IO5
#define DS2482_RCH_IO6		0x8E	//< DS2482 Select Channel IO6
#define DS2482_RCH_IO7		0x87	//< DS2482 Select Channel IO7
// DS2482 read pointer code defines
#define DS2482_READPTR_SR	0xF0	//< DS2482 Status Register
#define DS2482_READPTR_RDR	0xE1	//< DS2482 Read Data Register
#define DS2482_READPTR_CSR	0xD2	//< DS2482 Channel Selection Register
#define DS2482_READPTR_CR	0xC3	//< DS2482 Configuration Register

// DS2482 Funtion definition
unsigned char DS2482Reset(void);
unsigned char DS2482Detect(void);
unsigned char DS2482WriteConfig(unsigned char config);
unsigned char DS2482ChannelSelect(unsigned char channel);
unsigned char OneWireReset(void);
void OneWireWriteBit(unsigned char sendbit);
unsigned char OneWireReadBit(void);
unsigned char OneWireTouchBit(unsigned char sendbit);
void OneWireWriteByte(unsigned char sendbyte);
unsigned char OneWireReadByte(void);
void OneWireBlockTransfer(unsigned char *transfer_buffer, unsigned char length);
unsigned char OneWireTouchByte(unsigned char sendbyte);
unsigned char OneWireCRC8(unsigned char *addr, unsigned char len);
unsigned char OneWireSearch(unsigned char *newAddr);

#endif
