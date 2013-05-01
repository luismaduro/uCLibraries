#ifndef OneWire_h
#define OneWire_h

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

/** One Wire pin direction register*/
#define ONEWIRE_PIN_DIRECTION 	(TRISBbits.TRISB1)
/** One Wire pin register*/
#define ONEWIRE_PIN_WRITE       (LATBbits.LATB1)
#define ONEWIRE_PIN_READ        (PORTBbits.RB1)

#define InterruptsOFF()         (INTCONbits.GIEH = 0,INTCONbits.GIEL = 0)
#define InterruptsON()          (INTCONbits.GIEH = 1,INTCONbits.GIEL = 1)

// You can exclude certain features from OneWire.  In theory, this
// might save some space.  In practice, the compiler automatically
// removes unused code (technically, the linker, using -fdata-sections
// and -ffunction-sections when compiling, and Wl,--gc-sections
// when linking), so most of these will not result in any code size
// reduction.  Well, unless you try to use the missing features
// and redesign your program to not need them!  ONEWIRE_CRC8_TABLE
// is the exception, because it selects a fast but large algorithm
// or a small but slow algorithm.

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

// Select the table-lookup method of computing the 8-bit CRC
// by setting this to 1.  The lookup table enlarges code size by
// about 250 bytes.  It does NOT consume RAM (but did in very
// old versions of OneWire).  If you disable this, a slower
// but very compact algorithm is used.
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 0
#endif

// You can allow 16-bit CRC checks by defining this to 1
// (Note that ONEWIRE_CRC must also be 1.)
#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 0
#endif

#define FALSE 0
#define TRUE  1

/** Macro to return or set value of HIGH (1)*/
#define	HIGH	1
/** Macro to return or set value of LOW (0)*/
#define	LOW     0
/** Macro to define a port as output*/
#define	OUTPUT	0
/** Macro to define a port as input*/
#define	INPUT 	1

/**
 * Copy Scratchpad Command for the DS18B20.
 */
#define COPY_SCRATCHPAD         0x48

/**
 * Write Scratchpad Command for the DS18B20.
 */
#define WRITE_SCRATCHPAD        0x4E

/**
 * Read Scratchpad Command for the DS18B20.
 */
#define READ_SCRATCHPAD         0xBE

/**
 * Recall EEPROM to Scatchpad Command for the DS18B20.
 */
#define RECALL_E_E              0xB8

#if ONEWIRE_SEARCH
// global search state
volatile unsigned char ROM_NO[8];
volatile unsigned char LastDiscrepancy;
volatile unsigned char LastFamilyDiscrepancy;
volatile unsigned char LastDeviceFlag;
#endif

/**
 * @struct Lasered_ROM_Code
 *
 * @brief Structure to save the One Wire Device Rom Code.
 */
typedef union
{
    unsigned char Array[8];

    struct
    {
        unsigned char FamilyCode; /*!< One Wire Device Family*/
        unsigned char ROMCodeByte1; /*!< One Wire Device Rom Code Byte 1*/
        unsigned char ROMCodeByte2; /*!< One Wire Device Rom Code Byte 2*/
        unsigned char ROMCodeByte3; /*!< One Wire Device Rom Code Byte 3*/
        unsigned char ROMCodeByte4; /*!< One Wire Device Rom Code Byte 4*/
        unsigned char ROMCodeByte5; /*!< One Wire Device Rom Code Byte 5*/
        unsigned char ROMCodeByte6; /*!< One Wire Device Rom Code Byte 6*/
        unsigned char OWICRC; /*!< One Wire Device Rom Code CRC*/
    }; /*!< Variable type to store the devices Rom Codes.*/
} tLaseredROMCode;

void OneWireInit(void);

// Perform a 1-Wire reset cycle. Returns 1 if a device responds
// with a presence pulse.  Returns 0 if there is no device or the
// bus is shorted or otherwise held low for more than 250uS
unsigned char OneWireReset(void);

// Issue a 1-Wire rom select command, you do the reset first.
void OneWireSelect(tLaseredROMCode *device);

// Issue a 1-Wire rom skip command, to address all on bus.
void OneWireSkip(void);

// Write a byte. If 'power' is one then the wire is held high at
// the end for parasitically powered devices. You are responsible
// for eventually depowering it by calling depower() or doing
// another read or write.
void OneWireWrite(unsigned char v);

void OneWireWriteBytes(unsigned char *buf, unsigned int count);

// Read a byte.
unsigned char OneWireRead(void);

void OneWireReadBytes(unsigned char *buf, unsigned int count);

// Write a bit. The bus is always left powered at the end, see
// note in write() about that.
void OneWireWriteBit(unsigned char v);

// Read a bit.
unsigned char OneWireReadBit(void);

#if ONEWIRE_SEARCH
// Clear the search state so that if will start from the beginning again.
void OneWireResetSearch(void);

// Setup the search to find the device type 'family_code' on the next call
// to search(*newAddr) if it is present.
void OneWireTargetSearch(unsigned char family_code);

// Look for the next device. Returns 1 if a new address has been
// returned. A zero might mean that the bus is shorted, there are
// no devices, or you have already retrieved all of them.  It
// might be a good idea to check the CRC to make sure you didn't
// get garbage.  The order is deterministic. You will always get
// the same devices in the same order.
unsigned char OneWireSearch(void);
void OneWireFindAllDevicesOnBus(tLaseredROMCode *romcode, unsigned char *num_devices);

#endif

#if ONEWIRE_CRC
// Compute a Dallas Semiconductor 8 bit CRC, these are used in the
// ROM and scratchpad registers.
unsigned char OneWireCRC8(unsigned char *addr, unsigned char len);

#if ONEWIRE_CRC16
// Compute the 1-Wire CRC16 and compare it against the received CRC.
// Example usage (reading a DS2408):
//    // Put everything in a buffer so we can compute the CRC easily.
//    unsigned char buf[13];
//    buf[0] = 0xF0;    // Read PIO Registers
//    buf[1] = 0x88;    // LSB address
//    buf[2] = 0x00;    // MSB address
//    WriteBytes(net, buf, 3);    // Write 3 cmd bytes
//    ReadBytes(net, buf+3, 10);  // Read 6 data bytes, 2 0xFF, 2 CRC16
//    if (!CheckCRC16(buf, 11, &buf[11])) {
//        // Handle error.
//    }     
//          
// @param input - Array of bytes to checksum.
// @param len - How many bytes to use.
// @param inverted_crc - The two CRC16 bytes in the received data.
//                       This should just point into the received data,
//                       *not* at a 16-bit integer.
// @param crc - The crc starting value (optional)
// @return True, iff the CRC matches.
bool OneWireCheckCRC16(const unsigned char* input, unsigned int len, const unsigned char* inverted_crc, unsigned int crc);

// Compute a Dallas Semiconductor 16 bit CRC.  This is required to check
// the integrity of data received from many 1-Wire devices.  Note that the
// CRC computed here is *not* what you'll get from the 1-Wire network,
// for two reasons:
//   1) The CRC is transmitted bitwise inverted.
//   2) Depending on the endian-ness of your processor, the binary
//      representation of the two-byte return value may have a different
//      byte order than the two bytes you get from 1-Wire.
// @param input - Array of bytes to checksum.
// @param len - How many bytes to use.
// @param crc - The crc starting value (optional)
// @return The CRC16, as defined by Dallas Semiconductor.
unsigned int OneWireCRC16(const unsigned char* input, unsigned int len, unsigned int crc);
#endif
#endif
void delayMicroseconds(unsigned int uSec);
#endif
