/**
 *  @file       DS18B20.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Header File for DS18B20 Library
 */
#ifndef DS18B20_H
#define DS18B20_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "OneWire.h"

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
 * @def     READ_POWER_SUPPLY
 * @brief   Read Value of Power Supply Command for the DS18B20.
 */
#define READ_POWER_SUPPLY       0xB4

typedef enum
{
    DS18B20Resolution_9Bits = 0x1F,
    DS18B20Resolution_10Bits = 0x3F,
    DS18B20Resolution_11Bits = 0x5F,
    DS18B20Resolution_12Bits = 0x7F
} DS18B20Resolution;


bool DS18B20Configure(tLaseredROMCode *device, DS18B20Resolution resolution);
bool DS18B20IssueTemperatureConvertion(tLaseredROMCode *device);
bool DS18B20GetTemperature(tLaseredROMCode *device, float *temperature);

#endif