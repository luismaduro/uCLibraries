/**
 *  @file       1-Wire.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       Feb 2012
 *  @brief      Header File to 1-Wire Library.
 */

#ifndef _1_Wire_H
#define _1_Wire_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GenericTypeDefs.h>

/** Macro to return or set value of HIGH (1)*/
#define	HIGH	1
/** Macro to return or set value of LOW (0)*/
#define	LOW     0
/** Macro to define a port as output*/
#define	OUTPUT	0
/** Macro to define a port as input*/
#define	INPUT 	1 

#define ONE_WIRE_PIN_DIRECTION 	(TRISBbits.TRISB1)  /*!< One Wire pin direction register*/
#define ONE_WIRE_PIN            (LATBbits.LATB1)     /*!< One Wire pin register*/

#define DELAY_2Us       2       /*!< Value to be used on Wait() funtion for 2 microseconds delay.*/
#define DELAY_6Us	6       /*!< Value to be used on Wait() funtion for 6 microseconds delay.*/
#define DELAY_9Us	10      /*!< Value to be used on Wait() funtion for 9 microseconds delay.*/
#define DELAY_10Us	10      /*!< Value to be used on Wait() funtion for 10 microseconds delay.*/
#define DELAY_12Us	12      /*!< Value to be used on Wait() funtion for 12 microseconds delay.*/
#define DELAY_55Us	55      /*!< Value to be used on Wait() funtion for 55 microseconds delay.*/
#define DELAY_60Us	60      /*!< Value to be used on Wait() funtion for 60 microseconds delay.*/
#define DELAY_64Us	64      /*!< Value to be used on Wait() funtion for 64 microseconds delay.*/
#define DELAY_70Us	70      /*!< Value to be used on Wait() funtion for 70 microseconds delay.*/
#define DELAY_205Us	205     /*!< Value to be used on Wait() funtion for 205 microseconds delay.*/
#define DELAY_240Us	240     /*!< Value to be used on Wait() funtion for 240 microseconds delay.*/
#define DELAY_410Us	410     /*!< Value to be used on Wait() funtion for 410 microseconds delay.*/
#define DELAY_480Us	480     /*!< Value to be used on Wait() funtion for 500 microseconds delay.*/
/**
 * Enumerator that sets the TemperatureSource variable to the
 * specific source through all the available ones.
 */
typedef enum
{
    /**DS18B20 device identification.*/
    DeviceType_DS18B20,
    /**DS2438 device identification.*/
    DeviceType_DS2438

} tDeviceType;

/**
 * Return type for various funtions.
 */
typedef enum
{
    /** All gone well.*/
    SUCCESSFUL,
    /** Sensor requested does not respond.*/
    SENSOR_NOT_PRESENT,
    /** Cyclic Redundancy Check doesn't match.*/
    CRC_NOT_MATCH,
    /** Error between data conversions.*/
    CONVERTION_ERROR,
    /** Temperature not in range of the sensor hability.*/
    TEMPERATURE_NOT_IN_RANGE

} eReturnTypes;

/**
 * @struct Lasered_ROM_Code
 * 
 * @brief Structure to save the One Wire Device Rom Code.
 */
typedef struct
{
    unsigned char FamilyCode; /*!< One Wire Device Family*/
    unsigned char ROMCodeByte1; /*!< One Wire Device Rom Code Byte 1*/
    unsigned char ROMCodeByte2; /*!< One Wire Device Rom Code Byte 2*/
    unsigned char ROMCodeByte3; /*!< One Wire Device Rom Code Byte 3*/
    unsigned char ROMCodeByte4; /*!< One Wire Device Rom Code Byte 4*/
    unsigned char ROMCodeByte5; /*!< One Wire Device Rom Code Byte 5*/
    unsigned char ROMCodeByte6; /*!< One Wire Device Rom Code Byte 6*/
    unsigned char CRC; /*!< One Wire Device Rom Code CRC*/
} tLaseredROMCode; /*!< Variable type to store the devices Rom Codes.*/


unsigned char OneWireResetPulse(void);
void OneWireWriteBit(unsigned char write_data);
unsigned char OneWireReadBit(void);
void OneWireWriteByte(unsigned char write_data);
unsigned char OneWireReadByte(void);
unsigned char CalculateCRC(unsigned char *d, unsigned char len);
unsigned char DoCRC8(unsigned char value);
unsigned char OneWireFirst(void);
unsigned char OneWireNext(void);
unsigned char OneWireSearchBus(void);
unsigned char OneWireVerify(void);
void OneWireTargetSetup(unsigned char family_code);
void OneWireFamilySkipSetup(void);
void FindAllDevicesOneWire(tLaseredROMCode *romcode, unsigned char *num_devices);
void WaitUs(unsigned int uSec);

#endif