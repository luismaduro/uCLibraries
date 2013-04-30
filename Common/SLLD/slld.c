/* slld.c - Source Code for Spansion SPI Flash's Low Level Driver */

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


#include "slld.h"
#include "slld_hal.h"

#ifdef TRACE
#include "trace.h"
#endif

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
/* static variable to store SPI flash software protect status */
static DEV_SOFTWARE_PROTECT_STATUS sys_software_protect_status = FLASH_SOFTWARE_UNPROTECTED;
#endif

BYTE modebit_char;

#ifdef FLK_CMD

/******************************************************************************
 * slld_READ_SECURITY_Cmd - Read Security Register
 * This function issues the Read Security Register command to SPI Flash.
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadSecurityCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *read_buf, // data buffer
 BYTECOUNT len_in_bytes // number of bytes
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_READ(SPI_READ_SECURITY_CMD, sys_addr, read_buf, len_in_bytes);
    return (status);
}

SLLD_STATUS slld_ProgramSecurityCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *program_buf, // data buffer
 BYTECOUNT len_in_bytes // number of bytes
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PROGRAM_SECURITY_CMD, sys_addr, program_buf, len_in_bytes);
    return (status);
}

SLLD_STATUS slld_EraseSecurityCmd
(
 ADDRESS sys_addr // device address given by system
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_ERASE_SECURITY_CMD, sys_addr, BUFFER_NOT_USED, 0);
    return (status);
}

SLLD_STATUS slld_ProgramSecurityOp
(
 ADDRESS sys_addr, // device address given by system
 BYTE *program_buf, // data buffer
 BYTECOUNT len_in_bytes, // number of bytes
 DEVSTATUS *dev_status_ptr // variable to store device status
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_ProgramSecurityCmd(sys_addr, program_buf, len_in_bytes);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); // just in case SEOp is operated on a protected area

    return (status);
}

SLLD_STATUS slld_EraseSecurityOp
(
 ADDRESS sys_addr, // device address given by system
 DEVSTATUS *dev_status_ptr // variable to store device status
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_EraseSecurityCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); // just in case SEOp is operated on a protected area

    return (status);
}

/******************************************************************************
 * slld_ReadIdDualCmd - Read ID from SPI Flash
 * This function issues the Read ID Dual command to SPI Flash and reads out the ID.
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadIdDualCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *read_buf, // variable in which to store read data
 BYTE modebit, // mode bit
 BYTECOUNT len_in_bytes // length in bytes
 )
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS tmp_addr;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif
    if (sys_addr < 0x2)
        tmp_addr = sys_addr;
    else
        tmp_addr = (ADDRESS) 0;
    modebit_char = modebit;
    status = FLASH_RD(SPI_READID_DUAL_CMD, tmp_addr, read_buf, len_in_bytes);
    return (status);
}

/******************************************************************************
 * slld_ReadIdQuadCmd - Read ID from SPI Flash
 * This function issues the Read ID Quad command to SPI Flash and reads out the ID.
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadIdQuadCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *read_buf, // variable in which to store read data
 BYTE modebit, // mode bit
 BYTECOUNT len_in_bytes // length in bytes
 )
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS tmp_addr;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif
    if (sys_addr < 0x2)
        tmp_addr = sys_addr;
    else
        tmp_addr = (ADDRESS) 0;
    modebit_char = modebit;
    status = FLASH_RD(SPI_READID_QUAD_CMD, tmp_addr, read_buf, len_in_bytes);
    return (status);
}

/******************************************************************************
 * slld_BE32KB_Cmd - Block Erase 32KB
 * This function issues the Block Erase 32KB command to SPI Flash.
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_BE32KBCmd
(
 ADDRESS sys_addr // device address given by system
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_BE32KB_CMD, sys_addr, BUFFER_NOT_USED, 0);
    return (status);
}

SLLD_STATUS slld_BE32KBOp
(
 ADDRESS sys_addr, // device address given by system
 DEVSTATUS *dev_status_ptr // variable to store device status
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_BE32KBCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); // just in case SEOp is operated on a protected area

    return (status);
}

/******************************************************************************
 * slld_WordReadQuadCmd - Word Read Quad
 * This function issues the WordReadQuad command to SPI Flash and reads the word
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_WordReadQuadCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *read_buf, // variable in which to store read data
 BYTE modebit, // mode bit
 BYTECOUNT len_in_bytes // length in bytes
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(SPI_WORDREADQUAD_CMD, sys_addr, read_buf, len_in_bytes);

    return (status);
}

/******************************************************************************
 * slld_ReadUniqueIDCmd - Read Unique ID Number
 * This function issues the Read Unique ID command to SPI Flash and read 64-bit Unique Serial Number
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadUniqueIDCmd
(
 BYTE *read_buf // data buffer
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    // read 64-bit number that is unique
    status = FLASH_RD(SPI_READ_UNIQUE_ID_CMD, ADDRESS_NOT_USED, read_buf, 8);

    return (status);
}

/******************************************************************************
 * slld_SetBurstWrapCmd - Set Burst with Wrap
 * This function issues the Set Burst with Wrap command to SPI Flash
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_SetBurstWrapCmd
(
 BYTE *wrapbit_buf // wrapbit buffer
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_SETBURSTWRAP_CMD, ADDRESS_NOT_USED, wrapbit_buf, 1);

    return (status);
}

/******************************************************************************
 * slld_OctalWordReadQuadCmd - Octal Word Read Quad
 * This function issues the Octal Word Read Quad command to SPI Flash
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_OctalWordReadQuadCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *read_buf, // variable in which to store read data
 BYTE modebit, // mode bit
 BYTECOUNT len_in_bytes // length in bytes
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_OCTALWORDREADQUAD_CMD, sys_addr, read_buf, len_in_bytes);

    return (status);
}

/******************************************************************************
 * slld_WriteVolatileCmd - Write Enable for Volatile Status Register
 * This function issues the Write Volalite Status command to SPI Flash
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_WriteVolatileCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WRITE_VOLATILE_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}

#endif // FLK_CMD

#ifndef REMOVE_ReadCmd

/******************************************************************************
 * 
 * slld_ReadCmd - Read from SPI Flash
 *
 * This function issues the Read command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ReadCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_READ_CMD, sys_addr, target, len_in_bytes);

    return (status);
}

#endif /* REMOVE_ReadCmd */

#ifndef REMOVE_Read_4BCmd

/******************************************************************************
 * 
 * slld_Read_4BCmd - Read from SPI Flash in 4 bytes addressing scheme
 *
 * This function issues the Read_4B command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Read_4BCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_READ_4B_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_Read_4BCmd */

#ifndef REMOVE_Fast_ReadCmd

