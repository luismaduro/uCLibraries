/**
 * @file        HMC5883L.c
 * @author      Luis Maduro
 * @author      Jeff Rowberg
 * @version     1.0
 * @date        2011
 * @copyright 	MIT license
 */
#include "I2CDevice.h"
#include "HMC5883L.h"

unsigned char HMC5883LBuffer[6];
unsigned char mode;

/**
 * Power on and prepare for general usage.
 * This will prepare the magnetometer with default settings, ready for single-
 * use mode (very low power requirements). Default settings include 8-sample
 * averaging, 15 Hz data output rate, normal measurement bias, a,d 1090 gain (in
 * terms of LSB/Gauss). Be sure to adjust any settings you need specifically
 * after initialization, especially the gain settings if you happen to be seeing
 * a lot of -4096 values (see the datasheet for mor information).
 */
void HMC5883LInitialize(void)
{
    // write CONFIG_A register
    //I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,HMC5883L_RA_CONFIG_A, 0b01110000);
    I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE, HMC5883L_RA_CONFIG_A,
                       (HMC5883L_AVERAGING_8 << (HMC5883L_CRA_AVERAGE_BIT - HMC5883L_CRA_AVERAGE_LENGTH + 1)) |
                       (HMC5883L_RATE_15 << (HMC5883L_CRA_RATE_BIT - HMC5883L_CRA_RATE_LENGTH + 1)) |
                       (HMC5883L_BIAS_NORMAL << (HMC5883L_CRA_BIAS_BIT - HMC5883L_CRA_BIAS_LENGTH + 1)));

    // write CONFIG_B register
    HMC5883LSetGain(HMC5883L_GAIN_1090);

    // write MODE register
    HMC5883LSetMode(HMC5883L_MODE_SINGLE);
}

/**
 * Get number of samples averaged per measurement.
 * @return Current samples averaged per measurement (0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_AVERAGING_8
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
unsigned char HMC5883LGetSampleAveraging(void)
{
    return I2CDeviceReadBits(HMC5883L_ADDRESS_READ,
                             HMC5883L_RA_CONFIG_A,
                             HMC5883L_CRA_AVERAGE_BIT,
                             HMC5883L_CRA_AVERAGE_LENGTH);
}

/**
 * Set number of samples averaged per measurement.
 * @param averaging New samples averaged per measurement
 * setting(0-3 for 1/2/4/8 respectively)
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_AVERAGE_BIT
 * @see HMC5883L_CRA_AVERAGE_LENGTH
 */
void HMC5883LSetSampleAveraging(unsigned char averaging)
{
    I2CDeviceWriteBits(HMC5883L_ADDRESS_WRITE,
                       HMC5883L_RA_CONFIG_A,
                       HMC5883L_CRA_AVERAGE_BIT,
                       HMC5883L_CRA_AVERAGE_LENGTH,
                       averaging);
}

/**
 * Get data output rate value.
 * The Table below shows all selectable output rates in continuous measurement
 * mode. All three channels shall be measured within a given output rate. Other
 * output rates with maximum rate of 160 Hz can be achieved by monitoring DRDY
 * interrupt pin in single measurement mode.
 *
 * Value | Typical Data Output Rate (Hz)
 * ------+------------------------------
 * 0     | 0.75
 * 1     | 1.5
 * 2     | 3
 * 3     | 7.5
 * 4     | 15 (Default)
 * 5     | 30
 * 6     | 75
 * 7     | Not used
 *
 * @return Current rate of data output to registers
 * @see HMC5883LSetDataRate()
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
unsigned char HMC5883LGetDataRate(void)
{
    return I2CDeviceReadBits(HMC5883L_ADDRESS_READ,
                             HMC5883L_RA_CONFIG_A,
                             HMC5883L_CRA_RATE_BIT,
                             HMC5883L_CRA_RATE_LENGTH);
}

/**
 * Set data output rate value.
 * @param rate Rate of data output to registers
 * @see HMC5883LGetDataRate()
 * @see HMC5883L_RATE_15
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_RATE_BIT
 * @see HMC5883L_CRA_RATE_LENGTH
 */
void HMC5883LSetDataRate(unsigned char rate)
{
    I2CDeviceWriteBits(HMC5883L_ADDRESS_WRITE,
                       HMC5883L_RA_CONFIG_A,
                       HMC5883L_CRA_RATE_BIT,
                       HMC5883L_CRA_RATE_LENGTH,
                       rate);
}

/**
 * Get measurement bias value.
 * @return Current bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
unsigned char HMC5883LGetMeasurementBias(void)
{
    return I2CDeviceReadBits(HMC5883L_ADDRESS_READ,
                             HMC5883L_RA_CONFIG_A,
                             HMC5883L_CRA_BIAS_BIT,
                             HMC5883L_CRA_BIAS_LENGTH);
}

/**
 * Set measurement bias value.
 * @param bias New bias value (0-2 for normal/positive/negative respectively)
 * @see HMC5883L_BIAS_NORMAL
 * @see HMC5883L_RA_CONFIG_A
 * @see HMC5883L_CRA_BIAS_BIT
 * @see HMC5883L_CRA_BIAS_LENGTH
 */
