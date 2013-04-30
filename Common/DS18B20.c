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
 * @return  True or false if configuration suceded.
 */
bool DS18B20Configure(tLaseredROMCode *device, DS18B20Resolution resolution)
{
    unsigned char Data[10] = {0};

    if (OneWireReset() == 0)
        return false;

    OneWireSelect(device);
    OneWireWrite(WRITE_SCRATCHPAD);
    OneWireWrite(0x7D); //TH Alarm Set to 125ºC
    OneWireWrite(0xC9); //TL Alarm Set to -55ºC
    OneWireWrite(resolution);

    if (OneWireReset() == 0)
        return false;

    OneWireSelect(device);
    OneWireWrite(READ_SCRATCHPAD);
    Data[0] = OneWireRead();
    Data[1] = OneWireRead();
    Data[2] = OneWireRead();
    Data[3] = OneWireRead();
    Data[4] = OneWireRead();
    Data[5] = OneWireRead();
    Data[6] = OneWireRead();
    Data[7] = OneWireRead();
    Data[8] = OneWireRead();

    if (Data[8] != OneWireCRC8(&Data[0], 8))
        return false;

    if (OneWireReset() == 0)
        return false;

    OneWireSelect(device);
    OneWireWrite(COPY_SCRATCHPAD);

    return true;
}

/**
 * Issue a temperature convertion to the sensor.
 * @param device
 * @return Returns if all went well.
 */
bool DS18B20IssueTemperatureConvertion(tLaseredROMCode *device)
{
    if (OneWireReset() == 0)
        return false;

    OneWireSelect(device);
    OneWireWrite(CONVERT_TEMPERATURE);

    return true;
}

/**
 * Gets the value of the temperature from the configured sensor.
 * @param device Pointer to the structure containing the information of the sensor.
 * @return Result of the communication.
 */
bool DS18B20GetTemperature(tLaseredROMCode *device, float *temperature)
{
    int int_temp;
    float temp_float;
    unsigned char Data[10] = {0};

    if (OneWireReset() == 0)
        return false;

    OneWireSelect(device);
    OneWireWrite(READ_SCRATCHPAD);
    Data[0] = OneWireRead();
    Data[1] = OneWireRead();
    Data[2] = OneWireRead();
    Data[3] = OneWireRead();
    Data[4] = OneWireRead();
    Data[5] = OneWireRead();
    Data[6] = OneWireRead();
    Data[7] = OneWireRead();
    Data[8] = OneWireRead();

    if (Data[8] != OneWireCRC8(&Data[0], 8))
    {
        return false;
    }
    else
    {
        int_temp = (int) Data[1];
        int_temp <<= 8;
        int_temp |= Data[0];
        temp_float = (float) int_temp;
        temp_float *= 0.0625;
        *temperature = temp_float;
        return true;
    }
}
