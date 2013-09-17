#include "RFM23.h"

uint8_t _idleMode = RFM2X_XTON;
uint16_t _txGood = 0;
uint8_t rssi = 0;
tInterruptStatus1 RFM2xITStatus1;
tInterruptStatus2 RFM2xITStatus2;
tPackageFormat RFM2xRXPacket;
tPackageFormat RFM2xTXPacket;
bool NewPacketReceived;

uint8_t RFM2xReadByte(uint8_t reg)
{
    uint8_t val;

    WirelessSelectChip();
    SPIWrite(reg & ~RFM2X_SPI_WRITE_MASK);
    val = SPIRead();
    WirelessDeselectChip();

    return val;
}

void RFM2xWriteByte(uint8_t reg, uint8_t val)
{
    WirelessSelectChip();
    SPIWrite(reg | RFM2X_SPI_WRITE_MASK);
    SPIWrite(val);
    WirelessDeselectChip();
}

void RFM2xBurstReadByte(uint8_t reg, uint8_t *dest, uint8_t len)
{
    WirelessSelectChip();
    SPIWrite(reg & ~RFM2X_SPI_WRITE_MASK);
    while (len--)
        *dest++ = SPIRead();
    WirelessDeselectChip();
}

void RFM2xBurstWriteByte(uint8_t reg, uint8_t *src, uint8_t len)
{
    WirelessSelectChip();
    SPIWrite(reg | RFM2X_SPI_WRITE_MASK);
    while (len--)
        SPIWrite(*src++);
    WirelessDeselectChip();
}

void RFM2xInterruptHandler(void)
{
    RFM2xITStatus1.IRQReg = RFM2xReadByte(RFM2X_REG_03_INTERRUPT_STATUS1);
    RFM2xITStatus2.IRQReg = RFM2xReadByte(RFM2X_REG_04_INTERRUPT_STATUS2);

    if (RFM2xITStatus1.bits.PacketSentInterrupt)
    {
        RFM2xSetModeRX();
    }
    if (RFM2xITStatus1.bits.RXFIFOAlmostFull)
    {
        RFM2xReceiveData();
    }
    if (RFM2xITStatus2.bits.SyncDetected)
    {
        rssi = RFM2xReadRSSI();
    }
}

