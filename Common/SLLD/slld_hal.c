/* slld_hal_example.c - SLLD Hardware Abstraction Layer example Code */

/**************************************************************************
 * Copyright 2011 Spansion LLC. All Rights Reserved. 
 *
 * This software is owned and published by: 
 * Spansion LLC, 915 DeGuigne Drive, Sunnyvale, CA 94088 ("Spansion").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software constitutes driver source code for use in programming Spansion's 
 * Flash memory components. This software is licensed by Spansion to be adapted only 
 * for use in systems utilizing Spansion's Flash memories. Spansion is not be 
 * responsible for misuse or illegal use of this software for devices not 
 * supported herein.  Spansion is providing this source code "AS IS" and will 
 * not be responsible for issues arising from incorrect user implementation 
 * of the source code herein.  
 *
 * Spansion MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE, 
 * REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED 
 * USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY, 
 * FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  Spansion WILL 
 * HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR 
 * OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE, 
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS, 
 * EVEN IF Spansion HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  
 *
 * This software may be replicated in part or whole for the licensed use, 
 * with the restriction that this Copyright notice must be included with 
 * this software, whether used in part or whole, at all times.  
 */

#include <stdio.h>
#include "slld_targetspecific.h"
#include "slld.h"
#include "slld_hal.h"

// ***************************************************************************
//  FLASH_READ - HAL read function
//
//  input : device_num            device number to which operation will be done
//          command               write a single command byte to flash
//          sys_addr              system address to be used
//          data_buffer           Pointer to the data buffer where to store the read data
//          Number_Of_Read_Bytes  number of bytes to be read
//
//  return value : status of the operation - FAIL or SUCCESS
// ***************************************************************************

SLLD_STATUS FLASH_READ
(
 BYTE command, /* write a single command byte to flash */
 ADDRESS sys_addr, /* system address to be used */
 BYTE *data_buffer, /* Pointer to the data buffer containing data to be written */
 int Number_Of_Read_Bytes /* number of bytes to be read */
 )
{
    SLLD_STATUS status = SLLD_OK;
    int data_cycle, Number_Of_Dummy_Bytes = 0;

    FlashMemorySelect();

    // Write the command
    SPIWrite(command);

    // Write the address
    if (sys_addr != ADDRESS_NOT_USED)
    {
        switch (command)
        {
        case SPI_DUALIO_HPRD_CMD:
        case SPI_DUALIO_HPRD_4B_CMD:
        {
            break;
        }
        case SPI_QUADIO_HPRD_CMD:
        case SPI_QUADIO_HPRD_4B_CMD:
        {
            break;
        }
        default:
        {
            SPIWrite((BYTE) ((sys_addr >> 16) & 0x000000FF));
            SPIWrite((BYTE) ((sys_addr >> 8) & 0x000000FF));
            SPIWrite((BYTE) (sys_addr & 0x000000FF));
            break;
        }
        }
    }

    // Write the dummy bytes
    switch (command)
    {
    case SPI_FAST_READ_CMD:
    case SPI_FAST_READ_4B_CMD:
    case SPI_DUALIO_RD_CMD:
    case SPI_DUALIO_RD_4B_CMD:
    case SPI_QUADIO_RD_CMD:
    case SPI_QUADIO_RD_4B_CMD:
    case SPI_OTPR_CMD:
    {
        Number_Of_Dummy_Bytes = 1;
        // Write a dummy byte to the data bus
        for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
            SPIWrite(0x00);
        break;
    }
    case SPI_DUALIO_HPRD_CMD:
    case SPI_DUALIO_HPRD_4B_CMD:
    {
        Number_Of_Dummy_Bytes = 1;
        // Write a dummy byte to the data bus - This is actually the mode bit
        for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
            SPIWrite(0x00);
        break;
    }
    case SPI_QUADIO_HPRD_CMD:
    case SPI_QUADIO_HPRD_4B_CMD:
    {
        Number_Of_Dummy_Bytes = 3;
        // Write the dummy bytes to the data bus - The first byte is actually the mode bit
        for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
            SPIWrite(0x00);
        break;
    }
    case SPI_RES_CMD:
    {
        Number_Of_Dummy_Bytes = 3;
        // Write the dummy bytes to the data bus
        for (data_cycle = 0; data_cycle < Number_Of_Dummy_Bytes; data_cycle++)
            SPIWrite(0x00);
        break;
    }
    default:
    {
        Number_Of_Dummy_Bytes = 0;
        break;
    }
    }

    // Read the data
    if (Number_Of_Read_Bytes != 0)
    {
        switch (command)
        {
        case SPI_DUALIO_RD_CMD:
        case SPI_DUALIO_RD_4B_CMD:
        case SPI_DUALIO_HPRD_CMD:
        case SPI_DUALIO_HPRD_4B_CMD:
        {
            // Read the data using the relevant mode
            for (data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
                *(data_buffer + data_cycle) = SPIRead();
            break;
        }
        case SPI_QUADIO_RD_CMD:
        case SPI_QUADIO_RD_4B_CMD:
        case SPI_QUADIO_HPRD_CMD:
        case SPI_QUADIO_HPRD_4B_CMD:
        {
            // Read the data using the relevant mode
            for (data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
                *(data_buffer + data_cycle) = SPIRead();
            break;
        }
        default:
        {
            // Read the data using the relevant mode
            for (data_cycle = 0; data_cycle < Number_Of_Read_Bytes; data_cycle++)
                *(data_buffer + data_cycle) = SPIRead();
            break;
        }
        }
    }

    FlashMemoryDeselect();

    return (status);
}


// ***************************************************************************
//  FLASH_WRITE - HAL write function
//
//  input : device_num               device number to which operation will be done
//          command                  write a single command byte to flash
//          sys_addr                 system address to be used
//          data_buffer              Pointer to the data buffer where to store the written data
//          Number_Of_Written_Bytes  number of bytes to be written
//
//  return value : status of the operation - FAIL or SUCCESS
// ***************************************************************************

SLLD_STATUS FLASH_WRITE
(
 BYTE command, /* write a single command byte to flash */
 ADDRESS sys_addr, /* system address to be used */
 BYTE *data_buffer, /* Pointer to the data buffer containing data to be written */
 int Number_Of_Written_Bytes /* number of bytes to be written */
 )
{
    SLLD_STATUS status = SLLD_OK;
    int data_cycle;

    // Select the device
    FlashMemorySelect();

    // Write the command
    SPIWrite(command);

    // Write the address
    if (sys_addr != ADDRESS_NOT_USED)
    {
        SPIWrite((BYTE) ((sys_addr >> 16) & 0x000000FF));
        SPIWrite((BYTE) ((sys_addr >> 8) & 0x000000FF));
        SPIWrite((BYTE) (sys_addr & 0x000000FF));
    }

    // Write the data
    if (Number_Of_Written_Bytes != 0)
    {
        switch (command)
        {
        case SPI_QPP_CMD:
        case SPI_QPP_4B_CMD:
        {
            // Write the data using the relevant mode
            for (data_cycle = 0; data_cycle < Number_Of_Written_Bytes; data_cycle++)
                SPIWrite(*(data_buffer + data_cycle));
            break;
        }
        default:
        {
            // Write the data using the relevant mode
            for (data_cycle = 0; data_cycle < Number_Of_Written_Bytes; data_cycle++)
                SPIWrite(*(data_buffer + data_cycle));
            break;
        }
        }
    }

    // Deselect the device
    FlashMemoryDeselect();

    return (status);
}


/*****************************************************************************/
