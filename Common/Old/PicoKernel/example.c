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

#include <p18cxxx.h>
#include <stdio.h>
#include <stdlib.h>
#include "PIC18PicoKernel/kernel.h"

TaskDescriptor tDescriptorTask1 = {0u, 0u, NULL, NULL};
TaskDescriptor tDescriptorTask2 = {0u, 0u, NULL, NULL};
TaskDescriptor tDescriptorTask3 = {0u, 0u, NULL, NULL};
TaskDescriptor tDescriptorTask4 = {0u, 0u, NULL, NULL};
TaskDescriptor tDescriptorTask5 = {0u, 0u, NULL, NULL};
TaskDescriptor tDescriptorTask6 = {0u, 0u, NULL, NULL};

void SystemInit(void);

void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);
void Task5(void);
void Task6(void);

void interrupt HighIRQ(void)
{
    if (INTCONbits.TMR0IF == 1 && INTCONbits.TMR0IE == 1)
    {
        INTCONbits.TMR0IF = 0;
        TMR0H = 0xE0;
        TMR0L = 0xBE;
        usTickCount++; // Timer for the scheduler
    }
}

void interrupt low_priority LowIRQ(void)
{

}

void main(void)
{
    SystemInit();

    AddTask(&tDescriptorTask1, 50, Task1);
    AddTask(&tDescriptorTask2, 100, Task2);
    AddTask(&tDescriptorTask3, 200, Task3);
    AddTask(&tDescriptorTask4, 50, Task4);
    AddTask(&tDescriptorTask5, 100, Task5);
    AddTask(&tDescriptorTask6, 200, Task6);
    Scheduler();
}

void SystemInit(void)
{
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    TRISA = 0xFF;
    TRISB = 0b01111111;
    TRISC = 0b11111000;

    //Interrupts
    INTCON2bits.RABPU = 1;
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;

    //Pull-ups and interrupts on change
    WPUA = 0x00;
    IOCA = 0x00;
    WPUB = 0x00;
    IOCB = 0x00;

    //Analog pins select
    ANSEL = 0x00;
    ANSELH = 0x00;

    //Port slew rate
    SLRCON = 0x00;

    //Timer 0 - 1 ms @ 64MHz
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0;
    T0CONbits.T0SE = 0;
    T0CONbits.PSA = 0;
    T0CONbits.T0PS = 0b000;

    TMR0H = 0xE0;
    TMR0L = 0xBE;

    INTCONbits.TMR0IE = 1;
    INTCON2bits.TMR0IP = 1;
    INTCONbits.TMR0IF = 0;
}

void Task1(void)
{
    LATCbits.LATC0 = 1;
    SuspendTask(&tDescriptorTask1);
    ResumeTask(&tDescriptorTask4, 50);
}

void Task2(void)
{
    LATCbits.LATC1 = 1;
    SuspendTask(&tDescriptorTask2);
    ResumeTask(&tDescriptorTask5, 100);
}

void Task3(void)
{
    LATCbits.LATC2 = 1;
    SuspendTask(&tDescriptorTask3);
    ResumeTask(&tDescriptorTask6, 200);
}

void Task4(void)
{
    LATCbits.LATC0 = 0;
    SuspendTask(&tDescriptorTask4);
    ResumeTask(&tDescriptorTask1, 50);
}

void Task5(void)
{
    LATCbits.LATC1 = 0;
    SuspendTask(&tDescriptorTask5);
    ResumeTask(&tDescriptorTask2, 100);
}

void Task6(void)
{
    LATCbits.LATC2 = 0;
    SuspendTask(&tDescriptorTask6);
    ResumeTask(&tDescriptorTask3, 200);
}
