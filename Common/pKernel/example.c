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
#include "pKernel.h"

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

    pKernelAddTask(&tDescriptorTask1, 50, Task1);
    pKernelAddTask(&tDescriptorTask2, 100, Task2);
    pKernelAddTask(&tDescriptorTask3, 200, Task3);
    pKernelAddTask(&tDescriptorTask4, 50, Task4);
    pKernelAddTask(&tDescriptorTask5, 100, Task5);
    pKernelAddTask(&tDescriptorTask6, 200, Task6);
    pKernelScheduler();
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
    pKernelSuspendTask(&tDescriptorTask1);
    pKernelResumeTask(&tDescriptorTask4, 50);
}

void Task2(void)
{
    LATCbits.LATC1 = 1;
    pKernelSuspendTask(&tDescriptorTask2);
    pKernelResumeTask(&tDescriptorTask5, 100);
}

void Task3(void)
{
    LATCbits.LATC2 = 1;
    pKernelSuspendTask(&tDescriptorTask3);
    pKernelResumeTask(&tDescriptorTask6, 200);
}

void Task4(void)
{
    LATCbits.LATC0 = 0;
    pKernelSuspendTask(&tDescriptorTask4);
    pKernelResumeTask(&tDescriptorTask1, 50);
}

void Task5(void)
{
    LATCbits.LATC1 = 0;
    pKernelSuspendTask(&tDescriptorTask5);
    pKernelResumeTask(&tDescriptorTask2, 100);
}

void Task6(void)
{
    LATCbits.LATC2 = 0;
    pKernelSuspendTask(&tDescriptorTask6);
    pKernelResumeTask(&tDescriptorTask3, 200);
}
