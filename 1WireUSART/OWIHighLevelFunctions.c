// This file has been prepared for Doxygen automatic documentation generation.
/*! \file ********************************************************************
 *
 * Atmel Corporation
 *
 * \li File:               OWIHighLevelFunctions.c
 * \li Compiler:           IAR EWAAVR 3.20a
 * \li Support mail:       avr@atmel.com
 *
 * \li Supported devices:  All AVRs.
 *
 * \li Application Note:   AVR318 - Dallas 1-Wire(R) master.
 *
 *
 * \li Description:        High level functions for transmission of full bytes
 *                         on the 1-Wire(R) bus and implementations of ROM
 *                         commands.
 *
 *                         $Revision: 1.7 $
 *                         $Date: Thursday, August 19, 2004 14:27:18 UTC $
 ****************************************************************************/

#include "OWIUARTBitFunctions.h"
#include "OWIHighLevelFunctions.h"


/*! \brief  Sends one byte of data on the 1-Wire(R) bus(es).
 *  
 *  This function automates the task of sending a complete byte
 *  of data on the 1-Wire bus(es).
 *
 *  \param  data    The data to send on the bus(es).
 *  
 *  \param  pins    A bitmask of the buses to send the data to.
 */
void OWISendByte(unsigned char data)
{
    unsigned char temp;
    unsigned char i;

    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Determine if lsb is '0' or '1' and transmit corresponding
        // waveform on the bus.
        temp = data & 0x01;
        if (temp)
        {
            OWIWriteBit1();
        }
        else
        {
            OWIWriteBit0();
        }
        // Right shift the data to get next bit.
        data >>= 1;
    }
}

/*! \brief  Receives one byte of data from the 1-Wire(R) bus.
 *
 *  This function automates the task of receiving a complete byte 
 *  of data from the 1-Wire bus.
 *
 *  \param  pin     A bitmask of the bus to read from.
 *  
 *  \return     The byte read from the bus.
 */
unsigned char OWIReceiveByte(void)
{
    unsigned char data;
    unsigned char i;

    // Clear the temporary input variable.
    data = 0x00;

    // Do once for each bit
    for (i = 0; i < 8; i++)
    {
        // Shift temporary input variable right.
        data >>= 1;
        // Set the msb if a '1' value is read from the bus.
        // Leave as it is ('0') else.
        if (OWIReadBit())
        {
            // Set msb
            data |= 0x80;
        }
    }
    return data;
}

/*! \brief  Sends the SKIP ROM command to the 1-Wire bus(es).
 *
 *  \param  pins    A bitmask of the buses to send the SKIP ROM command to.
 */
void OWISkipRom(void)
{
    // Send the SKIP ROM command on the bus.
    OWISendByte(OWI_ROM_SKIP);
}

/*! \brief  Sends the READ ROM command and reads back the ROM id.
 *
 *  \param  romValue    A pointer where the id will be placed.
 *
 *  \param  pin     A bitmask of the bus to read from.
 */
void OWIReadROM(unsigned char *romValue)
{
    unsigned char bytesLeft = 8;

    // Send the READ ROM command on the bus.
    OWISendByte(OWI_ROM_READ);

    // Do 8 times.
    while (bytesLeft > 0)
    {
        // Place the received data in memory.
        *romValue++ = OWIReceiveByte();
        bytesLeft--;
    }
}

/*! \brief  Sends the MATCH ROM command and the ROM id to match against.
 *
 *  \param  romValue    A pointer to the ID to match against.
 *
 *  \param  pins    A bitmask of the buses to perform the MATCH ROM command on.
 */
void OWIMatchROM(unsigned char *romValue)
{
    unsigned char bytesLeft = 8;

    // Send the MATCH ROM command.
    OWISendByte(OWI_ROM_MATCH);

    // Do once for each byte.
    while (bytesLeft > 0)
    {
        // Transmit 1 byte of the ID to match.
        OWISendByte(*romValue++);
        bytesLeft--;
    }
}

/*! \brief  Sends the SEARCH ROM command and returns 1 id found on the 
 *          1-Wire(R) bus.
 *
 *  \param  bitPattern      A pointer to an 8 byte char array where the 
 *                          discovered identifier will be placed. When 
 *                          searching for several slaves, a copy of the 
 *                          last found identifier should be supplied in 
 *                          the array, or the search will fail.
 *
 *  \param  lastDeviation   The bit position where the algorithm made a 
 *                          choice the last time it was run. This argument 
 *                          should be 0 when a search is initiated. Supplying 
 *                          the return argument of this function when calling 
 *                          repeatedly will go through the complete slave 
 *                          search.
 *
 *  \param  pin             A bit-mask of the bus to perform a ROM search on.
 *
 *  \return The last bit position where there was a discrepancy between slave 
 *          addresses the last time this function was run. Returns
 *          OWI_ROM_SEARCH_FAILED if an error was detected (e.g. a device was
 *          connected to the bus during the search), or OWI_ROM_SEARCH_FINISHED
 *          when there are no more devices to be discovered.
 *
 *  \note   See main.c for an example of how to utilize this function.
 */
unsigned char OWISearchROM(unsigned char * bitPattern, unsigned char lastDeviation)
{
    unsigned char currentBit = 1;
    unsigned char newDeviation = 0;
    unsigned char bitMask = 0x01;
    unsigned char bitA;
    unsigned char bitB;

    // Send SEARCH ROM command on the bus.
    OWISendByte(OWI_ROM_SEARCH);

    // Walk through all 64 bits.
    while (currentBit <= 64)
    {
        // Read bit from bus twice.
        bitA = OWIReadBit();
        bitB = OWIReadBit();

        if (bitA && bitB)
        {
            // Both bits 1 (Error).
            newDeviation = OWI_ROM_SEARCH_FAILED;
            return;
        }
        else if (bitA ^ bitB)
        {
            // Bits A and B are different. All devices have the same bit here.
            // Set the bit in bitPattern to this value.
            if (bitA)
            {
                (*bitPattern) |= bitMask;
            }
            else
            {
                (*bitPattern) &= ~bitMask;
            }
        }
        else // Both bits 0
        {
            // If this is where a choice was made the last time,
            // a '1' bit is selected this time.
            if (currentBit == lastDeviation)
            {
                (*bitPattern) |= bitMask;
            }
                // For the rest of the id, '0' bits are selected when
                // discrepancies occur.
            else if (currentBit > lastDeviation)
            {
                (*bitPattern) &= ~bitMask;
                newDeviation = currentBit;
            }
                // If current bit in bit pattern = 0, then this is
                // out new deviation.
            else if (!(*bitPattern & bitMask))
            {
                newDeviation = currentBit;
            }
                // IF the bit is already 1, do nothing.
            else
            {

            }
        }


        // Send the selected bit to the bus.
        if ((*bitPattern) & bitMask)
        {
            OWIWriteBit1();
        }
        else
        {
            OWIWriteBit0();
        }

        // Increment current bit.    
        currentBit++;

        // Adjust bitMask and bitPattern pointer.    
        bitMask <<= 1;
        if (!bitMask)
        {
            bitMask = 0x01;
            bitPattern++;
        }
    }
    return newDeviation;
}

