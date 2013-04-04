/**
 *  @file       DS2438.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Header File for DS2438 Library
 */
#ifndef DS2438_H
#define DS2438_H

#include <p18cxxx.h>
#include "GlobalTypeDefs.h"
#include "1-Wire.h"

/** @cond IGNORE*/
#define READ_ROM_COMMAND        0x33
#define MATCH_ROM_COMMAND       0x55
#define SKIP_ROM_COMMAND        0xCC

#define WRITE_SCRATCHPAD        0x4E
#define READ_SCRATCHPAD         0xBE
#define COPY_SCRATCHPAD         0x48
#define RECALL_E_E              0xB8

#define DS2438_PAGE_0           0x00
#define DS2438_PAGE_1           0x01
#define DS2438_PAGE_2           0x02
#define DS2438_PAGE_3           0x03
#define DS2438_PAGE_4           0x04
#define DS2438_PAGE_5           0x05
#define DS2438_PAGE_6           0x06
#define DS2438_PAGE_7           0x07

#define CONVERT_VOLTAGE         0xB4
#define CONVERT_TEMPERATURE     0x44

#define STATUS                  0
#define TEMP_LSB                1
#define TEMP_MSB                2
#define VOLT_LSB                3
#define VOLT_MSB                4
#define CURR_LSB                5
#define CURR_MSB                6
#define THRESH                  7
#define CRC8                    8
/** @endcond*/

/**
 * @union Configuration
 * @brief Configuration register of the DS2438.*/
typedef union
{
    unsigned char Register; /*!< The configuration register.*/

    struct
    {
        unsigned char IAD : 1; /*!<Current A/D Control Bit*/
        unsigned char CA : 1; /*!<Current Accumulator Configuration*/
        unsigned char EE : 1; /*!<Current Accumulator Shadow Selector*/
        unsigned char AD : 1; /*!<Voltage A/D Input Select Bit*/
        unsigned char TB : 1; /*!<Temperature Busy Flag*/
        unsigned char NVB : 1; /*!<Nonvolatile Memory Busy Flag*/
        unsigned char ADB : 1; /*!<A/D Converter Busy Flag*/
        unsigned char : 1;
    } Bits /*!< Bits of the configuration register.*/;

} Configuration; /*!< Varable to configure the DS2438. */

eReturnTypes DS2438Configure(sDevice device, Configuration Config);
eReturnTypes DS2438GetTemperatureAndHumidity(sDevice *device);
eReturnTypes DS2438IssueTemperatureAndHumidityConvertion(sDevice *device);

#endif