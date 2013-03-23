/**
 * @file        ADXL345.h
 * @brief       Drivers for ADXL345 Accelerometer
 * @author      Luis Maduro
 * @author      Jeff Rowberg
 * @version     1.0
 * @date        2011
 * @copyright   GNU Public License.
 */

#ifndef _ADXL345_H_
#define _ADXL345_H_

#include "stdboolean.h"
#include "I2CDevice.h"

#define ADXL345_ADDRESS             0x53

#define ADXL345_RA_DEVID            0x00
#define ADXL345_RA_RESERVED1        0x01
#define ADXL345_RA_THRESH_TAP       0x1D
#define ADXL345_RA_OFSX             0x1E
#define ADXL345_RA_OFSY             0x1F
#define ADXL345_RA_OFSZ             0x20
#define ADXL345_RA_DUR              0x21
#define ADXL345_RA_LATENT           0x22
#define ADXL345_RA_WINDOW           0x23
#define ADXL345_RA_THRESH_ACT       0x24
#define ADXL345_RA_THRESH_INACT     0x25
#define ADXL345_RA_TIME_INACT       0x26
#define ADXL345_RA_ACT_INACT_CTL    0x27
#define ADXL345_RA_THRESH_FF        0x28
#define ADXL345_RA_TIME_FF          0x29
#define ADXL345_RA_TAP_AXES         0x2A
#define ADXL345_RA_ACT_TAP_STATUS   0x2B
#define ADXL345_RA_BW_RATE          0x2C
#define ADXL345_RA_POWER_CTL        0x2D
#define ADXL345_RA_INT_ENABLE       0x2E
#define ADXL345_RA_INT_MAP          0x2F
#define ADXL345_RA_INT_SOURCE       0x30
#define ADXL345_RA_DATA_FORMAT      0x31
#define ADXL345_RA_DATAX0           0x32
#define ADXL345_RA_DATAX1           0x33
#define ADXL345_RA_DATAY0           0x34
#define ADXL345_RA_DATAY1           0x35
#define ADXL345_RA_DATAZ0           0x36
#define ADXL345_RA_DATAZ1           0x37
#define ADXL345_RA_FIFO_CTL         0x38
#define ADXL345_RA_FIFO_STATUS      0x39

#define ADXL345_AIC_ACT_AC_BIT      7
#define ADXL345_AIC_ACT_X_BIT       6
#define ADXL345_AIC_ACT_Y_BIT       5
#define ADXL345_AIC_ACT_Z_BIT       4
#define ADXL345_AIC_INACT_AC_BIT    3
#define ADXL345_AIC_INACT_X_BIT     2
#define ADXL345_AIC_INACT_Y_BIT     1
#define ADXL345_AIC_INACT_Z_BIT     0

#define ADXL345_TAPAXIS_SUP_BIT     3
#define ADXL345_TAPAXIS_X_BIT       2
#define ADXL345_TAPAXIS_Y_BIT       1
#define ADXL345_TAPAXIS_Z_BIT       0

#define ADXL345_TAPSTAT_ACTX_BIT    6
#define ADXL345_TAPSTAT_ACTY_BIT    5
#define ADXL345_TAPSTAT_ACTZ_BIT    4
#define ADXL345_TAPSTAT_ASLEEP_BIT  3
#define ADXL345_TAPSTAT_TAPX_BIT    2
#define ADXL345_TAPSTAT_TAPY_BIT    1
#define ADXL345_TAPSTAT_TAPZ_BIT    0

#define ADXL345_BW_LOWPOWER_BIT     4
#define ADXL345_BW_RATE_BIT         3
#define ADXL345_BW_RATE_LENGTH      4

#define ADXL345_RATE_3200           0b1111
#define ADXL345_RATE_1600           0b1110
#define ADXL345_RATE_800            0b1101
#define ADXL345_RATE_400            0b1100
#define ADXL345_RATE_200            0b1011
#define ADXL345_RATE_100            0b1010
#define ADXL345_RATE_50             0b1001
#define ADXL345_RATE_25             0b1000
#define ADXL345_RATE_12P5           0b0111
#define ADXL345_RATE_6P25           0b0110
#define ADXL345_RATE_3P13           0b0101
#define ADXL345_RATE_1P56           0b0100
#define ADXL345_RATE_0P78           0b0011
#define ADXL345_RATE_0P39           0b0010
#define ADXL345_RATE_0P20           0b0001
#define ADXL345_RATE_0P10           0b0000

#define ADXL345_PCTL_LINK_BIT       5
#define ADXL345_PCTL_AUTOSLEEP_BIT  4
#define ADXL345_PCTL_MEASURE_BIT    3
#define ADXL345_PCTL_SLEEP_BIT      2
#define ADXL345_PCTL_WAKEUP_BIT     1
#define ADXL345_PCTL_WAKEUP_LENGTH  2

#define ADXL345_WAKEUP_8HZ          0b00
#define ADXL345_WAKEUP_4HZ          0b01
#define ADXL345_WAKEUP_2HZ          0b10
#define ADXL345_WAKEUP_1HZ          0b11

