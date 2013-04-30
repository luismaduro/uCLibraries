/* slld.h - Header file for Spansion SPI Low Level Driver */

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

#ifndef __INC_lldh
#define __INC_lldh

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "slld_targetspecific.h"
     
#define SLLD_VERSION          "12.2.3"

// ==================
// Status error flags
// ==================
#define HAS_NOTHING          0x00
#define HAS_STATBIT5_ERROR   0x01
#define HAS_STATBIT6_ERROR   0x02

// ========================
// Product specific defines
// ========================

#ifdef FL_D
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD

#define REMOVE_WRRCmd
#define REMOVE_RCRCmd
#define REMOVE_READ_IDENTIFICATIONCmd
#define REMOVE_MULTIOMODE_FUNCTIONS
#define REMOVE_MPMCmd

#define REMOVE_Read_4BCmd
#define REMOVE_Fast_Read_4BCmd
#define REMOVE_DUALOUTPUT_READ_4BCmd
#define REMOVE_QUADOUTPUT_READ_4BCmd
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd
#define REMOVE_PP_4BCmd
#define REMOVE_P8E_4BCmd
#define REMOVE_SE_4BCmd
#define REMOVE_ERS_SSPCmd
#define REMOVE_ERS_RESCmd
#define REMOVE_SRSTCmd
#define REMOVE_RASPCmd
#define REMOVE_WASPCmd
#define REMOVE_RPWDCmd
#define REMOVE_WPWDCmd
#define REMOVE_ECCRDCmd
#define REMOVE_BRRDCmd
#define REMOVE_ABRDCmd
#define REMOVE_BRWRCmd
#define REMOVE_ABWRCmd
#define REMOVE_RCVRCmd
#define REMOVE_RCSPCmd
#define REMOVE_RCRSCmd
#define REMOVE_PPB_PGCmd
#define REMOVE_DYB_PGCmd
#define REMOVE_PP_4BOp
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp
#define REMOVE_P8E_4BOp
#define REMOVE_SE_4BOp
#define REMOVE_BufferedProgram_4BOp
#define REMOVE_DYB_PGOp
#define REMOVE_PPB_PGOp
#define REMOVE_WPWDOp
#define REMOVE_ABWROp
#define REMOVE_BRWROp
#define REMOVE_WASPOp
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp
// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (0xff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  HAS_NOTHING

#endif

#ifdef FL_A
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD

#define REMOVE_WRRCmd
#define REMOVE_RCRCmd
#define REMOVE_READ_IDENTIFICATIONCmd
#define REMOVE_MULTIOMODE_FUNCTIONS
#define REMOVE_MPMCmd

#define REMOVE_Read_4BCmd
#define REMOVE_Fast_Read_4BCmd
#define REMOVE_DUALOUTPUT_READ_4BCmd
#define REMOVE_QUADOUTPUT_READ_4BCmd
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd
#define REMOVE_PP_4BCmd
#define REMOVE_P8E_4BCmd
#define REMOVE_SE_4BCmd
#define REMOVE_ERS_SSPCmd
#define REMOVE_ERS_RESCmd
#define REMOVE_SRSTCmd
#define REMOVE_RASPCmd
#define REMOVE_WASPCmd
#define REMOVE_RPWDCmd
#define REMOVE_WPWDCmd
#define REMOVE_ECCRDCmd
#define REMOVE_BRRDCmd
#define REMOVE_ABRDCmd
#define REMOVE_BRWRCmd
#define REMOVE_ABWRCmd
#define REMOVE_RCVRCmd
#define REMOVE_RCSPCmd
#define REMOVE_RCRSCmd
#define REMOVE_PPB_PGCmd
#define REMOVE_DYB_PGCmd
#define REMOVE_PP_4BOp
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp
#define REMOVE_P8E_4BOp
#define REMOVE_SE_4BOp
#define REMOVE_BufferedProgram_4BOp
#define REMOVE_DYB_PGOp
#define REMOVE_PPB_PGOp
#define REMOVE_WPWDOp
#define REMOVE_ABWROp
#define REMOVE_BRWROp
#define REMOVE_WASPOp
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp

// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (0xff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  HAS_NOTHING

#endif

#if (defined FL_P_SINGLE_BIT_IO) || (defined FL_R)
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD
#define SPI_READ_IDENTIFICATION_CMD SPI_READ_ID_9F_CMD

#define REMOVE_CLSRCmd
#define REMOVE_DUALOUTPUT_READCmd
#define REMOVE_QUADOUTPUT_READCmd
#define REMOVE_DUALOUTPUT_HP_READCmd
#define REMOVE_QUADOUTPUT_HP_READCmd
#define REMOVE_QUAD_PAGE_PROGRAMCmd
#define REMOVE_QUAD_PAGE_PROGRAMOp
#define REMOVE_P4EOp
#define REMOVE_P8EOp
#define REMOVE_P4ECmd
#define REMOVE_P8ECmd
#define REMOVE_OTPPCmd
#define REMOVE_OTPPOp
#define REMOVE_QPPPOp
#define REMOVE_OTPRCmd
#define REMOVE_MPMCmd
#define REMOVE_RMB

#define REMOVE_Read_4BCmd
#define REMOVE_Fast_Read_4BCmd
#define REMOVE_DUALOUTPUT_READ_4BCmd
#define REMOVE_QUADOUTPUT_READ_4BCmd
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd
#define REMOVE_PP_4BCmd
#define REMOVE_P8E_4BCmd
#define REMOVE_SE_4BCmd
#define REMOVE_ERS_SSPCmd
#define REMOVE_ERS_RESCmd
#define REMOVE_SRSTCmd
#define REMOVE_RASPCmd
#define REMOVE_WASPCmd
#define REMOVE_RPWDCmd
#define REMOVE_WPWDCmd
#define REMOVE_ECCRDCmd
#define REMOVE_BRRDCmd
#define REMOVE_ABRDCmd
#define REMOVE_BRWRCmd
#define REMOVE_ABWRCmd
#define REMOVE_RCVRCmd
#define REMOVE_RCSPCmd
#define REMOVE_RCRSCmd
#define REMOVE_PPB_PGCmd
#define REMOVE_DYB_PGCmd
#define REMOVE_PP_4BOp
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp
#define REMOVE_P8E_4BOp
#define REMOVE_SE_4BOp
#define REMOVE_BufferedProgram_4BOp
#define REMOVE_DYB_PGOp
#define REMOVE_PPB_PGOp
#define REMOVE_WPWDOp
#define REMOVE_ABWROp
#define REMOVE_BRWROp
#define REMOVE_WASPOp
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp

// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (0xff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  HAS_NOTHING

#endif

#ifdef FL_P_MULTI_BIT_IO
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD
#define SPI_READ_IDENTIFICATION_CMD SPI_READID_90_CMD
#define SPI_READ_IDCFI_CMD SPI_READ_ID_9F_CMD

#define REMOVE_MPMCmd

#define REMOVE_Read_4BCmd
#define REMOVE_Fast_Read_4BCmd
#define REMOVE_DUALOUTPUT_READ_4BCmd
#define REMOVE_QUADOUTPUT_READ_4BCmd
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd
#define REMOVE_PP_4BCmd
#define REMOVE_P8E_4BCmd
#define REMOVE_SE_4BCmd
#define REMOVE_ERS_SSPCmd
#define REMOVE_ERS_RESCmd
#define REMOVE_SRSTCmd
#define REMOVE_RASPCmd
#define REMOVE_WASPCmd
#define REMOVE_RPWDCmd
#define REMOVE_WPWDCmd
#define REMOVE_ECCRDCmd
#define REMOVE_BRRDCmd
#define REMOVE_ABRDCmd
#define REMOVE_BRWRCmd
#define REMOVE_ABWRCmd
#define REMOVE_RCVRCmd
#define REMOVE_RCSPCmd
#define REMOVE_RCRSCmd
#define REMOVE_PPB_PGCmd
#define REMOVE_DYB_PGCmd
#define REMOVE_PP_4BOp
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp
#define REMOVE_P8E_4BOp
#define REMOVE_SE_4BOp
#define REMOVE_BufferedProgram_4BOp
#define REMOVE_DYB_PGOp
#define REMOVE_PPB_PGOp
#define REMOVE_WPWDOp
#define REMOVE_ABWROp
#define REMOVE_BRWROp
#define REMOVE_WASPOp
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp

// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (0xff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  (HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR)

#endif

#ifdef FL_K
#define FLK_CMD

#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD
#define SPI_READ_IDENTIFICATION_CMD SPI_READID_90_CMD
#define SPI_READ_IDCFI_CMD SPI_READ_ID_9F_CMD

#define INCL_SPI_READSFDPCMD
#define INCL_SPI_READMODERESETCMD

#define REMOVE_OTPRCmd
//#define REMOVE_SPCmd
#define REMOVE_MPMCmd
#define REMOVE_Read_4BCmd
#define REMOVE_Fast_Read_4BCmd
#define REMOVE_DUALOUTPUT_READ_4BCmd
#define REMOVE_QUADOUTPUT_READ_4BCmd
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd
#define REMOVE_PP_4BCmd
#define REMOVE_P8E_4BCmd
#define REMOVE_SE_4BCmd
//#define REMOVE_ERS_SSPCmd
//#define REMOVE_ERS_RESCmd
#define REMOVE_SRSTCmd
#define REMOVE_RASPCmd
#define REMOVE_WASPCmd
#define REMOVE_RPWDCmd
#define REMOVE_WPWDCmd
#define REMOVE_ECCRDCmd
#define REMOVE_BRRDCmd
#define REMOVE_ABRDCmd
#define REMOVE_BRWRCmd
#define REMOVE_ABWRCmd
#define REMOVE_RCVRCmd
#define REMOVE_RCSPCmd
#define REMOVE_RCRSCmd
#define REMOVE_PPB_PGCmd
#define REMOVE_DYB_PGCmd
#define REMOVE_PP_4BOp
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp
#define REMOVE_P8E_4BOp
#define REMOVE_SE_4BOp
#define REMOVE_BufferedProgram_4BOp
#define REMOVE_DYB_PGOp
#define REMOVE_PPB_PGOp
#define REMOVE_WPWDOp
#define REMOVE_ABWROp
#define REMOVE_BRWROp
#define REMOVE_WASPOp
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp

// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (0xff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  (HAS_NOTHING)

#endif

#if (defined FL_S)
#define FLS_CMD

#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD
#define SPI_READ_IDENTIFICATION_CMD SPI_READID_90_CMD
#define SPI_READ_IDCFI_CMD SPI_READ_ID_9F_CMD

#define INCL_SPI_READSFDPCMD
#define INCL_SPI_READMODERESETCMD
#define REMOVE_MPMCmd
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp

// Flash page size
#define PAGE_SIZE (512) // (512)
#define PAGE_MASK (PAGE_SIZE-1)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  (HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR)

#endif

#if (defined FL_T)
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD
#define SPI_READ_IDENTIFICATION_CMD SPI_READID_90_CMD
#define SPI_READ_IDCFI_CMD SPI_READ_ID_9F_CMD

#define REMOVE_P4EOp
#define REMOVE_P4ECmd
#define REMOVE_MPMCmd
#define REMOVE_BLOCKERASECmd
#define REMOVE_BLOCKERASEOp

// Flash page size
#define PAGE_SIZE (512)
#define PAGE_MASK (0x1ff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  (HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR)

#endif


#ifdef FL_2K

#define FL2K_CMD
#define SPI_READ_ID_CMD SPI_RDID_AB_CMD
#define SPI_READ_IDENTIFICATION_CMD SPI_READID_90_CMD
#define SPI_RDID_CMD SPI_RDID_9F_CMD