/******************************************************************************
 * 
 * slld_Fast_ReadCmd - Fast Read from SPI Flash
 *
 * This function issues the Fast Read command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Fast_ReadCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes on which to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_FAST_READ_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_Fast_ReadCmd */

#ifndef REMOVE_Fast_Read_4BCmd

/******************************************************************************
 * 
 * slld_Fast_Read_4BCmd - Fast Read from SPI Flash in 4 bytes addressing scheme
 *
 * This function issues the 4-bytes Fast Read command to SPI Flash and reads from the array.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Fast_Read_4BCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes on which to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_FAST_READ_4B_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_Fast_Read_4BCmd */


#ifndef REMOVE_Read_IDCmd

/******************************************************************************
 * 
 * slld_Read_IDCmd - Read ID from SPI Flash
 *
 * This function issues the Read_ID command to SPI Flash and reads out the ID.
 * This command sets the target device in software-unprotected state
 * so this function also sets sys_software_protect_status[device_num] to FLASH_SOFTWARE_UNPROTECTED.
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Read_IDCmd
(
 BYTE *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = FLASH_RD(SPI_READ_ID_CMD, (ADDRESS) ADDRESS_NOT_USED, target, 1);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_UNPROTECTED;
#endif

    return (status);
}
#endif /* REMOVE_Read_IDCmd */

#ifndef REMOVE_RDIDCmd

/******************************************************************************
 * 
 * slld_RDIDCmd - Read Edentification from SPI Flash
 *
 * This function issues the RDID command to SPI Flash and reads out the ID.
 * On some devices this command is documented in the data sheet as
 * Release from deep power down and read electronic signature
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDIDCmd
(
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_RDID_CMD, (ADDRESS) ADDRESS_NOT_USED, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_RDIDCmd */

#ifndef REMOVE_READ_IDENTIFICATIONCmd

/******************************************************************************
 * 
 * slld_Read_IdentificationCmd - Read ID from SPI Flash
 *
 * This function issues the Read_ID command to SPI Flash and reads out the ID.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Read_IdentificationCmd
(
 BYTE *target, /* variable in which to store read data */
 ADDRESS addr /* address offset for the command */
 )
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS tmp_addr;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif
    if (addr < 0x2)
        tmp_addr = addr;
    else
        tmp_addr = (ADDRESS) 0;

    status = FLASH_RD(SPI_READ_IDENTIFICATION_CMD, tmp_addr, target, 2);

    return (status);
}
#endif /* REMOVE_READ_IDENTIFICATIONCmd */

#ifndef REMOVE_RDSRCmd

/******************************************************************************
 * 
 * slld_RDRSCmd - Read from Status Register
 *
 * This function issues the RDSR command to SPI Flash and reads from status register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDSRCmd
(
 BYTE *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif
    status = FLASH_RD(SPI_RDSR_CMD, (ADDRESS) ADDRESS_NOT_USED, target, 1);

    return (status);
}
#endif /* REMOVE_RDSRCmd */

#ifndef REMOVE_RASPCmd

/******************************************************************************
 * 
 * slld_RASPCmd - Read ASP Register
 *
 * This function issues the RASP command to SPI Flash and reads from ASP register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RASPCmd
(
 WORD *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_RASP_CMD, (ADDRESS) ADDRESS_NOT_USED, (BYTE *) target, 2);

    return (status);
}
#endif /* REMOVE_RASPCmd */

#ifndef REMOVE_BRRDCmd

/******************************************************************************
 * 
 * slld_BRRDCmd - Read Bank addressing register
 *
 * This function issues the BRRD command to SPI Flash and reads from the bank addressing register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BRRDCmd
(
 BYTE *target /* variable in which to store the bank addressing register value */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_BRRD_CMD, (ADDRESS) ADDRESS_NOT_USED, target, 1);

    return (status);
}
#endif /* REMOVE_BRRDCmd */

#ifndef REMOVE_ABRDCmd

/******************************************************************************
 * 
 * slld_ABRDCmd - Read Autoboot register
 *
 * This function issues the read autoboot register command to SPI Flash and reads it.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ABRDCmd
(
 DWORD *target /* variable in which to store the autoboot register value */

 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_ABRD_CMD, (ADDRESS) ADDRESS_NOT_USED, (BYTE *) target, 4);

    return (status);
}
#endif /* REMOVE_ABRDCmd */

#ifndef REMOVE_ECCRDCmd

/******************************************************************************
 * 
 * slld_ECCRDCmd - Read ECC Register per cache line
 *
 * This function issues the ECCRD command to SPI Flash and reads from ECC register per cache line
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ECCRDCmd
(
 ADDRESS sys_addr, /* cache line address given by system */
 BYTE *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_ECCRD_CMD, sys_addr, target, 1);

    return (status);
}
#endif /* REMOVE_ECCRDCmd */

#ifndef REMOVE_RPWDCmd

/******************************************************************************
 * 
 * slld_RPWDCmd - Read password
 *
 * This function issues the RPWD command to SPI Flash and reads the password
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RPWDCmd
(
 BYTE *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_RPWD_CMD, (ADDRESS) ADDRESS_NOT_USED, target, 8);

    return (status);
}
#endif /* REMOVE_RPWDCmd */

#ifndef REMOVE_WRENCmd

/******************************************************************************
 * 
 * slld_WRENCmd - Issue the write enable command
 *
 * This function issues the WREN command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRENCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WREN_CMD, (ADDRESS) ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_WRENCmd */

#ifndef REMOVE_WRDICmd

/******************************************************************************
 * 
 * slld_WRDICmd - Issue the write disable command
 *
 * This function issues the WRDI command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRDICmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WRDI_CMD, (ADDRESS) ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_WRDICmd */

#ifndef REMOVE_WRSRCmd

/******************************************************************************
 * 
 * slld_WRSRCmd - Write to Status Register
 *
 * This function issues a write Status Register command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRSRCmd
(
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WRSR_CMD, (ADDRESS) ADDRESS_NOT_USED, data_buf, 1);

    return (status);
}
#endif /* REMOVE_WRSRCmd */

void slld_VersionCmd(void)
{
    printf("\n%s\n", SLLD_VERSION);
}

/******************************************************************************
 * 
 * slld_SRSTCmd - Software reset
 *
 * This function issues a software reset command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SRSTCmd(void)
{
    SLLD_STATUS status = SLLD_OK;
    static unsigned int print_version = 1;

    if (!print_version)
    {
        slld_VersionCmd();
        print_version = 1;
    }

#ifndef REMOVE_SRSTCmd

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_SOFTWARE_RESET, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

#endif /* REMOVE_SRSTCmd */

    return (status);
}

#ifndef REMOVE_PPCmd

