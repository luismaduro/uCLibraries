/**
 *  @file       USARTDevice.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       24 de Novembro de 2012
 *  @brief      Source file for USART PIC18 Module.
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

#ifndef USARTDEVICE_H
#define	USARTDEVICE_H

#include <p18cxxx.h>

#define BAUDRATE                115200
#define USART_FOSC              64000000
#define USART_SPBRG             ((USART_FOSC/BAUDRATE/16)-1)
#define USART_TXSTATbits        TXSTAbits
#define USART_RCSTATbits        RCSTAbits
#define USART_BAUDCONbits       BAUDCONbits
#define SPBRG_LOW               SPBRG
#define SPBRG_HIGH              SPBRGH
#define USART_RX_REG            RCREG
#define USART_TX_REG            TXREG
#define USART_RX_FLAG           PIR1bits.RC1IF
#define USART_TX_FLAG           PIR1bits.TX1IF


void USARTInit(void);
void USARTSetBaudrate(unsigned long baudrate);
void USARTPutChar(char c);
char USARTGetChar(void);
void USARTSendRAMString(char *string);
void USARTSendROMString(const char *string);
#endif	/* USARTDEVICE_H */

