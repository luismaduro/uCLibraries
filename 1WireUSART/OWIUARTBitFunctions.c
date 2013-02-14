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

#include "OWIPolled.h"

#ifdef OWI_UART_DRIVER

#include "OWIBitFunctions.h"

/*! \brief Initialization of the one wire bus. (Polled UART driver)
 *  
 *  This function initializes the 1-Wire bus by configuring the UART.
 */
void OWI_Init()
{
    // Choose single or double UART speed.
    OWI_UART_STATCTRL_REG_A = (OWI_UART_2X << OWI_U2X);

    // Enable UART transmitter and receiver.
    OWI_UART_STATCTRL_REG_B = (1 << OWI_TXEN) | (1 << OWI_RXEN);

    // Set up asynchronous mode, 8 data bits, no parity, 1 stop bit.
    // (Initial value, can be removed)
#ifdef URSEL
    OWI_UART_STATCTRL_REG_C = (1 << OWI_URSEL) | (1 << OWI_UCSZ1) | (1 << OWI_UCSZ0);
#else
    OWI_UART_STATCTRL_REG_C = (1 << OWI_UCSZ1) | (1 << OWI_UCSZ0);
#endif

    OWI_UART_BAUD_RATE_REG_L = OWI_UBRR_115200;
}

/*! \brief  Write and read one bit to/from the 1-Wire bus. (Polled UART driver)
 *
 *  Writes one bit to the bus and returns the value read from the bus.
 *
 *  \param  outValue    The value to transmit on the bus.
 *
 *  \return The value received by the UART from the bus.
 */
unsigned char OWI_TouchBit(unsigned char outValue)
{
    // Place the output value in the UART transmit buffer, and wait
    // until it is received by the UART receiver.
    OWI_UART_DATA_REGISTER = outValue;
    while (!(OWI_UART_STATCTRL_REG_A & (1 << OWI_RXC)))
    {

    }
    // Set the UART Baud Rate back to 115200kbps when finished.
    OWI_UART_BAUD_RATE_REG_L = OWI_UBRR_115200;
    return OWI_UART_DATA_REGISTER;
}

/*! \brief Write a '1' bit to the bus(es). (Polled UART DRIVER)
 *
 *  Generates the waveform for transmission of a '1' bit on the 1-Wire
 *  bus.
 */
void OWI_WriteBit1()
{
    OWI_TouchBit(OWI_UART_WRITE1);
}

/*! \brief  Write a '0' to the bus(es). (Polled UART DRIVER)
 *
 *  Generates the waveform for transmission of a '0' bit on the 1-Wire(R)
 *  bus.
 */
void OWI_WriteBit0()
{
    OWI_TouchBit(OWI_UART_WRITE0);
}

/*! \brief  Read a bit from the bus(es). (Polled UART DRIVER)
 *
 *  Generates the waveform for reception of a bit on the 1-Wire(R) bus(es).
 *
 *  \return The value read from the bus (0 or 1).
 */
unsigned char OWI_ReadBit()
{
    // Return 1 if the value received matches the value sent.
    // Return 0 else. (A slave held the bus low).
    return (OWI_TouchBit(OWI_UART_READ_BIT) == OWI_UART_READ_BIT);
}

/*! \brief  Send a Reset signal and listen for Presence signal. (Polled 
 *  UART DRIVER)
 *
 *  Generates the waveform for transmission of a Reset pulse on the 
 *  1-Wire(R) bus and listens for presence signals.
 *
 *  \return A bitmask of the buses where a presence signal was detected.
 */
unsigned char OWI_DetectPresence()
{
    // Reset UART receiver to clear RXC register.
    OWI_UART_STATCTRL_REG_B &= ~(1 << OWI_RXEN);
    OWI_UART_STATCTRL_REG_B |= (1 << OWI_RXEN);

    // Set UART Baud Rate to 9600 for Reset/Presence signalling.
    OWI_UART_BAUD_RATE_REG_L = OWI_UBRR_9600;

    // Return 0 if the value received matches the value sent.
    // return 1 else. (Presence detected)
    return (OWI_TouchBit(OWI_UART_RESET) != OWI_UART_RESET);
}


#endif
