/**
 *  @file       DS18B20.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Library to interface with DS18B20 temperature sensor.
 */
#include "DS18B20.h"


/**
 * @fn      Return_Types DS18B20_Configure(unsigned char resolution)
 * @param   resolution  Value to set the resolution of the sensor.
 * @return  Returns an ::Return_Types enum to be verified if all went well.
 */
eReturnTypes DS18B20Configure(tLaseredROMCode device, unsigned char resolution)
{
    unsigned char DS18B20Data[10] = {0};

    if (OneWireResetPulse() == 1)
        return SENSOR_NOT_PRESENT;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.CRC);
    OneWireWriteByte(WRITE_SCRATCHPAD);
    OneWireWriteByte(0x7D); //TH Alarm Set to 125ºC
    OneWireWriteByte(0xC9); //TL Alarm Set to -55ºC
    OneWireWriteByte(resolution);
    DelayMS(10);

    if (OneWireResetPulse() == 1)
        return SENSOR_NOT_PRESENT;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.CRC);
    OneWireWriteByte(READ_SCRATCHPAD);
    DS18B20Data[0] = OneWireReadByte();
    DS18B20Data[1] = OneWireReadByte();
    DS18B20Data[2] = OneWireReadByte();
    DS18B20Data[3] = OneWireReadByte();
    DS18B20Data[4] = OneWireReadByte();
    DS18B20Data[5] = OneWireReadByte();
    DS18B20Data[6] = OneWireReadByte();
    DS18B20Data[7] = OneWireReadByte();
    DS18B20Data[8] = OneWireReadByte();

    if (DS18B20Data[8] != CalculateCRC((unsigned char *) DS18B20Data, 8))
        return CRC_NOT_MATCH;

    if (OneWireResetPulse() == 1)
        return SENSOR_NOT_PRESENT;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.CRC);
    OneWireWriteByte(COPY_SCRATCHPAD);

    DelayMS(10);

    return SUCCESSFUL;
}


/**
 * Issue a temperature convertion to the sensor.
 * @param device
 * @return Returns enum to verify if all went well.
 */
eReturnTypes DS18B20IssueTemperatureConvertion(tLaseredROMCode device)
{
    if (OneWireResetPulse() == 1)
        return SENSOR_NOT_PRESENT;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.CRC);
    OneWireWriteByte(CONVERT_TEMPERATURE);

    return SUCCESSFUL;
}


/**
 * Gets the value of the temperature from the configured sensor.
 * @param device Pointer to the structure containing the information of the sensor.
 * @return Result of the communication.
 */
eReturnTypes DS18B20GetTemperature(tLaseredROMCode device, float *temperature)
{
    int int_temp;
    float temp_float;
    unsigned char DS18B20Data[10] = {0};

    if (OneWireResetPulse() == 1)
        return SENSOR_NOT_PRESENT;

    OneWireWriteByte(MATCH_ROM_COMMAND);
    OneWireWriteByte(device.FamilyCode);
    OneWireWriteByte(device.ROMCodeByte1);
    OneWireWriteByte(device.ROMCodeByte2);
    OneWireWriteByte(device.ROMCodeByte3);
    OneWireWriteByte(device.ROMCodeByte4);
    OneWireWriteByte(device.ROMCodeByte5);
    OneWireWriteByte(device.ROMCodeByte6);
    OneWireWriteByte(device.CRC);
    OneWireWriteByte(READ_SCRATCHPAD);
    DS18B20Data[0] = OneWireReadByte();
    DS18B20Data[1] = OneWireReadByte();
    DS18B20Data[2] = OneWireReadByte();
    DS18B20Data[3] = OneWireReadByte();
    DS18B20Data[4] = OneWireReadByte();
    DS18B20Data[5] = OneWireReadByte();
    DS18B20Data[6] = OneWireReadByte();
    DS18B20Data[7] = OneWireReadByte();
    DS18B20Data[8] = OneWireReadByte();

    if (DS18B20Data[8] != CalculateCRC((unsigned char *) DS18B20Data, 8))
        return CRC_NOT_MATCH;

    else
    {
        int_temp = (int)DS18B20Data[1];
        int_temp <<= 8;
        int_temp |= DS18B20Data[0];
        temp_float = (float)int_temp;
        *temperature = temp_float * 0.0625;
        return SUCCESSFUL;
    }
}