#define REMOVE_PP_4BOp
#define REMOVE_BufferedProgramOp
#define REMOVE_BufferedProgram_4BOp
#define REMOVE_SEOp
#define REMOVE_SE_4BOp
#define REMOVE_BlockProtectOp
#define REMOVE_Read_4BCmd 
#define REMOVE_Fast_Read_4BCmd
#define REMOVE_SRSTCmd
#define REMOVE_RASPCmd
#define REMOVE_BRRDCmd
#define REMOVE_ABRDCmd
#define REMOVE_ECCRDCmd
#define REMOVE_RPWDCmd
#define REMOVE_MULTIOMODE_FUNCTIONS
#define REMOVE_WRRCmd
#define REMOVE_WRROp
#define REMOVE_WASPCmd
#define REMOVE_WASPOp
#define REMOVE_BRWRCmd
#define REMOVE_BRWROp
#define REMOVE_ABWRCmd
#define REMOVE_ABWROp
#define REMOVE_WPWDCmd
#define REMOVE_WPWDOp
#define REMOVE_RCRCmd
#define REMOVE_OTPRCmd
#define REMOVE_OTPPCmd
#define REMOVE_OTPPOp
#define REMOVE_P8ECmd
#define REMOVE_P8E_4BCmd
#define REMOVE_P8EOp
#define REMOVE_P8E_4BOp
#define REMOVE_DUALOUTPUT_READ_4BCmd
#define REMOVE_QUADOUTPUT_READCmd
#define REMOVE_QUADOUTPUT_READ_4BCmd
#define REMOVE_DUALOUTPUT_HP_READCmd
#define REMOVE_DUALOUTPUT_HP_READ_4BCmd
#define REMOVE_QUADOUTPUT_HP_READCmd
#define REMOVE_QUADOUTPUT_HP_READ_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAMCmd
#define REMOVE_QUAD_PAGE_PROGRAM_4BCmd
#define REMOVE_QUAD_PAGE_PROGRAMOp
#define REMOVE_MULTIOMODE_FUNCTIONS
#define REMOVE_QUAD_PAGE_PROGRAM_4BOp
#define REMOVE_MPMCmd
#define REMOVE_PP_4BCmd
#define REMOVE_PPB_PGCmd
#define REMOVE_PPB_PGOp
#define REMOVE_DYB_PGCmd
#define REMOVE_DYB_PGOp
#define REMOVE_SE_4BCmd
#define REMOVE_ERS_SSPCmd
#define REMOVE_ERS_RESCmd
#define REMOVE_RCSPCmd
#define REMOVE_RCRSCmd
#define REMOVE_RCVRCmd
#define REMOVE_SPCmd
#define REMOVE_WDBRDCmd 
#define REMOVE_WDBRDOp
#define REMOVE_WDBPCmd
#define REMOVE_WDBPOp

// Flash page size
#define PAGE_SIZE (256)
#define PAGE_MASK (0xff)

// Status register error bits definition
#define STATUS_ERROR_FLAGS  (HAS_NOTHING)

#endif 

// =======================
// SPI Flash Commands info
// =======================

#define SPI_WRSR_CMD                (0x01)
#define SPI_WRR_CMD                 (0x01)
#define SPI_PP_CMD                  (0x02)
#define SPI_READ_CMD                (0x03)
#define SPI_WRDI_CMD                (0x04)
#define SPI_RDSR_CMD                (0x05)
#define SPI_WREN_CMD                (0x06)
#define SPI_RDSR2_CMD               (0x07)  // Read Status Register-2
#define SPI_FAST_READ_CMD           (0x0B)
#define SPI_FAST_READ_4B_CMD        (0x0C)
#define SPI_FAST_READ_DDR_CMD       (0x0D)  // Reserved
#define SPI_FAST_READ_DDR_4B_CMD    (0x0E)  // Reserved
#define SPI_PP_4B_CMD               (0x12)
#define SPI_READ_4B_CMD             (0x13)
#define SPI_ABRD_CMD                (0x14)  // AutoBoot Register Read
#define SPI_ABWR_CMD                (0x15)  // AutoBoot Register Write
#define SPI_BRRD_CMD                (0x16)  // Bank Register Read
#define SPI_BRWR_CMD                (0x17)  // Bank Register Write
#define SPI_ECCRD_CMD               (0x18)  // ECC Read
#define SPI_P4E_CMD                 (0x20)
#define SPI_P4E4_CMD                (0x21)  // Parameter 4K-sector Erase (4Byte Addr)
#define SPI_RASP_CMD                (0x2B)
#define SPI_WASP_CMD                (0x2F)
#define SPI_CLSR_CMD                (0x30)
#define SPI_QPP_CMD                 (0x32)
#define SPI_QPP_4B_CMD              (0x34)
#define SPI_RCR_CMD                 (0x35)
#define SPI_QPP2_CMD                (0x38)  // Quad Page Program (3Byte Addr)
#define SPI_DUALIO_RD_CMD           (0x3B)
#define SPI_DUALIO_RD_4B_CMD        (0x3C)
#define SPI_P8E_CMD                 (0x40)
#define SPI_DLPRD_CMD               (0x41)  // Read Data Learning Pattern
#define SPI_OTPP_CMD                (0x42)
#define SPI_PROGRAM_SECURITY_CMD    (0x42)  // Program Security Register
#define SPI_PNVDLR_CMD              (0x43)  // Program NV Data Learning Register
#define SPI_ERASE_SECURITY_CMD      (0x44)  // Erase Security Register
#define SPI_READ_SECURITY_CMD       (0x48)  // Read Security Register
#define SPI_WVDLR_CMD               (0x4A)  // Write Volatile Data Learning Register
#define SPI_OTPR_CMD                (0x4B)
#define SPI_READ_UNIQUE_ID_CMD      (0x4B)  // Read Unique ID Number
#define SPI_P8E_4B_CMD              (0x4C)
#define SPI_WRITE_VOLATILE_CMD      (0x50)  // Write Enable for Volatile Status Register
#define SPI_BE32KB_CMD              (0x52)  // Block Erase 32KB
#define SPI_READ_SFDP_CMD           (0x5A)  // Read Serial Flash Discoverable Parameter Register
#define SPI_BE1_CMD                 (0x60)  // Bulk Erase
#define SPI_QUADIO_RD_CMD           (0x6B)
#define SPI_QUADIO_RD_4B_CMD        (0x6C)
#define SPI_ERS_SSP_CMD             (0x75)  // Erase / Program Suspend
#define SPI_SETBURSTWRAP_CMD        (0x77)  // Set Burst with Wrap
#define SPI_ERS_RES_CMD             (0x7A)  // Erase / Program Resume
#define SPI_PGSP_CMD                (0x85)  // Program Suspend
#define SPI_PGRS_CMD                (0x8A)  // Program Resume
#define SPI_READID_90_CMD           (0x90)
#define SPI_READID_DUAL_CMD         (0x92)  // Read Device ID by Dual
#define SPI_READID_QUAD_CMD         (0x94)  // Read Device ID by Quad
#define SPI_RDID_9F_CMD             (0x9F)
#define SPI_READ_ID_9F_CMD          (0x9F)
#define SPI_MPM_CMD                 (0xA3)
#define SPI_PLBWR_CMD               (0xA6)  // PPB Lock Bit Write
#define SPI_PLBRD_CMD               (0xA7)  // PPB Lock Bit Read
#define SPI_READ_ID_AB_CMD          (0xAB)
#define SPI_RDID_AB_CMD             (0xAB)
#define SPI_RES_CMD                 (0xAB)
#define SPI_SP_CMD                  (0xB9)
#define SPI_DP_CMD                  (0xB9)
#define SPI_DUALIO_HPRD_CMD         (0xBB)
#define SPI_DUALIO_HPRD_4B_CMD      (0xBC)
#define SPI_DDR_DUALIO_HPRD_CMD     (0xBD)  // Reserved
#define SPI_DDR_DUALIO_HPRD_4B_CMD  (0xBE)  // Reserved
#define SPI_BE_CMD                  (0xC7)  // BULK ERASE
#define SPI_BLOCK_ERASE_CMD         (0xD8)  // BLOCK ERASE
#define SPI_SE_CMD                  (0xD8)
#define SPI_SE_4B_CMD               (0xDC)
#define SPI_DYB_RD_CMD              (0xE0)  // DYB Read
#define SPI_DYB_PG_CMD              (0xE1)  // DYB Write
#define SPI_PPB_RD_CMD              (0xE2)  // PPB Read
#define SPI_PPB_PG_CMD              (0xE3)  // PPB Write
#define SPI_OCTALWORDREADQUAD_CMD   (0xE3)  // Octal Word Read Quad
#define SPI_PPB_ERS_CMD             (0xE4)  // PPB Erase
#define SPI_WDBRD_CMD               (0xE5)  // WDB Read
#define SPI_WDBP_CMD                (0xE6)  // WDB Program
#define SPI_PASSRD_CMD              (0xE7)  // Password Read
#define SPI_RPWD_CMD                (0xE7)
#define SPI_WORDREADQUAD_CMD        (0xE7)  // Word Read Quad
#define SPI_PASSP_CMD               (0xE8)  // Password Program
#define SPI_WPWD_CMD                (0xE8)
#define SPI_PASSU_CMD               (0xE9)  // Password Unlock
#define SPI_QUADIO_HPRD_CMD         (0xEB)
#define SPI_QUADIO_HPRD_4B_CMD      (0xEC)
#define SPI_DDR_QUADIO_HPRD_CMD     (0xED)  // Reserved
#define SPI_DDR_QUADIO_HPRD_4B_CMD  (0xEE)  // Reserved
#define SPI_SOFTWARE_RESET          (0xF0)
#define SPI_RMB_CMD                 (0xFF)
#define SPI_READMODE_RESET_CMD      (0xFF)  // Continuous Read Mode Reset

