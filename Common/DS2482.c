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

#include "DS2482.h"

unsigned char ROM_NO[8];
unsigned char LastDiscrepancy;
unsigned char LastFamilyDiscrepancy;
unsigned char LastDeviceFlag;

unsigned char DS2482Reset(void)
{
    unsigned char status;

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_DRST);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CNotAck();
    I2CStop();
    status = I2CRead();

    // check for failure due to incorrect read back of status
    return ((status & 0xf7) == 0x10);

}

unsigned char DS2482Detect(void)
{
    if (!DS2482Reset())
        return false;

    if (!DS2482WriteConfig(DS2482_CFG_APU))
        return false;

    return true;
}

unsigned char DS2482WriteConfig(unsigned char config)
{

    unsigned char read_config;

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_WCFG);
    I2CWrite(config | (~config << 4));
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CNotAck();
    I2CStop();
    read_config = I2CRead();

    // check for failure due to incorrect read back
    if (config != read_config)
    {
        DS2482Reset();
        return false;
    }

    return true;

}

unsigned char DS2482ChannelSelect(unsigned char channel)
{

    unsigned char ch, ch_read, read_channel;

    switch (channel)
    {
    default:
    case 0:
        ch = DS2482_CH_IO0;
        ch_read = DS2482_RCH_IO0;
        break;
    case 1:
        ch = DS2482_CH_IO1;
        ch_read = DS2482_RCH_IO1;
        break;
    case 2:
        ch = DS2482_CH_IO2;
        ch_read = DS2482_RCH_IO2;
        break;
    case 3:
        ch = DS2482_CH_IO3;
        ch_read = DS2482_RCH_IO3;
        break;
    case 4:
        ch = DS2482_CH_IO4;
        ch_read = DS2482_RCH_IO4;
        break;
    case 5:
        ch = DS2482_CH_IO5;
        ch_read = DS2482_RCH_IO5;
        break;
    case 6:
        ch = DS2482_CH_IO6;
        ch_read = DS2482_RCH_IO6;
        break;
    case 7:
        ch = DS2482_CH_IO7;
        ch_read = DS2482_RCH_IO7;
        break;
    };

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_CHSL);
    I2CWrite(ch);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CNotAck();
    I2CStop();
    read_channel = I2CRead();

    return (read_channel == ch_read);

}

unsigned char OneWireReset(void)
{

    unsigned char status = 0;
    int poll_count = 0;

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_1WRS);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CAck();

    do
    {
        status = I2CRead();
    }
    while ((status & DS2482_STATUS_1WB) && (poll_count++ < POLL_LIMIT));

    I2CNotAck();
    I2CStop();
    status = I2CRead();

    if (poll_count >= POLL_LIMIT)
    {
        DS2482Reset();
        return false;
    }

    // check for presence detect
    if (status & DS2482_STATUS_PPD)
        return true;
    else
        return false;

}

void OneWireWriteBit(unsigned char sendbit)
{
    OneWireTouchBit(sendbit);
}

unsigned char OneWireReadBit(void)
{
    return OneWireTouchBit(0x01);
}

unsigned char OneWireTouchBit(unsigned char sendbit)
{
    unsigned char status;
    int poll_count = 0;

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_1WSB);
    I2CWrite(sendbit ? 0x80 : 0x00);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CAck();

    do
    {
        status = I2CRead();
    }
    while ((status & DS2482_STATUS_1WB) && (poll_count++ < POLL_LIMIT));

    I2CNotAck();
    I2CStop();
    status = I2CRead();

    if (poll_count >= POLL_LIMIT)
    {
        DS2482Reset();
        return false;
    }

    // check for single bit result
    if (status & DS2482_STATUS_SBR)
        return true;
    else
        return false;

}

