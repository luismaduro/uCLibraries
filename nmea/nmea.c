 /**
 *  @file       nmea.c
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

#include "nmea/nmea.h"

nmeaGPRMC GPRMC;

bool nmeaParseSentence(char *sentence)
{
    bool allOK = true;

    char checksumCalculated, checksumReceived;

    if ((unsigned char) strlen(sentence) < MINIMUM_SENTENCE_LENGTH)
        return false;

    checksumCalculated = nmeaCalculateChecksum(sentence);
    checksumReceived = nmeaGetChecksumReceived(sentence);

    if (checksumCalculated != checksumReceived)
        return false;

    if (strncmppgm2ram(sentence, (const MEM_MODEL rom char *) "$GPRMC,", 7) == 0)
        allOK &= nmeaParseGPRMC(sentence);

    if (allOK == true)
        return true;
    else
        return false;
}

bool nmeaParseGPRMC(char *sentence)
{
    char tempChar[10] = {0};
    char *tempCharPtr = &tempChar[0];

    strncpy(tempCharPtr, sentence + 7, 2);
    GPRMC.UTC.Hour = atob(tempCharPtr);

    strncpy(tempCharPtr, sentence + 9, 2);
    GPRMC.UTC.Minutes = atob(tempCharPtr);

    strncpy(tempCharPtr, sentence + 11, 2);
    GPRMC.UTC.Seconds = atob(tempCharPtr);

    strncpy(tempCharPtr, sentence + 14, 1);
    GPRMC.Status = *tempCharPtr;

    strncpy(tempCharPtr, sentence + 16, 7);
    GPRMC.Latitude = atof(tempCharPtr);
}

/**
 * Function to calculate the checksum of the received NMEA sentence.
 * @param sentence Pointer to the firt character of the sentence.
 * @return The value o the calculated checksum in hexadecimal.
 */
char nmeaCalculateChecksum(char *sentence)
{
    char checksum, i;

    i = 1;

    do
    {
        checksum = checksum ^ sentence[i++];
    }
    while (sentence[i] != '*');

    return checksum;
}

/**
 * Funtion to get the checksum from the received NMEA sentence.
 * @param sentence Pointer to the firt character of the sentence.
 * @return The value of the checksum present on the sentence in hexadecimal.
 * @todo Better implementation of this method!!!! Ugly as hell!!! Garantir que
 * está tudo em uppercase e tirar 55.
 */
char nmeaGetChecksumReceived(char *sentence)
{
    char checksum, i, c;

    i = 0;
    checksum = 0;

    while (sentence[i] != '*')
        i++;

    c = toupper(sentence[++i]);
    checksum += (c - 0x30) << 4;

    c = toupper(sentence[++i]);
    checksum += (c - 0x30);

    return checksum;
}