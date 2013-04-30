/*
Copyright (c) 2007, Jim Studt  (original old version - many contributors since)

The latest version of this library may be found at:
  http://www.pjrc.com/teensy/td_libs_OneWire.html

OneWire has been maintained by Paul Stoffregen (paul@pjrc.com) since
January 2010.  At the time, it was in need of many bug fixes, but had
been abandoned the original author (Jim Studt).  None of the known
contributors were interested in maintaining OneWire.  Paul typically
works on OneWire every 6 to 12 months.  Patches usually wait that
long.  If anyone is interested in more actively maintaining OneWire,
please contact Paul.

Version 2.2:
  Teensy 3.0 compatibility, Paul Stoffregen, paul@pjrc.com
  Arduino Due compatibility, http://arduino.cc/forum/index.php?topic=141030
  Fix DS18B20 example negative temperature
  Fix DS18B20 example's low res modes, Ken Butcher
  Improve reset timing, Mark Tillotson
  Add const qualifiers, Bertrik Sikken
  Add initial value input to crc16, Bertrik Sikken
  Add target_search() function, Scott Roberts

Version 2.1:
  Arduino 1.0 compatibility, Paul Stoffregen
  Improve temperature example, Paul Stoffregen
  DS250x_PROM example, Guillermo Lovato
  PIC32 (chipKit) compatibility, Jason Dangel, dangel.jason AT gmail.com
  Improvements from Glenn Trewitt:
  - crc16() now works
  - check_crc16() does all of calculation/checking work.
  - Added read_bytes() and write_bytes(), to reduce tedious loops.
  - Added ds2408 example.
  Delete very old, out-of-date readme file (info is here)

Version 2.0: Modifications by Paul Stoffregen, January 2010:
http://www.pjrc.com/teensy/td_libs_OneWire.html
  Search fix from Robin James
    http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1238032295/27#27
  Use direct optimized I/O in all cases
  Disable InterruptsON during timing critical sections
    (this solves many random communication errors)
  Disable InterruptsON during read-modify-write I/O
  Reduce RAM consumption by eliminating unnecessary
    variables and trimming many to 8 bits
  Optimize both crc8 - table version moved to flash

Modified to work with larger numbers of devices - avoids loop.
Tested in Arduino 11 alpha with 12 sensors.
26 Sept 2008 -- Robin James
http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1238032295/27#27

Updated to work with arduino-0008 and to include skip() as of
2007/07/06. --RJL20

Modified to calculate the 8-bit CRC directly, avoiding the need for
the 256-byte lookup table to be loaded in RAM.  Tested in arduino-0010
-- Tom Pollard, Jan 23, 2008

Jim Studt's original library was modified by Josh Larios.

Tom Pollard, pollard@alum.mit.edu, contributed around May 20, 2008

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Much of the code was inspired by Derek Yerger's code, though I don't
think much of that remains.  In any event that was..
    (copyleft) 2006 by Derek Yerger - Free to distribute freely.

The CRC code was excerpted and inspired by the Dallas Semiconductor
sample code bearing this copyright.
//---------------------------------------------------------------------------
// Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Except as contained in this notice, the name of Dallas Semiconductor
// shall not be used except as stated in the Dallas Semiconductor
// Branding Policy.
//--------------------------------------------------------------------------
 */

#include "OneWire.h"

void OneWireInit(void)
{
    //Timer 3 incretments at 1us @ 32MHz
    T3GCONbits.TMR3GE = 0;
    T3GCONbits.T3GTM = 0;
    T3GCONbits.T3GSPM = 0;

    T3CONbits.TMR3CS = 0b00;
    T3CONbits.T3CKPS = 0b11;

    T3CONbits.T3SYNC = 1;
    T3CONbits.RD16 = 1;

    PIE2bits.TMR3IE = 0;
    PIR2bits.TMR3IF = 0;
    IPR2bits.TMR3IP = 0;

    TMR3L = 0;
    TMR3H = 0;

    T3CONbits.TMR3ON = 0;

#if ONEWIRE_SEARCH
    OneWireResetSearch();
#endif
}