void OneWireWriteByte(unsigned char sendbyte)
{
    unsigned char status;
    int poll_count = 0;

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_1WWB);
    I2CWrite(sendbyte);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CAck();

    do
    {
        status = I2CRead();
    }
    while ((status & DS2482_STATUS_1WB) && (poll_count++ < POLL_LIMIT));

    I2CNotAck();
    I2CStop();
    status = I2CRead();

    if (poll_count >= POLL_LIMIT)
        DS2482Reset();

}

unsigned char OneWireReadByte(void)
{
    unsigned char data, status;
    int poll_count = 0;

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_1WRB);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CAck();

    do
    {
        status = I2CRead();
    }
    while ((status & DS2482_STATUS_1WB) && (poll_count++ < POLL_LIMIT));

    I2CNotAck();
    I2CStop();
    I2CRead();

    if (poll_count >= POLL_LIMIT)
    {
        DS2482Reset();
        return false;
    }

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_SRP);
    I2CWrite(DS2482_READPTR_RDR);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CNotAck();
    I2CStop();
    data = I2CRead();

    return data;
}

/**
 * Use the DS2482 help command '1-Wire triplet' to perform one bit of a 1-Wire 
 * search. This command does two read bits and one write bit. The write bit is 
 * either the default direction (all device have same bit) or in case of a 
 * discrepancy, the 'search_direction' parameter is used.
 * 
 * @param search_direction
 * @return The DS2482 status byte result from the triplet command
 */
unsigned char DS2482SearchTriplet(int search_direction)
{
    unsigned char status;
    int poll_count = 0;

    // 1-Wire Triplet (Case B)
    //   S AD,0 [A] 1WT [A] SS [A] Sr AD,1 [A] [Status] A [Status] A\ P
    //                                         \--------/
    //                           Repeat until 1WB bit has changed to 0
    //  [] indicates from slave
    //  SS indicates byte containing search direction bit value in msbit

    I2CStart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Transmitter);
    I2CWrite(DS2482_CMD_1WT);
    I2CWrite(search_direction ? 0x80 : 0x00);
    I2CRestart();
    I2CSendAddress(DS2482_I2C_ADDR, I2C_Direction_Receiver);
    I2CAck();

    do
    {
        status = I2CRead();
    }
    while ((status & DS2482_STATUS_1WB) && (poll_count++ < POLL_LIMIT));

    I2CNotAck();
    I2CStop();
    I2CRead();

    // check for failure due to poll limit reached
    if (poll_count >= POLL_LIMIT)
    {
        DS2482Reset();
        return false;
    }

    // return status byte
    return status;
}

void OneWireBlockTransfer(unsigned char *transfer_buffer, unsigned char length)
{
    unsigned char i;
    for (i = 0; i < length; i++)
        transfer_buffer[i] = OneWireTouchByte(transfer_buffer[i]);

}

unsigned char OneWireTouchByte(unsigned char sendbyte)
{
    if (sendbyte == 0xff)
        return OneWireReadByte();
    else
    {
        OneWireWriteByte(sendbyte);
        return sendbyte;
    }
}

