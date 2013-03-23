/**
 * @file        ITG3200.h
 * @author      Luis Maduro
 * @author      Jeff Rowberg
 * @version     1.0
 * @date        2011
 * @copyright	MIT license
 */

#ifndef _ITG3200_H_
#define _ITG3200_H_

#include "stdboolean.h"
#include "I2CDevice.h"

#define ITG3200_ADDRESS             0x68

#define ITG3200_RA_WHO_AM_I         0x00
#define ITG3200_RA_SMPLRT_DIV       0x15
#define ITG3200_RA_DLPF_FS          0x16
#define ITG3200_RA_INT_CFG          0x17
#define ITG3200_RA_INT_STATUS       0x1A
#define ITG3200_RA_TEMP_OUT_H       0x1B
#define ITG3200_RA_TEMP_OUT_L       0x1C
#define ITG3200_RA_GYRO_XOUT_H      0x1D
#define ITG3200_RA_GYRO_XOUT_L      0x1E
#define ITG3200_RA_GYRO_YOUT_H      0x1F
#define ITG3200_RA_GYRO_YOUT_L      0x20
#define ITG3200_RA_GYRO_ZOUT_H      0x21
#define ITG3200_RA_GYRO_ZOUT_L      0x22
#define ITG3200_RA_PWR_MGM          0x3E

#define ITG3200_DEVID_BIT           6
#define ITG3200_DEVID_LENGTH        6

#define ITG3200_DF_FS_SEL_BIT       4
#define ITG3200_DF_FS_SEL_LENGTH    2
#define ITG3200_DF_DLPF_CFG_BIT     2
#define ITG3200_DF_DLPF_CFG_LENGTH  3

#define ITG3200_FULLSCALE_2000      0x03

#define ITG3200_DLPF_BW_256         0x00
#define ITG3200_DLPF_BW_188         0x01
#define ITG3200_DLPF_BW_98          0x02
#define ITG3200_DLPF_BW_42          0x03
#define ITG3200_DLPF_BW_20          0x04
#define ITG3200_DLPF_BW_10          0x05
#define ITG3200_DLPF_BW_5           0x06

#define ITG3200_INTCFG_ACTL_BIT             7
#define ITG3200_INTCFG_OPEN_BIT             6
#define ITG3200_INTCFG_LATCH_INT_EN_BIT     5
#define ITG3200_INTCFG_INT_ANYRD_2CLEAR_BIT 4
#define ITG3200_INTCFG_ITG_RDY_EN_BIT       2
#define ITG3200_INTCFG_RAW_RDY_EN_BIT       0

#define ITG3200_INTMODE_ACTIVEHIGH  0x00
#define ITG3200_INTMODE_ACTIVELOW   0x01

#define ITG3200_INTDRV_PUSHPULL     0x00
#define ITG3200_INTDRV_OPENDRAIN    0x01

#define ITG3200_INTLATCH_50USPULSE  0x00
#define ITG3200_INTLATCH_WAITCLEAR  0x01

#define ITG3200_INTCLEAR_STATUSREAD 0x00
#define ITG3200_INTCLEAR_ANYREAD    0x01

#define ITG3200_INTSTAT_ITG_RDY_BIT         2
#define ITG3200_INTSTAT_RAW_DATA_READY_BIT  0

#define ITG3200_PWR_H_RESET_BIT     7
#define ITG3200_PWR_SLEEP_BIT       6
#define ITG3200_PWR_STBY_XG_BIT     5
#define ITG3200_PWR_STBY_YG_BIT     4
#define ITG3200_PWR_STBY_ZG_BIT     3
#define ITG3200_PWR_CLK_SEL_BIT     2
#define ITG3200_PWR_CLK_SEL_LENGTH  3

#define ITG3200_CLOCK_INTERNAL      0x00
#define ITG3200_CLOCK_PLL_XGYRO     0x01
#define ITG3200_CLOCK_PLL_YGYRO     0x02
#define ITG3200_CLOCK_PLL_ZGYRO     0x03
#define ITG3200_CLOCK_PLL_EXT32K    0x04
#define ITG3200_CLOCK_PLL_EXT19M    0x05

void ITG3200SetClockSource(unsigned char source);
unsigned char ITG3200GetClockSource(void);
void ITG3200SetStandbyZEnabled(boolean enabled);
boolean ITG3200GetStandbyZEnabled(void);
void ITG3200SetStandbyYEnabled(boolean enabled);
boolean ITG3200GetStandbyYEnabled(void);
void ITG3200SetStandbyXEnabled(boolean enabled);
boolean ITG3200GetStandbyXEnabled(void);
void ITG3200SetSleepEnabled(boolean enabled);
boolean ITG3200GetSleepEnabled(void);
void ITG3200Reset(void);
int ITG3200GetRotationZ(void);
int ITG3200GetRotationY(void);
int ITG3200GetRotationX(void);
void ITG3200GetRotation(int *x,int *y,int *z);
int ITG3200GetTemperature(void);
boolean ITG3200GetIntDataReadyStatus(void);
boolean ITG3200GetIntDeviceReadyStatus(void);
void ITG3200SetIntDataReadyEnabled(boolean enabled);
boolean ITG3200GetIntDataReadyEnabled(void);
void ITG3200SetIntDeviceReadyEnabled(boolean enabled);
boolean ITG3200GetIntDeviceReadyEnabled(void);
void ITG3200SetInterruptLatchClear(boolean clear);
boolean ITG3200GetInterruptLatchClear(void);
void ITG3200SetInterruptLatch(boolean latch);
boolean ITG3200GetInterruptLatch(void);
void ITG3200SetInterruptDrive(boolean drive);
boolean ITG3200getInterruptDrive(void);
void ITG3200SetInterruptMode(boolean mode);
boolean ITG3200GetInterruptMode(void);
void ITG3200SetDLPFBandwidth(unsigned char bandwidth);
unsigned char ITG3200GetDLPFBandwidth(void);
void ITG3200SetFullScaleRange(unsigned char range);
unsigned char ITG3200GetFullScaleRange(void);
void ITG3200SetRate(unsigned char rate);
unsigned char ITG3200GetRate(void);
void ITG3200SetDeviceID(unsigned char id);
unsigned char ITG3200GetDeviceID(void);
void ITG3200Initialize(void);

#endif

/**
 * @}
 */