/******************************************************************************
 * 
 * slld_PPCmd - Page Program
 *
 * This function issues a Page Program command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PP_CMD, sys_addr, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_PPCmd */

#ifndef REMOVE_PP_4BCmd

/******************************************************************************
 * 
 * slld_PP_4BCmd - Page Program
 *
 * This function issues the Page Program command to SPI Flash in 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PP_4BCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PP_4B_CMD, sys_addr, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_PP_4BCmd */

#ifndef REMOVE_PPB_PGCmd

/******************************************************************************
 * 
 * slld_PPB_PGCmd - PPB Program
 *
 * This function issues the PPB Program command to SPI Flash and programs it.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPB_PGCmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PPB_PG_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_PPB_PGCmd */

#ifndef REMOVE_PPB_PGOp

/******************************************************************************
 * 
 * slld_PPB_PGOp - PPB Program Operation
 *
 * This function issues the PPB Program command to SPI Flash and programs it, then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPB_PGOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_PPB_PGCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case PPOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_PPB_PGOp */

#ifndef REMOVE_DYB_PGCmd

/******************************************************************************
 * 
 * slld_DYB_PGCmd - DYB Program
 *
 * This function issues the DYB Program command to SPI Flash and programs it.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DYB_PGCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_DYB_PG_CMD, sys_addr, data_buf, 1);

    return (status);
}
#endif /* REMOVE_DYB_PGCmd */

#ifndef REMOVE_DYB_PGOp

/******************************************************************************
 * 
 * slld_DYB_PGOp - DYB Program operation
 *
 * This function issues the DYB Program command to SPI Flash and programs it, then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DYB_PGOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_DYB_PGCmd(sys_addr, data_buf);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case PPOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_DYB_PGOp */


#ifndef REMOVE_SECmd

/******************************************************************************
 * 
 * slld_SECmd - Sector Erase
 *
 * This function issues the Sector Erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SECmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_SE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_SECmd */

#ifndef REMOVE_SE_4BCmd

/******************************************************************************
 * 
 * slld_SE_4BCmd - Sector Erase using 4-bytes addressing scheme
 *
 * This function issues the Sector Erase command to SPI Flash using 4-bytes addressing scheme
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SE_4BCmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_SE_4B_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_SE_4BCmd */

#ifndef REMOVE_ERS_SSPCmd

/******************************************************************************
 * 
 * slld_ERS_SSPCmd - Sector Erase suspend
 *
 * This function issues the Sector Erase suspend command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ERS_SSPCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_ERS_SSP_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_ERS_SSPCmd */

#ifndef REMOVE_ERS_RESCmd

/******************************************************************************
 * 
 * slld_ERS_RESCmd - Sector Erase resume
 *
 * This function issues the Sector Erase resume command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ERS_RESCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_ERS_RES_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_ERS_RESCmd */

#ifndef REMOVE_RCSPCmd

/******************************************************************************
 * 
 * slld_RCSPCmd - Recovery suspend
 *
 * This function issues the recovery suspend command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RCSPCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_RCSP_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_RCSPCmd */

#ifndef REMOVE_RCRSCmd

/******************************************************************************
 * 
 * slld_RCRSCmd - Recovery resume
 *
 * This function issues the recovery resume command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RCRSCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_RCRS_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_RCRSCmd */


#ifndef REMOVE_BECmd

/******************************************************************************
 * 
 * slld_BECmd - Bulk Erase
 *
 * This function issues the Bulk Erase command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BECmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_BE_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}

SLLD_STATUS slld_BE1Cmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_BE1_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}

#endif /* REMOVE_BECmd */


#ifndef REMOVE_BLOCKERASECmd

/******************************************************************************
 * 
 * slld_BECmd - BLOCK Erase
 *
 * This function issues the BLOCK Erase command to SPI Flash. The Block Erase 
 * command sets all bits in hte addressed 64 KB block to 1 (all bytes are FFh).
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BlockEraseCmd(ADDRESS sys_addr)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_BLOCK_ERASE_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}

#endif  //REMOVE_BLOCKERASECmd



#ifndef REMOVE_RCVRCmd

/******************************************************************************
 * 
 * slld_RCVRCmd - Initiate recovery mode (manually refreshing ECC)
 *
 * This function issues the initiate recovery mode command to SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RCVRCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_RCVR_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_RCVRCmd */

#ifndef REMOVE_DPCmd

/******************************************************************************
 * 
 * slld_DPCmd - Deep Power-down
 *
 * This function issues the Deep Power-down command to SPI Flash.
 * This command sets the target device in deep power-down state to reduce power consumption
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DPCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_DP_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_PROTECTED;
#endif

    return (status);
}
#endif /* REMOVE_DPCmd */

#ifndef REMOVE_SPCmd

/******************************************************************************
 * 
 * slld_SPCmd - Software Protect
 *
 * This function issues the Software Protect command to SPI Flash.
 * This command sets the target device in software-protected state
 * so this function also sets sys_software_protect_status[device_num] to FLASH_SOFTWARE_PROTECTED.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SPCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_SP_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_PROTECTED;
#endif

    return (status);
}
#endif /* REMOVE_SPCmd */

#ifndef REMOVE_RESCmd

/******************************************************************************
 * 
 * slld_RESCmd - Release from Software Protect (Deep power-down)
 *
 * This function issues the Release from Software Protect command to SPI Flash.
 * This command sets the target device in software-unprotected state
 * so this function also sets sys_software_protect_status[device_num] to FLASH_SOFTWARE_UNPROTECTED.
 *
 * RETURNS: SLLD_OK or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RESCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

    status = FLASH_WR(SPI_RES_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    sys_software_protect_status = FLASH_SOFTWARE_UNPROTECTED;
#endif

    return (status);
}
#endif /* REMOVE_RESCmd */

#ifndef REMOVE_MULTIOMODE_FUNCTIONS
#ifndef REMOVE_CLSRCmd

/******************************************************************************
 * 
 * slld_ClearStatusRegisterCmd - Performs a clear status register command.
 *
 * This function issues the clear status register command to the SPI flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ClearStatusRegisterCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_CLSR_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_CLSRCmd */

#ifndef REMOVE_WRRCmd

/******************************************************************************
 * 
 * slld_WRRCmd - Write to Status and Config Registers
 *
 * This function issues the Write Registers command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRRCmd
(
 BYTE *status_val, /* variable containing data to program the status register */
 BYTE *config_val /* variable containing data to program the config register */
 )
{
    SLLD_STATUS status = SLLD_OK;
    BYTE Buffer[2];

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    Buffer[0] = *status_val;
    Buffer[1] = *config_val;
    status = FLASH_WR(SPI_WRR_CMD, ADDRESS_NOT_USED, Buffer, 2);

    return (status);
}
#endif /* REMOVE_WRRCmd */