/**
 * @fn void Wait(unsigned char uSec)
 * @brief Performs a delay of a specified value in microseconds.
 * @param uSec Value in microseconds to wait.
 */
void delayMicroseconds(unsigned int uSec)
{
    T3CONbits.TMR3ON = 0;
    PIR2bits.TMR3IF = 0;
    uSec = 0xFFFF - uSec;
    TMR3H = 0;
    TMR3L = 0;
    TMR3H = (uSec >> 8) & 0x00FF;
    TMR3L = uSec & 0x00FF;
    T3CONbits.TMR3ON = 1;
    while (!PIR2bits.TMR3IF);
    T3CONbits.TMR3ON = 0;
}


// Perform the onewire reset function.  We will wait up to 250uS for
// the bus to come high, if it doesn't then it is broken or shorted
// and we return a 0;
//
// Returns 1 if a device asserted a presence pulse, 0 otherwise.
//

unsigned char OneWireReset(void)
{
    unsigned char r;
    unsigned char retries = 125;

    InterruptsOFF();
    ONEWIRE_PIN_DIRECTION = INPUT;
    InterruptsON();
    // wait until the wire is high... just in case
    do
    {
        if (--retries == 0) return 0;
        delayMicroseconds(2);
    }
    while (!ONEWIRE_PIN_READ);

    InterruptsOFF();
    ONEWIRE_PIN_WRITE = LOW;
    ONEWIRE_PIN_DIRECTION = OUTPUT; // drive output low
    InterruptsON();
    delayMicroseconds(480);
    InterruptsOFF();
    ONEWIRE_PIN_DIRECTION = INPUT; // allow it to float
    delayMicroseconds(70);
    r = !ONEWIRE_PIN_READ;
    InterruptsON();
    delayMicroseconds(410);
    return r;
}

//
// Write a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//

void OneWireWriteBit(unsigned char v)
{
    if (v & 1)
    {
        InterruptsOFF();
        ONEWIRE_PIN_WRITE = LOW;
        ONEWIRE_PIN_DIRECTION = OUTPUT; // drive output low
        delayMicroseconds(10);
        ONEWIRE_PIN_WRITE = HIGH; // drive output high
        InterruptsON();
        delayMicroseconds(55);
    }
    else
    {
        InterruptsOFF();
        ONEWIRE_PIN_WRITE = LOW;
        ONEWIRE_PIN_DIRECTION = OUTPUT; // drive output low
        delayMicroseconds(65);
        ONEWIRE_PIN_WRITE = HIGH; // drive output high
        InterruptsON();
        delayMicroseconds(5);
    }
}

//
// Read a bit. Port and bit is used to cut lookup time and provide
// more certain timing.
//

unsigned char OneWireReadBit(void)
{
    unsigned char r;

    InterruptsOFF();
    ONEWIRE_PIN_DIRECTION = OUTPUT;
    ONEWIRE_PIN_WRITE = LOW;
    delayMicroseconds(3);
    ONEWIRE_PIN_DIRECTION = INPUT; // let pin float, pull up will raise
    delayMicroseconds(10);
    r = ONEWIRE_PIN_READ;
    InterruptsON();
    delayMicroseconds(53);
    return r;
}

//
// Write a byte. The writing code uses the active drivers to raise the
// pin high, if you need power after the write (e.g. DS18S20 in
// parasite power mode) then set 'power' to 1, otherwise the pin will
// go tri-state at the end of the write to avoid heating in a short or
// other mishap.
//

void OneWireWrite(unsigned char v)
{
    unsigned char bitMask;

    for (bitMask = 0x01; bitMask; bitMask <<= 1)
    {
        OneWireWriteBit((bitMask & v) ? 1 : 0);
    }
}

void OneWireWriteBytes(unsigned char *buf, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
        OneWireWrite(buf[i]);
}

//
// Read a byte
//

