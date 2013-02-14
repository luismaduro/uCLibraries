// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * \li File:               OWIDeviceSpecific.h
 * \li Compiler:           IAR EWAAVR 3.20a
 * \li Support mail:       avr@atmel.com
 *
 * \li Supported devices:  All AVRs with UART or USART module.
 *
 * \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
 *
 *
 * \li Description:        Device specific defines that expands to correct
 *                         register and bit definition names for the selected
 *                         device.
 *
 *                         $Revision: 1.7 $
 *                         $Date: Thursday, August 19, 2004 14:27:16 UTC $
 ****************************************************************************/

#ifndef _OWI_DEVICE_SPECIFIC_H_
#define _OWI_DEVICE_SPECIFIC_H_


#if defined(__AT90Tiny2313__) | defined(__ATtiny2313__)

#define OWI_UART_STATCTRL_REG_A     UCSRA
#define OWI_UART_STATCTRL_REG_B     UCSRB
#define OWI_UART_STATCTRL_REG_C     UCSRC
#define OWI_UART_DATA_REGISTER      UDR
#define OWI_UART_BAUD_RATE_REG_L    UBRRL
#define OWI_UART_BAUD_RATE_REG_H    UBRRH

#define OWI_U2X                     U2X
#define OWI_RXEN                    RXEN
#define OWI_TXEN                    TXEN
#define OWI_RXCIE                   RXCIE
#define OWI_UCSZ1                   UCSZ1
#define OWI_UCSZ0                   UCSZ0
#define OWI_UDRIE                   UDRIE
#define OWI_FE                      FE
#define OWI_RXC                     RXC

#define OWI_UART_RXC_VECT           USART0_RX_vect
#define OWI_UART_UDRE_VECT          USART0_UDRE_vect

#endif


#if defined(__AT90Mega8__) | defined(__ATmega8__) | \
    defined(__AT90Mega16__) | defined(__ATmega16__) | \
    defined(__AT90Mega32__) | defined(__ATmega32__) 

#define OWI_UART_STATCTRL_REG_A     UCSRA
#define OWI_UART_STATCTRL_REG_B     UCSRB
#define OWI_UART_STATCTRL_REG_C     UCSRC
#define OWI_UART_DATA_REGISTER      UDR
#define OWI_UART_BAUD_RATE_REG_L    UBRRL
#define OWI_UART_BAUD_RATE_REG_H    UBRRH

#define OWI_U2X                     U2X
#define OWI_RXEN                    RXEN
#define OWI_TXEN                    TXEN
#define OWI_RXCIE                   RXCIE
#define OWI_UCSZ1                   UCSZ1
#define OWI_UCSZ0                   UCSZ0
#define OWI_UDRIE                   UDRIE
#define OWI_FE                      FE
#define OWI_URSEL                   URSEL
#define OWI_RXC                     RXC

#define OWI_UART_RXC_VECT           USART_RXC_vect
#define OWI_UART_UDRE_VECT          USART_UDRE_vect

#endif


#if defined(__AT90Mega48__) | defined(__ATmega48__) | \
    defined(__AT90Mega88__) | defined(__ATmega88__) | \
    defined(__AT90Mega168__) | defined(__ATmega168__)

#define OWI_UART_STATCTRL_REG_A     UCSR0A
#define OWI_UART_STATCTRL_REG_B     UCSR0B
#define OWI_UART_STATCTRL_REG_C     UCSR0C
#define OWI_UART_DATA_REGISTER      UDR0
#define OWI_UART_BAUD_RATE_REG_L    UBRR0L
#define OWI_UART_BAUD_RATE_REG_H    UBRR0H

#define OWI_U2X                     U2X0
#define OWI_RXEN                    RXEN0
#define OWI_TXEN                    TXEN0
#define OWI_RXCIE                   RXCIE0
#define OWI_UCSZ1                   UCSZ01
#define OWI_UCSZ0                   UCSZ00
#define OWI_UDRIE                   UDRIE0
#define OWI_FE                      FE0
#define OWI_RXC                     RXC0

#define OWI_UART_RXC_VECT           USART_RX_vect
#define OWI_UART_UDRE_VECT          USART_UDRE_vect

#endif


#if defined(__AT90Mega169__) | defined(__ATmega169__)

#define OWI_UART_STATCTRL_REG_A     UCSR0A
#define OWI_UART_STATCTRL_REG_B     UCSR0B
#define OWI_UART_STATCTRL_REG_C     UCSR0C
#define OWI_UART_DATA_REGISTER      UDR0
#define OWI_UART_BAUD_RATE_REG_L    UBRR0L
#define OWI_UART_BAUD_RATE_REG_H    UBRR0H

#define OWI_U2X                     U2X0
#define OWI_RXEN                    RXEN0
#define OWI_TXEN                    TXEN0
#define OWI_RXCIE                   RXCIE0
#define OWI_UCSZ1                   UCSZ01
#define OWI_UCSZ0                   UCSZ00
#define OWI_UDRIE                   UDRIE0
#define OWI_FE                      FE0
#define OWI_RXC                     RXC0

#define OWI_UART_RXC_VECT           USART0_RXC_vect
#define OWI_UART_UDRE_VECT          USART0_UDRE_vect

#endif


#if defined(__AT90Mega64__) | defined(__ATmega64__) | \
    defined(__AT90Mega128__) | defined(__ATmega128__)

#define OWI_UART_STATCTRL_REG_A     UCSR0A
#define OWI_UART_STATCTRL_REG_B     UCSR0B
#define OWI_UART_STATCTRL_REG_C     UCSR0C
#define OWI_UART_DATA_REGISTER      UDR0
#define OWI_UART_BAUD_RATE_REG_L    UBRR0L
#define OWI_UART_BAUD_RATE_REG_H    UBRR0H

#define OWI_U2X                     U2X0
#define OWI_RXEN                    RXEN0
#define OWI_TXEN                    TXEN0
#define OWI_RXCIE                   RXCIE0
#define OWI_UCSZ1                   UCSZ01
#define OWI_UCSZ0                   UCSZ00
#define OWI_UDRIE                   UDRIE0
#define OWI_FE                      FE0
#define OWI_RXC                     RXC0

#define OWI_UART_RXC_VECT           USART0_RXC_vect
#define OWI_UART_UDRE_VECT          USART0_UDRE_vect

#endif



#endif
