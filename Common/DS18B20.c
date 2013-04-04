/**
 *  @file       DS18B20.c
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Library to interface with DS18B20 temperature sensor.
 */
#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include "DS18B20.h"
#include "1-Wire.h"
#include "GlobalTypeDefs.h"
#include <delays.h>


/**
 * @fn      Return_Types DS18B20_Configure(unsigned char resolution)
 * @param   resolution  Value to set the resolution of the sensor.
 * @return  Returns an ::Return_Types enum to be verified if all went well.
 */
eReturnTypes DS18B20Configure(sDevice device, unsigned char resolution)
{
    unsigned char DS18B20Data[10] = {0};

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
    One_Wire_Write_Byte(0x7D); //TH Alarm Set to 125ºC
    One_Wire_Write_Byte(0xC9); //TL Alarm Set to -55ºC
    One_Wire_Write_Byte(resolution);
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
    DS18B20Data[0] = One_Wire_Read_Byte();
    DS18B20Data[1] = One_Wire_Read_Byte();
    DS18B20Data[2] = One_Wire_Read_Byte();
    DS18B20Data[3] = One_Wire_Read_Byte();
    DS18B20Data[4] = One_Wire_Read_Byte();
    DS18B20Data[5] = One_Wire_Read_Byte();
    DS18B20Data[6] = One_Wire_Read_Byte();
    DS18B20Data[7] = One_Wire_Read_Byte();
    DS18B20Data[8] = One_Wire_Read_Byte();

    if (DS18B20Data[8] != Calculate_CRC((unsigned char *) DS18B20Data, 8))
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

    Delay_10mS();

    return SUCCESSFUL;
}


/**
 * Issue a temperature convertion to the sensor.
 * @param device
 * @return Returns enum to verify if all went well.
 */
eReturnTypes DS18B20IssueTemperatureConvertion(sDevice device)
{
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
    One_Wire_Write_Byte(CONVERT_TEMPERATURE);

    return SUCCESSFUL;
}


/**
 * Gets the value of the temperature from the configured sensor.
 * @param device Pointer to the structure containing the information of the sensor.
 * @return Result of the communication.
 */
eReturnTypes DS18B20GetTemperature(sDevice *device)
{
    int int_temp;
    float temp_float;
    unsigned char DS18B20Data[10] = {0};

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
    DS18B20Data[0] = One_Wire_Read_Byte();
    DS18B20Data[1] = One_Wire_Read_Byte();
    DS18B20Data[2] = One_Wire_Read_Byte();
    DS18B20Data[3] = One_Wire_Read_Byte();
    DS18B20Data[4] = One_Wire_Read_Byte();
    DS18B20Data[5] = One_Wire_Read_Byte();
    DS18B20Data[6] = One_Wire_Read_Byte();
    DS18B20Data[7] = One_Wire_Read_Byte();
    DS18B20Data[8] = One_Wire_Read_Byte();

    if (DS18B20Data[8] != Calculate_CRC((unsigned char *) DS18B20Data, 8))
        return CRC_NOT_MATCH;

    else
    {
        int_temp = (int)DS18B20Data[1];
        int_temp <<= 8;
        int_temp |= DS18B20Data[0];
        temp_float = (float)int_temp;
        device->temperature = temp_float * 0.0625;
        return SUCCESSFUL;
    }
}
