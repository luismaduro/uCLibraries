/**
 *  @file       STC3100.c
 *  @brief      Fuel Gauge Library
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       25/11/2012
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

#include <p18cxxx.h>
#include "I2CDevice.h"
#include "STC3100.h"

tSTC31000Data STC3100Data;
tBatteryData BatteryData;

void STC3100ReadChip(void)
{
    I2CDeviceSetDeviceAddress(STC3100_ADDRESS);
    I2CDeviceReadBytes(REG_MODE, 32, &(STC3100Data.ByteArray[0]));
}

void STC3100WriteChip(void)
{
    I2CDeviceSetDeviceAddress(STC3100_ADDRESS);
    I2CDeviceWriteBytes(REG_MODE, 32, &(STC3100Data.ByteArray[0]));
}

void UpdateBatteryData(void)
{
    unsigned int high_byte = 0;
    int value = 0;

    STC3100ReadChip();

    high_byte = (unsigned int) STC3100Data.VoltageHigh;
    high_byte <<= 8;
    value = (high_byte & 0xFF00) | STC3100Data.VoltageLow;
    BatteryData.Voltage = (float) value * 2.44;

    high_byte = (unsigned int) STC3100Data.CurrentHigh;
    high_byte <<= 8;
    value = (high_byte & 0xFF00) | STC3100Data.CurrentLow;
    value <<= 2;
    BatteryData.Current = ((((float) value * 11.77) / 10.0) / 4.0) > 0.0 ?
            ((((float) value * 11.77) / 10.0) / 4.0) :
            -((((float) value * 11.77) / 10.0) / 4.0);
    BatteryData.Current -= 54.0;

    high_byte = (unsigned int) STC3100Data.TemperatureHigh;
    high_byte <<= 8;
    value = (high_byte & 0xFF00) | STC3100Data.TemperatureLow;
    value <<= 4;
    BatteryData.Temperature = ((float) value * 0.125) / 16.0;

    high_byte = (unsigned int) STC3100Data.ChargeHigh;
    high_byte <<= 8;
    value = (high_byte & 0xFF00) | STC3100Data.ChargeLow;
    BatteryData.Charge = ((float) value * 6.70) / 10.0;

    Nop();
}
