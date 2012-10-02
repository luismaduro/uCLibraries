/* 
 * File:   AD5241.h
 * Author: Luis Maduro
 *
 * Created on 25 de Setembro de 2012, 19:24
 */

#ifndef AD5241_H
#define	AD5241_H

#include "I2CDevice.h"

#ifdef	__cplusplus
extern "C"
{
#endif

#define AD5241_DEVICE_ADDRESS   0b00101100


void AD524XSetRDAC1Value(unsigned char value);
void AD524XSetRDAC2Value(unsigned char value);
void AD524XSetOutput1(void);
void AD524XSetOutput2(void);
void AD524XClearOutput1(void);
void AD524XClearOutput2(void);
void AD524XSetRDAC1Midscale(void);
void AD524XSetRDAC2Midscale(void);

#ifdef	__cplusplus
}
#endif

#endif	/* AD5241_H */