#define SPI_WRB_PP_CMD              (0x00) // Reserved: Write buffer autodetect programming
#define SPI_RCVR_CMD                (0x00) // Reserved: Initiate recovery mode (manually refreshing ECC)
#define SPI_RCSP_CMD                (0x00) // Reserved: Recovery suspend
#define SPI_RCRS_CMD                (0x00) // Reserved: Recovery resume

/* Voltage States */
#define SLLD_P_VIL                  (0x0001)
#define SLLD_P_VIH                  (0x0002)

/* LLD System Specific Typedefs */
typedef unsigned short LLD_UINT16;  /* 16 bits wide */
typedef unsigned long  LLD_UINT32;  /* 32 bits wide */
typedef LLD_UINT32     ADDRESS;     /* Used for system level addressing */

/* SLLD System Specific Typedefs */
typedef unsigned char  BYTE;   /* 8 bits wide */
typedef unsigned short WORD;   /* 16 bits wide */
typedef unsigned long  DWORD;  /* 32 bits wide */
typedef BYTE FLASHDATA;

/* SLLD Internal Data Types */
//typedef DWORD PARAM;            /* MUST be at least 32 bits wide */
typedef unsigned long BYTECOUNT;  /* used for multi-byte operations */

/* boolean macros */
#ifndef TRUE
 #define TRUE  (1)
#endif
#ifndef FALSE
 #define FALSE (0)
#endif

/* data mask */
#define B0_MASK                  (0x01)
#define B1_MASK                  (0x02)
#define B2_MASK                  (0x04)
#define B3_MASK                  (0x08)
#define B5_MASK                  (0x20)
#define B6_MASK                  (0x40)
#define B7_MASK                  (0x80)

#define LLD_DEV_READ_MASK        (0xFF)
#define BLOCK_PROTECT_BITS_MASK  (0x1C)
#define LLD_BYTES_PER_OP         (0x01)

/*Device status */
/*Flash Software protect status */
typedef enum {
 FLASH_SOFTWARE_UNPROTECTED = 0,
 FLASH_SOFTWARE_PROTECTED
} DEV_SOFTWARE_PROTECT_STATUS;

// Flash embedded operation status
typedef enum {
 dev_status_unknown = 0,
 dev_not_busy,
 dev_program_error,
 dev_erase_error,
 dev_suspend,
 dev_busy
} DEVSTATUS;

// SLLD Returned values
typedef enum {
 SLLD_OK = 0x0,
 SLLD_E_DEVICE_SOFTWARE_PROTECTED,
 SLLD_E_HAL_ERROR = 0x200,
 SLLD_ERROR = 0xFFFF
} SLLD_STATUS;

// FLASH_RD and FLASH_WR functions needed defines
#define ADDRESS_NOT_USED 0xFFFFFFFF
#define BUFFER_NOT_USED  (BYTE*)0

