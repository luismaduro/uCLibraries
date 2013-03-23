/*********************************************************************
 *
 *                  General Delay rouines
 *
 *********************************************************************
 * Author               Date    Comment
 *********************************************************************
 * Nilesh Rajbharti     5/9/02  Original        (Rev 1.0)
 * Nilesh Rajbharti     6/10/02 Fixed C18 ms and us routines
 * Howard Schlunder     4/04/06	Changed for C30
 ********************************************************************/
#ifndef __DELAY_H
#define __DELAY_H

#include <delays.h>

#define SYSTEM_CLOCK        64000000UL
#define INSTRUCTION_CLOCK   (SYSTEM_CLOCK/4)

#define Delay10us(us)   Delay10TCYx(((INSTRUCTION_CLOCK/1000000)*(us)))
#define DelayMs(ms)                                                 \
    do                                                              \
    {                                                               \
        unsigned int _iTemp = (ms);                                 \
        while(_iTemp--)                                             \
            Delay1KTCYx((INSTRUCTION_CLOCK+999999)/1000000);    \
    } while(0)
#endif
