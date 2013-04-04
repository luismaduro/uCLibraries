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

/** Maximum number of devices on the system.*/
#define MAX_SENSORS 3

/** Macro to return or set value of HIGH (1)*/
#define	HIGH	1
/** Macro to return or set value of LOW (0)*/
#define	LOW     0
/** Macro to define a port as output*/
#define	OUTPUT	0
/** Macro to define a port as input*/
#define	INPUT 	1 

#define ONE_WIRE_PIN_DIRECTION 	(TRISBbits.TRISB1)  /*!< One Wire pin direction register*/
#define ONE_WIRE_PIN            (PORTBbits.RB1)     /*!< One Wire pin register*/

#define DELAY_2Us       2       /*!< Value to be used on Wait() funtion for 2 microseconds delay.*/
#define DELAY_6Us	6       /*!< Value to be used on Wait() funtion for 6 microseconds delay.*/
#define DELAY_10Us	10      /*!< Value to be used on Wait() funtion for 10 microseconds delay.*/
#define DELAY_12Us	12      /*!< Value to be used on Wait() funtion for 12 microseconds delay.*/
#define DELAY_55Us	55      /*!< Value to be used on Wait() funtion for 55 microseconds delay.*/
#define DELAY_60Us	60      /*!< Value to be used on Wait() funtion for 60 microseconds delay.*/
#define DELAY_64Us	64      /*!< Value to be used on Wait() funtion for 64 microseconds delay.*/
#define DELAY_70Us	70      /*!< Value to be used on Wait() funtion for 70 microseconds delay.*/
#define DELAY_205Us	205     /*!< Value to be used on Wait() funtion for 205 microseconds delay.*/
#define DELAY_240Us	240     /*!< Value to be used on Wait() funtion for 240 microseconds delay.*/

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

#define CONFIGURATION_SIZE 22

/**
 * Structure that store every possible elements of each device.
 */
typedef union
{
    struct
    {
        /**Flag to state if the sensor is used or not.*/
        bool used;                      //1
        /**Device type.*/
        tDeviceType deviceType;         //1
        /**Device rom code.*/
        tLaseredROMCode romCode;        //8
        /**Address of the device on the system. [A...Z]*/
        char addressTemperature;        //1
        /**Address of the device on the system. [A...Z]*/
        char addressHumidity;           //1
        /**Variable to store the temperature if the device has it.*/
        float temperature;              //4
        /**Variable to store the humidity if the device has it.*/
        float humidity;                 //4
        /**Flag to see if the last reading is valid.*/
        bool readingValid;              //1
        /**Keep tracking of the number of errors in the sensor.*/
        unsigned char numberOfErrors;   //1
    }Data;

    struct
    {
        /**Bytes of the content for easier manipulation.*/
        unsigned char number[CONFIGURATION_SIZE];
    } Byte;
} sDevice;

/** List of the sensor on the system.*/
extern sDevice sensorDevicesList[MAX_SENSORS];
extern sDevice *sensorDevices;
/** Device indentification for the old command respose.*/
extern sDevice sourceForOldCommand;

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
void WaitUs(unsigned char uSec);

#endif