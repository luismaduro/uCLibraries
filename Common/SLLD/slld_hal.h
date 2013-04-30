/* slld_hal.h - HAL Header file for Spansion SPI-flash Low Level Driver */
 
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

#ifndef __INC_lld_halh
#define __INC_lld_halh

// Users should define what controller has been used for their customerized 
// enviroment. The following code is just an implementation example, which should
// be modified by the user accordingly to match their specific hardware environment.  

#include "stm32f10x.h"
#include "SPIDevice.h"
#include "uwn_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* public function prototypes */

// HAL read functions API
SLLD_STATUS FLASH_READ
(
BYTE    command,                                    /* write a single command byte to flash */
ADDRESS sys_addr,                                   /* system address to be used */
BYTE   *data_buffer,                                /* Pointer to the data buffer where to store the read data */
int     Number_Of_Read_Bytes                        /* number of bytes to be read */
);

// HAL write functions API
SLLD_STATUS FLASH_WRITE
(
BYTE    command,                                    /* write a single command byte to flash */
ADDRESS sys_addr,                                   /* system address to be used */
BYTE   *data_buffer,                                /* Pointer to the data buffer containing data to be written */
int     Number_Of_Written_Bytes                     /* number of bytes to be written */
);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INC_lld_halh */

