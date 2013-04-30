/* slld_targetspecific.h - Device configuration file for the SLLD */

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

#ifndef  __INC_slldtargetspecifich
#define __INC_slldtargetspecifich

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// Turn software trace on/off
//#define TRACE

/**********************************************************
 * Define LLD target device: flash.                        *
 ***********************************************************/
#define SLLD_DEV_FLASH
//define SLLD_DEV_SIM

//
// Uncomment the line that corresponds to the product type you want to use
//

// #define FL_D
// #define FL_A
// #define FL_P_SINGLE_BIT_IO
// #define FL_P_MULTI_BIT_IO
// #define FL_R
#define FL_S
// #define FL_T
// #define FL_K
// #define FL_2K

/* Uncomment next line to enable software protect check in each function */
/* #define INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK    */

/* If you are worried about code size, you can      */
/* remove an LLD function by un-commenting the      */
/* appropriate macro below.                         */
/*                                                  */
/* #define REMOVE_PPOp                              */
/* #define REMOVE_PP_4BOp                           */
/* #define REMOVE_BufferedProgramOp                 */
/* #define REMOVE_SEOp                              */
/* #define REMOVE_SE_4BOp                           */
/* #define REMOVE_BEOp                              */
/* #define REMOVE_WRSROp                            */
/* #define REMOVE_BlockProtectOp                    */
/* #define REMOVE_ReadCmd                           */
/* #define REMOVE_Read_4BCmd                        */
/* #define REMOVE_Fast_ReadCmd                      */
/* #define REMOVE_Fast_Read_4BCmd                   */
/* #define REMOVE_Read_IDCmd                        */
/* #define REMOVE_RDIDCmd                           */
/* #define REMOVE_SRSTCmd                           */
/* #define REMOVE_RDSRCmd                           */
/* #define REMOVE_RASPCmd                           */
/* #define REMOVE_RBNKCmd                           */
/* #define REMOVE_RABTCmd                           */
/* #define REMOVE_RECCCmd                           */
/* #define REMOVE_RPWDCmd                           */
/* #define REMOVE_WRENCmd                           */
/* #define REMOVE_WRDICmd                           */
/* #define REMOVE_WRSRCmd                           */
/* #define REMOVE_PPCmd                             */
/* #define REMOVE_PP_4BCmd                          */
/* #define REMOVE_SECmd                             */
/* #define REMOVE_SE_4BCmd                          */
/* #define REMOVE_BECmd                             */
/* #define REMOVE_SPCmd                             */
/* #define REMOVE_RESCmd                            */

/* #define REMOVE_PPB_PGCmd                         */
/* #define REMOVE_PPB_PGOp                          */
/* #define REMOVE_DYB_PGCmd                         */
/* #define REMOVE_DYB_PGOp                          */
/* #define REMOVE_ERS_SSPCmd                        */
/* #define REMOVE_ERS_RESCmd                        */
/* #define REMOVE_RCSPCmd                           */
/* #define REMOVE_RCRSCmd                           */
/* #define REMOVE_RCVRCmd                           */

/*  slld_poll and slld_StatusGet utility functions are 
 *  used in Operation function, like slld_PPOp, so should
 *  be included when Operation functions are used.
 */

/* #define REMOVE_Poll                              */
/* #define REMOVE_StatusGet                         */
/* #define REMOVE_SoftwareProtectStatusGet          */

/*  The folowing Macros will remove the new functions
 *  that are required for dual and quad mode SPI devices
 */

#define REMOVE_MULTIOMODE_FUNCTIONS              
#define REMOVE_READ_IDENTIFICATIONCmd            
#define REMOVE_READ_IDCFI                        
#define REMOVE_CLSRCmd                           
#define REMOVE_WRRCmd                            
#define REMOVE_WRROp                             
#define REMOVE_WASPCmd                           
#define REMOVE_WASPOp                            
#define REMOVE_WBNKCmd                           
#define REMOVE_WBNKOp                            
#define REMOVE_WABTCmd                           
#define REMOVE_WABTOp                            
#define REMOVE_WPWDCmd                           
#define REMOVE_WPWDOp                            
#define REMOVE_RCRCmd                           
#define REMOVE_DUALOUTPUT_READCmd                
#define REMOVE_DUALOUTPUT_READ_4BCmd             
#define REMOVE_QUADOUTPUT_READ                   
#define REMOVE_QUADOUTPUT_READ_4BCmd             
#define REMOVE_DUALOUTPUT_HP_READ                
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd          
#define REMOVE_QUADOUTPUT_HP_READ                
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd          
#define REMOVE_QUAD_PAGE_PROGRAMCmd              
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd           
#define REMOVE_QUAD_PAGE_PROGRAMOp               
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp            
#define REMOVE_P4EOp                             
#define REMOVE_P8EOp                             
#define REMOVE_P8E_4BOp                          
#define REMOVE_P4ECmd                            
#define REMOVE_P8ECmd                            
#define REMOVE_P8E_4BCmd                         
#define REMOVE_OTPPCmd                           
#define REMOVE_OTPPOp                            
#define REMOVE_OTPRCmd                           
#define REMOVE_MPMCmd                            

//************************************************************************
// enables code to execute commands from a file instead of directly from *
// the command line.													 *
//************************************************************************
//#define ENABLE_SCRIPTING_MACRO

/************************************************************************
 * Creates lld_printf.log file and writes all the LLD_PRINTFs to it      *
 ************************************************************************/
//#define LLD_PRINTF_LOGFILE

//*******************************************************
// Define flash simulator R/W debug macro (no trace)    *
//*******************************************************
#ifdef __FLASHMODEL
// The number of clocks to write various types of data
#define WRITEDATALEN 8
#define WRITECMDLEN  8
#define WRITEADDR24  24
#define WRITEADDR32  32

#define DEBUG_FLASH_WR(b,d)   spiSignalManagerAdapter->SM_ADPT_WR((UINT32)(b), WRITEDATA, d, WRITEDATALEN)
#define DEBUG_FLASH_RD(b)     spiSignalManagerAdapter->SM_ADPT_RD()
#else
// The number of clocks to write various types of data
// these values are only used by the model
#define WRITEDATALEN 0
#define WRITECMDLEN  0
#define WRITEADDR24  0
#define WRITEADDR32  0

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INC_slldtargetspecifich */