void HMC5883LSetMeasurementBias(unsigned char bias)
{
    I2CDeviceWriteBits(HMC5883L_ADDRESS_WRITE,
                       HMC5883L_RA_CONFIG_A,
                       HMC5883L_CRA_BIAS_BIT,
                       HMC5883L_CRA_BIAS_LENGTH,
                       bias);
}

/**
 * Get magnetic field gain value.
 * The table below shows nominal gain settings. Use the �Gain� column to convert
 * counts to Gauss. Choose a lower gain value (higher GN#) when total field
 * strength causes overflow in one of the data output registers (saturation).
 * The data output range for all settings is 0xF800-0x07FF (-2048 - 2047).
 *
 * Value | Field Range | Gain (LSB/Gauss)
 * ------+-------------+-----------------
 * 0     | +/- 0.88 Ga | 1370
 * 1     | +/- 1.3 Ga  | 1090 (Default)
 * 2     | +/- 1.9 Ga  | 820
 * 3     | +/- 2.5     | 660
 * 4     | +/- 4.0     | 440
 * 5     | +/- 4.7     | 390
 * 6     | +/- 5.6     | 330
 * 7     | +/- 8.1     | 230
 *
 * @return Current magnetic field gain value
 * @see HMC5883LSetGain()
 * @see HMC5883L_GAIN_1090
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
unsigned char HMC5883LGetGain(void)
{
    return I2CDeviceReadBits(HMC5883L_ADDRESS_READ,
                             HMC5883L_RA_CONFIG_B,
                             HMC5883L_CRB_GAIN_BIT,
                             HMC5883L_CRB_GAIN_LENGTH);
}

/**
 * Set magnetic field gain value.
 * @param gain New magnetic field gain value
 * @see HMC5883LGetGain()
 * @see HMC5883L_RA_CONFIG_B
 * @see HMC5883L_CRB_GAIN_BIT
 * @see HMC5883L_CRB_GAIN_LENGTH
 */
void HMC5883LSetGain(unsigned char gain)
{
    // use this method to guarantee that bits 4-0 are set to zero, which is a
    // requirement specified in the datasheet; it's actually more efficient than
    // using the I2Cdev.writeBits method
    I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,
                       HMC5883L_RA_CONFIG_B,
                       gain << (HMC5883L_CRB_GAIN_BIT - HMC5883L_CRB_GAIN_LENGTH + 1));
}

/**
 * Get measurement mode.
 * In continuous-measurement mode, the device continuously performs measurements
 * and places the result in the data register. RDY goes high when new data is
 * placed in all three registers. After a power-on or a write to the mode or
 * configuration register, the first measurement set is available from all three
 * data output registers after a period of 2/fDO and subsequent measurements are
 * available at a frequency of fDO, where fDO is the frequency of data output.
 *
 * When single-measurement mode (default) is selected, device performs a single
 * measurement, sets RDY high and returned to idle mode. Mode register returns
 * to idle mode bit values. The measurement remains in the data output register
 * and RDY remains high until the data output register is read or another
 * measurement is performed.
 *
 * @return Current measurement mode
 * @see HMC5883LSetMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
unsigned char HMC5883LGetMode(void)
{
    return I2CDeviceReadBits(HMC5883L_ADDRESS_READ,
                             HMC5883L_RA_MODE,
                             HMC5883L_MODEREG_BIT,
                             HMC5883L_MODEREG_LENGTH);
}

/**
 * Set measurement mode.
 * @param newMode New measurement mode
 * @see HMC5883LGetMode()
 * @see HMC5883L_MODE_CONTINUOUS
 * @see HMC5883L_MODE_SINGLE
 * @see HMC5883L_MODE_IDLE
 * @see HMC5883L_RA_MODE
 * @see HMC5883L_MODEREG_BIT
 * @see HMC5883L_MODEREG_LENGTH
 */
void HMC5883LSetMode(unsigned char newMode)
{
    // use this method to guarantee that bits 7-2 are set to zero, which is a
    // requirement specified in the datasheet; it's actually more efficient than
    // using the I2Cdev.writeBits method
    I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,
                       HMC5883L_RA_MODE,
                       mode << (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));
    mode = newMode; // track to tell if we have to clear bit 7 after a read
}

/**
 * Get 3-axis heading measurements.
 * In the event the ADC reading overflows or underflows for the given channel,
 * or if there is a math overflow during the bias measurement, this data
 * register will contain the value -4096. This register value will clear when
 * after the next valid measurement is made. Note that this method automatically
 * clears the appropriate bit in the MODE register if Single mode is active.
 * @param x 16-bit signed integer container for X-axis heading
 * @param y 16-bit signed integer container for Y-axis heading
 * @param z 16-bit signed integer container for Z-axis heading
 * @see HMC5883L_RA_DATAX_H
 */