#ifndef REMOVE_WRROp

/******************************************************************************
 * 
 * slld_WRROp - Write to Status and Config Registers
 *
 * This function issues the Write Registers command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRROp
(
 BYTE *status_val, /* variable containing data to program the status register */
 BYTE *config_val, /* variable containing data to program the config register */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_WRRCmd(status_val, config_val);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case WRROp is operated on protected area */

    return (status);
}
#endif /* REMOVE_WRROp */


#ifndef REMOVE_WASPCmd

/******************************************************************************
 * 
 * slld_WASPCmd - Write to ASP Register
 *
 * This function issues the Write ASP register command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WASPCmd
(
 WORD *asp_val /* variable containing data to program to the ASP register */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WASP_CMD, ADDRESS_NOT_USED, (BYTE *) asp_val, 2);

    return (status);
}
#endif /* REMOVE_WASPCmd */

#ifndef REMOVE_WASPOp

/******************************************************************************
 * 
 * slld_WASPOP - Write to ASP Register operation
 *
 * This function issues the Write ASP register command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WASPOp
(
 WORD *asp_val, /* variable containing data to program to the ASP register */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_WASPCmd(asp_val);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case WASPOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_WASPOp */

#ifndef REMOVE_BRWRCmd

/******************************************************************************
 * 
 * slld_BRWRCmd - Write to the bank addressing register
 *
 * This function issues the Write bank addressing register command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BRWRCmd
(
 BYTE *bnk_val /* variable containing data to program to the bank addressing register */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_BRWR_CMD, ADDRESS_NOT_USED, bnk_val, 1);

    return (status);
}
#endif /* REMOVE_BRWRCmd */

#ifndef REMOVE_BRWROp

/******************************************************************************
 * 
 * slld_BRWROp - Write to the bank addressing register operation
 *
 * This function issues the Write bank addressing register command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BRWROp
(
 BYTE *bnk_val, /* variable containing data to program to the bank addressing register */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_BRWRCmd(bnk_val);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case BRWROp is operated on protected area */

    return (status);
}
#endif /* REMOVE_BRWROp */

#ifndef REMOVE_ABWRCmd

/******************************************************************************
 * 
 * slld_ABWRCmd - Write to the autoboot register
 *
 * This function issues the Write autoboot register command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ABWRCmd
(
 DWORD *abt_val /* variable containing data to program to the autoboot register */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_ABWR_CMD, ADDRESS_NOT_USED, (BYTE*) abt_val, 4);

    return (status);
}
#endif /* REMOVE_ABWRCmd */

#ifndef REMOVE_ABWROp

/******************************************************************************
 * 
 * slld_ABWROp - Write to the autoboot register
 *
 * This function issues the Write autoboot register command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_ABWROp
(
 DWORD *abt_val, /* variable containing data to program to the autoboot register */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_ABWRCmd(abt_val);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case ABWROp is operated on protected area */

    return (status);
}
#endif /* REMOVE_ABWROp */

#ifndef REMOVE_WPWDCmd

/******************************************************************************
 * 
 * slld_WPWDCmd - Write the password
 *
 * This function issues the Write password command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WPWDCmd
(
 BYTE *target /* variable containing data to program to the ASP password */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WPWD_CMD, ADDRESS_NOT_USED, target, 1);

    return (status);
}
#endif /* REMOVE_WPWDCmd */

#ifndef REMOVE_WPWDOp

/******************************************************************************
 * 
 * slld_WPWDOp - Write to ASP password operation
 *
 * This function issues the Write password command to the SPI Flash then polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WPWDOp
(
 BYTE *target, /* variable containing data to program to the ASP password */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_WPWDCmd(target);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case WPWDOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_WPWDOp */

#ifndef REMOVE_RCRCmd

/******************************************************************************
 * 
 * slld_RCRCmd - Read from the configuration register
 *
 * This function issues the RCR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RCRCmd
(
 BYTE *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_RCR_CMD, ADDRESS_NOT_USED, target, 1);

    return (status);
}
#endif /* REMOVE_RCRCmd */

#ifndef REMOVE_OTPRCmd

/******************************************************************************
 * 
 * slld_OTPRCmd - Read from the OTP area
 *
 * This function issues the OTPR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_OTPRCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_OTPR_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_OTPRCmd */

#ifndef REMOVE_OTPPCmd

/******************************************************************************
 * 
 * slld_OTPPCmd - OTP Program
 *
 * This function issues the OTP Program command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_OTPPCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_OTPP_CMD, sys_addr, data_buf, 1);

    return (status);
}
#endif /* REMOVE_OTPPCmd */

#ifndef REMOVE_P4ECmd

/******************************************************************************
 * 
 * slld_P4ECmd - Erase 4K parameter Sector
 *
 * This function issues the P4E command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P4ECmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_P4E_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_P4ECmd */

#ifndef REMOVE_P8ECmd

/******************************************************************************
 * 
 * slld_P8ECmd - Erase 8K parameter Sector
 *
 * This function issues the P8E command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P8ECmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_P8E_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_P8ECmd */

#ifndef REMOVE_P8E_4BCmd

/******************************************************************************
 * 
 * slld_P8ECmd - Erase 8K parameter Sector using 4-bytes addressing scheme
 *
 * This function issues the P8E command to the SPI Flash using 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P8E_4BCmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_P8E_4B_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_P8E_4BCmd */

#ifndef REMOVE_DUALOUTPUT_READCmd

/******************************************************************************
 * 
 * slld_DualIOReadCmd - Read flash using dual IO
 *
 * This function issues the dual IO read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DualIOReadCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_DUALIO_RD_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_DUALOUTPUT_READCmd */

#ifndef REMOVE_DUALOUTPUT_READ_4BCmd

/******************************************************************************
 * 
 * slld_DualIORead_4BCmd - Read flash using dual IO in 4-bytes addressing scheme
 *
 * This function issues the dual IO read command and read the requested data using the 4-bytes 
 * addressing scheme
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DualIORead_4BCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_DUALIO_RD_4B_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_DUALOUTPUT_READ_4BCmd */

#ifndef REMOVE_QUADOUTPUT_READCmd

/******************************************************************************
 * 
 * slld_QuadIOReadCmd - Read flash using quad IO
 *
 * This function issues the quad IO read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QuadIOReadCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_QUADIO_RD_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUADOUTPUT_READCmd */

#ifndef REMOVE_QUADOUTPUT_READ_4BCmd

/******************************************************************************
 * 
 * slld_QuadIORead_4BCmd - Read flash using quad IO in 4-bytes addressing scheme
 *
 * This function issues the quad IO read command and reads the requested data in 4-bytes
 * addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QuadIORead_4BCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_QUADIO_RD_4B_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUADOUTPUT_READ_4BCmd */

