// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * \li File:               OWISWBitFunctions.c
 * \li Compiler:           IAR EWAAVR 3.20a
 * \li Support mail:       avr@atmel.com
 *
 * \li Supported devices:  All AVRs.
 *
 * \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
 *
 *
 * \li Description:        Polled software only implementation of the basic
 *                         bit-level signalling in the 1-Wire(R) protocol.
 *
 *                         $Revision: 1.7 $
 *                         $Date: Thursday, August 19, 2004 14:27:18 UTC $
 ****************************************************************************/

#include "OWIPolled.h"

#ifdef OWI_SOFTWARE_DRIVER

#include "OWIBitFunctions.h"

/*! \brief Initialization of the one wire bus(es). (Software only driver)
 *  
 *  This function initializes the 1-Wire bus(es) by releasing it and
 *  waiting until any presence sinals are finished.
 *
 *  \param  pins    A bitmask of the buses to initialize.
 */
void OWI_Init(unsigned char pins)
{
    OWI_RELEASE_BUS(pins);
    // The first rising edge can be interpreted by a slave as the end of a
    // Reset pulse. Delay for the required reset recovery time (H) to be 
    // sure that the real reset is interpreted correctly.
    __delay_cycles(OWI_DELAY_H_STD_MODE);
}

/*! \brief  Write a '1' bit to the bus(es). (Software only driver)
 *
 *  Generates the waveform for transmission of a '1' bit on the 1-Wire
 *  bus.
 *
 *  \param  pins    A bitmask of the buses to write to.
 */
void OWI_WriteBit1(unsigned char pins)
{
    unsigned char intState;

    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();

    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_A_STD_MODE);

    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_B_STD_MODE);

    // Restore interrupts.
    __restore_interrupt(intState);
}

/*! \brief  Write a '0' to the bus(es). (Software only driver)
 *
 *  Generates the waveform for transmission of a '0' bit on the 1-Wire(R)
 *  bus.
 *
 *  \param  pins    A bitmask of the buses to write to.
 */
void OWI_WriteBit0(unsigned char pins)
{
    unsigned char intState;

    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();

    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_C_STD_MODE);

    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_D_STD_MODE);

    // Restore interrupts.
    __restore_interrupt(intState);
}

/*! \brief  Read a bit from the bus(es). (Software only driver)
 *
 *  Generates the waveform for reception of a bit on the 1-Wire(R) bus(es).
 *
 *  \param  pins    A bitmask of the bus(es) to read from.
 *
 *  \return A bitmask of the buses where a '1' was read.
 */
unsigned char OWI_ReadBit(unsigned char pins)
{
    unsigned char intState;
    unsigned char bitsRead;

    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();

    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_A_STD_MODE);

    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_E_STD_MODE);

    // Sample bus and delay.
    bitsRead = OWI_PIN & pins;
    __delay_cycles(OWI_DELAY_F_STD_MODE);

    // Restore interrupts.
    __restore_interrupt(intState);

    return bitsRead;
}

/*! \brief  Send a Reset signal and listen for Presence signal. (software
 *  only driver)
 *
 *  Generates the waveform for transmission of a Reset pulse on the 
 *  1-Wire(R) bus and listens for presence signals.
 *
 *  \param  pins    A bitmask of the buses to send the Reset signal on.
 *
 *  \return A bitmask of the buses where a presence signal was detected.
 */
unsigned char OWI_DetectPresence(unsigned char pins)
{
    unsigned char intState;
    unsigned char presenceDetected;

    // Disable interrupts.
    intState = __save_interrupt();
    __disable_interrupt();

    // Drive bus low and delay.
    OWI_PULL_BUS_LOW(pins);
    __delay_cycles(OWI_DELAY_H_STD_MODE);

    // Release bus and delay.
    OWI_RELEASE_BUS(pins);
    __delay_cycles(OWI_DELAY_I_STD_MODE);

    // Sample bus to detect presence signal and delay.
    presenceDetected = ((~OWI_PIN) & pins);
    __delay_cycles(OWI_DELAY_J_STD_MODE);

    // Restore interrupts.
    __restore_interrupt(intState);

    return presenceDetected;
}


#endif