unsigned char OneWireCRC8(unsigned char *addr, unsigned char len)
{
    unsigned char crc = 0;
    unsigned char i;

    while (len--)
    {
        unsigned char inbyte = *addr++;
        for (i = 8; i; i--)
        {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

/**
 * Find the 'first' devices on the 1-Wire network
 * @retval true device found, ROM number in ROM_NO buffer
 * @retval false no device present
 */
unsigned char OneWireFirst(void)
{
    // reset the search state
    LastDiscrepancy = 0;
    LastDeviceFlag = false;
    LastFamilyDiscrepancy = 0;

    return OneWireSearch();
}

/**
 * Find the 'next' devices on the 1-Wire network
 * @retval true device found, ROM number in ROM_NO buffer
 * @retval false device not found, end of search
 */
unsigned char OneWireNext(void)
{
    // leave the search state alone
    return OneWireSearch();
}

//-------------------------------------------------------------------------
//
// Returns:   TRUE (1) : when a 1-Wire device was found and its
//                       Serial Number placed in the global ROM
//            FALSE (0): when no new device was found.  Either the
//                       last search was the last device or there
//                       are no devices on the 1-Wire Net.
//

/**
 * The 'OneWireSearch' function does a general search. This function continues from
 * the previous search state. The search state can be reset by using the 
 * 'OneWireFirst' function.
 * 
 * @retval true When a 1-Wire device was found and its Serial Number placed in 
 *              the global ROM
 * @retval false        When no new device was found. Either the last search was
 *                      the last device or there are no devices on the 1-Wire Net.
 */
unsigned char OneWireSearch(void)
{
    unsigned char id_bit_number;
    unsigned char last_zero, rom_byte_number, search_result;
    unsigned char id_bit, cmp_id_bit;
    unsigned char rom_byte_mask, search_direction, status;

    // initialize for search
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = false;

    // if the last call was not the last one
    if (!LastDeviceFlag)
    {
        // 1-Wire reset
        if (!OneWireReset())
        {
            // reset the search
            LastDiscrepancy = 0;
            LastDeviceFlag = false;
            LastFamilyDiscrepancy = 0;
            return false;
        }

        // issue the search command
        OneWireWriteByte(0xF0);

        // loop to do the search
        do
        {
            // if this discrepancy if before the Last Discrepancy
            // on a previous next then pick the same as last time
            if (id_bit_number < LastDiscrepancy)
            {
                if ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0)
                    search_direction = 1;
                else
                    search_direction = 0;
            }
            else
            {
                // if equal to last pick 1, if not then pick 0
                if (id_bit_number == LastDiscrepancy)
                    search_direction = 1;
                else
                    search_direction = 0;
            }

            // Perform a triple operation on the DS2482 which will perform
            // 2 read bits and 1 write bit
            status = DS2482SearchTriplet(search_direction);

            // check bit results in status byte
            id_bit = ((status & DS2482_STATUS_SBR) == DS2482_STATUS_SBR);
            cmp_id_bit = ((status & DS2482_STATUS_TSB) == DS2482_STATUS_TSB);
            search_direction =
                    ((status & DS2482_STATUS_DIR) == DS2482_STATUS_DIR) ? (unsigned char) 1 : (unsigned char) 0;

            // check for no devices on 1-Wire
            if ((id_bit) && (cmp_id_bit))
                break;
            else
            {
                if ((!id_bit) && (!cmp_id_bit) && (search_direction == 0))
                {
                    last_zero = id_bit_number;

                    // check for Last discrepancy in family
                    if (last_zero < 9)
                        LastFamilyDiscrepancy = last_zero;
                }

                // set or clear the bit in the ROM byte rom_byte_number
                // with mask rom_byte_mask
                if (search_direction == 1)
                    ROM_NO[rom_byte_number] |= rom_byte_mask;
                else
                    ROM_NO[rom_byte_number] &= (unsigned char) ~rom_byte_mask;

                // increment the byte counter id_bit_number
                // and shift the mask rom_byte_mask
                id_bit_number++;
                rom_byte_mask <<= 1;

                // if the mask is 0 then go to new SerialNum byte rom_byte_number
                // and reset mask
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
            // search successful so set LastDiscrepancy,LastDeviceFlag
            // search_result
            LastDiscrepancy = last_zero;

            // check for last device
            if (LastDiscrepancy == 0)
                LastDeviceFlag = true;

            search_result = true;
        }
    }

    // if no device found then reset counters so next
    // 'search' will be like a first

    if (!search_result || (ROM_NO[0] == 0))
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = false;
        LastFamilyDiscrepancy = 0;
        search_result = false;
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

    *num_devices = 0;

    rslt = OneWireFirst();

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

        *romcode++ = temp;
        devNum++;

        rslt = OneWireNext();
    }

    *num_devices = devNum;
}