#ifndef REMOVE_DUALOUTPUT_HP_READCmd

/******************************************************************************
 * 
 * slld_DualIOHPReadCmd - Read flash using dual IO HP
 *
 * This function issues the dual IO HP read command and reads the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DualIOHPReadCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *target, // variable in which to store read data
 BYTE modebit, // mode bit
 BYTECOUNT len_in_bytes // number of bytes to read
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(SPI_DUALIO_HPRD_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_DUALOUTPUT_HP_READCmd */

#ifndef REMOVE_DUALOUTPUT_HP_READ_4BCmd

/******************************************************************************
 * 
 * slld_DualIOHPRead_4BCmd - Read flash using dual IO HP in 4-bytes addressing scheme
 *
 * This function issues the dual IO HP read command and reads the requested data in 4-bytes
 * addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DualIOHPRead_4BCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *target, // variable in which to store read data
 BYTE modebit, // The read mode to be passed to the device
 BYTECOUNT len_in_bytes // number of bytes to read
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(SPI_DUALIO_HPRD_4B_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_DUALOUTPUT_HP_READ_4BCmd */

#ifndef REMOVE_QUADOUTPUT_HP_READCmd

/******************************************************************************
 * 
 * slld_QuadIOHPReadCmd - Read flash using quad IO HP
 *
 * This function issues quad IO HP read command and read the requested data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QuadIOHPReadCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *target, // variable in which to store read data
 BYTE modebit, // The read mode to be passed to the device
 BYTECOUNT len_in_bytes // number of bytes to read
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(SPI_QUADIO_HPRD_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUADOUTPUT_HP_READCmd */

#ifndef REMOVE_QUADOUTPUT_HP_READ_4BCmd

/******************************************************************************
 * 
 * slld_QuadIOHPRead_4BCmd - Read flash using quad IO in 4-bytes addressing scheme
 *
 * This function issues the quad IO HP read command and reads the requested data in 
 * 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QuadIOHPRead_4BCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *target, // variable in which to store read data
 BYTE modebit, // The read mode to be passed to the device
 BYTECOUNT len_in_bytes // number of bytes to read
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    modebit_char = modebit;
    status = FLASH_RD(SPI_QUADIO_HPRD_4B_CMD, sys_addr, target, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUADOUTPUT_HP_READ_4BCmd */

#ifndef REMOVE_QUAD_PAGE_PROGRAMCmd

/******************************************************************************
 * 
 * slld_QPPCmd - Page Program using quad IO
 *
 * This function issues the Page Program command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPPCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_QPP_CMD, sys_addr, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUAD_PAGE_PROGRAMCmd */

#ifndef REMOVE_QUAD_PAGE_PROGRAM_4BCmd

/******************************************************************************
 * 
 * slld_QPP_4BCmd - Page Program using quad IO in 4-bytes addressing scheme
 *
 * This function issues the Page Program command to the SPI Flash in 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPP_4BCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_QPP_4B_CMD, sys_addr, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUAD_PAGE_PROGRAM_4BCmd */


#ifndef REMOVE_MPMCmd

/******************************************************************************
 * 
 * slld_MPMCmd - Charges the charge pumps for fast Multi-IO operation
 *
 * This function charges the charge pumps for fast Multi-IO operation.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_MPMCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_MPM_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_MPMCmd */
#endif /* REMOVE_MULTIOMODE_FUNCTIONS */

#ifndef REMOVE_Poll
#ifdef SLLD_DEV_FLASH

/******************************************************************************
 * 
 * slld_Poll - Polls flash device for embedded operation completion
 *
 * This function polls the Flash device to determine when an embedded
 * operation is finished.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_Poll
(
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    *dev_status_ptr = dev_status_unknown;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    do
    {
        status = slld_StatusGet(dev_status_ptr);
        if (status != SLLD_OK)
            return (status);
    }
    while (*dev_status_ptr == dev_busy);

    return (status);
}
#endif /* SLLD_DEV_FLASH */
#endif /* REMOVE_Poll */

#ifndef REMOVE_StatusGet

/******************************************************************************
 *    
 * slld_StatusGet - Determines Flash Status
 *
 * This function gets the device status from the SPI flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_StatusGet
(
 DEVSTATUS* dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;
    BYTE poll_data;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = slld_RDSRCmd(&poll_data);
    if (status != SLLD_OK)
        return (status);

    // Check if steady state
    if (poll_data & B0_MASK)
    { // If b0 = 1 then device is busy
        // If b5 = 1 then there was an erase error
        if ((STATUS_ERROR_FLAGS & HAS_STATBIT5_ERROR) && (poll_data & B5_MASK))
            *dev_status_ptr = dev_erase_error;
            // If b6 = 1 then there was a program error
        else if ((STATUS_ERROR_FLAGS & HAS_STATBIT6_ERROR) && (poll_data & B6_MASK))
            *dev_status_ptr = dev_program_error;
        else
            *dev_status_ptr = dev_busy;
    }
    else
    {
        // If b0 = 0 then device is not busy
        *dev_status_ptr = dev_not_busy;
    }

    return (status);
}
#endif /* REMOVE_StatusGet */

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
#ifndef REMOVE_SoftwareProtectStatusGet

/******************************************************************************
 *    
 * slld_SoftwareProtectStatusGet - Determines Flash Software Protection Status
 *
 * This function determines the flash software protection status.
 *
 * RETURNS: SLLD_OK.
 *
 */
SLLD_STATUS slld_SoftwareProtectStatusGet
(
 DEV_SOFTWARE_PROTECT_STATUS* dev_softwareprotect_status_ptr /* variable to store device software protect status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    *dev_softwareprotect_status_ptr = sys_software_protect_status;

    return (status);
}
#endif /* REMOVE_SoftwareProtectStatusGet */
#endif /* INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK */

#ifndef REMOVE_PPOp

/******************************************************************************
 * 
 * slld_PPOp - Performs a Page Programming Operation.
 *
 * This function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    if (len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR; /* Data Bytes are larger than Page_Size */
        return (status);
    }

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_PPCmd(sys_addr, data_buf, len_in_bytes);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case PPOp is operated on protected area */

    return (status);
}


#endif /* REMOVE_PPOp */

#ifndef REMOVE_PP_4BOp

/******************************************************************************
 * 
 * slld_PP_4BOp - Performs a Page Programming Operation using 4-bytes addressing scheme.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PP_4BOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    if (len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR; /* Data Bytes are larger than Page_Size */
        return (status);
    }

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_PP_4BCmd(sys_addr, data_buf, len_in_bytes);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case PPOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_PP_4BOp */