uint8_t RFM2xInit(void)
{
    WirelessShutdownMode();
    TaskerDelayMiliseconds(20);
    WirelessNormalMode();
    TaskerDelayMiliseconds(20);

    /**Make a software reset*/
    RFM2xWriteByte(RFM2X_REG_07_OPERATING_MODE1, RFM2X_SWRES);

    /**Wait until chip is ready*/
    TaskerDelayMiliseconds(100);

    /**Enable the necessary registers, put the module into ready mode*/
    RFM2xWriteByte(RFM2X_REG_05_INTERRUPT_ENABLE1, RFM2X_ENRXFFAFULL | RFM2X_ENPKSENT);
    RFM2xWriteByte(RFM2X_REG_06_INTERRUPT_ENABLE2, RFM2X_ENSWDET);
    RFM2xWriteByte(RFM2X_REG_07_OPERATING_MODE1, 0x01);
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, 0x00);

    /**Just load the POR value on capacitance*/
    RFM2xWriteByte(RFM2X_REG_09_OSCILLATOR_LOAD_CAPACITANCE, 0x7F);

    /**Configure GPIOs*/
    RFM2xWriteByte(RFM2X_REG_0A_UC_OUTPUT_CLOCK, 0x05); //2Mhz (no use)
    RFM2xWriteByte(RFM2X_REG_0B_GPIO_CONFIGURATION0, 0x00);
    RFM2xWriteByte(RFM2X_REG_0C_GPIO_CONFIGURATION1, 0x00);
    RFM2xWriteByte(RFM2X_REG_0D_GPIO_CONFIGURATION2, 0x00);
    RFM2xWriteByte(RFM2X_REG_0E_IO_PORT_CONFIGURATION, 0x00);

    /**ADC Configuration*/
    RFM2xWriteByte(RFM2X_REG_0F_ADC_CONFIGURATION, 0x00);
    RFM2xWriteByte(RFM2X_REG_10_ADC_SENSOR_AMP_OFFSET, 0x00);
    RFM2xWriteByte(RFM2X_REG_12_TEMPERATURE_SENSOR_CALIBRATION, 0x00);
    RFM2xWriteByte(RFM2X_REG_13_TEMPERATURE_VALUE_OFFSET, 0x00);

    //RX Data Rate @ 128kbps
    RFM2xWriteByte(RFM2X_REG_1C_IF_FILTER_BANDWIDTH, 0x82);
    RFM2xWriteByte(RFM2X_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE, 0x40);
    RFM2xWriteByte(RFM2X_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE, 0x5E);
    RFM2xWriteByte(RFM2X_REG_21_CLOCK_RECOVERY_OFFSET2, 0x01);
    RFM2xWriteByte(RFM2X_REG_22_CLOCK_RECOVERY_OFFSET1, 0x5D);
    RFM2xWriteByte(RFM2X_REG_23_CLOCK_RECOVERY_OFFSET0, 0x86);
    RFM2xWriteByte(RFM2X_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1, 0x06);
    RFM2xWriteByte(RFM2X_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0, 0xFB);
    RFM2xWriteByte(RFM2X_REG_2A_AFC_LIMITER, 0x50);

    //Disable package handle, msb first, disable crc.
    RFM2xWriteByte(RFM2X_REG_30_DATA_ACCESS_CONTROL, 0x61);
    //No address and header check
    RFM2xWriteByte(RFM2X_REG_32_HEADER_CONTROL1, 0x00);
    //Header not used for header length,packet length not included, sync on 3&2
    RFM2xWriteByte(RFM2X_REG_33_HEADER_CONTROL2, 0x0A);
    //Preamble has 6 byte (12 nibbles)
    RFM2xWriteByte(RFM2X_REG_34_PREAMBLE_LENGTH, 0x0C);
    //Preamble must have at least 6 nible to be correct
    RFM2xWriteByte(RFM2X_REG_35_PREAMBLE_DETECTION_CONTROL1, 0x32);
    //Only use 1 sync word, in word 3
    RFM2xWriteByte(RFM2X_REG_36_SYNC3, 0x2D);
    RFM2xWriteByte(RFM2X_REG_37_SYNC2, 0xD4);
    RFM2xWriteByte(RFM2X_REG_38_SYNC1, 0x00);
    RFM2xWriteByte(RFM2X_REG_39_SYNC0, 0x00);

    //TX Data Rate @ 9.6kbps
    RFM2xWriteByte(RFM2X_REG_69_AGC_OVERRIDE1, 0x60);
    //Configure the power output to maximum with LNA control
    RFM2xWriteByte(RFM2X_REG_6D_TX_POWER,
                   RFM2X_TXPOW_20DBM | RFM2X_TXPOW_LNA_SW);
    RFM2xWriteByte(RFM2X_REG_6E_TX_DATA_RATE1, 0x20);
    RFM2xWriteByte(RFM2X_REG_6F_TX_DATA_RATE0, 0xC5);
    //Disable manchester
    RFM2xWriteByte(RFM2X_REG_70_MODULATION_CONTROL1, 0x04);
    //FSK, fd[8] =0, no invert for Tx/Rx data, fifo mode
    RFM2xWriteByte(RFM2X_REG_71_MODULATION_CONTROL2, 0x2B);
    //Modulation Frequency Deviation
    RFM2xWriteByte(RFM2X_REG_72_FREQUENCY_DEVIATION, 0x50);
    RFM2xWriteByte(RFM2X_REG_73_FREQUENCY_OFFSET1, 0x00);
    RFM2xWriteByte(RFM2X_REG_74_FREQUENCY_OFFSET2, 0x00);
    RFM2xWriteByte(RFM2X_REG_75_FREQUENCY_BAND_SELECT, 0x73);
    RFM2xWriteByte(RFM2X_REG_76_NOMINAL_CARRIER_FREQUENCY1, 0x64);
    RFM2xWriteByte(RFM2X_REG_77_NOMINAL_CARRIER_FREQUENCY0, 0x00);
    RFM2xWriteByte(RFM2X_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT, 0x00);
    RFM2xWriteByte(RFM2X_REG_7A_FREQUENCY_HOPPING_STEP_SIZE, 0x00);
    RFM2xWriteByte(RFM2X_REG_7C_TX_FIFO_CONTROL1, RFM2X_TXFFAFULL_THRESHOLD);
    RFM2xWriteByte(RFM2X_REG_7D_TX_FIFO_CONTROL2, RFM2X_TXFFAEM_THRESHOLD);
    RFM2xWriteByte(RFM2X_REG_7E_RX_FIFO_CONTROL, RFM2X_RXFFAFULL_THRESHOLD);

    RFM2xSetModeStandby();

    return 0;
}