#define ADXL345_INT_DATA_READY_BIT  7
#define ADXL345_INT_SINGLE_TAP_BIT  6
#define ADXL345_INT_DOUBLE_TAP_BIT  5
#define ADXL345_INT_ACTIVITY_BIT    4
#define ADXL345_INT_INACTIVITY_BIT  3
#define ADXL345_INT_FREE_FALL_BIT   2
#define ADXL345_INT_WATERMARK_BIT   1
#define ADXL345_INT_OVERRUN_BIT     0

#define ADXL345_FORMAT_SELFTEST_BIT 7
#define ADXL345_FORMAT_SPIMODE_BIT  6
#define ADXL345_FORMAT_INTMODE_BIT  5
#define ADXL345_FORMAT_FULL_RES_BIT 3
#define ADXL345_FORMAT_JUSTIFY_BIT  2
#define ADXL345_FORMAT_RANGE_BIT    1
#define ADXL345_FORMAT_RANGE_LENGTH 2

#define ADXL345_RANGE_2G            0b00
#define ADXL345_RANGE_4G            0b01
#define ADXL345_RANGE_8G            0b10
#define ADXL345_RANGE_16G           0b11

#define ADXL345_FIFO_MODE_BIT       7
#define ADXL345_FIFO_MODE_LENGTH    2
#define ADXL345_FIFO_TRIGGER_BIT    5
#define ADXL345_FIFO_SAMPLES_BIT    4
#define ADXL345_FIFO_SAMPLES_LENGTH 5

#define ADXL345_FIFO_MODE_BYPASS    0b00
#define ADXL345_FIFO_MODE_FIFO      0b01
#define ADXL345_FIFO_MODE_STREAM    0b10
#define ADXL345_FIFO_MODE_TRIGGER   0b11

#define ADXL345_FIFOSTAT_TRIGGER_BIT        7
#define ADXL345_FIFOSTAT_LENGTH_BIT         5
#define ADXL345_FIFOSTAT_LENGTH_LENGTH      6

