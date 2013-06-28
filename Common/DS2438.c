/**
 *  @file       DS2438.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Library to interface with DS2438 temperature sensor.
 */

#include <stdio.h>
#include <stdlib.h>
#include "DS2438.h"

/**
 * This funtion configures the DS2438 with the specified confguration.
 * @param Config Configuration that will be sent to the DS2438. 
 * This is a Configuration type variable.
 * @return Returns an ::Return_Types enum to be verified if all went well.
 */
unsigned char DS2438Configure(tLaseredROMCode device, tDS2438Config Config)
{
    unsigned char DS2438Data[9] = {0};

    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.OWICRC);

    OneWireWriteByte(WRITE_SCRATCHPAD);
    OneWireWriteByte(DS2438_PAGE_0);
    OneWireWriteByte(Config.Register);

    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.OWICRC);

    OneWireWriteByte(READ_SCRATCHPAD);
    OneWireWriteByte(DS2438_PAGE_0);
    DS2438Data[STATUS] = OneWireReadByte();
    DS2438Data[TEMP_LSB] = OneWireReadByte();
    DS2438Data[TEMP_MSB] = OneWireReadByte();
    DS2438Data[VOLT_LSB] = OneWireReadByte();
    DS2438Data[VOLT_MSB] = OneWireReadByte();
    DS2438Data[CURR_LSB] = OneWireReadByte();
    DS2438Data[CURR_MSB] = OneWireReadByte();
    DS2438Data[THRESH] = OneWireReadByte();
    DS2438Data[CRC8] = OneWireReadByte();

    if (DS2438Data[CRC8] != OneWireCRC8(&DS2438Data[0], 8))
        return false;

    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.OWICRC);

    OneWireWriteByte(COPY_SCRATCHPAD);
    OneWireWriteByte(DS2438_PAGE_0);

    return true;
}

/**
 * This funtion gets the values available on the DS2438. Specifically the the
 * voltage and temperature from the sensor.
 * @param temperature Pointer to the variable that stores the temperature.
 * @param humidity Pointer to the variable that stores the humidity.
 * @return Returns an ::Return_Types enum to be verified if all went well.
 */
