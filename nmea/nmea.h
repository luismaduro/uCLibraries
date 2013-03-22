 /**
 *  @file       nmea.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       15/01/2013
 *  @brief      NMEA 0183 sentence decoding library.
 *
 *  Copyright (C) 2013  Luis Maduro
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

#ifndef NMEA_H
#define NMEA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/** Number of characters of the smaller NMEA sentence*/
#define MINIMUM_SENTENCE_LENGTH     70
/** GPRMC datatype identifier*/
#define GPRMC_HEADER                "$GPRMC,"

/**
 * RMC packet information structure (Recommended Minimum sentence C)
 */
typedef struct _nmeaGPRMC
{

    /** UTC of position (just time)*/
    struct
    {
        /** Years since 1900 */
        unsigned char Year;
        /** Months since January - [0,11] */
        unsigned char Month;
        /** Day of the month - [1,31] */
        unsigned char Day;
        /** Hours since midnight - [0,23] */
        unsigned char Hour;
        /** Minutes after the hour - [0,59] */
        unsigned char Minutes;
        /** Seconds after the minute - [0,59] */
        unsigned char Seconds;

    } UTC;

    /** Status (A = active or V = Invalid) */
    char Status;
    /** Latitude in NDEG - [degree][min].[sec/60] */
    double Latitude;
    /** [N]orth or [S]outh */
    char North_South;
    /** Longitude in NDEG - [degree][min].[sec/60] */
    double Longitude;
    /** [E]ast or [W]est */
    char East_West;
    /** Speed over the ground in knots */
    double Speed;
    /** True course angle in degrees */
    double True_Course;
    /** Magnetic variation degrees (Easterly var. subtracts from true course)*/
    double Declination;
    /** [E]ast or [W]est */
    char Declination_Direction;
    /** Mode indicator of fix type (A = autonomous, D = differential,
     * E = estimated, N = not valid, S = simulator) */
    char Mode;
    /** Message Checksum*/
    unsigned char Checksum;

} nmeaGPRMC;

extern nmeaGPRMC GPRMC;

bool nmeaParseSentence(char *sentence);
bool nmeaParseGPRMC(char *sentence);
char nmeaCalculateChecksum(char * sentence);
char nmeaGetChecksumReceived(char *sentence);

#endif
