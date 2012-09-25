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

#include <p18cxxx.h>
#include "MCP4XXXX.h"

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
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_SHUTDOWN_POT_0);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
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
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_SHUTDOWN_POT_1);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
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
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_SHUTDOWN_BOTH_POTS);
    SPIWrite(0x00);

    *port |= (0x01 << portBit);
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
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_WRITE_DATA_POT_0);
    SPIWrite(resistance);

    *port |= (0x01 << portBit);
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
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_WRITE_DATA_POT_1);
    SPIWrite(resistance);

    *port |= (0x01 << portBit);
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
    *port &= ~(0x01 << portBit);

    SPIWrite(MCP4XXXX_WRITE_DATA_BOTH_POTS);
    SPIWrite(resistance);

    *port |= (0x01 << portBit);
}