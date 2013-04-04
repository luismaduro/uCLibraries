/**
 *  @file       1-Wire.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Library to interface with 1-Wire devices.
 */
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include "1-Wire.h"

unsigned char ROMNumber[8]; /*!< Variable that stores the list of Devices found
                          * on the one wire bus.*/
int LastDiscrepancy = 0; /*!< Variable used in One_Wire_Search_Bus(). Does not
                          *  have use to the user.*/
int LastFamilyDiscrepancy = 0; /*!< Variable used in One_Wire_Search_Bus().
                                * Does not have use to the user.*/
int LastDeviceFlag = 0; /*!< Variable used in One_Wire_Search_Bus(). Does not
                         * have use to the user.*/
unsigned char CRCTemp = 0; /*!< Variable used in DoCRC8(). Does not have use
                            * to the user.*/

sDevice sensorDevicesList[MAX_SENSORS];
sDevice *sensorDevices = &sensorDevicesList[0];

const unsigned char dscrc_table[] = {
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

/**
 * @fn      unsigned char One_Wire_Reset_Pulse(void)
 * @brief   Initialization sequence start with reset pulse.
 *          This code generates reset sequence as per the protocol.
 * @return  Return the Presense Pulse from the slave.
 * @retval  0   At least one device is present in the bus.
 * @retval  1   No device is present on the bus.
 */
unsigned char OneWireResetPulse(void)
{
    unsigned char presence_detect;

    ONE_WIRE_PIN_DIRECTION = OUTPUT; // Drive the bus low
    ONE_WIRE_PIN = LOW;

    WaitUs(DELAY_240Us); // delay 480 microsecond (us)
    WaitUs(DELAY_240Us);

    ONE_WIRE_PIN_DIRECTION = INPUT; // Release the bus

    WaitUs(DELAY_70Us); // delay 70 microsecond (us)

    if (ONE_WIRE_PIN == HIGH)
    {
        presence_detect = 1;
    }

    else
    {
        presence_detect = 0;
    }

    WaitUs(DELAY_205Us); // delay 410 microsecond (us)
    WaitUs(DELAY_205Us);

    return presence_detect;
}

/**
 * @fn      void One_Wire_Write_Bit (unsigned char write_bit)
 * @brief   This function used to transmit a single bit to slave device.
 * @param   write_bit Write a bit to 1-wire slave device.
 */
void OneWireWriteBit(unsigned char write_bit)
{
    if (write_bit)
    {
        //writing a bit '1'
        ONE_WIRE_PIN_DIRECTION = OUTPUT; // Drive the bus low
        ONE_WIRE_PIN = LOW;
        WaitUs(DELAY_6Us); // delay 6 microsecond (us)
        ONE_WIRE_PIN_DIRECTION = INPUT; // Release the bus
        WaitUs(DELAY_64Us); // delay 64 microsecond (us)
    }

    else
    {
        //writing a bit '0'
        ONE_WIRE_PIN_DIRECTION = OUTPUT; // Drive the bus low
        ONE_WIRE_PIN = LOW;
        WaitUs(DELAY_60Us); // delay 60 microsecond (us)
        ONE_WIRE_PIN_DIRECTION = INPUT; // Release the bus
        WaitUs(DELAY_10Us); // delay 10 microsecond for
        //recovery (us)
    }
}

/**
 * @fn      unsigned char One_Wire_Read_Bit (void)
 * @brief   This function used to read a single bit from the slave device.
 * @return  Return the status of the ONE_WIRE_PIN
 */
unsigned char OneWireReadBit(void)
{
    unsigned char read_data;

    //reading a bit
    ONE_WIRE_PIN_DIRECTION = OUTPUT; // Drive the bus low
    ONE_WIRE_PIN = LOW;
    WaitUs(DELAY_2Us); // delay 6 microsecond (us)
    ONE_WIRE_PIN_DIRECTION = INPUT; // Release the bus
    WaitUs(DELAY_12Us); // delay 9 microsecond (us)

    if (ONE_WIRE_PIN == HIGH)
    {
        read_data = 1;
    }

    else
    {
        read_data = 0;
    }

    WaitUs(DELAY_55Us); // delay 55 microsecond (us)

    return read_data;
}

/**
 * @fn      void One_Wire_Write_Byte (unsigned char write_data)
 * @brief   This function used to transmit a complete byte to slave device.
 * @param   write_data  Byte to be sent to 1-wire slave device.
 */
void OneWireWriteByte(unsigned char write_data)
{
    unsigned char loop;

    for (loop = 0; loop < 8; loop++)
    {
        OneWireWriteBit(write_data & 0x01); //Sending LS-bit first
        write_data >>= 1; //shift the data byte
        //for the next bit to send
    }
}

/**
 * @fn      unsigned char One_Wire_Read_Byte (void)
 * @brief   This function used to read a complete byte from the slave device.
 * @return  Return the read byte from slave device.
 */
unsigned char OneWireReadByte(void)
{
    unsigned char loop, result = 0;

    for (loop = 0; loop < 8; loop++)
    {
        result >>= 1; // shift the result to get it ready
        //for the next bit to receive
        if (OneWireReadBit())
            result |= 0x80; // if result is one, then set MS-bit
    }
    return result;
}

/**
 * @fn      unsigned char Calculate_CRC(unsigned char *addr,unsigned char len)
 * @brief   This funtion calculates the CRC from the array of data bytes
 *          for 1-Wire devices.
 * @param   addr    Address of the first byte of the array.
 * @param   len     Length of the array of bytes
 * @return  Returns the CRC
 */
unsigned char CalculateCRC(unsigned char *addr, unsigned char len)
{
    unsigned char crc = 0, inbyte = 0, i, mix;

    while (len--)
    {
        inbyte = *addr++;

        for (i = 8; i; i--)
        {
            mix = (crc ^ inbyte) & 0x01;

            crc >>= 1;

            if (mix)
            {
                crc ^= 0x8C;
            }

            inbyte >>= 1;
        }
    }
    return crc;
}

/**
 * @fn unsigned char One_Wire_First(void)
 * @brief Prepare the global variables for the 1-Wire Bus Search algorithm.
 * @return Returns the result of One_Wire_Search_Bus().
 */
unsigned char OneWireFirst(void)
{
    // reset the search state
    LastDiscrepancy = 0;
    LastDeviceFlag = false;
    LastFamilyDiscrepancy = 0;

    return OneWireSearchBus();
}

/**
 * @fn unsigned char One_Wire_Next(void)
 * @brief Find the 'next' devices on the 1-Wire bus
 * @return Result of the search
 * @retval true Device found, ROM number in ROM_NO buffer
 * @retval false Device not found, end of search
 */
unsigned char OneWireNext(void)
{
    // leave the search state alone
    return OneWireSearchBus();
}

/**
 * @fn unsigned char One_Wire_Search_Bus(void)
 * @brief Perform the 1-Wire Search Algorithm on the 1-Wire bus using the
 * existing search state.
 * @return Result of the Search
 * @retval true Device found, ROM number in ROM_NO buffer
 * @retval false Device not found, end of search
 */
unsigned char OneWireSearchBus(void)
{
    int id_bit_number;
    int last_zero, rom_byte_number, search_result;
    int id_bit, cmp_id_bit;
    unsigned char rom_byte_mask, search_direction;

    // initialize for search
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = 0;
    CRCTemp = 0;

    // if the last call was not the last one
    if (!LastDeviceFlag)
    {
        // 1-Wire reset
        if (OneWireResetPulse())
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
                        search_direction = ((ROMNumber[rom_byte_number] & rom_byte_mask) > 0);
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
                    ROMNumber[rom_byte_number] |= rom_byte_mask;
                else
                    ROMNumber[rom_byte_number] &= ~rom_byte_mask;

                // serial number search direction write bit
                OneWireWriteBit(search_direction);

                // increment the byte counter id_bit_number
                // and shift the mask rom_byte_mask
                id_bit_number++;
                rom_byte_mask <<= 1;

                // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
                if (rom_byte_mask == 0)
                {
                    DoCRC8(ROMNumber[rom_byte_number]); // accumulate the CRC
                    rom_byte_number++;
                    rom_byte_mask = 1;
                }
            }
        }
        while (rom_byte_number < 8); // loop until through all ROM bytes 0-7

        // if the search was successful then
        if (!((id_bit_number < 65) || (CRCTemp != 0)))
        {
            // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
            LastDiscrepancy = last_zero;

            // check for last device
            if (LastDiscrepancy == 0)
                LastDeviceFlag = true;

            search_result = true;
        }
    }

    // if no device found then reset counters so next 'search' will be like a first
    if (!search_result || !ROMNumber[0])
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = false;
        LastFamilyDiscrepancy = 0;
        search_result = false;
    }

    return search_result;
}

