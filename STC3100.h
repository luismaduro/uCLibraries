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

/**I2C Device Address*/
#define STC3100_ADDRESS    0b01110000

//INTERNAL REGISTERS MAP (NOT CURRENTLY USED)
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
    /**Used for simpler readings of the chip.*/
    unsigned char ByteArray[64];

    struct
    {
        /**Mode register*/
        unsigned char Mode;
        /**Control and status register*/
        unsigned char ControlStatus;
        /**Gas gauge charge data, bits 0-7*/
        unsigned char ChargeLow;
        /**Gas gauge charge data, bits 8-15*/
        unsigned char ChargeHigh;
        /**Number of conversions, bits 0-7*/
        unsigned char CounterLow;
        /**Number of conversions, bits 8-15*/
        unsigned char CounterHigh;
        /**Battery current value, bits 0-7*/
        unsigned char CurrentLow;
        /**Battery current value, bits 8-15*/
        unsigned char CurrentHigh;
        /**Battery voltage value, bits 0-7*/
        unsigned char VoltageLow;
        /**Battery voltage value, bits 8-15*/
        unsigned char VoltageHigh;
        /**Temperature value, bits 0-7*/
        unsigned char TemperatureLow;
        /**Temperature value, bits 8-15*/
        unsigned char TemperatureHigh;
        /**Not implemented*/
        unsigned char Reserved[13];
        /**Part type ID = 10h*/
        unsigned char ID0;
        /**Unique part ID, bits 0-7*/
        unsigned char ID1;
        /**Unique part ID, bits 8-15*/
        unsigned char ID2;
        /**Unique part ID, bits 16-23*/
        unsigned char ID3;
        /**Unique part ID, bits 24-31*/
        unsigned char ID4;
        /**Unique part ID, bits 32-39*/
        unsigned char ID5;
        /**Unique part ID, bits 40-47*/
        unsigned char ID6;
        /**Device ID CRC*/
        unsigned char ID7;
        /**General-purpose RAM register 0*/
        unsigned char RAM0;
        /**General-purpose RAM register 1*/
        unsigned char RAM1;
        /**General-purpose RAM register 2*/
        unsigned char RAM2;
        /**General-purpose RAM register 3*/
        unsigned char RAM3;
        /**General-purpose RAM register 4*/
        unsigned char RAM4;
        /**General-purpose RAM register 5*/
        unsigned char RAM5;
        /**General-purpose RAM register 6*/
        unsigned char RAM6;
        /**General-purpose RAM register 7*/
        unsigned char RAM7;
        /**General-purpose RAM register 8*/
        unsigned char RAM8;
        /**General-purpose RAM register 9*/
        unsigned char RAM9;
        /**General-purpose RAM register 10*/
        unsigned char RAM10;
        /**General-purpose RAM register 11*/
        unsigned char RAM11;
        /**General-purpose RAM register 12*/
        unsigned char RAM12;
        /**General-purpose RAM register 13*/
        unsigned char RAM13;
        /**General-purpose RAM register 14*/
        unsigned char RAM14;
        /**General-purpose RAM register 15*/
        unsigned char RAM15;
        /**General-purpose RAM register 16*/
        unsigned char RAM16;
        /**General-purpose RAM register 17*/
        unsigned char RAM17;
        /**General-purpose RAM register 18*/
        unsigned char RAM18;
        /**General-purpose RAM register 19*/
        unsigned char RAM19;
        /**General-purpose RAM register 20*/
        unsigned char RAM20;
        /**General-purpose RAM register 21*/
        unsigned char RAM21;
        /**General-purpose RAM register 22*/
        unsigned char RAM22;
        /**General-purpose RAM register 23*/
        unsigned char RAM23;
        /**General-purpose RAM register 24*/
        unsigned char RAM24;
        /**General-purpose RAM register 25*/
        unsigned char RAM25;
        /**General-purpose RAM register 26*/
        unsigned char RAM26;
        /**General-purpose RAM register 27*/
        unsigned char RAM27;
        /**General-purpose RAM register 28*/
        unsigned char RAM28;
        /**General-purpose RAM register 29*/
        unsigned char RAM29;
        /**General-purpose RAM register 30*/
        unsigned char RAM30;
        /**General-purpose RAM register 31*/
        unsigned char RAM31;
    };

    struct
    {
        /**32,768 Hz clock source: 0: auto-detect, 1: external clock*/
        unsigned char ClockSource : 1;
        /**Gas gauge ADC resolution: 00:14 bits, 01:13 bits, 10:12 bits*/
        unsigned char GasGaugeADCResolution : 2;
        /**ADC Calibration: 0: no effect, 1: used to calibrate the AD converters*/
        unsigned char CalibrateAD : 1;
        /**
         * 0: standby mode. Accumulator and counter registers are frozen, gas
         * gauge and battery monitor functions are in standby.
         * 1: operating mode.
         */
        unsigned char StandbyOperating : 1;
        /**Unused*/
        unsigned char : 3;
    } MODEbits;

    struct
    {
        /**Unused*/
        unsigned char :8;
        /**
         * Port IO0 data status:
         * 0 = IO0 input is low
         * 1 = IO0 input is high
         * 
         * Port IO0 data output drive:
         * 0 = IO0 output is driven low
         * 1 = IO0 output is open
         */
        unsigned char IO0Data : 1;
        /**
         * Gas Gauge Reset
         * 0: no effect
         * 1: resets the charge accumulator and conversion counter
         * GG_RST is a self-clearing bit.
         */
        unsigned char GaugeReset : 1;
        /**
         * Set at the end of a battery current conversion cycle.
         * Clears upon reading.
         */
        unsigned char CurrentConvertionStatus : 1;
        /**
         * Set at the end of a battery voltage or temperature conversion cycle.
         * Clears upon reading.
         */
        unsigned char VoltageTemperatureConvertionStatus : 1;
        /**
         * Power on reset (POR) detection bit:
         * 0 = no POR event occurred,
         * 1 = POR event occurred
         *
         * Soft reset:
         * 0 = release the soft-reset and clear the POR detection bit,
         * 1 = assert the soft-reset and set the POR detection bit
         */
        unsigned char PowerOnReset : 1;
        /**Unused*/
        unsigned char : 3;
    } CONTROLSTATUSbits;


} tSTC31000Data;

typedef struct
{
    /**Value of the voltage in mV.*/
    float Voltage;
    /**Value of the current in mA*/
    float Current;
    /**Value of the temperature in ºC*/
    float Temperature;
    /**Value of the current charge of the battery in mAh*/
    float Charge;
} tBatteryData;

extern tSTC31000Data STC3100Data;
extern tBatteryData BatteryData;

void STC3100ReadChip(void);
void STC3100WriteChip(void);
void UpdateBatteryData(void);

#endif