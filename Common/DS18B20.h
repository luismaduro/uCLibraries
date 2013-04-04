/**
 *  @file       DS18B20.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Header File for DS18B20 Library
 */
#ifndef DS18B20_H
#define DS18B20_H

#include <p18cxxx.h>
#include "GlobalTypeDefs.h"
#include "1-Wire.h"

/**
 * @def     READ_ROM_COMMAND
 * @brief   Read Rom Command for the DS18B20.
 */
#define READ_ROM_COMMAND        0x33

/**
 * @def     MATCH_ROM_COMMAND
 * @brief   Match Rom Command for the DS18B20.
 */
#define MATCH_ROM_COMMAND       0x55

/**
 * @def     ALARM_SEARCH_COMMAND
 * @brief   Alarm Search Command for the DS18B20.
 */
#define ALARM_SEARCH_COMMAND    0xEC

/**
 * @def     SKIP_ROM_COMMAND
 * @brief   Skip Rom Command for the DS18B20.
 */
#define SKIP_ROM_COMMAND        0xCC

/**
 * @def     CONVERT_TEMPERATURE
 * @brief   Convert Temperature Command for the DS18B20.
 */
#define CONVERT_TEMPERATURE     0x44

/**
 * @def     COPY_SCRATCHPAD
 * @brief   Copy Scratchpad Command for the DS18B20.
 */
#define COPY_SCRATCHPAD         0x48

/**
 * @def     WRITE_SCRATCHPAD
 * @brief   Write Scratchpad Command for the DS18B20.
 */
#define WRITE_SCRATCHPAD        0x4E

/**
 * @def     READ_SCRATCHPAD
 * @brief   Read Scratchpad Command for the DS18B20.
 */
#define READ_SCRATCHPAD         0xBE

/**
 * @def     RECALL_E_E
 * @brief   Recall EEPROM to Scatchpad Command for the DS18B20.
 */
#define RECALL_E_E              0xB8

/**
 * @def     READ_POWER_SUPPLY
 * @brief   Read Value of Power Supply Command for the DS18B20.
 */
#define READ_POWER_SUPPLY       0xB4

/**
 * @def     RESOLUTION_9BITS
 * @brief   Value to configure the DS18B20 for 9 bits temperature readings.
 *          To be used in DS18B20_Configure() funtion.
 */
#define RESOLUTION_9BITS        0x1F

/**
 * @def     RESOLUTION_10BITS
 * @brief   Value to configure the DS18B20 for 10 bits temperature readings.
 *          To be used in DS18B20_Configure() funtion.
 */
#define RESOLUTION_10BITS       0x3F

/**
 * @def     RESOLUTION_11BITS
 * @brief   Value to configure the DS18B20 for 11 bits temperature readings.
 *          To be used in DS18B20_Configure() funtion.
 */
#define RESOLUTION_11BITS       0x5F

/**
 * @def     RESOLUTION_12BITS
 * @brief   Value to configure the DS18B20 for 12 bits temperature readings.
 *          To be used in DS18B20_Configure() funtion.
 */
#define RESOLUTION_12BITS       0x7F


//eReturnTypes DS18B20GetROMCode(void);
eReturnTypes DS18B20Configure(sDevice device, unsigned char resolution);
eReturnTypes DS18B20IssueTemperatureConvertion(sDevice device);
eReturnTypes DS18B20GetTemperature(sDevice *device);

#endif