extern BYTE modebit_char;

// Include Nor Super Tests
//#define NST_TESTS

/* Include for Competitive Analysis Test */
/* #define CA_TEST */

/* public function prototypes */

/* Operation Functions */

#ifdef FLK_CMD
extern SLLD_STATUS slld_ReadSecurityCmd 
(
ADDRESS   sys_addr,                           // device address given by system
BYTE     *read_buf,                           // data buffer
BYTECOUNT len_in_bytes                        // number of bytes
);

extern SLLD_STATUS slld_ProgramSecurityCmd 
(
ADDRESS   sys_addr,                           // device address given by system
BYTE     *program_buf,                        // variable in which to store programmed data
BYTECOUNT len_in_bytes                        // number of bytes
);

extern SLLD_STATUS slld_EraseSecurityCmd 
(
ADDRESS   sys_addr                           // device address given by system
);

extern SLLD_STATUS slld_ProgramSecurityOp
(
ADDRESS                    sys_addr,        // device address given by system
BYTE                    *program_buf,       // variable in which to store programmed data
BYTECOUNT          len_in_bytes,            // number of bytes
DEVSTATUS          *dev_status_ptr          // variable to store device status
);

extern SLLD_STATUS slld_EraseSecurityOp
(
ADDRESS                    sys_addr,        // device address given by system
DEVSTATUS          *dev_status_ptr          // variable to store device status
);

extern SLLD_STATUS slld_ReadIdDualCmd
(
ADDRESS                    sys_addr,       // device address given by system
BYTE                    *read_buf,         // variable in which to store read data
BYTE                    modebit,           // mode bit
BYTECOUNT          len_in_bytes            // length in bytes
);

extern SLLD_STATUS slld_ReadIdQuadCmd
(
ADDRESS                    sys_addr,      // device address given by system
BYTE                    *read_buf,        // variable in which to store read data
BYTE                    modebit,          // mode bit
BYTECOUNT          len_in_bytes           // length in bytes
);

extern SLLD_STATUS slld_BE32KBCmd
(
ADDRESS  sys_addr                         // device address given by system
);

extern SLLD_STATUS slld_BE32KBOp
(
ADDRESS  sys_addr,                         // device address given by system
DEVSTATUS          *dev_status_ptr         // variable to store device status
);

extern SLLD_STATUS slld_WordReadQuadCmd
(
ADDRESS                    sys_addr,       // device address given by system
BYTE                    *read_buf,         // variable in which to store read data
BYTE                    modebit,           // mode bit
BYTECOUNT          len_in_bytes            // length in bytes
);

extern SLLD_STATUS slld_ReadUniqueIDCmd
(
BYTE      *read_buf                        // variable in which to store read data
);


extern SLLD_STATUS slld_SetBurstWrapCmd
(
BYTE      *warpbit_buf                     // variable in which to store wrapbit data
);

extern SLLD_STATUS slld_OctalWordReadQuadCmd
(
ADDRESS                    sys_addr,       // device address given by system
BYTE                    *read_buf,         // variable in which to store read data
BYTE                    modebit,           // mode bit
BYTECOUNT          len_in_bytes            // length in bytes
);

extern SLLD_STATUS slld_WriteVolatileCmd(void);

#endif // FLK_CMD



#ifndef REMOVE_BLOCKERASEOp

SLLD_STATUS slld_BlockEraseOp
(
ADDRESS     sys_addr,             /* device address given by system */
DEVSTATUS  *dev_status_ptr       /* variable to store device status */
);
#endif

#ifndef REMOVE_PPOp
extern SLLD_STATUS slld_PPOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
BYTECOUNT   len_in_bytes,             /* number of bytes on which to operate */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);

#endif

#ifndef REMOVE_PP_4BOp
extern SLLD_STATUS slld_PP_4BOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
BYTECOUNT   len_in_bytes,             /* number of bytes on which to operate */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_BufferedProgramOp
extern SLLD_STATUS slld_BufferedProgramOp
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes,              /* number of bytes on which to operate */
DEVSTATUS *dev_status_ptr             /* variable to store device status */
);
#endif

#ifndef REMOVE_BufferedProgram_4BOp
extern SLLD_STATUS slld_BufferedProgram_4BOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
BYTECOUNT   len_in_bytes,             /* number of bytes on which to operate */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_SEOp
extern SLLD_STATUS slld_SEOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_SE_4BOp
extern SLLD_STATUS slld_SE_4BOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_BEOp
extern SLLD_STATUS slld_BEOp
(
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);

