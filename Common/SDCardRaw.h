/**
 *  @file       SDCardRaw.h
 *  @brief      Library to write in raw mode to sdcard
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       11 de Outubro de 2012
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

#ifndef SD_RAW_H
#define SD_RAW_H

#include <string.h>
#include "SPIDevice.h"

#define configure_pin_mosi()        (TRISCbits.TRISC4 = 1)
#define configure_pin_sck()         (TRISCbits.TRISC3 = 0)
#define configure_pin_ss()          (TRISBbits.RB4 = 0)
#define configure_pin_miso()        (TRISCbits.TRISC5 = 0)

#define select_card()               (TRISBbits.RB4 = 0)
#define unselect_card()             (TRISBbits.RB4 = 1)

#define get_pin_available()         (0)
#define get_pin_locked()            (0)

#if SD_RAW_SDHC
typedef uint64_t offset_t;
#else
typedef unsigned long offset_t;
#endif

/**
 * The card's layout is harddisk-like, which means it contains
 * a master boot record with a partition table.
 */
#define SD_RAW_FORMAT_HARDDISK 1
/**
 * The card contains a single filesystem and no partition table.
 */
#define SD_RAW_FORMAT_SUPERFLOPPY 1
/**
 * The card's layout follows the Universal File Format.
 */
#define SD_RAW_FORMAT_UNIVERSAL 2
/**
 * The card's layout is unknown.
 */
#define SD_RAW_FORMAT_UNKNOWN 3
/**
 * Controls MMC/SD write support.
 *
 * Set to 1 to enable MMC/SD write support, set to 0 to disable it.
 */
#define SD_RAW_WRITE_SUPPORT 1

/**
 * Controls MMC/SD write buffering.
 *
 * Set to 1 to buffer write accesses, set to 0 to disable it.
 *
 * \note This option has no effect when SD_RAW_WRITE_SUPPORT is 0.
 */
#define SD_RAW_WRITE_BUFFERING 1

/**
 * Controls MMC/SD access buffering.
 *
 * Set to 1 to save static RAM, but be aware that you will
 * lose performance.
 *
 * \note When SD_RAW_WRITE_SUPPORT is 1, SD_RAW_SAVE_RAM will
 *       be reset to 0.
 */
#define SD_RAW_SAVE_RAM 1

/**
 * Controls support for SDHC cards.
 *
 * Set to 1 to support so-called SDHC memory cards, i.e. SD
 * cards with more than 2 gigabytes of memory.
 */
#define SD_RAW_SDHC 0

/* configuration checks */
#if SD_RAW_WRITE_SUPPORT
#undef SD_RAW_SAVE_RAM
#define SD_RAW_SAVE_RAM 0
#else
#undef SD_RAW_WRITE_BUFFERING
#define SD_RAW_WRITE_BUFFERING 0
#endif

/* CMD0: response R1 */
#define CMD_GO_IDLE_STATE 0x00
/* CMD1: response R1 */
#define CMD_SEND_OP_COND 0x01
/* CMD8: response R7 */
#define CMD_SEND_IF_COND 0x08
/* CMD9: response R1 */
#define CMD_SEND_CSD 0x09
/* CMD10: response R1 */
#define CMD_SEND_CID 0x0a
/* CMD12: response R1 */
#define CMD_STOP_TRANSMISSION 0x0c
/* CMD13: response R2 */
#define CMD_SEND_STATUS 0x0d
/* CMD16: arg0[31:0]: block length, response R1 */
#define CMD_SET_BLOCKLEN 0x10
/* CMD17: arg0[31:0]: data address, response R1 */
#define CMD_READ_SINGLE_BLOCK 0x11
/* CMD18: arg0[31:0]: data address, response R1 */
#define CMD_READ_MULTIPLE_BLOCK 0x12
/* CMD24: arg0[31:0]: data address, response R1 */
#define CMD_WRITE_SINGLE_BLOCK 0x18
/* CMD25: arg0[31:0]: data address, response R1 */
#define CMD_WRITE_MULTIPLE_BLOCK 0x19
/* CMD27: response R1 */
#define CMD_PROGRAM_CSD 0x1b
/* CMD28: arg0[31:0]: data address, response R1b */
#define CMD_SET_WRITE_PROT 0x1c
/* CMD29: arg0[31:0]: data address, response R1b */
#define CMD_CLR_WRITE_PROT 0x1d
/* CMD30: arg0[31:0]: write protect data address, response R1 */
#define CMD_SEND_WRITE_PROT 0x1e
/* CMD32: arg0[31:0]: data address, response R1 */
#define CMD_TAG_SECTOR_START 0x20
/* CMD33: arg0[31:0]: data address, response R1 */
#define CMD_TAG_SECTOR_END 0x21
/* CMD34: arg0[31:0]: data address, response R1 */
#define CMD_UNTAG_SECTOR 0x22
/* CMD35: arg0[31:0]: data address, response R1 */
#define CMD_TAG_ERASE_GROUP_START 0x23
/* CMD36: arg0[31:0]: data address, response R1 */
#define CMD_TAG_ERASE_GROUP_END 0x24
/* CMD37: arg0[31:0]: data address, response R1 */
#define CMD_UNTAG_ERASE_GROUP 0x25
/* CMD38: arg0[31:0]: stuff bits, response R1b */
#define CMD_ERASE 0x26
/* ACMD41: arg0[31:0]: OCR contents, response R1 */
#define CMD_SD_SEND_OP_COND 0x29
/* CMD42: arg0[31:0]: stuff bits, response R1b */
#define CMD_LOCK_UNLOCK 0x2a
/* CMD55: arg0[31:0]: stuff bits, response R1 */
#define CMD_APP 0x37
/* CMD58: arg0[31:0]: stuff bits, response R3 */
#define CMD_READ_OCR 0x3a
/* CMD59: arg0[31:1]: stuff bits, arg0[0:0]: crc option, response R1 */
#define CMD_CRC_ON_OFF 0x3b

