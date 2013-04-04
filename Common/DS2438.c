/**
 *  @file       DS2438.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Library to interface with DS2438 temperature sensor.
 */
#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include "DS2438.h"
#include "1-Wire.h"

/**
 * This funtion configures the DS2438 with the specified confguration.
 * @param Config Configuration that will be sent to the DS2438. 
 * This is a Configuration type variable.
 * @return Returns an ::Return_Types enum to be verified if all went well.
 */
eReturnTypes DS2438Configure(sDevice device, Configuration Config)
{
    unsigned char DS2438Data[9] = {0};

    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device.romCode.Family_Code);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device.romCode.CRC);

    One_Wire_Write_Byte(WRITE_SCRATCHPAD);
    One_Wire_Write_Byte(DS2438_PAGE_0);
    One_Wire_Write_Byte(Config.Register);

    Delay_10mS();

    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device.romCode.Family_Code);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device.romCode.CRC);

    One_Wire_Write_Byte(READ_SCRATCHPAD);
    One_Wire_Write_Byte(DS2438_PAGE_0);
    DS2438Data[STATUS] = One_Wire_Read_Byte();
    DS2438Data[TEMP_LSB] = One_Wire_Read_Byte();
    DS2438Data[TEMP_MSB] = One_Wire_Read_Byte();
    DS2438Data[VOLT_LSB] = One_Wire_Read_Byte();
    DS2438Data[VOLT_MSB] = One_Wire_Read_Byte();
    DS2438Data[CURR_LSB] = One_Wire_Read_Byte();
    DS2438Data[CURR_MSB] = One_Wire_Read_Byte();
    DS2438Data[THRESH] = One_Wire_Read_Byte();
    DS2438Data[CRC8] = One_Wire_Read_Byte();

    if (DS2438Data[CRC8] != Calculate_CRC((unsigned char *) DS2438Data, 8))
        return CRC_NOT_MATCH;

    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device.romCode.Family_Code);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device.romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device.romCode.CRC);

    One_Wire_Write_Byte(COPY_SCRATCHPAD);
    One_Wire_Write_Byte(DS2438_PAGE_0);

    Delay_10mS();

    return SUCCESSFUL;
}

/**
 * This funtion gets the values available on the DS2438. Specifically the the
 * voltage and temperature from the sensor.
 * @param temperature Pointer to the variable that stores the temperature.
 * @param humidity Pointer to the variable that stores the humidity.
 * @return Returns an ::Return_Types enum to be verified if all went well.
 */
eReturnTypes DS2438GetTemperatureAndHumidity(sDevice *device)
{
    int int_temp = 0;
    float float_temp;
    int int_voltage = 0;
    float float_voltage;
    float float_humidity;
    unsigned char DS2438Data[9] = {0};

    //recall memory
    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device->romCode.Family_Code);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device->romCode.CRC);
    One_Wire_Write_Byte(RECALL_E_E);
    One_Wire_Write_Byte(DS2438_PAGE_0);

    //read scratchpad
    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device->romCode.Family_Code);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device->romCode.CRC);
    One_Wire_Write_Byte(READ_SCRATCHPAD);
    One_Wire_Write_Byte(DS2438_PAGE_0);
    DS2438Data[STATUS] = One_Wire_Read_Byte();
    DS2438Data[TEMP_LSB] = One_Wire_Read_Byte();
    DS2438Data[TEMP_MSB] = One_Wire_Read_Byte();
    DS2438Data[VOLT_LSB] = One_Wire_Read_Byte();
    DS2438Data[VOLT_MSB] = One_Wire_Read_Byte();
    DS2438Data[CURR_LSB] = One_Wire_Read_Byte();
    DS2438Data[CURR_MSB] = One_Wire_Read_Byte();
    DS2438Data[THRESH] = One_Wire_Read_Byte();
    DS2438Data[CRC8] = One_Wire_Read_Byte();

    if (DS2438Data[CRC8] != Calculate_CRC((unsigned char *) DS2438Data, 8))
        return CRC_NOT_MATCH;

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



    return SUCCESSFUL;
}

eReturnTypes DS2438IssueTemperatureAndHumidityConvertion(sDevice *device)
{
    //Convert Temperature
    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device->romCode.Family_Code);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device->romCode.CRC);
    One_Wire_Write_Byte(CONVERT_TEMPERATURE);

    //Convert voltage (humidity)
    if (One_Wire_Reset_Pulse() == 1)
        return SENSOR_NOT_PRESENT;

    One_Wire_Write_Byte(MATCH_ROM_COMMAND);
    One_Wire_Write_Byte(device->romCode.Family_Code);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_1);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_2);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_3);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_4);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_5);
    One_Wire_Write_Byte(device->romCode.ROM_Code_Byte_6);
    One_Wire_Write_Byte(device->romCode.CRC);
    One_Wire_Write_Byte(CONVERT_VOLTAGE);

    return SUCCESSFUL;
}