unsigned char OneWireRead(void)
{
    unsigned char bitMask;
    unsigned char r = 0;

    for (bitMask = 0x01; bitMask; bitMask <<= 1)
    {
        if (OneWireReadBit()) r |= bitMask;
    }
    return r;
}

void OneWireReadBytes(unsigned char *buf, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
        buf[i] = OneWireRead();
}

//
// Do a ROM select
//

void OneWireSelect(tLaseredROMCode *device)
{
    unsigned char i;

    OneWireWrite(0x55); // Choose ROM

    for (i = 0; i < 8; i++)
        OneWireWrite(device->Array[i]);
}

//
// Do a ROM skip
//
void OneWireSkip(void)
{
    OneWireWrite(0xCC); // Skip ROM
}

#if ONEWIRE_SEARCH

//
// You need to use this function to start a search again from the beginning.
// You do not need to do it for the first search, though you could.
//

void OneWireResetSearch(void)
{
    // reset the search state
    LastDiscrepancy = 0;
    LastDeviceFlag = FALSE;
    LastFamilyDiscrepancy = 0;
    for (int i = 7;; i--)
    {
        ROM_NO[i] = 0;
        if (i == 0) break;
    }
}

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
//

void OneWireTargetSearch(unsigned char family_code)
{
    // set the search state to find SearchFamily type devices
    ROM_NO[0] = family_code;
    for (unsigned char i = 1; i < 8; i++)
        ROM_NO[i] = 0;
    LastDiscrepancy = 64;
    LastFamilyDiscrepancy = 0;
    LastDeviceFlag = FALSE;
}

//
// Perform a search. If this function returns a '1' then it has
// enumerated the next device and you may retrieve the ROM from the
// OneWire::address variable. If there are no devices, no further
// devices, or something horrible happens in the middle of the
// enumeration then a 0 is returned.  If a new device is found then
// its address is copied to newAddr.  Use OneWire::reset_search() to
// start over.
//
// --- Replaced by the one from the Dallas Semiconductor web site ---
//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//

unsigned char OneWireSearch()
{
    unsigned char id_bit_number;
    unsigned char last_zero, rom_byte_number, search_result;
    unsigned char id_bit, cmp_id_bit;

    unsigned char rom_byte_mask, search_direction;

    // initialize for search
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = 0;

    // if the last call was not the last one
    if (!LastDeviceFlag)
    {
        // 1-Wire reset
        if (!OneWireReset())
        {
            // reset the search
            LastDiscrepancy = 0;
            LastDeviceFlag = FALSE;
            LastFamilyDiscrepancy = 0;
            return FALSE;
        }

        // issue the search command
        OneWireWrite(0xF0);

        // loop to do the search
        do
        {
            // read a bit and its complement
            id_bit = OneWireReadBit();
            cmp_id_bit = OneWireReadBit();

            // check for no devices on 1-wire
            if ((id_bit == 1) && (cmp_id_bit == 1))
                break;
            else
            {
                // all devices coupled have 0 or 1
                if (id_bit != cmp_id_bit)
                    search_direction = id_bit; // bit write value for search
                else
                {
                    // if this discrepancy if before the Last Discrepancy
                    // on a previous next then pick the same as last time
                    if (id_bit_number < LastDiscrepancy)
                        search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
                    else
                        // if equal to last pick 1, if not then pick 0
                        search_direction = (id_bit_number == LastDiscrepancy);

                    // if 0 was picked then record its position in LastZero
                    if (search_direction == 0)
                    {
                        last_zero = id_bit_number;

                        // check for Last discrepancy in family
                        if (last_zero < 9)
                            LastFamilyDiscrepancy = last_zero;
                    }
                }

                // set or clear the bit in the ROM byte rom_byte_number
                // with mask rom_byte_mask
                if (search_direction == 1)
                    ROM_NO[rom_byte_number] |= rom_byte_mask;
                else
                    ROM_NO[rom_byte_number] &= ~rom_byte_mask;

                // serial number search direction write bit
                OneWireWriteBit(search_direction);

                // increment the byte counter id_bit_number
                // and shift the mask rom_byte_mask
                id_bit_number++;
                rom_byte_mask <<= 1;

                // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
                if (rom_byte_mask == 0)
                {
                    rom_byte_number++;
                    rom_byte_mask = 1;
                }
            }
        }
        while (rom_byte_number < 8); // loop until through all ROM bytes 0-7

        // if the search was successful then
        if (!(id_bit_number < 65))
        {
            // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
            LastDiscrepancy = last_zero;

            // check for last device
            if (LastDiscrepancy == 0)
                LastDeviceFlag = TRUE;

            search_result = TRUE;
        }
    }

    // if no device found then reset counters so next 'search' will be like a first
    if (!search_result || !ROM_NO[0])
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = FALSE;
        LastFamilyDiscrepancy = 0;
        search_result = FALSE;
    }

    return search_result;
}