/**
 * @fn unsigned char One_Wire_Verify(void)
 * @brief Verify the device with the ROM number in ROM_NO buffer is present.
 * @return Result of the search.
 * @retval true Device verified present
 * @retval false Device not present
 */
unsigned char OneWireVerify(void)
{
    unsigned char rom_backup[8];
    unsigned char i, rslt, ld_backup, ldf_backup, lfd_backup;

    // keep a backup copy of the current state
    for (i = 0; i < 8; i++)
        rom_backup[i] = ROMNumber[i];

    ld_backup = LastDiscrepancy;
    ldf_backup = LastDeviceFlag;
    lfd_backup = LastFamilyDiscrepancy;

    // set search to find the same device
    LastDiscrepancy = 64;
    LastDeviceFlag = false;

    if (OneWireSearchBus())
    {
        // check if same device found
        rslt = true;
        for (i = 0; i < 8; i++)
        {
            if (rom_backup[i] != ROMNumber[i])
            {
                rslt = false;
                break;
            }
        }
    }
    else
        rslt = false;

    // restore the search state
    for (i = 0; i < 8; i++)
        ROMNumber[i] = rom_backup[i];

    LastDiscrepancy = ld_backup;
    LastDeviceFlag = ldf_backup;
    LastFamilyDiscrepancy = lfd_backup;

    // return the result of the verify
    return rslt;
}