#ifndef REMOVE_SEOp

/******************************************************************************
 * 
 * slld_SEOp - Performs a Sector Erase Operation
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SEOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_SECmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case SEOp is operated on a protected area */

    return (status);
}
#endif /* REMOVE_SEOp */

#ifndef REMOVE_SE_4BOp

/******************************************************************************
 * 
 * slld_SE_4BOp - Performs a Sector Erase Operation using 4-bytes addressing scheme
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion using 4-bytes addressing scheme.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_SE_4BOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_SE_4BCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case SE_4BOp is operated on a protected area */

    return (status);
}
#endif /* REMOVE_SE_4BOp */

#ifndef REMOVE_BEOp

/******************************************************************************
 * 
 * slld_BEOp - Performs a Bulk Erase Operation.
 *
 * Function erase all data in a device.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BEOp
(
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_BECmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case BEOp is operated on protected area */

    return (status);
}

SLLD_STATUS slld_BE1Op
(
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_BE1Cmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case BEOp is operated on protected area */

    return (status);
}

#endif /* REMOVE_BEOp */


#ifndef REMOVE_BLOCKERASEOp

/******************************************************************************
 * 
 * slld_BEOp - Performs a Bulk Erase Operation.
 *
 * Function erase all data in a device.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */


SLLD_STATUS slld_BlockEraseOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_BlockEraseCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case BEOp is operated on protected area */

    return (status);
}

#endif /* REMOVE_BLOCKERASEOp */



#ifndef REMOVE_WRSROp

/******************************************************************************
 * 
 * slld_WSROp - Write Status Register Operation
 *
 * Function writes data to Status Register.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WRSROp
(
 BYTE *data_buf, /* variable containing data to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_WRSRCmd(data_buf);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case WRSROp is operated on protected area */

    return (status);
}
#endif /*REMOVE_WRSROp */

#ifndef REMOVE_BufferedProgramOp

/******************************************************************************
 * 
 * slld_BufferedProgramOp - Performs a Programming Operation.
 *
 * Function programs data to the specfied address.
 * Function issues all required commands and polls for completion. Address doesn't have to be 
 * page aligned.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BufferedProgramOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes on which to operate */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS current_sys_addr; /* current address to which program operation operates */
    BYTECOUNT current_len_in_bytes; /* number of bytes to program at a time */
    BYTE *current_data_ptr; /* pointer to data to program */

    /* initial setting of variables */
    current_sys_addr = sys_addr;
    current_data_ptr = data_buf;

    current_len_in_bytes = PAGE_SIZE - (sys_addr & PAGE_MASK);
    if (current_len_in_bytes > len_in_bytes)
        current_len_in_bytes = len_in_bytes;

    do
    {
        status = slld_PPOp(current_sys_addr, current_data_ptr, current_len_in_bytes, dev_status_ptr);
        if (status != SLLD_OK)
            return (status);

        len_in_bytes -= current_len_in_bytes;

        /* set variables for next programming */
        current_sys_addr += current_len_in_bytes;
        current_data_ptr += current_len_in_bytes;

        if (len_in_bytes > PAGE_SIZE)
        {
            current_len_in_bytes = PAGE_SIZE;
        }
        else
        {
            current_len_in_bytes = len_in_bytes;
        }
    }
    while (len_in_bytes != 0);

    status = slld_WRDICmd(); /* just in case BufferedProgramOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_BufferedProgramOp */

#ifndef REMOVE_BufferedProgram_4BOp

/******************************************************************************
 * 
 * slld_BufferedProgram_4BOp - Performs a Programming Operation using 4-bytes addressing scheme.
 *
 * Function programs data to the specfied address.
 * Function issues all required commands and polls for completion. Address doesn't have to be 
 * page aligned.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BufferedProgram_4BOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes on which to operate */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;
    ADDRESS current_sys_addr; /* current address to which program operation operates */
    BYTECOUNT current_len_in_bytes; /* number of bytes to program at a time */
    BYTE *current_data_ptr; /* pointer to data to program */

    /* initial setting of variables */
    current_sys_addr = sys_addr;
    current_data_ptr = data_buf;

    current_len_in_bytes = PAGE_SIZE - (sys_addr & PAGE_MASK);
    if (current_len_in_bytes > len_in_bytes)
        current_len_in_bytes = len_in_bytes;

    do
    {
        status = slld_PP_4BOp(current_sys_addr, current_data_ptr, current_len_in_bytes, dev_status_ptr);
        if (status != SLLD_OK)
            return (status);

        len_in_bytes -= current_len_in_bytes;

        /* set variables for next programming */
        current_sys_addr += current_len_in_bytes;
        current_data_ptr += current_len_in_bytes;

        if (len_in_bytes > PAGE_SIZE)
        {
            current_len_in_bytes = PAGE_SIZE;
        }
        else
        {
            current_len_in_bytes = len_in_bytes;
        }
    }
    while (len_in_bytes != 0);

    status = slld_WRDICmd(); /* just in case BufferedProgram_4BOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_BufferedProgram_4BOp */

#ifndef REMOVE_BlockProtectOp

/******************************************************************************
 * 
 * slld_BlockProtectOp - Performs a Block Protection Operation.
 *
 * Function sets Block Protect bits to protect specified memory area.
 * Function issues all required commands and polls for completion.
 *
 * Valid <bpb_value> values are:  (please see the datasheet for each device)
 *  0x00 - 0x03 : for S25FL002D, S25FL001D...
 *  0x00 - 0x07 : for S25FL004D, S25FL032P...
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_BlockProtectOp
(
 BYTE bpb_value, /* block protect bit value */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;
    BYTE current_status_register; /* current state of Status Register */
    BYTE new_status_register; /*new state of Status Register */

    status = slld_RDSRCmd(&current_status_register);
    if (status != SLLD_OK)
        return (status);

    new_status_register = (current_status_register & (~BLOCK_PROTECT_BITS_MASK) | (bpb_value << 2));

    status = slld_WRSROp(&new_status_register, dev_status_ptr);

    return (status);
}
#endif /* REMOVE_BlockProtectOp */

#ifndef REMOVE_MULTIOMODE_FUNCTIONS
#ifndef REMOVE_QUAD_PAGE_PROGRAMOp

/******************************************************************************
 * 
 * slld_QPPOp - Performs a Quad-Io Page Programming Operation.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPPOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    if (len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR; /* Data Bytes are larger than Page_Size */
        return (status);
    }

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_QPPCmd(sys_addr, data_buf, len_in_bytes);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case QPPOp is operated on protected area */

    return (status);
}

