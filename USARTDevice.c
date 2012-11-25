/**
 *  @file       USARTDevice.c
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
#include <p18cxxx.h>
#include "USARTDevice.h"

void USARTInit(void)
{
    USART_TXSTATbits.CSRC = 1;
    USART_TXSTATbits.TX9 = 0;
    USART_TXSTATbits.TXEN = 1;
    USART_TXSTATbits.SYNC = 0;
    USART_TXSTATbits.SENDB = 0;
    USART_TXSTATbits.BRGH = 1;
    USART_TXSTATbits.TRMT = 1;
    USART_TXSTATbits.TX9D = 0;

    USART_RCSTATbits.SPEN = 1;
    USART_RCSTATbits.RX9 = 0;
    USART_RCSTATbits.SREN = 0;
    USART_RCSTATbits.CREN = 1;
    USART_RCSTATbits.ADDEN = 1;
    USART_RCSTATbits.FERR = 0;
    USART_RCSTATbits.OERR = 0;
    USART_RCSTATbits.RX9D = 0;

    USART_BAUDCONbits.ABDOVF = 0;
    USART_BAUDCONbits.RCIDL = 1;
    USART_BAUDCONbits.RXDTP = 0;
    USART_BAUDCONbits.TXCKP = 1;
    USART_BAUDCONbits.BRG16 = 0;
    USART_BAUDCONbits.WUE = 0;
    USART_BAUDCONbits.ABDEN = 0;

    USARTSetBaudrate();

}

void USARTSetBaudrate(void)
{
    unsigned int baud = 0;
    unsigned char low, high;

    baud = USART_SPBRG;
    low = (unsigned char) (baud & 0x00FF);
    high = (unsigned char) (baud >> 8);
    SPBRG_LOW = low;
    SPBRG_HIGH = high;

}

/**
 * Print a character on the RS485 bus.
 * @param c Char to be printed.
 */
void USARTPutChar(char c)
{
    while (!USART_TXSTATbits.TRMT);
    TXREG1 = c;
    while (!USART_TXSTATbits.TRMT);
}

char USARTGetChar(void)
{
    if (USART_RX_FLAG == 1)
    {
        return USART_RX_REG;
    }
    else
    {
        return 0x00;
    }
}

/**
 * Sends a string stored in ram memory throught USART
 * @param string String to be sent.
 */
void USARTSendRAMString(char *string)
{
    char c;

    do
    {
        c = *string++;
        if (c != 0) USARTPutChar(c);
    }
    while (c != 0);
}

/**
 * Sends a string stored in rom memory throught USART
 * @param string String to be sent.
 */
void USARTSendROMString(const rom char *string)
{
    char c;

    do
    {
        c = *string++;
        if (c != 0) USARTPutChar(c);
    }
    while (c != 0);
}