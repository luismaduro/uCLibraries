// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * \li File:               OWIUARTFunctions.c
 * \li Compiler:           IAR EWAAVR 3.20a
 * \li Support mail:       avr@atmel.com
 *
 * \li Supported devices:  All AVRs.
 *
 * \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
 *
 *
 * \li Description:        Polled UART implementation of the basic bit-level
 *                         signalling in the 1-Wire(R) protocol.
 *
 *                         $Revision: 1.7 $
 *                         $Date: Thursday, August 19, 2004 14:27:18 UTC $
 ****************************************************************************/

/*****************************************************************************
 *
 * Atmel Corporation
 *
 * File              : OWIUARTFunctions.c
 * Compiler          : IAR EWAAVR 3.20a
 * Revision          : $Revision: 1.7 $
 * Date              : $Date: Thursday, August 19, 2004 14:27:18 UTC $
 * Updated by        : $Author: tsundre $
 *
 * Support mail      : avr@atmel.com
 *
 * Supported devices : All AVRs with UART or USART module.
 *
 * AppNote           : AVR318 - 1-Wire(R) interface Master Implementation
 *
 * Description       : Polled UART implementation of the basic bit-level
 *                     signalling in the 1-Wire(R) protocol.
 *
 ****************************************************************************/

#include "../USARTDevice.h"
#include "OWIUARTBitFunctions.h"

/*! \brief Initialization of the one wire bus. (Polled UART driver)
 *  
 *  This function initializes the 1-Wire bus by configuring the UART.
 */
void OWIInit(void)
{
    USARTInit();
}

/*! \brief  Write and read one bit to/from the 1-Wire bus. (Polled UART driver)
 *
 *  Writes one bit to the bus and returns the value read from the bus.
 *
 *  \param  outValue    The value to transmit on the bus.
 *
 *  \return The value received by the UART from the bus.
 */
unsigned char OWITouchBit(unsigned char outValue)
{
    // Place the output value in the UART transmit buffer, and wait
    // until it is received by the UART receiver.
    USART_TX_REG = outValue;
    while (!USART_RX_FLAG);
    // Set the UART Baud Rate back to 115200kbps when finished.
    USARTSetBaudrate(115200);
    return USART_RX_REG;
}

/*! \brief Write a '1' bit to the bus(es). (Polled UART DRIVER)
 *
 *  Generates the waveform for transmission of a '1' bit on the 1-Wire
 *  bus.
 */
void OWIWriteBit1(void)
{
    OWITouchBit(OWI_UART_WRITE1);
}

/*! \brief  Write a '0' to the bus(es). (Polled UART DRIVER)
 *
 *  Generates the waveform for transmission of a '0' bit on the 1-Wire(R)
 *  bus.
 */
void OWIWriteBit0(void)
{
    OWITouchBit(OWI_UART_WRITE0);
}

/*! \brief  Read a bit from the bus(es). (Polled UART DRIVER)
 *
 *  Generates the waveform for reception of a bit on the 1-Wire(R) bus(es).
 *
 *  \return The value read from the bus (0 or 1).
 */
unsigned char OWIReadBit(void)
{
    // Return 1 if the value received matches the value sent.
    // Return 0 else. (A slave held the bus low).
    return (OWITouchBit(OWI_UART_READ_BIT) == OWI_UART_READ_BIT);
}

/*! \brief  Send a Reset signal and listen for Presence signal. (Polled 
 *  UART DRIVER)
 *
 *  Generates the waveform for transmission of a Reset pulse on the 
 *  1-Wire(R) bus and listens for presence signals.
 *
 *  \return A bitmask of the buses where a presence signal was detected.
 */
unsigned char OWIDetectPresence(void)
{
    // Reset UART receiver to clear RXC register.
    USART_RCSTATbits.SPEN = 0;
    USART_RCSTATbits.SPEN = 1;

    // Set UART Baud Rate to 9600 for Reset/Presence signalling.
    USARTSetBaudrate(9600);

    // Return 0 if the value received matches the value sent.
    // return 1 else. (Presence detected)
    return (OWITouchBit(OWI_UART_RESET) != OWI_UART_RESET);
}