/* 
 * File:   newmain.c
 * Author: Luis Maduro
 *
 * Created on 11 de Fevereiro de 2013, 0:11
 */

#pragma config FOSC = HS
#pragma config PLLEN = ON
#pragma config PCLKEN = ON
#pragma config FCMEN = ON
#pragma config IESO = ON
#pragma config PWRTEN = ON
#pragma config BOREN = OFF
#pragma config BORV = 19
#pragma config WDTEN = OFF
#pragma config WDTPS = 32768
#pragma config HFOFST = OFF
#pragma config MCLRE = ON
#pragma config STVREN = ON
#pragma config LVP = ON
#pragma config BBSIZ = OFF
#pragma config XINST = OFF
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF
#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRTC = OFF
#pragma config WRTB = OFF
#pragma config WRTD = OFF
#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
#pragma config EBTRB = OFF

#include<xc.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */
int main(int argc, char** argv)
{

    return (EXIT_SUCCESS);
}

