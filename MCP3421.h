/**
 *  @file       MCP3421.h
 *  @brief      Header file library for 18-Bit ADC MCP3421
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       9 de Outubro de 2012, 15:10
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

#ifndef MCP3421_H
#define	MCP3421_H

/**MCP3421 device address*/
#define MCP3421_DEVICE_ADDRESS      0b01101000

/**Position of the Ready Bit*/
#define MCP3421_READY_BIT               (1 << 7)
/**Position of the Convertion Mode Bit*/
#define MCP3421_CONVERTION_MODE         (1 << 4)
/**Position of the Sample Rate Bits*/
#define MCP3421_SAMPLE_RATE             (3 << 2)
/**Position of the Gain Selection Bits*/
#define MCP3421_GAIN_SELECTION          (3 << 0)

/**Sets the ADC to 240 samples per second (12 bits of resolution)*/
#define MCP3421_SAMPLE_RATE_240_SPS     0b00
/**Sets the ADC to 60 samples per second (14 bits of resolution)*/
#define MCP3421_SAMPLE_RATE_60_SPS      0b01
/**Sets the ADC to 15 samples per second (16 bits of resolution)*/
#define MCP3421_SAMPLE_RATE_15_SPS      0b10
/**Sets the ADC to 3.75 samples per second (18 bits of resolution)*/
#define MCP3421_SAMPLE_RATE_3_75_SPS    0b11

/**Sets the gain of the PGA to 1*/
#define MCP3421_GAIN_1                  0b00
/**Sets the gain of the PGA to 2*/
#define MCP3421_GAIN_2                  0b01
/**Sets the gain of the PGA to 4*/
#define MCP3421_GAIN_4                  0b10
/**Sets the gain of the PGA to 8*/
#define MCP3421_GAIN_8                  0b11

/**Indentifies if the convertion is completed.*/
#define MCP3421_CONVERTION_READY        0
/**Indentifies if the convertion is in action.*/
#define MCP3421_CONVERTION_NOT_READY    1
/**Value to initiate a new convertion on the ReadyBit if One-Shot mode is selected.*/
#define MCP3421_INITIATE_CONVERTION     1
/**Sets the ADC to continuous convertion mode.*/
#define MCP3421_MODE_CONTINUOUS         1
/**Sets the ADC to One-Shot mode, convertion are asked by the user.*/
#define MCP3421_MODE_ONE_SHOT           0
/**Identifies that output register is updated with the lastest convertion.*/
#define MCP3421_VALUE_IS_UPDATED        0
/**Identifies that output register is not updated with the lastest convertion.*/
#define MCP3421_VALUE_IS_NOT_UPDATED    1

/**Value of the LSB for 12 bits of resolution.*/
#define MCP3421_LSB_12BITS              (1E-3)
/**Value of the LSB for 14 bits of resolution.*/
#define MCP3421_LSB_14BITS              (250E-6)
/**Value of the LSB for 16 bits of resolution.*/
#define MCP3421_LSB_16BITS              (62.5E-6)
/**Value of the LSB for 18 bits of resolution.*/
#define MCP3421_LSB_18BITS              (15.625E-6)

/**Decimal absolute value of the gain selected for formula use, this case 1*/
#define MCP3421_GAIN_1_VALUE            1
/**Decimal absolute value of the gain selected for formula use, this case 2*/
#define MCP3421_GAIN_2_VALUE            2
/**Decimal absolute value of the gain selected for formula use, this case 4*/
#define MCP3421_GAIN_4_VALUE            4
/**Decimal absolute value of the gain selected for formula use, this case 8*/
#define MCP3421_GAIN_8_VALUE            8

/**Structure of the internal configuration register.*/
typedef union
{
    /**Byte of the present configuration*/
    unsigned char byte;

    struct
    {
        /**PGA Gain Selection Bits*/
        unsigned char PGAGainSelectionBits : 2;
        /**Sample Rate Selection Bit*/
        unsigned char SampleRateSelectionBits : 2;
        /**Conversion Mode Bit*/
        unsigned char ConvertionModeBit : 1;
        unsigned char : 2;
        /**Ready Bit*/
        unsigned char ReadyBit : 1;
    };
    /**MCP3421 Configuration Register Format*/
} MCP3421ConfigurationRegister;

/**Union to simplify the convertion of the values. From the bytes retrieved by
 the MCP3421 a signed short long is updated (24 bits)*/
typedef union
{
    /**Value retrieved from the ADC signed.*/
    signed short long value;

    struct
    {
        /**Lower data byte from the ADC*/
        unsigned char lower;
        /**Middle data byte from the ADC*/
        unsigned char middle;
        /**Upper data byte  from the ADC*/
        unsigned char upper;
    };
    /**MCP3421 Data Format*/
} MCP3421Data;

void MCP3421InitiateConvertion(void);
void MCP3421SetConvertionModeContinuous(void);
void MCP3421SetConvertionModeOneShot(void);
void MCP3421SetConvertionRate(unsigned char value);
void MCP3421SetPGAGain(unsigned char value);
float MCP3421GetValue(void);

#endif