/* command responses */
/* R1: size 1 byte */
#define R1_IDLE_STATE 0
#define R1_ERASE_RESET 1
#define R1_ILL_COMMAND 2
#define R1_COM_CRC_ERR 3
#define R1_ERASE_SEQ_ERR 4
#define R1_ADDR_ERR 5
#define R1_PARAM_ERR 6
/* R1b: equals R1, additional busy bytes */
/* R2: size 2 bytes */
#define R2_CARD_LOCKED 0
#define R2_WP_ERASE_SKIP 1
#define R2_ERR 2
#define R2_CARD_ERR 3
#define R2_CARD_ECC_FAIL 4
#define R2_WP_VIOLATION 5
#define R2_INVAL_ERASE 6
#define R2_OUT_OF_RANGE 7
#define R2_CSD_OVERWRITE 7
#define R2_IDLE_STATE (R1_IDLE_STATE + 8)
#define R2_ERASE_RESET (R1_ERASE_RESET + 8)
#define R2_ILL_COMMAND (R1_ILL_COMMAND + 8)
#define R2_COM_CRC_ERR (R1_COM_CRC_ERR + 8)
#define R2_ERASE_SEQ_ERR (R1_ERASE_SEQ_ERR + 8)
#define R2_ADDR_ERR (R1_ADDR_ERR + 8)
#define R2_PARAM_ERR (R1_PARAM_ERR + 8)
/* R3: size 5 bytes */
#define R3_OCR_MASK (0xffffffffUL)
#define R3_IDLE_STATE (R1_IDLE_STATE + 32)
#define R3_ERASE_RESET (R1_ERASE_RESET + 32)
#define R3_ILL_COMMAND (R1_ILL_COMMAND + 32)
#define R3_COM_CRC_ERR (R1_COM_CRC_ERR + 32)
#define R3_ERASE_SEQ_ERR (R1_ERASE_SEQ_ERR + 32)
#define R3_ADDR_ERR (R1_ADDR_ERR + 32)
#define R3_PARAM_ERR (R1_PARAM_ERR + 32)
/* Data Response: size 1 byte */
#define DR_STATUS_MASK 0x0e
#define DR_STATUS_ACCEPTED 0x05
#define DR_STATUS_CRC_ERR 0x0a
#define DR_STATUS_WRITE_ERR 0x0c

/* status bits for card types */
#define SD_RAW_SPEC_1 0
#define SD_RAW_SPEC_2 1
#define SD_RAW_SPEC_SDHC 2

/**
 * This struct is used by sd_raw_get_info() to return
 * manufacturing and status information of the card.
 */
struct SDCardInfo
{
    /**
     * A manufacturer code globally assigned by the SD card organization.
     */
    unsigned char manufacturer;
    /**
     * A string describing the card's OEM or content, globally assigned by the
     * SD card organization.
     */
    char oem[2];
    /**
     * A product name.
     */
    char product[5];
    /**
     * The card's revision, coded in packed BCD.
     *
     * For example, the revision value \c 0x32 means "3.2".
     */
    unsigned char revision;
    /**
     * A serial number assigned by the manufacturer.
     */
    unsigned long serial;
    /**
     * The year of manufacturing.
     *
     * A value of zero means year 2000.
     */
    unsigned char manufacturing_year;
    /**
     * The month of manufacturing.
     */
    unsigned char manufacturing_month;
    /**
     * The card's total capacity in bytes.
     */
    unsigned long capacity;
    /**
     * Defines wether the card's content is original or copied.
     *
     * A value of \c 0 means original, \c 1 means copied.
     */
    unsigned char flag_copy;
    /**
     * Defines wether the card's content is write-protected.
     *
     * \note This is an internal flag and does not represent the
     *       state of the card's mechanical write-protect switch.
     */
    unsigned char flag_write_protect;
    /**
     * Defines wether the card's content is temporarily write-protected.
     *
     * \note This is an internal flag and does not represent the
     *       state of the card's mechanical write-protect switch.
     */
    unsigned char flag_write_protect_temp;
    /**
     * The card's data layout.
     *
     * See the \c SD_RAW_FORMAT_* constants for details.
     *
     * \note This value is not guaranteed to match reality.
     */
    unsigned char format;
};
typedef unsigned long uintptr_t;
typedef unsigned char (*sd_raw_read_interval_handler_t)(unsigned char* buffer, offset_t offset, void* p);
typedef uintptr_t(*sd_raw_write_interval_handler_t)(unsigned char* buffer, offset_t offset, void* p);

unsigned char sd_raw_init();
unsigned char sd_raw_available();
unsigned char sd_raw_locked();

unsigned char sd_raw_read(offset_t offset, unsigned char* buffer, uintptr_t length);
unsigned char sd_raw_read_interval(offset_t offset, unsigned char* buffer, uintptr_t interval, uintptr_t length, sd_raw_read_interval_handler_t callback, void* p);
unsigned char sd_raw_write(offset_t offset, const unsigned char* buffer, uintptr_t length);
unsigned char sd_raw_write_interval(offset_t offset, unsigned char* buffer, uintptr_t length, sd_raw_write_interval_handler_t callback, void* p);
unsigned char sd_raw_sync();

unsigned char sd_raw_get_info(struct SDCardInfo* info);

#endif