/**
 * @brief Performs a full search for devices on 1-Wire Bus.
 * @param romcode Pointer to Lasered_ROM_Code type array to store the devices
 *                      found on the bus.
 * @param num_devices Pointer to the variable to store the number of devices
 *                      found on the bus.
 */
void OneWireFindAllDevicesOnBus(tLaseredROMCode *romcode, unsigned char *num_devices)
{
    tLaseredROMCode temp;
    unsigned char rslt = 0, devNum = 0;

    OneWireResetSearch();

    *num_devices = 0;

    rslt = OneWireSearch();

    while (rslt)
    {
        temp.FamilyCode = ROM_NO[0];
        temp.ROMCodeByte1 = ROM_NO[1];
        temp.ROMCodeByte2 = ROM_NO[2];
        temp.ROMCodeByte3 = ROM_NO[3];
        temp.ROMCodeByte4 = ROM_NO[4];
        temp.ROMCodeByte5 = ROM_NO[5];
        temp.ROMCodeByte6 = ROM_NO[6];
        temp.OWICRC = ROM_NO[7];

        if (ROM_NO[7] == OneWireCRC8(&(temp.Array[0]), 7))
        {
            *romcode++ = temp;
            devNum++;
        }

        rslt = OneWireSearch();
    }

    *num_devices = devNum;
}
#endif

#if ONEWIRE_CRC
// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
//

#if ONEWIRE_CRC8_TABLE
// This table comes from Dallas sample code where it is freely reusable,
// though Copyright (C) 2000 Dallas Semiconductor Corporation
static const unsigned char dscrc_table[] = {
    0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
    157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
    35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
    190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
    70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
    219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
    101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
    248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
    140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
    17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
    175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
    50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
    202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
    87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
    233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
    116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//

unsigned char OneWireCRC8(const unsigned char *addr, unsigned char len)
{
    unsigned char crc = 0;

    while (len--)
    {
        crc = dscrc_table[crc ^ *addr++];
    }
    return crc;
}
#else
//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//

unsigned char OneWireCRC8(unsigned char *addr, unsigned char len)
{
    unsigned char crc = 0;

    while (len--)
    {
        unsigned char inbyte = *addr++;
        for (unsigned char i = 8; i; i--)
        {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}
#endif

#if ONEWIRE_CRC16

bool OneWireCheckCRC16(const unsigned char* input, unsigned int len, const unsigned char* inverted_crc, unsigned int crc)
{
    crc = ~OneWireCRC16(input, len, crc);
    return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

unsigned int OneWireCRC16(const unsigned char* input, unsigned int len, unsigned int crc)
{
    static const unsigned char oddparity[16] = {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};

    for (unsigned int i = 0; i < len; i++)
    {
        // Even though we're just copying a byte from the input,
        // we'll be doing 16-bit computation with it.
        unsigned int cdata = input[i];
        cdata = (cdata ^ crc) & 0xff;
        crc >>= 8;

        if (oddparity[cdata & 0x0F] ^ oddparity[cdata >> 4])
            crc ^= 0xC001;

        cdata <<= 6;
        crc ^= cdata;
        cdata <<= 1;
        crc ^= cdata;
    }
    return crc;
}
#endif

#endif