extern SLLD_STATUS slld_BE1Op
(
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_WRSROp
extern SLLD_STATUS slld_WRSROp
(
BYTE       *data_buf,                 /* variable containing data to program */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_BlockProtectOp
extern SLLD_STATUS slld_BlockProtectOp
(
BYTE        bpb_value,                /* block protect bit value */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

/* Command Functions */

/* Read Operations */

#ifndef REMOVE_ReadCmd
extern SLLD_STATUS slld_ReadCmd
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *target,                   /* variable in which to store read data */
BYTECOUNT   len_in_bytes              /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_Read_4BCmd
extern SLLD_STATUS slld_Read_4BCmd
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *target,                   /* variable in which to store read data */
BYTECOUNT   len_in_bytes              /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_Fast_ReadCmd
extern SLLD_STATUS slld_Fast_ReadCmd
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *target,                   /* variable in which to store read data */
BYTECOUNT   len_in_bytes              /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_Fast_Read_4BCmd
extern SLLD_STATUS slld_Fast_Read_4BCmd
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *target,                   /* variable in which to store read data */
BYTECOUNT   len_in_bytes              /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_Read_IDCmd
extern SLLD_STATUS slld_Read_IDCmd
(
BYTE    *target                       /* variable in which to store read data */
);
#endif

#ifndef REMOVE_RDIDCmd
extern SLLD_STATUS slld_RDIDCmd
(
BYTE      *target,                    /* variable in which to store read data */
BYTECOUNT  len_in_bytes               /* number of bytes to read */
);
#endif

#ifndef REMOVE_READ_IDENTIFICATIONCmd
SLLD_STATUS slld_Read_IdentificationCmd
(
BYTE    *target,                      /* variable in which to store read data */
ADDRESS  addr                         /* address offset for the command */
);
#endif

void slld_VersionCmd (void);

#ifndef REMOVE_SRSTCmd
SLLD_STATUS slld_SRSTCmd (void);
#endif

/* Registers Operations */

#ifndef REMOVE_RDSRCmd
extern SLLD_STATUS slld_RDSRCmd
(
BYTE    *target                       /* variable in which to store read data */
);
#endif

#ifndef REMOVE_RASPCmd
extern SLLD_STATUS slld_RASPCmd
(
WORD    *target                       /* variable in which to store read data */
);
#endif

#ifndef REMOVE_BRRDCmd
extern SLLD_STATUS slld_BRRDCmd
(
BYTE    *target                       /* variable in which to store the bank addressing register value */
);
#endif

#ifndef REMOVE_ABRDCmd
extern SLLD_STATUS slld_ABRDCmd
(
DWORD    *target                       /* variable in which to store the autoboot register value */
);
#endif

#ifndef REMOVE_ECCRDCmd
extern SLLD_STATUS slld_ECCRDCmd
(
ADDRESS  sys_addr,                    /* cache line address given by system */
BYTE     *target                      /* variable in which to store read data */
);
#endif

#ifndef REMOVE_RPWDCmd
extern SLLD_STATUS slld_RPWDCmd
(
BYTE    *target                       /* variable in which to store read data */
);
#endif

#ifndef REMOVE_WRENCmd
extern SLLD_STATUS slld_WRENCmd (void);
#endif

#ifndef REMOVE_WRDICmd
extern SLLD_STATUS slld_WRDICmd (void);
#endif

#ifndef REMOVE_WRSRCmd
extern SLLD_STATUS slld_WRSRCmd
(
BYTE   *data_buf                      /* variable containing data to program */
);
#endif

#ifndef REMOVE_MULTIOMODE_FUNCTIONS

#ifndef REMOVE_CLSRCmd
extern SLLD_STATUS slld_ClearStatusRegisterCmd (void);
#endif

#ifndef REMOVE_P4ECmd
extern SLLD_STATUS slld_P4ECmd
(
ADDRESS  sys_addr                     /* device address given by system */
);
#endif

#ifndef REMOVE_P4EOp
extern SLLD_STATUS slld_P4EOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_WRRCmd
SLLD_STATUS slld_WRRCmd
(
BYTE    *status_val,                  /* variable containing data to program the status register */
BYTE    *config_val                   /* variable containing data to program the config register */
);
#endif

#ifndef REMOVE_WRROp
SLLD_STATUS slld_WRROp
(
BYTE       *status_val,               /* variable containing data to program the status register */
BYTE       *config_val,               /* variable containing data to program the config register */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_WASPCmd
SLLD_STATUS slld_WASPCmd
(
WORD    *asp_val                      /* variable containing data to program to the ASP register */
);
#endif

#ifndef REMOVE_WASPOp
SLLD_STATUS slld_WASPOp
(
WORD       *asp_val,                  /* variable containing data to program to the ASP register */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_BRWRCmd
SLLD_STATUS slld_BRWRCmd
(
BYTE    *bnk_val                      /* variable containing data to program to the bank addressing register */
);
#endif

#ifndef REMOVE_BRWROp
SLLD_STATUS slld_BRWROp
(
BYTE       *bnk_val,                  /* variable containing data to program to the bank addressing register */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_ABWRCmd
SLLD_STATUS slld_ABWRCmd
(
DWORD    *abt_val                      /* variable containing data to program to the autoboot register */
);
#endif

#ifndef REMOVE_ABWROp
SLLD_STATUS slld_ABWROp
(
DWORD       *abt_val,                  /* variable containing data to program to the autoboot register */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_WPWDCmd
SLLD_STATUS slld_WPWDCmd
(
BYTE     *target                      /* variable containing data to program to the ASP password */
);
#endif

#ifndef REMOVE_WPWDOp
SLLD_STATUS slld_WPWDOp
(
BYTE       *target,                   /* variable containing data to program to the ASP password */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_RCRCmd
SLLD_STATUS slld_RCRCmd
(
BYTE    *target                       /* variable in which to store read data */
);
#endif

#ifndef REMOVE_OTPRCmd
SLLD_STATUS slld_OTPRCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *target,                    /* variable in which to store read data */
BYTECOUNT  len_in_bytes               /* number of bytes to read */
);
#endif

#ifndef REMOVE_OTPPCmd
SLLD_STATUS slld_OTPPCmd
(
ADDRESS  sys_addr,                    /* device address given by system */
BYTE    *data_buf                     /* variable containing data to program */
);
#endif

#ifndef REMOVE_OTPPOp
SLLD_STATUS slld_OTPPOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_P8ECmd
extern SLLD_STATUS slld_P8ECmd
(
ADDRESS  sys_addr                     /* device address given by system */
);
#endif

#ifndef REMOVE_P8E_4BCmd
extern SLLD_STATUS slld_P8E_4BCmd
(
ADDRESS  sys_addr                     /* device address given by system */
);
#endif

#ifndef REMOVE_P8EOp
extern SLLD_STATUS slld_P8EOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_P8E_4BOp
extern SLLD_STATUS slld_P8E_4BOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_DUALOUTPUT_READCmd
extern SLLD_STATUS slld_DualIOReadCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *target,                    /* variable in which to store read data */
BYTECOUNT  len_in_bytes               /* number of bytes to read */
);
#endif

#ifndef REMOVE_DUALOUTPUT_READ_4BCmd
extern SLLD_STATUS slld_DualIORead_4BCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *target,                    /* variable in which to store read data */
BYTECOUNT  len_in_bytes               /* number of bytes to read */
);
#endif

#ifndef REMOVE_QUADOUTPUT_READCmd
extern SLLD_STATUS slld_QuadIOReadCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *target,                    /* variable in which to store read data */
BYTECOUNT  len_in_bytes               /* number of bytes to read */
);
#endif

#ifndef REMOVE_QUADOUTPUT_READ_4BCmd
extern SLLD_STATUS slld_QuadIORead_4BCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *target,                    /* variable in which to store read data */
BYTECOUNT  len_in_bytes               /* number of bytes to read */
);
#endif

#ifndef REMOVE_DUALOUTPUT_HP_READCmd
extern SLLD_STATUS slld_DualIOHPReadCmd
(
ADDRESS                    sys_addr,     // device address given by system */
BYTE                    *target,         // variable in which to store read data */
BYTE                    modebit,         // The read mode to be passed to the device */
BYTECOUNT          len_in_bytes          // number of bytes to read */
);
#endif

#ifndef REMOVE_DUALOUTPUT_HP_READ_4BCmd
extern SLLD_STATUS slld_DualIOHPRead_4BCmd
(
ADDRESS                    sys_addr,       // device address given by system
BYTE                    *target,           // variable in which to store read data
BYTE                    modebit,           // The read mode to be passed to the device
BYTECOUNT          len_in_bytes            // number of bytes to read
);
#endif

#ifndef REMOVE_QUADOUTPUT_HP_READCmd
extern SLLD_STATUS slld_QuadIOHPReadCmd
(
ADDRESS                    sys_addr,        // device address given by system
BYTE                    *target,            // variable in which to store read data
BYTE                    modebit,            // The read mode to be passed to the device
BYTECOUNT          len_in_bytes             // number of bytes to read
);
#endif

#ifndef REMOVE_QUADOUTPUT_HP_READ_4BCmd
extern SLLD_STATUS slld_QuadIOHPRead_4BCmd
(
ADDRESS                    sys_addr,         // device address given by system
BYTE                    *target,             // variable in which to store read data
BYTE                    modebit,             // The read mode to be passed to the device
BYTECOUNT          len_in_bytes              // number of bytes to read
);
#endif

#ifndef REMOVE_QUAD_PAGE_PROGRAMCmd
extern SLLD_STATUS slld_QPPCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_QUAD_PAGE_PROGRAM_4BCmd
extern SLLD_STATUS slld_QPP_4BCmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_QUAD_PAGE_PROGRAMOp
extern SLLD_STATUS slld_QPPOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
BYTECOUNT   len_in_bytes,             /* number of bytes on which to operate */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
extern SLLD_STATUS slld_QuadWriteOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
BYTECOUNT   len_in_bytes,             /* number of bytes on which to operate */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_QUAD_PAGE_PROGRAM_4BOp
extern SLLD_STATUS slld_QPP_4BOp
(
ADDRESS     sys_addr,                  /* device address given by system */
BYTE       *data_buf,                  /* variable containing data to program */
BYTECOUNT   len_in_bytes,              /* number of bytes on which to operate */
DEVSTATUS  *dev_status_ptr             /* variable to store device status */
);
#endif

#ifndef REMOVE_MPMCmd
extern SLLD_STATUS slld_MPMCmd (void);
#endif

#endif

/* Program Operations */

#ifndef REMOVE_PPCmd
extern SLLD_STATUS slld_PPCmd
(
ADDRESS      sys_addr,                /* device address given by system */
BYTE        *data_buf,                /* variable containing data to program */
BYTECOUNT    len_in_bytes             /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_PP_4BCmd
extern SLLD_STATUS slld_PP_4BCmd
(
ADDRESS      sys_addr,                /* device address given by system */
BYTE        *data_buf,                /* variable containing data to program */
BYTECOUNT    len_in_bytes             /* number of bytes on which to operate */
);
#endif

#ifndef REMOVE_PPB_PGCmd
extern SLLD_STATUS slld_PPB_PGCmd
(
ADDRESS  sys_addr                     /* device address given by system */
);
#endif

#ifndef REMOVE_PPB_PGOp
extern SLLD_STATUS slld_PPB_PGOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_DYB_PGCmd
extern SLLD_STATUS slld_DYB_PGCmd
(
ADDRESS   sys_addr,                   /* device address given by system */
BYTE     *data_buf                    /* variable containing data to program */
);
#endif

#ifndef REMOVE_DYB_PGOp
extern SLLD_STATUS slld_DYB_PGOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

/* Erase Operations */

#ifndef REMOVE_SECmd
extern SLLD_STATUS slld_SECmd
(
ADDRESS  sys_addr                     /* device address given by system */
);
#endif

#ifndef REMOVE_SE_4BCmd
extern SLLD_STATUS slld_SE_4BCmd
(
ADDRESS  sys_addr                     /* device address given by system */
);
#endif

#ifndef REMOVE_ERS_SSPCmd
extern SLLD_STATUS slld_ERS_SSPCmd (void);
#endif

#ifndef REMOVE_ERS_RESCmd
extern SLLD_STATUS slld_ERS_RESCmd (void);
#endif

#ifndef REMOVE_RCSPCmd
extern SLLD_STATUS slld_RCSPCmd (void);
#endif

#ifndef REMOVE_RCRSCmd
extern SLLD_STATUS slld_RCRSCmd (void);
#endif

#ifndef REMOVE_BECmd
extern SLLD_STATUS slld_BECmd (void);
extern SLLD_STATUS slld_BE1Cmd (void);
#endif

#ifndef REMOVE_BLOCKERASECmd
SLLD_STATUS slld_BlockEraseCmd (ADDRESS  sys_addr);
#endif

#ifndef REMOVE_RCVRCmd
extern SLLD_STATUS slld_RCVRCmd (void);
#endif

#ifndef REMOVE_SPCmd
extern SLLD_STATUS slld_SPCmd (void);
#endif


/* Power Saving Mode Operation */
#ifndef REMOVE_DPCmd
extern SLLD_STATUS slld_DPCmd (void);
#endif

#ifndef REMOVE_RESCmd
extern SLLD_STATUS slld_RESCmd (void);
#endif

/* Utility Functions */

#ifndef REMOVE_Poll
extern SLLD_STATUS slld_Poll
(
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifndef REMOVE_StatusGet
extern SLLD_STATUS slld_StatusGet
(
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#ifdef INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK
#ifndef REMOVE_SoftwareProtectStatusGet
extern SLLD_STATUS slld_SoftwareProtectStatusGet
(
DEV_SOFTWARE_PROTECT_STATUS*  dev_softwareprotect_status_ptr
                                      /* variable to store device software protect status */
);
#endif
#endif // INCLUDE_SOFTWARE_PROTECT_STATUS_CHECK

#ifdef FLS_CMD

#ifndef REMOVE_RDSR2Cmd
extern SLLD_STATUS slld_RDSR2Cmd
(
BYTE    *target                        /* variable in which to store read data */
);
#endif

#ifndef REMOVE_P4E_4BCmd
extern SLLD_STATUS slld_P4E_4BCmd
(
ADDRESS  sys_addr                          /* device address given by system */
);
#endif

#ifndef REMOVE_QUAD_PAGE_PROGRAMCmd
extern SLLD_STATUS slld_QPP2Cmd
(
ADDRESS    sys_addr,                  /* device address given by system */
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes to operate */
);
#endif

#ifndef REMOVE_DLPRDCmd
extern SLLD_STATUS slld_DLPRDCmd
(
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes to operate */
);
#endif

#ifndef REMOVE_PNVDLRCmd
extern SLLD_STATUS slld_PNVDLRCmd
(
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes to operate */
);
#endif

#ifndef REMOVE_WVDLRCmd
extern SLLD_STATUS slld_WVDLRCmd
(
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes to operate */
);
#endif

#ifndef REMOVE_PGSPCmd
extern SLLD_STATUS slld_PGSPCmd(void);
#endif

#ifndef REMOVE_PGRSCmd
extern SLLD_STATUS slld_PGRSCmd(void);
#endif

#ifndef REMOVE_PLBWRCmd
extern SLLD_STATUS slld_PLBWRCmd(void);
#endif

#ifndef REMOVE_PLBRDCmd
extern SLLD_STATUS slld_PLBRDCmd
(
BYTE      *data_buf,                  /* variable containing data to program */
BYTECOUNT  len_in_bytes               /* number of bytes to operate */
);
#endif

#ifndef REMOVE_DYB_RDCmd
extern SLLD_STATUS slld_DYB_RDCmd(void);
#endif

#ifndef REMOVE_PPB_RDCmd
extern SLLD_STATUS slld_PPB_RDCmd
(
ADDRESS   sys_addr,                      /* device address given by system */
BYTE     *data_buf                       /* variable containing data to program */
);
#endif /* REMOVE_PPB_RDCmd */

#ifndef REMOVE_PPB_ERSCmd
extern SLLD_STATUS slld_PPB_ERSCmd(void);
#endif /* REMOVE_PPB_ERSCmd */

#ifndef REMOVE_WDBRDCmd
SLLD_STATUS slld_WDBRDCmd
(
ADDRESS   sys_addr,                      /* device address given by system */
BYTE     *data_buf                       /* variable containing data to program */
);
#endif /* REMOVE_WDBRDCmd */

#ifndef REMOVE_WDBPCmd
extern SLLD_STATUS slld_WDBPCmd
(
ADDRESS   sys_addr                      /* device address given by system */
);
#endif /* REMOVE_WDBPCmd */

#ifndef REMOVE_PASSRDCmd
SLLD_STATUS slld_PASSRDCmd
(
ADDRESS   sys_addr,                      /* device address given by system */
BYTE     *data_buf                       /* variable containing data to program */
);
#endif /* REMOVE_PASSRDCmd */

#ifndef REMOVE_PASSPCmd
SLLD_STATUS slld_PASSPCmd
(
ADDRESS   sys_addr,                      /* device address given by system */
BYTE     *data_buf                       /* variable containing data to program */
);
#endif /* REMOVE_PASSPCmd */

#ifndef REMOVE_PASSUCmd
SLLD_STATUS slld_PASSUCmd
(
ADDRESS   sys_addr,                      /* device address given by system */
BYTE     *data_buf                       /* variable containing data to program */
);
#endif /* REMOVE_PASSUCmd */

#ifndef REMOVE_P4E_4BOp
extern SLLD_STATUS slld_P4E_4BOp
(
ADDRESS     sys_addr,                 /* device address given by system */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);
#endif

#endif // FLS_CMD


#ifdef INCL_SPI_READSFDPCMD
extern SLLD_STATUS slld_ReadSFDPCmd
(
ADDRESS    sys_addr,             // device address given by system
BYTE      *read_buf               // data buffer
);
#endif


#ifdef INCL_SPI_READMODERESETCMD
extern SLLD_STATUS slld_ReadModeResetCmd(void);
#endif

/*****************************************************
* Define Flash read/write macro to be used by SLLD   *
*****************************************************/
#ifdef TRACE
  #define FLASH_WR(c,a,d,n)    FlashWrite(c,a,d,n)
  #define FLASH_RD(c,a,d,n)    FlashRead(c,a,d,n)
#elif defined BOARD226
  #define FLASH_WR(c,a,d,n)    FLASH_WRITE(c,a,d,n)
  #define FLASH_RD(c,a,d,n)    FLASH_READ(c,a,d,n)
#else
  #define FLASH_WR(c,a,d,n)    FLASH_WRITE(c,a,d,n)
  #define FLASH_RD(c,a,d,n)    FLASH_READ(c,a,d,n)
#endif

#ifndef UCHAR
typedef unsigned char   UCHAR;
#endif
#ifndef USHORT
typedef unsigned short  USHORT;
#endif
#ifndef UINT
typedef unsigned int    UINT;
#endif
#ifndef ULONG
typedef unsigned long   ULONG;
#endif
#ifdef BOARD226
#include "slld_hal_226.h"
#endif

extern SLLD_STATUS slld_ReadOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *target,                   /* variable in which to store read data */
BYTECOUNT   len_in_bytes              /* number of bytes to read */

);

extern SLLD_STATUS slld_WriteOp
(
ADDRESS     sys_addr,                 /* device address given by system */
BYTE       *data_buf,                 /* variable containing data to program */
BYTECOUNT   len_in_bytes,             /* number of bytes to program */
DEVSTATUS  *dev_status_ptr            /* variable to store device status */
);

/*****************************************************************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INC_lldh */