void ADXL345Initialize(void);
unsigned char ADXL345GetDeviceID(void);
unsigned char ADXL345GetTapThreshold(void);
void ADXL345SetTapThreshold(unsigned char threshold);
void ADXL345GetOffset(char* x, char* y, char* z);
void ADXL345SetOffset(char x, char y, char z);
char ADXL345GetOffsetX(void);
void ADXL345SetOffsetX(char x);
char ADXL345GetOffsetY(void);
void ADXL345SetOffsetY(char y);
char ADXL345GetOffsetZ(void);
void ADXL345SetOffsetZ(char z);
unsigned char ADXL345GetTapDuration(void);
void ADXL345SetTapDuration(unsigned char duration);
unsigned char ADXL345GetDoubleTapLatency(void);
void ADXL345SetDoubleTapLatency(unsigned char latency);
unsigned char ADXL345GetDoubleTapWindow(void);
void ADXL345SetDoubleTapWindow(unsigned char window);
unsigned char ADXL345GetActivityThreshold(void);
void ADXL345SetActivityThreshold(unsigned char threshold);
unsigned char ADXL345GetInactivityThreshold(void);
void ADXL345SetInactivityThreshold(unsigned char threshold);
unsigned char ADXL345GetInactivityTime(void);
void ADXL345SetInactivityTime(unsigned char time);
boolean ADXL345GetActivityAC(void);
void ADXL345SetActivityAC(boolean enabled);
boolean ADXL345GetActivityXEnabled(void);
void ADXL345SetActivityXEnabled(boolean enabled);
boolean ADXL345GetActivityYEnabled(void);
void ADXL345SetActivityYEnabled(boolean enabled);
boolean ADXL345GetActivityZEnabled(void);
void ADXL345SetActivityZEnabled(boolean enabled);
boolean ADXL345GetInactivityAC(void);
void ADXL345SetInactivityAC(boolean enabled);
boolean ADXL345GetInactivityXEnabled(void);
void ADXL345SetInactivityXEnabled(boolean enabled);
boolean ADXL345GetInactivityYEnabled(void);
void ADXL345SetInactivityYEnabled(boolean enabled);
boolean ADXL345GetInactivityZEnabled(void);
void ADXL345SetInactivityZEnabled(boolean enabled);
unsigned char ADXL345GetFreefallThreshold(void);
void ADXL345SetFreefallThreshold(unsigned char threshold);
unsigned char ADXL345GetFreefallTime(void);
void ADXL345SetFreefallTime(unsigned char time);
boolean ADXL345GetTapAxisSuppress(void);
void ADXL345SetTapAxisSuppress(boolean enabled);
boolean ADXL345GetTapAxisXEnabled(void);
void ADXL345SetTapAxisXEnabled(boolean enabled);
boolean ADXL345GetTapAxisYEnabled(void);
void ADXL345SetTapAxisYEnabled(boolean enabled);
boolean ADXL345GetTapAxisZEnabled(void);
void ADXL345SetTapAxisZEnabled(boolean enabled);
boolean ADXL345GetActivitySourceX(void);
boolean ADXL345GetActivitySourceY(void);
boolean ADXL345GetActivitySourceZ(void);
boolean ADXL345GetAsleep(void);
boolean ADXL345GetTapSourceX(void);
boolean ADXL345GetTapSourceY(void);
boolean ADXL345GetTapSourceZ(void);
boolean ADXL345GetLowPowerEnabled(void);
void ADXL345SetLowPowerEnabled(boolean enabled);
unsigned char ADXL345GetRate(void);
void ADXL345SetRate(unsigned char rate);
boolean ADXL345GetLinkEnabled(void);
void ADXL345SetLinkEnabled(boolean enabled);
boolean ADXL345GetAutoSleepEnabled(void);
void ADXL345SetAutoSleepEnabled(boolean enabled);
boolean ADXL345GetMeasureEnabled(void);
void ADXL345SetMeasureEnabled(boolean enabled);
boolean ADXL345GetSleepEnabled(void);
void ADXL345SetSleepEnabled(boolean enabled);
unsigned char ADXL345GetWakeupFrequency(void);
void ADXL345SetWakeupFrequency(unsigned char frequency);
boolean ADXL345GetIntDataReadyEnabled(void);
void ADXL345SetIntDataReadyEnabled(boolean enabled);
boolean ADXL345GetIntSingleTapEnabled(void);
void ADXL345SetIntSingleTapEnabled(boolean enabled);
boolean ADXL345GetIntDoubleTapEnabled(void);
void ADXL345SetIntDoubleTapEnabled(boolean enabled);
boolean ADXL345GetIntActivityEnabled(void);
void ADXL345SetIntActivityEnabled(boolean enabled);
boolean ADXL345GetIntInactivityEnabled(void);
void ADXL345SetIntInactivityEnabled(boolean enabled);
boolean ADXL345GetIntFreefallEnabled(void);
void ADXL345SetIntFreefallEnabled(boolean enabled);
boolean ADXL345GetIntWatermarkEnabled(void);
void ADXL345SetIntWatermarkEnabled(boolean enabled);
boolean ADXL345GetIntOverrunEnabled(void);
void ADXL345SetIntOverrunEnabled(boolean enabled);
unsigned char ADXL345GetIntDataReadyPin(void);
void ADXL345SetIntDataReadyPin(unsigned char pin);
unsigned char ADXL345GetIntSingleTapPin(void);
void ADXL345SetIntSingleTapPin(unsigned char pin);
unsigned char ADXL345GetIntDoubleTapPin(void);
void ADXL345SetIntDoubleTapPin(unsigned char pin);
unsigned char ADXL345GetIntActivityPin(void);
void ADXL345SetIntActivityPin(unsigned char pin);
unsigned char ADXL345GetIntInactivityPin(void);
void ADXL345SetIntInactivityPin(unsigned char pin);
unsigned char ADXL345GetIntFreefallPin(void);
void ADXL345SetIntFreefallPin(unsigned char pin);
unsigned char ADXL345GetIntWatermarkPin(void);
void ADXL345SetIntWatermarkPin(unsigned char pin);
unsigned char ADXL345GetIntOverrunPin(void);
void ADXL345SetIntOverrunPin(unsigned char pin);
unsigned char ADXL345GetIntDataReadySource(void);
unsigned char ADXL345GetIntSingleTapSource(void);
unsigned char ADXL345GetIntDoubleTapSource(void);
unsigned char ADXL345GetIntActivitySource(void);
unsigned char ADXL345GetIntInactivitySource(void);
unsigned char ADXL345GetIntFreefallSource(void);
unsigned char ADXL345GetIntWatermarkSource(void);
unsigned char ADXL345GetIntOverrunSource(void);
unsigned char ADXL345GetSelfTestEnabled(void);
void ADXL345SetSelfTestEnabled(unsigned char enabled);
unsigned char ADXL345GetSPIMode(void);
void ADXL345SetSPIMode(unsigned char mode);
unsigned char ADXL345GetInterruptMode(void);
void ADXL345SetInterruptMode(unsigned char mode);
unsigned char ADXL345GetFullResolution(void);
void ADXL345SetFullResolution(unsigned char resolution);
unsigned char ADXL345GetDataJustification(void);
void ADXL345SetDataJustification(unsigned char justification);
unsigned char ADXL345GetRange(void);
void ADXL345SetRange(unsigned char range);
void ADXL345GetAcceleration(int* x, int* y, int* z);
int ADXL345GetAccelerationX(void);
int ADXL345GetAccelerationY(void);
int ADXL345GetAccelerationZ(void);
unsigned char ADXL345GetFIFOMode(void);
void ADXL345SetFIFOMode(unsigned char mode);
unsigned char ADXL345GetFIFOTriggerInterruptPin(void);
void ADXL345SetFIFOTriggerInterruptPin(unsigned char _interrupt);
unsigned char ADXL345GetFIFOSamples(void);
void ADXL345SetFIFOSamples(unsigned char size);
boolean ADXL345GetFIFOTriggerOccurred(void);
unsigned char ADXL345GetFIFOLength(void);



#endif
