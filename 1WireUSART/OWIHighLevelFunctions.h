// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * \li File:               OWIHighLevelFunctions.h
 * \li Compiler:           IAR EWAAVR 3.20a
 * \li Support mail:       avr@atmel.com
 *
 * \li Supported devices:  All AVRs.
 *
 * \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
 *
 *
 * \li Description:        Header file for OWIHighLevelFunctions.c
 *
 *                         $Revision: 1.7 $
 *                         $Date: Thursday, August 19, 2004 14:27:18 UTC $
 ****************************************************************************/

#ifndef _OWI_ROM_FUNCTIONS_H_
#define _OWI_ROM_FUNCTIONS_H_


void OWI_SendByte(unsigned char data, unsigned char pins);
unsigned char OWI_ReceiveByte(unsigned char pin);
void OWI_SkipRom(unsigned char pins);
void OWI_ReadRom(unsigned char * romValue, unsigned char pins);
void OWI_MatchRom(unsigned char * romValue, unsigned char pins);
unsigned char OWI_SearchRom(unsigned char * bitPattern, unsigned char lastDeviation, unsigned char pins);

#endif