SLLD_STATUS slld_QuadWriteOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;
    BYTECOUNT length;

    while (len_in_bytes)
    {
        if (len_in_bytes < PAGE_SIZE)
            length = len_in_bytes;
        else
            length = PAGE_SIZE;

        status = slld_QPPOp(sys_addr, data_buf, length, dev_status_ptr);
        if (status != SLLD_OK)
            return (status);

        len_in_bytes -= length;
        sys_addr += length;
        data_buf += length;
    }

    return (status);
}

#endif /* REMOVE_QUAD_PAGE_PROGRAMOp */

#ifndef REMOVE_QUAD_PAGE_PROGRAM_4BOp

/******************************************************************************
 * 
 * slld_QPP_4BOp - Performs a Quad-Io Page Programming Operation using 4-bytes addressing scheme.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPP_4BOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    if (len_in_bytes > PAGE_SIZE)
    {
        status = SLLD_ERROR; /* Data Bytes are larger than Page_Size */
        return (status);
    }

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_QPP_4BCmd(sys_addr, data_buf, len_in_bytes);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case QPPOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_QUAD_PAGE_PROGRAM_4BOp */

#ifndef REMOVE_OTPPOp

/******************************************************************************
 * 
 * slld_OTPPOp - Performs a OTP Programming Operation.
 *
 * Function programs location in a page to the specified data.
 * Function issues all required commands and polls for completion.
 * Data size to program must be within PAZE_SIZE.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_OTPPOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_OTPPCmd(sys_addr, data_buf);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case OTPPOp is operated on a protected area */

    return (status);
}
#endif /* REMOVE_OTPPOp */

#ifndef REMOVE_P4EOp

/******************************************************************************
 * 
 * slld_P4EOp - Performs a 4K Sector Erase Operation.
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P4EOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_P4ECmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case P8EOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_P4EOp */

#ifndef REMOVE_P8EOp

/******************************************************************************
 * 
 * slld_P8EOp - Performs a 8K Sector Erase Operation.
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P8EOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_P8ECmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case P8EOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_P8EOp */

#ifndef REMOVE_P8E_4BOp

/******************************************************************************
 * 
 * slld_P8E_4BOp - Performs a 8K Sector Erase Operation using 4-bytes addressing scheme.
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P8E_4BOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_P8E_4BCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case P8E_4BOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_P8E_4BOp */

#endif // REMOVE_MULTIOMODE_FUNCTIONS

#define FLS_CMD

#ifdef FLS_CMD
#ifndef REMOVE_RDSR2Cmd

/******************************************************************************
 * 
 * slld_RDRS2Cmd - Read from Status Register-2
 *
 * This function issues the RDSR2 command to SPI Flash and reads from status register.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_RDSR2Cmd
(
 BYTE *target /* variable in which to store read data */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_RDSR2_CMD, (ADDRESS) ADDRESS_NOT_USED, target, 1);
#ifdef BOARD226
    status = FLASH_RD(SPI_RDSR2_CMD, (ADDRESS) ADDRESS_NOT_USED, target, 1);
#endif

    return (status);
}
#endif /* REMOVE_RDSR2Cmd */

#ifndef REMOVE_P4E_4BCmd

/******************************************************************************
 * 
 * slld_P4E_4BCmd - Erase 4K parameter Sector (4Byte Addr)
 *
 * This function issues the P4E4 command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P4E_4BCmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_P4E4_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_P4E4Cmd */

#ifndef REMOVE_QUAD_PAGE_PROGRAMCmd

/******************************************************************************
 * 
 * slld_QPPCmd - Page Program using quad IO
 *
 * This function issues the QPP command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_QPP2Cmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_QPP2_CMD, sys_addr, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_QUAD_PAGE_PROGRAMCmd */

#ifndef REMOVE_DLPRDCmd

/******************************************************************************
 * 
 * slld_DLPRDCmd - Read Data Learning Pattern
 *
 * This function issues the DLPRD command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DLPRDCmd
(
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_DLPRD_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_DLPRDCmd */

#ifndef REMOVE_PNVDLRCmd

/******************************************************************************
 * 
 * slld_PNVDLRCmd - Program NV Data Learning Register
 *
 * This function issues the PNVDLR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PNVDLRCmd
(
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PNVDLR_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_PNVDLRCmd */

#ifndef REMOVE_WVDLRCmd

/******************************************************************************
 * 
 * slld_WVDLRCmd - Write Volatile Data Learning Register
 *
 * This function issues the WVDLR command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WVDLRCmd
(
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WVDLR_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_WVDLRCmd */

#ifndef REMOVE_PGSPCmd

/******************************************************************************
 * 
 * slld_PGSPCmd - Program Suspend
 *
 * This function issues the PGSP command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PGSPCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PGSP_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_PGSPCmd */

#ifndef REMOVE_PGRSCmd

/******************************************************************************
 * 
 * slld_PGRSCmd - Program Resume
 *
 * This function issues the PGRS command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PGRSCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PGRS_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_PGRSCmd */

#ifndef REMOVE_PLBWRCmd

/******************************************************************************
 * 
 * slld_PLBWRCmd - PPB Lock Bit Write 
 *
 * This function issues the PLBWR command to SPI Flash 
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PLBWRCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PLBWR_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_PLBWRCmd */

#ifndef REMOVE_PLBRDCmd

/******************************************************************************
 * 
 * slld_PLBRDCmd - PPB Lock Bit Read
 *
 * This function issues the PLBRD command to the SPI Flash.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PLBRDCmd
(
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes /* number of bytes to operate */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_PLBRD_CMD, ADDRESS_NOT_USED, data_buf, len_in_bytes);

    return (status);
}
#endif /* REMOVE_PLBRDCmd */


#ifndef REMOVE_DYB_RDCmd

/******************************************************************************
 * 
 * slld_DYB_RDCmd - DYB Read
 *
 * This function issues the DYB Read command to SPI Flash and read data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_DYB_RDCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_DYB_RD_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_DYB_RDCmd */

#ifndef REMOVE_PPB_RDCmd

/******************************************************************************
 * 
 * slld_PPB_RDCmd - PPB Read
 *
 * This function issues the PPB Read command to SPI Flash and read data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPB_RDCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PPB_RD_CMD, sys_addr, data_buf, 1);

    return (status);
}
#endif /* REMOVE_PPB_RDCmd */

#ifndef REMOVE_PPB_ERSCmd

/******************************************************************************
 * 
 * slld_PPB_ERSCmd - PPB Erase
 *
 * This function issues the PPB Erase command to SPI Flash 
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PPB_ERSCmd(void)
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PPB_ERS_CMD, ADDRESS_NOT_USED, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_PPB_ERSCmd */

#ifndef REMOVE_WDBRDCmd

