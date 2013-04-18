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
unsigned char DS2438Configure(tLaseredROMCode device, DS2438_Typedef Config)
{
    unsigned char DS2438Data[9] = {0};

    if (OneWireReset() == 1)
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

    DelayMiliSeconds(10);

    if (OneWireReset() == 1)
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

    if (OneWireReset() == 1)
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

    DelayMiliSeconds(10);

    return true;
}

/**
 * This funtion gets the values available on the DS2438. Specifically the the
 * voltage and temperature from the sensor.
 * @param temperature Pointer to the variable that stores the temperature.
 * @param humidity Pointer to the variable that stores the humidity.
 * @return Returns an ::Return_Types enum to be verified if all went well.
 */
unsigned char DS2438GetTemperatureAndHumidity(tLaseredROMCode *device)
{
    unsigned char DS2438Data[9] = {0};

    //recall memory
    if (OneWireReset() == 1)
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
    if (OneWireReset() == 1)
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

/*
    int_temp = (int) DS2438Data[TEMP_MSB];
    int_temp <<= 8;
    int_temp |= DS2438Data[TEMP_LSB];
    float_temp = (float) int_temp;
    device->temperature = float_temp * 0.00390625;

    int_voltage = (int) DS2438Data[VOLT_MSB];
    int_voltage <<= 8;
    int_voltage |= DS2438Data[VOLT_LSB];
    float_voltage = (float) int_voltage;

    float_voltage *= 0.01;

    float_humidity = ((float_voltage / 5.0F) - 0.16F) / 0.0062;

    device->humidity = (float) (float_humidity / (1.0546 - 0.00216 * device->temperature));

    if (float_humidity > 100.0)
    {
        device->humidity = 100.0;
    }
    else if (float_humidity < 0.0)
    {
        device->humidity = 0.0;
    }
*/

    return true;
}

unsigned char DS2438IssueTemperatureAndHumidityConvertion(tLaseredROMCode *device)
{
    //Convert Temperature
    if (OneWireReset() == 1)
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

    //Convert voltage (humidity)
    if (OneWireReset() == 1)
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