unsigned char DS2438GetData(tLaseredROMCode *device, float *temperature,
                            float *voltage, float *current, float *energy)
{
    uint8_t DS2438Data[9] = {0};

    int16_t temp;

    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device->FamilyCode);
    OneWireWriteByte(device->ROMCodeByte1);
    OneWireWriteByte(device->ROMCodeByte2);
    OneWireWriteByte(device->ROMCodeByte3);
    OneWireWriteByte(device->ROMCodeByte4);
    OneWireWriteByte(device->ROMCodeByte5);
    OneWireWriteByte(device->ROMCodeByte6);
    OneWireWriteByte(device->OWICRC);
    OneWireWriteByte(RECALL_E_E);
    OneWireWriteByte(DS2438_PAGE_0);

    //read scratchpad
    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device->FamilyCode);
    OneWireWriteByte(device->ROMCodeByte1);
    OneWireWriteByte(device->ROMCodeByte2);
    OneWireWriteByte(device->ROMCodeByte3);
    OneWireWriteByte(device->ROMCodeByte4);
    OneWireWriteByte(device->ROMCodeByte5);
    OneWireWriteByte(device->ROMCodeByte6);
    OneWireWriteByte(device->OWICRC);
    OneWireWriteByte(READ_SCRATCHPAD);
    OneWireWriteByte(DS2438_PAGE_0);
    DS2438Data[STATUS] = OneWireReadByte();
    DS2438Data[TEMP_LSB] = OneWireReadByte();
    DS2438Data[TEMP_MSB] = OneWireReadByte();
    DS2438Data[VOLT_LSB] = OneWireReadByte();
    DS2438Data[VOLT_MSB] = OneWireReadByte();
    DS2438Data[CURR_LSB] = OneWireReadByte();
    DS2438Data[CURR_MSB] = OneWireReadByte();
    DS2438Data[THRESH] = OneWireReadByte();
    DS2438Data[CRC8] = OneWireReadByte();

    if (DS2438Data[CRC8] != OneWireCRC8(&DS2438Data[0], 8))
        return false;

    temp = (int16_t) DS2438Data[TEMP_MSB];
    temp <<= 8;
    temp |= DS2438Data[TEMP_LSB];
    *temperature = (float) temp;
    *temperature *= 0.00390625;

    temp = (int16_t) DS2438Data[VOLT_MSB];
    temp <<= 8;
    temp |= DS2438Data[VOLT_LSB];
    *voltage = (float) temp;

    *voltage *= 0.01;

    temp = (int16_t) DS2438Data[CURR_MSB];
    temp <<= 8;
    temp |= DS2438Data[CURR_MSB];
    *current = (float) temp;

    *current /= (4096.0 * 0.05);

    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device->FamilyCode);
    OneWireWriteByte(device->ROMCodeByte1);
    OneWireWriteByte(device->ROMCodeByte2);
    OneWireWriteByte(device->ROMCodeByte3);
    OneWireWriteByte(device->ROMCodeByte4);
    OneWireWriteByte(device->ROMCodeByte5);
    OneWireWriteByte(device->ROMCodeByte6);
    OneWireWriteByte(device->OWICRC);
    OneWireWriteByte(RECALL_E_E);
    OneWireWriteByte(DS2438_PAGE_1);

    //read scratchpad
    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device->FamilyCode);
    OneWireWriteByte(device->ROMCodeByte1);
    OneWireWriteByte(device->ROMCodeByte2);
    OneWireWriteByte(device->ROMCodeByte3);
    OneWireWriteByte(device->ROMCodeByte4);
    OneWireWriteByte(device->ROMCodeByte5);
    OneWireWriteByte(device->ROMCodeByte6);
    OneWireWriteByte(device->OWICRC);
    OneWireWriteByte(READ_SCRATCHPAD);
    OneWireWriteByte(DS2438_PAGE_1);
    DS2438Data[0] = OneWireReadByte();
    DS2438Data[1] = OneWireReadByte();
    DS2438Data[2] = OneWireReadByte();
    DS2438Data[3] = OneWireReadByte();
    DS2438Data[4] = OneWireReadByte();
    DS2438Data[5] = OneWireReadByte();
    DS2438Data[6] = OneWireReadByte();
    DS2438Data[7] = OneWireReadByte();
    DS2438Data[8] = OneWireReadByte();

    if (DS2438Data[CRC8] != OneWireCRC8(&DS2438Data[0], 8))
        return false;

    temp = (int16_t) DS2438Data[0];
    *energy = (float) temp;

    *energy /= (2048.0 * 0.05);

    return true;
}

unsigned char DS2438IssueConvertions(tLaseredROMCode *device)
{
    //Convert Temperature
    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device->FamilyCode);
    OneWireWriteByte(device->ROMCodeByte1);
    OneWireWriteByte(device->ROMCodeByte2);
    OneWireWriteByte(device->ROMCodeByte3);
    OneWireWriteByte(device->ROMCodeByte4);
    OneWireWriteByte(device->ROMCodeByte5);
    OneWireWriteByte(device->ROMCodeByte6);
    OneWireWriteByte(device->OWICRC);
    OneWireWriteByte(CONVERT_TEMPERATURE);

    if (OneWireReset() == 0)
        return false;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device->FamilyCode);
    OneWireWriteByte(device->ROMCodeByte1);
    OneWireWriteByte(device->ROMCodeByte2);
    OneWireWriteByte(device->ROMCodeByte3);
    OneWireWriteByte(device->ROMCodeByte4);
    OneWireWriteByte(device->ROMCodeByte5);
    OneWireWriteByte(device->ROMCodeByte6);
    OneWireWriteByte(device->OWICRC);
    OneWireWriteByte(CONVERT_VOLTAGE);

    return true;
}
