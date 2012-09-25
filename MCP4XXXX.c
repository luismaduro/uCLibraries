/**
 *  @file      MCP4XXXX.c
 *  @brief     Library to interface with the MCP4XXXX digital potenciometers.
 *  @author    Luis Maduro
 *  @version   1.0
 *  @date      September 2012
 *
 *
 * 1.0 - Initial Release - Luis Maduro
 * 
 */

#include <xc.h>
#include "MCP4XXXX.h"

void SPIWrite(unsigned char data);
unsigned char SPIRead(void);

/**
 * Hardware dependent funtion to write to SPI module. The module must be
 * configured by the user.
 * @param data Byte to be sent by the SPI module.
 */
void SPIWrite(unsigned char data)
{
    unsigned char TempVar;

    TempVar = SSP1BUF; // Clears BF

    PIR1bits.SSP1IF = 0; // Clear interrupt flag

    SSP1CON1bits.WCOL = 0; //Clear any previous write collision

    SSP1BUF = data; // write byte to SSP1BUF register

    if (SSP1CON1 & 0x80) // test if write collision occurred
        return; // if WCOL bit is set return negative #
    else
        while (!PIR1bits.SSP1IF); // wait until bus cycle complete

    return; // if WCOL bit is not set return non-negative#
}

/**
 * Hardware dependent funtion to read from SPI module. The module must be
 * configured by the user.
 * @return Byte read from the SPI bus.
 */
unsigned char SPIRead(void)
{
    unsigned char TempVar;

    TempVar = SSP1BUF; //Clear BF

    PIR1bits.SSP1IF = 0; //Clear interrupt flag

    SSP1BUF = 0x00; // initiate bus cycle

    while (!PIR1bits.SSP1IF); //wait until cycle complete

    return (SSP1BUF); // return with byte read
}

/**
 * Sends an shutdown command to the Potenciometer 0 of the MCP4XXXX chip.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP4XXXXShutdownPotenciometer0(volatile unsigned char *port,
                                    unsigned char portBit)
{
    unsigned char dummy = ~(0x01 << portBit);
    *port &= dummy;

    SPIWrite(MCP4XXXX_SHUTDOWN_POT_0);
    SPIWrite(0x00);

    dummy = ~dummy;
    *port |= dummy;
}

/**
 * Sends an shutdown command to the Potenciometer 1 of the MCP4XXXX chip. Not
 * valid for MCP41XXX devices.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP4XXXXShutdownPotenciometer1(volatile unsigned char *port,
                                    unsigned char portBit)
{
    unsigned char dummy = ~(0x01 << portBit);
    *port &= dummy;

    SPIWrite(MCP4XXXX_SHUTDOWN_POT_1);
    SPIWrite(0x00);

    dummy = ~dummy;
    *port |= dummy;
}

/**
 * Sends an shutdown command to all potenciometers of the MCP4XXXX chip.
 *
 * @param port      PORT address of the CS pin.
 *                  E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit   PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1, etc.
 */
void MCP4XXXXShutdownBothPotenciometers(volatile unsigned char *port,
                                        unsigned char portBit)
{
    unsigned char dummy = ~(0x01 << portBit);
    *port &= dummy;

    SPIWrite(MCP4XXXX_SHUTDOWN_BOTH_POTS);
    SPIWrite(0x00);

    dummy = ~dummy;
    *port |= dummy;
}

/**
 * Sends the byte with the value to be write to the potenciometer 0 of the
 * MCP4XXXX chip.
 *
 * @param port          PORT address of the CS pin.
 *                      E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit       PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1,
 *                      etc.
 *
 * @param resistance    Byte value to be sent to the potenciometer.
 */
void MCP4XXXXSetPotenciometer0Value(volatile unsigned char *port,
                                    unsigned char portBit, 
                                    unsigned char resistance)
{
    unsigned char dummy = ~(0x01 << portBit);
    *port &= dummy;

    SPIWrite(MCP4XXXX_WRITE_DATA_POT_0);
    SPIWrite(resistance);

    dummy = ~dummy;
    *port |= dummy;
}

/**
 * Sends the byte with the value to be write to the potenciometer 1 of the
 * MCP4XXXX chip. Not valid for MCP41XXX devices.
 *
 * @param port          PORT address of the CS pin.
 *                      E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit       PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1,
 *                      etc.
 *
 * @param resistance    Byte value to be sent to the potenciometer.
 */
void MCP4XXXXSetPotenciometer1Value(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance)
{
    unsigned char dummy = ~(0x01 << portBit);
    *port &= dummy;

    SPIWrite(MCP4XXXX_WRITE_DATA_POT_1);
    SPIWrite(resistance);

    dummy = ~dummy;
    *port |= dummy;
}

/**
 * Sends the byte with the value to be write to all potenciometers on the
 * MCP4XXXX chip.
 *
 * @param port          PORT address of the CS pin.
 *                      E.g. &PORTA to PORTA, &PORTB to PORTB, etc.
 *
 * @param portBit       PORT bit value of the CS pin. E.g. 0 to RB0, 1 to RB1,
 *                      etc.
 *
 * @param resistance    Byte value to be sent to the potenciometer.
 */
void MCP4XXXXSetPotenciometersValue(volatile unsigned char *port,
                                    unsigned char portBit,
                                    unsigned char resistance)
{
    unsigned char dummy = ~(0x01 << portBit);
    *port &= dummy;

    SPIWrite(MCP4XXXX_WRITE_DATA_BOTH_POTS);
    SPIWrite(resistance);

    dummy = ~dummy;
    *port |= dummy;
}