/**
 * @fn void One_Wire_Target_Setup(unsigned char family_code)
 * @brief Setup the search to find the device type 'family_code' on the next call to One_Wire_Next() if it is present.
 * @param family_code Family Code of the One Wire Devices to search
 */
void OneWireTargetSetup(unsigned char family_code)
{
    unsigned char i;

    // set the search state to find SearchFamily type devices
    ROMNumber[0] = family_code;
    for (i = 1; i < 8; i++)
        ROMNumber[i] = 0;
    LastDiscrepancy = 64;
    LastFamilyDiscrepancy = 0;
    LastDeviceFlag = false;
}

/**
 * @fn void One_Wire_Family_Skip_Setup(void)
 * @brief Setup the search to skip the current device type on the next call to OneWireNext().
 */
void OneWireFamilySkipSetup(void)
{
    // set the Last discrepancy to last family discrepancy
    LastDiscrepancy = LastFamilyDiscrepancy;
    LastFamilyDiscrepancy = 0;

    // check for end of list
    if (LastDiscrepancy == 0)
        LastDeviceFlag = true;
}

/**
 * @fn void Find_All_Devices_One_Wire(Lasered_ROM_Code *romcode, unsigned char *num_devices)
 * @brief Performs a full search for devices on 1-Wire Bus.
 * @param romcode Pointer to Lasered_ROM_Code type array to store the devices found on the bus.
 * @param num_devices Pointer to the variable to store the number of devices found on the bus.
 */
void FindAllDevicesOneWire(tLaseredROMCode *romcode, unsigned char *num_devices)
{
    tLaseredROMCode temp;
    unsigned char rslt = 0;
    unsigned char devices = 0;

    *num_devices = 0;
    
    rslt = OneWireFirst();

    while (rslt)
    {
        temp.FamilyCode = ROMNumber[0];
        temp.ROMCodeByte1 = ROMNumber[1];
        temp.ROMCodeByte2 = ROMNumber[2];
        temp.ROMCodeByte3 = ROMNumber[3];
        temp.ROMCodeByte4 = ROMNumber[4];
        temp.ROMCodeByte5 = ROMNumber[5];
        temp.ROMCodeByte6 = ROMNumber[6];
        temp.CRC = ROMNumber[7];

        *romcode++ = temp;
        devices++;
        *num_devices = devices;

        rslt = OneWireNext();
    }
}

/**
 * @fn unsigned char DoCRC8(unsigned char value)
 * @brief Calculate the CRC8 of the byte value provided with the current global 'crc8' value.
 * @param value The value to "add" to the 'crc8' variable value.
 * @return The resultant crc value.
 */
unsigned char DoCRC8(unsigned char value)
{
    // See Application Note 27 of Maxim

    // TEST BUILD
    CRCTemp = dscrc_table[CRCTemp ^ value];
    return CRCTemp;
}

/**
 * @fn void Wait(unsigned char uSec)
 * @brief Performs a delay of a specified value in microseconds.
 * @param uSec Value in microseconds to wait.
 */
void WaitUs(unsigned char uSec)
{
    PR2 = uSec;
    TMR2 = 0;
    PIR1bits.TMR2IF = 0;
    T2CONbits.TMR2ON = 1;
    while(!PIR1bits.TMR2IF);
    T2CONbits.TMR2ON = 0;
}