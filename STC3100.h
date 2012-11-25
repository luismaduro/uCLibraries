/**
 *  @file       STC3100.h
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
#ifndef STC3100_H
#define STC3100_H

#define STC3100_ADDRESS    0b01110000

//Device control registers
#define REG_MODE                0
#define REG_CTRL                1
#define REG_CHARGE_LOW          2
#define REG_CHARGE_HIGH         3
#define REG_COUNTER_LOW         4
#define REG_COUNTER_HIGH        5
#define REG_CURRENT_LOW         6
#define REG_CURRENT_HIGH        7
#define REG_VOLTAGE_LOW         8
#define REG_VOLTAGE_HIGH        9
#define REG_TEMPERATURE_LOW     10
#define REG_TEMPERATURE_HIGH    11
#define REG_ID0                 24
#define REG_ID1                 25
#define REG_ID2                 26
#define REG_ID3                 27
#define REG_ID4                 28
#define REG_ID5                 29
#define REG_ID6                 30
#define REG_ID7                 24
#define REG_RAM0                32
#define REG_RAM1                33
#define REG_RAM2                34
#define REG_RAM3                35
#define REG_RAM4                36
#define REG_RAM5                37
#define REG_RAM6                38
#define REG_RAM7                39
#define REG_RAM8                40
#define REG_RAM9                41
#define REG_RAM10               42
#define REG_RAM11               43
#define REG_RAM12               44
#define REG_RAM13               45
#define REG_RAM14               46
#define REG_RAM15               47
#define REG_RAM16               48
#define REG_RAM17               49
#define REG_RAM18               50
#define REG_RAM19               51
#define REG_RAM20               52
#define REG_RAM21               53
#define REG_RAM22               54
#define REG_RAM23               55
#define REG_RAM24               56
#define REG_RAM25               57
#define REG_RAM26               58
#define REG_RAM27               59
#define REG_RAM28               60
#define REG_RAM29               61
#define REG_RAM30               62
#define REG_RAM31               63

typedef union
{
    unsigned char ByteArray[32];

    struct
    {
        unsigned char Mode;
        unsigned char ControlStatus;
        unsigned char ChargeLow;
        unsigned char ChargeHigh;
        unsigned char CounterLow;
        unsigned char CounterHigh;
        unsigned char CurrentLow;
        unsigned char CurrentHigh;
        unsigned char VoltageLow;
        unsigned char VoltageHigh;
        unsigned char TemperatureLow;
        unsigned char TemperatureHigh;
        unsigned char Reserved[13];
        unsigned char ID0;
        unsigned char ID1;
        unsigned char ID2;
        unsigned char ID3;
        unsigned char ID4;
        unsigned char ID5;
        unsigned char ID6;
        unsigned char ID7;
    };

    struct
    {
        unsigned char ClockSource : 1;
        unsigned char GasGaugeADCResolution : 2;
        unsigned char CalibrateAD : 1;
        unsigned char StandbyOperating : 1;
        unsigned char : 3;
    } MODEbits;

    struct
    {
        unsigned char :8;
        unsigned char IO0Data : 1;
        unsigned char GaugeReset : 1;
        unsigned char CurrentConvertionStatus : 1;
        unsigned char VoltageTemperatureConvertionStatus : 1;
        unsigned char PowerOnReset : 1;
        unsigned char : 3;
    } CONTROLSTATUSbits;


} tSTC31000Data;

typedef struct
{
    float Voltage;
    float Current;
    float Temperature;
    float Charge;
} tBatteryData;

extern tSTC31000Data STC3100Data;
extern tBatteryData BatteryData;

void STC3100ReadChip(void);
void STC3100WriteChip(void);
void UpdateBatteryData(void);

#endif