/**
 *  @file       DS2438.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Header File for DS2438 Library
 */
#ifndef DS2438_H
#define DS2438_H

#include <stdbool.h>
#include "DS2482.h"

/** @cond IGNORE*/
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
    } Bits; /*!< Bits of the configuration register.*/

} tDS2438Config; /*!< Varable to configure the DS2438. */

unsigned char DS2438Configure(tLaseredROMCode device, tDS2438Config Config);
unsigned char DS2438GetData(tLaseredROMCode *device, float *temperature,
                            float *voltage, float *current, float *energy);
unsigned char DS2438IssueConvertions(tLaseredROMCode *device);

#endif