uint8_t RFM2xStatusRead(void)
{
    return RFM2xReadByte(RFM2X_REG_02_DEVICE_STATUS);
}

void RFM2xSetMode(uint8_t mode)
{
    RFM2xWriteByte(RFM2X_REG_07_OPERATING_MODE1, mode);
}

void RFM2xSetModeStandby(void)
{
    RFM2xSetMode(RFM2X_STDBY);
}

void RFM2xSetModeIdle(void)
{
    RFM2xSetMode(_idleMode);
}

void RFM2xSetModeRX(void)
{
    RFM2xSetMode(_idleMode | RFM2X_RXON);
}

void RFM2xSetModeTX(void)
{
    RFM2xSetMode(_idleMode | RFM2X_TXON);
    // Hmmm, if you dont clear the RX FIFO here, then it appears that going
    // to transmit mode in the middle of a receive can corrupt the
    // RX FIFO
    RFM2xResetRXFIFO();
}

void RFM2xSendData(void)
{
    RFM2xSetModeIdle();
    RFM2xResetTXFIFO();
    RFM2xBurstWriteByte(RFM2X_REG_7F_FIFO_ACCESS, (uint8_t *) &(RFM2xTXPacket.Preamble[0]), RFM2X_FIFO_SIZE);
    RFM2xSetModeTX();
}

void RFM2xReceiveData(void)
{
    RFM2xSetModeIdle();
    RFM2xBurstReadByte(RFM2X_REG_7F_FIFO_ACCESS, (uint8_t *) &(RFM2xRXPacket.CommandID), RFM2X_RXFFAFULL_THRESHOLD);
    RFM2xRXPacket.SignalStrength = rssi;
    RFM2xResetRXFIFO();
}

void RFM2xResetRXFIFO(void)
{
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, RFM2X_FFCLRRX);
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, 0x00);
}

void RFM2xResetTXFIFO(void)
{
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, RFM2X_FFCLRTX);
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, 0x00);
}

void RFM2xResetAllFIFO(void)
{
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, RFM2X_FFCLRRX | RFM2X_FFCLRTX);
    RFM2xWriteByte(RFM2X_REG_08_OPERATING_MODE2, 0x00);
}

uint8_t RFM2xReadRSSI(void)
{
    return RFM2xReadByte(RFM2X_REG_26_RSSI);
}

// Returns true if centre + (fhch * fhs) is within limits
// Caution, different versions of the RF22 support different max freq
// so YMMV

bool RFM2xSetFrequency(float centre, float afcPullInRange)
{
    uint8_t fbsel = RFM2X_SBSEL;
    uint8_t afclimiter;

    if (centre < 240.0 || centre > 960.0) // 930.0 for early silicon
        return false;

    if (centre >= 480.0)
    {
        if (afcPullInRange < 0.0 || afcPullInRange > 0.318750)
            return false;
        centre /= 2;
        fbsel |= RFM2X_HBSEL;
        afclimiter = (uint8_t) (afcPullInRange * 1000000.0 / 1250.0);
    }
    else
    {
        if (afcPullInRange < 0.0 || afcPullInRange > 0.159375)
            return false;
        afclimiter = (uint8_t) (afcPullInRange * 1000000.0 / 625.0);
    }
    centre /= 10.0;

    float integerPart = (int) (centre);
    float fractionalPart = centre - integerPart;

    uint8_t fb = (uint8_t) integerPart - 24; // Range 0 to 23
    fbsel |= fb;
    unsigned int fc = (unsigned int) (fractionalPart * 64000);
    RFM2xWriteByte(RFM2X_REG_73_FREQUENCY_OFFSET1, 0); // REVISIT
    RFM2xWriteByte(RFM2X_REG_74_FREQUENCY_OFFSET2, 0);
    RFM2xWriteByte(RFM2X_REG_75_FREQUENCY_BAND_SELECT, fbsel);
    RFM2xWriteByte(RFM2X_REG_76_NOMINAL_CARRIER_FREQUENCY1, fc >> 8);
    RFM2xWriteByte(RFM2X_REG_77_NOMINAL_CARRIER_FREQUENCY0, fc & 0xff);
    RFM2xWriteByte(RFM2X_REG_2A_AFC_LIMITER, afclimiter);
    return !(RFM2xStatusRead() & RFM2X_FREQERR);
}