void HMC5883LGetHeading(int *x, int *y, int *z)
{
    I2CDeviceReadBytes(HMC5883L_ADDRESS_READ, HMC5883L_RA_DATAX_H, 6,
                       HMC5883LBuffer);

    if (mode == HMC5883L_MODE_SINGLE)
        I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,
                           HMC5883L_RA_MODE,
                           HMC5883L_MODE_SINGLE <<
                           (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));

    *x = (((int) HMC5883LBuffer[0]) << 8) | HMC5883LBuffer[1];
    *y = (((int) HMC5883LBuffer[4]) << 8) | HMC5883LBuffer[5];
    *z = (((int) HMC5883LBuffer[2]) << 8) | HMC5883LBuffer[3];
}

/**
 * Get X-axis heading measurement.
 * @return 16-bit signed integer with X-axis heading
 * @see HMC5883L_RA_DATAX_H
 */
int HMC5883LGetHeadingX(void)
{
    // each axis read requires that ALL axis registers be read, even if only
    // one is used; this was not done ineffiently in the code by accident
    I2CDeviceReadBytes(HMC5883L_ADDRESS_READ, HMC5883L_RA_DATAX_H, 6,
                       HMC5883LBuffer);
    if (mode == HMC5883L_MODE_SINGLE)
        I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,
                           HMC5883L_RA_MODE,
                           HMC5883L_MODE_SINGLE <<
                           (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));

    return (((int) HMC5883LBuffer[0]) << 8) | HMC5883LBuffer[1];
}

/**
 * Get Y-axis heading measurement.
 * @return 16-bit signed integer with Y-axis heading
 * @see HMC5883L_RA_DATAY_H
 */
int HMC5883LGetHeadingY(void)
{
    // each axis read requires that ALL axis registers be read, even if only
    // one is used; this was not done ineffiently in the code by accident
    I2CDeviceReadBytes(HMC5883L_ADDRESS_READ, HMC5883L_RA_DATAX_H, 6,
                       HMC5883LBuffer);
    if (mode == HMC5883L_MODE_SINGLE)
        I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,
                           HMC5883L_RA_MODE,
                           HMC5883L_MODE_SINGLE <<
                           (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));

    return (((int) HMC5883LBuffer[4]) << 8) | HMC5883LBuffer[5];
}

/**
 * Get Z-axis heading measurement.
 * @return 16-bit signed integer with Z-axis heading
 * @see HMC5883L_RA_DATAZ_H
 */
int HMC5883LGetHeadingZ(void)
{
    // each axis read requires that ALL axis registers be read, even if only
    // one is used; this was not done ineffiently in the code by accident
    I2CDeviceReadBytes(HMC5883L_ADDRESS_READ, HMC5883L_RA_DATAX_H, 6,
                       HMC5883LBuffer);

    if (mode == HMC5883L_MODE_SINGLE)
        I2CDeviceWriteByte(HMC5883L_ADDRESS_WRITE,
                           HMC5883L_RA_MODE,
                           HMC5883L_MODE_SINGLE <<
                           (HMC5883L_MODEREG_BIT - HMC5883L_MODEREG_LENGTH + 1));

    return (((int) HMC5883LBuffer[2]) << 8) | HMC5883LBuffer[3];
}

/**
 * Get data output register lock status.
 * This bit is set when this some but not all for of the six data output
 * registers have been read. When this bit is set, the six data output registers
 * are locked and any new data will not be placed in these register until one of
 * three conditions are met: one, all six bytes have been read or the mode
 * changed, two, the mode is changed, or three, the measurement configuration is
 * changed.
 * @return Data output register lock status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_LOCK_BIT
 */
bool HMC5883LGetLockStatus(void)
{
    return I2CDeviceReadBit(HMC5883L_ADDRESS_READ,
                            HMC5883L_RA_STATUS,
                            HMC5883L_STATUS_LOCK_BIT);
}

/**
 * Get data ready status.
 * This bit is set when data is written to all six data registers, and cleared
 * when the device initiates a write to the data output registers and after one
 * or more of the data output registers are written to. When RDY bit is clear it
 * shall remain cleared for 250 us. DRDY pin can be used as an alternative to
 * the status register for monitoring the device for measurement data.
 * @return Data ready status
 * @see HMC5883L_RA_STATUS
 * @see HMC5883L_STATUS_READY_BIT
 */
bool HMC5883LGetReadyStatus(void)
{
    return I2CDeviceReadBit(HMC5883L_ADDRESS_READ,
                            HMC5883L_RA_STATUS,
                            HMC5883L_STATUS_READY_BIT);
}

/**
 * Get identification byte A
 * @return ID_A byte (should be 01001000, ASCII value 'H')
 */
unsigned char HMC5883LGetIDA(void)
{
    return I2CDeviceReadByte(HMC5883L_ADDRESS_READ,
                             HMC5883L_RA_ID_A);
}

/**
 * Get identification byte B
 * @return ID_A byte (should be 00110100, ASCII value '4')
 */
unsigned char HMC5883LGetIDB(void)
{
    return I2CDeviceReadByte(HMC5883L_ADDRESS_READ, HMC5883L_RA_ID_B);
}

/**
 * Get identification byte C
 * @return ID_A byte (should be 00110011, ASCII value '3')
 */
unsigned char HMC5883LGetIDC(void)
{
    return I2CDeviceReadByte(HMC5883L_ADDRESS_READ, HMC5883L_RA_ID_C);
}