/******************************************************************************
 * 
 * slld_WDBRDCmd - WDB Read
 *
 * This function issues the WDB Read command to SPI Flash and read data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WDBRDCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_RD(SPI_WDBRD_CMD, sys_addr, data_buf, 1);

    return (status);
}
#endif /* REMOVE_WDBRDCmd */

#ifndef REMOVE_WDBPCmd

/******************************************************************************
 * 
 * slld_WDBPCmd - WDB Program
 *
 * This function issues the WDB Program command to SPI Flash
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_WDBPCmd
(
 ADDRESS sys_addr /* device address given by system */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_WDBP_CMD, sys_addr, BUFFER_NOT_USED, 0);

    return (status);
}
#endif /* REMOVE_WDBPCmd */

#ifndef REMOVE_PASSRDCmd

/******************************************************************************
 * 
 * slld_PASSRDCmd - Password Read
 *
 * This function issues the Password Read command to SPI Flash and read data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PASSRDCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PASSRD_CMD, sys_addr, data_buf, 8); // 64-bit password

    return (status);
}
#endif /* REMOVE_PASSRDCmd */

#ifndef REMOVE_PASSPCmd

/******************************************************************************
 * 
 * slld_PASSPCmd - Password Program
 *
 * This function issues the Password Program command to SPI Flash and write data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PASSPCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PASSP_CMD, sys_addr, data_buf, 8); // 64-bit password

    return (status);
}
#endif /* REMOVE_PASSPCmd */

#ifndef REMOVE_PASSUCmd

/******************************************************************************
 * 
 * slld_PASSUCmd - Password Unlock
 *
 * This function issues the Password Unlock command to SPI Flash and unlock data.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_PASSUCmd
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf /* variable containing data to program */
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    status = FLASH_WR(SPI_PASSU_CMD, sys_addr, data_buf, 8); // 64-bit password

    return (status);
}
#endif /* REMOVE_PASSUCmd */

#ifndef REMOVE_P4E_4BOp

/******************************************************************************
 * 
 * slld_P4E_4BOp - Performs a 4K Sector Erase Operation (4Byte Address).
 *
 * Function erases specified sector.
 * Function issues all required commands and polls for completion.
 *
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 *
 */
SLLD_STATUS slld_P4E_4BOp
(
 ADDRESS sys_addr, /* device address given by system */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;

    status = slld_WRENCmd();
    if (status != SLLD_OK)
        return (status);

    status = slld_P4E_4BCmd(sys_addr);
    if (status != SLLD_OK)
        return (status);

    status = slld_Poll(dev_status_ptr);
    if (status != SLLD_OK)
        return (status);

    status = slld_WRDICmd(); /* just in case P8EOp is operated on protected area */

    return (status);
}
#endif /* REMOVE_P4E_4BOp */

SLLD_STATUS slld_ReadOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *target, /* variable in which to store read data */
 BYTECOUNT len_in_bytes /* number of bytes to read */
 )
{
    SLLD_STATUS status = SLLD_OK;
    unsigned long length;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    while (len_in_bytes)
    {
#ifdef CONTROLLER_BUFFER_SIZE
        if (len_in_bytes < CONTROLLER_BUFFER_SIZE)
            length = len_in_bytes;
        else
            length = CONTROLLER_BUFFER_SIZE;
#else
        length = len_in_bytes;
#endif

#ifdef USE_FAST_READ
        status = FLASH_RD(SPI_FAST_READ_CMD, sys_addr, target, length);
#elif defined USE_DUAL_READ
        status = FLASH_RD(SPI_DUALIO_RD_CMD, sys_addr, target, length);
#elif defined USE_QUAD_READ
        status = FLASH_RD(SPI_QUADIO_RD_CMD, sys_addr, target, length);
#else
        status = FLASH_RD(SPI_READ_CMD, sys_addr, target, length);
#endif

        len_in_bytes -= length;
        sys_addr += length;
        target += length;
    }

    return (status);
}

SLLD_STATUS slld_WriteOp
(
 ADDRESS sys_addr, /* device address given by system */
 BYTE *data_buf, /* variable containing data to program */
 BYTECOUNT len_in_bytes, /* number of bytes to program */
 DEVSTATUS *dev_status_ptr /* variable to store device status */
 )
{
    SLLD_STATUS status = SLLD_OK;
    unsigned long length;

    while (len_in_bytes)
    {
        if (len_in_bytes < PAGE_SIZE)
            length = len_in_bytes;
        else
            length = PAGE_SIZE;

        status = slld_WRENCmd();
        if (status != SLLD_OK)
            return (status);

#ifdef USE_QUAD_WRITE
        status = slld_QPPCmd(sys_addr, data_buf, length);
#else
        status = slld_PPCmd(sys_addr, data_buf, length);
#endif

        if (status != SLLD_OK)
            return (status);

        status = slld_Poll(dev_status_ptr);
        if (status != SLLD_OK)
            return (status);

        status = slld_WRDICmd();
        if (status != SLLD_OK)
            return (status);

        len_in_bytes -= length;
        sys_addr += length;
        data_buf += length;
    }

    return (status);
}


#endif // FLS_CMD

#ifdef INCL_SPI_READSFDPCMD

/******************************************************************************
 * slld_ReadSFDPCmd - Read Serial Flash Discoverable Parameter
 * This function issues the Read SFDP command to SPI Flash
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadSFDPCmd
(
 ADDRESS sys_addr, // device address given by system
 BYTE *read_buf // data buffer
 )
{
    SLLD_STATUS status = SLLD_OK;

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    // read 256-byte serial flash discoverable parameter (SFDP) register
    status = FLASH_RD(SPI_READ_SFDP_CMD, sys_addr, read_buf, 256);

    return (status);
}

#endif  // INCL_SPI_READSFDPCMD



#ifdef  INCL_SPI_READMODERESETCMD

/******************************************************************************
 * slld_ReadModeResetCmd - Continuous Read Mode Reset
 * This function issues the Read Mode Reset command to SPI Flash
 * RETURNS: SLLD_OK, SLLD_E_DEVICE_SOFTWARE_PROTECTED or SLLD_E_HAL_ERROR
 */
SLLD_STATUS slld_ReadModeResetCmd(void)
{
    SLLD_STATUS status = SLLD_OK;
    BYTE Buffer[1];

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
    /* check if target device is software protected */
    if (sys_software_protect_status != FLASH_SOFTWARE_UNPROTECTED)
    {
        status = SLLD_E_DEVICE_SOFTWARE_PROTECTED;
        return (status);
    }
#endif

    Buffer[0] = 0xff;
    status = FLASH_WR(SPI_READMODE_RESET_CMD, ADDRESS_NOT_USED, Buffer, 1);

    return (status);
}
#endif
