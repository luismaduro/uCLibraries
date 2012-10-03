/**
 *  @file       AD524X.h
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       4 de Outubro de 2012, 0:39
 *  @brief      Source file for AD524X digital potenciometer.
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

