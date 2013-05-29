/* 
 * File:   RFM2X.h
 * Author: Luis Maduro
 *
 * Created on 27 de Março de 2013, 17:49
 */

#ifndef RFM2X_H
#define	RFM2X_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <xc.h>
#include <math.h>
#include <stdbool.h>

#ifdef MASTER_RFM23
#include "../PIC18F/SPIDevice.h"
#include "Tasker/Tasker.h"
#define WirelessTurnSupplyON()          LATCbits.LATC2 = 0
#define WirelessTurnSupplyOFF()         LATCbits.LATC2 = 1
#define WirelessSelectChip()            LATBbits.LATB4 = 0
#define WirelessDeselectChip()          LATBbits.LATB4 = 1
#define WirelessShutdownMode()          LATBbits.LATB3 = 1
#define WirelessNormalMode()            LATBbits.LATB3 = 0
#endif

#ifdef RECEIVER_RFM23
#include "PIC18F/SPIDevice.h"
#include "Common/Tasker/Tasker.h"
#define WirelessSelectChip()            LATAbits.LATA3 = 0
#define WirelessDeselectChip()          LATAbits.LATA3 = 1
#define WirelessShutdownMode()          LATAbits.LATA2 = 1
#define WirelessNormalMode()            LATAbits.LATA2 = 0
#endif

// This is the bit in the SPI address that marks it as a write
#define RFM2X_SPI_WRITE_MASK 0x80

// This is the maximum message length that can be supported by this library. Limited by
// the single message length octet in the header.
// Yes, 255 is correct even though the FIFO size in the RFM2X is only
// 64 octets. We use interrupts to refill the Tx FIFO during transmission and to empty the
// Rx FIFO during reception
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header

//#define RFM2X_MAX_MESSAGE_LEN 255
#define RFM2X_MAX_MESSAGE_LEN 64

// Max number of octets the RFM2X Rx and Tx FIFOs can hold
#define RFM2X_FIFO_SIZE 64

// These values we set for FIFO thresholds are actually the same as the POR values
#define RFM2X_TXFFAEM_THRESHOLD                         20
#define RFM2X_TXFFAFULL_THRESHOLD                       63
#define RFM2X_RXFFAFULL_THRESHOLD                       (63-7)

#define SUPPORTED_DEVICE_TYPE                           0x08
#define SUPPORTED_DEVICE_VERSION                        0x06

// Register names
#define RFM2X_REG_00_DEVICE_TYPE                         0x00
#define RFM2X_REG_01_VERSION_CODE                        0x01
#define RFM2X_REG_02_DEVICE_STATUS                       0x02
#define RFM2X_REG_03_INTERRUPT_STATUS1                   0x03
#define RFM2X_REG_04_INTERRUPT_STATUS2                   0x04
#define RFM2X_REG_05_INTERRUPT_ENABLE1                   0x05
#define RFM2X_REG_06_INTERRUPT_ENABLE2                   0x06
#define RFM2X_REG_07_OPERATING_MODE1                     0x07
#define RFM2X_REG_08_OPERATING_MODE2                     0x08
#define RFM2X_REG_09_OSCILLATOR_LOAD_CAPACITANCE         0x09
#define RFM2X_REG_0A_UC_OUTPUT_CLOCK                     0x0a
#define RFM2X_REG_0B_GPIO_CONFIGURATION0                 0x0b
#define RFM2X_REG_0C_GPIO_CONFIGURATION1                 0x0c
#define RFM2X_REG_0D_GPIO_CONFIGURATION2                 0x0d
#define RFM2X_REG_0E_IO_PORT_CONFIGURATION               0x0e
#define RFM2X_REG_0F_ADC_CONFIGURATION                   0x0f
#define RFM2X_REG_10_ADC_SENSOR_AMP_OFFSET               0x10
#define RFM2X_REG_11_ADC_VALUE                           0x11
#define RFM2X_REG_12_TEMPERATURE_SENSOR_CALIBRATION      0x12
#define RFM2X_REG_13_TEMPERATURE_VALUE_OFFSET            0x13
#define RFM2X_REG_14_WAKEUP_TIMER_PERIOD1                0x14
#define RFM2X_REG_15_WAKEUP_TIMER_PERIOD2                0x15
#define RFM2X_REG_16_WAKEUP_TIMER_PERIOD3                0x16
#define RFM2X_REG_17_WAKEUP_TIMER_VALUE1                 0x17
#define RFM2X_REG_18_WAKEUP_TIMER_VALUE2                 0x18
#define RFM2X_REG_19_LDC_MODE_DURATION                   0x19
#define RFM2X_REG_1A_LOW_BATTERY_DETECTOR_THRESHOLD      0x1a
#define RFM2X_REG_1B_BATTERY_VOLTAGE_LEVEL               0x1b
#define RFM2X_REG_1C_IF_FILTER_BANDWIDTH                 0x1c
#define RFM2X_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE         0x1d
#define RFM2X_REG_1E_AFC_TIMING_CONTROL                  0x1e
#define RFM2X_REG_1F_CLOCK_RECOVERY_GEARSHIFT_OVERRIDE   0x1f
#define RFM2X_REG_20_CLOCK_RECOVERY_OVERSAMPLING_RATE    0x20
#define RFM2X_REG_21_CLOCK_RECOVERY_OFFSET2              0x21
#define RFM2X_REG_22_CLOCK_RECOVERY_OFFSET1              0x22
#define RFM2X_REG_23_CLOCK_RECOVERY_OFFSET0              0x23
#define RFM2X_REG_24_CLOCK_RECOVERY_TIMING_LOOP_GAIN1    0x24
#define RFM2X_REG_25_CLOCK_RECOVERY_TIMING_LOOP_GAIN0    0x25
#define RFM2X_REG_26_RSSI                                0x26
#define RFM2X_REG_27_RSSI_THRESHOLD                      0x27
#define RFM2X_REG_28_ANTENNA_DIVERSITY1                  0x28
#define RFM2X_REG_29_ANTENNA_DIVERSITY2                  0x29
#define RFM2X_REG_2A_AFC_LIMITER                         0x2a
#define RFM2X_REG_2B_AFC_CORRECTION_READ                 0x2b
#define RFM2X_REG_2C_OOK_COUNTER_VALUE_1                 0x2c
#define RFM2X_REG_2D_OOK_COUNTER_VALUE_2                 0x2d
#define RFM2X_REG_2E_SLICER_PEAK_HOLD                    0x2e
#define RFM2X_REG_30_DATA_ACCESS_CONTROL                 0x30
#define RFM2X_REG_31_EZMAC_STATUS                        0x31
#define RFM2X_REG_32_HEADER_CONTROL1                     0x32
#define RFM2X_REG_33_HEADER_CONTROL2                     0x33
#define RFM2X_REG_34_PREAMBLE_LENGTH                     0x34
#define RFM2X_REG_35_PREAMBLE_DETECTION_CONTROL1         0x35
#define RFM2X_REG_36_SYNC3                               0x36
#define RFM2X_REG_37_SYNC2                               0x37
#define RFM2X_REG_38_SYNC1                               0x38
#define RFM2X_REG_39_SYNC0                               0x39
#define RFM2X_REG_3A_TRANSMIT_HEADER3                    0x3a
#define RFM2X_REG_3B_TRANSMIT_HEADER2                    0x3b
#define RFM2X_REG_3C_TRANSMIT_HEADER1                    0x3c
#define RFM2X_REG_3D_TRANSMIT_HEADER0                    0x3d
#define RFM2X_REG_3E_PACKET_LENGTH                       0x3e
#define RFM2X_REG_3F_CHECK_HEADER3                       0x3f
#define RFM2X_REG_40_CHECK_HEADER2                       0x40
#define RFM2X_REG_41_CHECK_HEADER1                       0x41
#define RFM2X_REG_42_CHECK_HEADER0                       0x42
#define RFM2X_REG_43_HEADER_ENABLE3                      0x43
#define RFM2X_REG_44_HEADER_ENABLE2                      0x44
#define RFM2X_REG_45_HEADER_ENABLE1                      0x45
#define RFM2X_REG_46_HEADER_ENABLE0                      0x46
#define RFM2X_REG_47_RECEIVED_HEADER3                    0x47
#define RFM2X_REG_48_RECEIVED_HEADER2                    0x48
#define RFM2X_REG_49_RECEIVED_HEADER1                    0x49
#define RFM2X_REG_4A_RECEIVED_HEADER0                    0x4a
#define RFM2X_REG_4B_RECEIVED_PACKET_LENGTH              0x4b
#define RFM2X_REG_50_ANALOG_TEST_BUS_SELECT              0x50
#define RFM2X_REG_51_DIGITAL_TEST_BUS_SELECT             0x51
#define RFM2X_REG_52_TX_RAMP_CONTROL                     0x52
#define RFM2X_REG_53_PLL_TUNE_TIME                       0x53
#define RFM2X_REG_55_CALIBRATION_CONTROL                 0x55
#define RFM2X_REG_56_MODEM_TEST                          0x56
#define RFM2X_REG_57_CHARGE_PUMP_TEST                    0x57
#define RFM2X_REG_58_CHARGE_PUMP_CURRENT_TRIMMING        0x58
#define RFM2X_REG_59_DIVIDER_CURRENT_TRIMMING            0x59
#define RFM2X_REG_5A_VCO_CURRENT_TRIMMING                0x5a
#define RFM2X_REG_5B_VCO_CALIBRATION                     0x5b
#define RFM2X_REG_5C_SYNTHESIZER_TEST                    0x5c
#define RFM2X_REG_5D_BLOCK_ENABLE_OVERRIDE1              0x5d
#define RFM2X_REG_5E_BLOCK_ENABLE_OVERRIDE2              0x5e
#define RFM2X_REG_5F_BLOCK_ENABLE_OVERRIDE3              0x5f
#define RFM2X_REG_60_CHANNEL_FILTER_COEFFICIENT_ADDRESS  0x60
#define RFM2X_REG_61_CHANNEL_FILTER_COEFFICIENT_VALUE    0x61
#define RFM2X_REG_62_CRYSTAL_OSCILLATOR_POR_CONTROL      0x62
#define RFM2X_REG_63_RC_OSCILLATOR_COARSE_CALIBRATION    0x63
#define RFM2X_REG_64_RC_OSCILLATOR_FINE_CALIBRATION      0x64
#define RFM2X_REG_65_LDO_CONTROL_OVERRIDE                0x65
#define RFM2X_REG_66_LDO_LEVEL_SETTINGS                  0x66
#define RFM2X_REG_67_DELTA_SIGMA_ADC_TUNING1             0x67
#define RFM2X_REG_68_DELTA_SIGMA_ADC_TUNING2             0x68
#define RFM2X_REG_69_AGC_OVERRIDE1                       0x69
#define RFM2X_REG_6A_AGC_OVERRIDE2                       0x6a
#define RFM2X_REG_6B_GFSK_FIR_FILTER_COEFFICIENT_ADDRESS 0x6b
#define RFM2X_REG_6C_GFSK_FIR_FILTER_COEFFICIENT_VALUE   0x6c
#define RFM2X_REG_6D_TX_POWER                            0x6d
#define RFM2X_REG_6E_TX_DATA_RATE1                       0x6e
#define RFM2X_REG_6F_TX_DATA_RATE0                       0x6f
#define RFM2X_REG_70_MODULATION_CONTROL1                 0x70
#define RFM2X_REG_71_MODULATION_CONTROL2                 0x71
#define RFM2X_REG_72_FREQUENCY_DEVIATION                 0x72
#define RFM2X_REG_73_FREQUENCY_OFFSET1                   0x73
#define RFM2X_REG_74_FREQUENCY_OFFSET2                   0x74
#define RFM2X_REG_75_FREQUENCY_BAND_SELECT               0x75
#define RFM2X_REG_76_NOMINAL_CARRIER_FREQUENCY1          0x76
#define RFM2X_REG_77_NOMINAL_CARRIER_FREQUENCY0          0x77
#define RFM2X_REG_79_FREQUENCY_HOPPING_CHANNEL_SELECT    0x79
#define RFM2X_REG_7A_FREQUENCY_HOPPING_STEP_SIZE         0x7a
#define RFM2X_REG_7C_TX_FIFO_CONTROL1                    0x7c
#define RFM2X_REG_7D_TX_FIFO_CONTROL2                    0x7d
#define RFM2X_REG_7E_RX_FIFO_CONTROL                     0x7e
#define RFM2X_REG_7F_FIFO_ACCESS                         0x7f

// These register masks etc are named wherever possible
// corresponding to the bit and field names in the RF-22 Manual
// RFM2X_REG_00_DEVICE_TYPE                      0x00
#define RFM2X_DEVICE_TYPE_RX_TRX                 0x08
#define RFM2X_DEVICE_TYPE_TX                     0x07

// RFM2X_REG_02_DEVICE_STATUS                    0x02
#define RFM2X_FFOVL                              0x80
#define RFM2X_FFUNFL                             0x40
#define RFM2X_RXFFEM                             0x20
#define RFM2X_HEADERR                            0x10
#define RFM2X_FREQERR                            0x08
#define RFM2X_LOCKDET                            0x04
#define RFM2X_CPS                                0x03
#define RFM2X_CPS_IDLE                           0x00
#define RFM2X_CPS_RX                             0x01
#define RFM2X_CPS_TX                             0x10

// RFM2X_REG_03_INTERRUPT_STATUS1                0x03
#define RFM2X_IFFERROR                           0x80
#define RFM2X_ITXFFAFULL                         0x40
#define RFM2X_ITXFFAEM                           0x20
#define RFM2X_IRXFFAFULL                         0x10
#define RFM2X_IEXT                               0x08
#define RFM2X_IPKSENT                            0x04
#define RFM2X_IPKVALID                           0x02
#define RFM2X_ICRCERROR                          0x01

// RFM2X_REG_04_INTERRUPT_STATUS2                0x04
#define RFM2X_ISWDET                             0x80
#define RFM2X_IPREAVAL                           0x40
#define RFM2X_IPREAINVAL                         0x20
#define RFM2X_IRSSI                              0x10
#define RFM2X_IWUT                               0x08
#define RFM2X_ILBD                               0x04
#define RFM2X_ICHIPRDY                           0x02
#define RFM2X_IPOR                               0x01

// RFM2X_REG_05_INTERRUPT_ENABLE1                0x05
#define RFM2X_ENFFERR                            0x80
#define RFM2X_ENTXFFAFULL                        0x40
#define RFM2X_ENTXFFAEM                          0x20
#define RFM2X_ENRXFFAFULL                        0x10
#define RFM2X_ENEXT                              0x08
#define RFM2X_ENPKSENT                           0x04
#define RFM2X_ENPKVALID                          0x02
#define RFM2X_ENCRCERROR                         0x01

// RFM2X_REG_06_INTERRUPT_ENABLE2                0x06
#define RFM2X_ENSWDET                            0x80
#define RFM2X_ENPREAVAL                          0x40
#define RFM2X_ENPREAINVAL                        0x20
#define RFM2X_ENRSSI                             0x10
#define RFM2X_ENWUT                              0x08
#define RFM2X_ENLBDI                             0x04
#define RFM2X_ENCHIPRDY                          0x02
#define RFM2X_ENPOR                              0x01

// RFM2X_REG_07_OPERATING_MODE                   0x07
#define RFM2X_SWRES                              0x80
#define RFM2X_ENLBD                              0x40
#define RFM2X_ENWT                               0x20
#define RFM2X_X32KSEL                            0x10
#define RFM2X_TXON                               0x08
#define RFM2X_RXON                               0x04
#define RFM2X_PLLON                              0x02
#define RFM2X_XTON                               0x01
#define RFM2X_STDBY                              0x00

// RFM2X_REG_08_OPERATING_MODE2                  0x08
#define RFM2X_ANTDIV                             0xc0
#define RFM2X_RXMPK                              0x10
#define RFM2X_AUTOTX                             0x08
#define RFM2X_ENLDM                              0x04
#define RFM2X_FFCLRRX                            0x02
#define RFM2X_FFCLRTX                            0x01

// RFM2X_REG_0F_ADC_CONFIGURATION                0x0f
#define RFM2X_ADCSTART                           0x80
#define RFM2X_ADCDONE                            0x80
#define RFM2X_ADCSEL                             0x70
#define RFM2X_ADCSEL_INTERNAL_TEMPERATURE_SENSOR 0x00
#define RFM2X_ADCSEL_GPIO0_SINGLE_ENDED          0x10
#define RFM2X_ADCSEL_GPIO1_SINGLE_ENDED          0x20
#define RFM2X_ADCSEL_GPIO2_SINGLE_ENDED          0x30
#define RFM2X_ADCSEL_GPIO0_GPIO1_DIFFERENTIAL    0x40
#define RFM2X_ADCSEL_GPIO1_GPIO2_DIFFERENTIAL    0x50
#define RFM2X_ADCSEL_GPIO0_GPIO2_DIFFERENTIAL    0x60
#define RFM2X_ADCSEL_GND                         0x70
#define RFM2X_ADCREF                             0x0c
#define RFM2X_ADCREF_BANDGAP_VOLTAGE             0x00
#define RFM2X_ADCREF_VDD_ON_3                    0x08
#define RFM2X_ADCREF_VDD_ON_2                    0x0c
#define RFM2X_ADCGAIN                            0x03

// RFM2X_REG_10_ADC_SENSOR_AMP_OFFSET            0x10
#define RFM2X_ADCOFFS                            0x0f

// RFM2X_REG_12_TEMPERATURE_SENSOR_CALIBRATION   0x12
#define RFM2X_TSRANGE                            0xc0
#define RFM2X_TSRANGE_M64_64C                    0x00
#define RFM2X_TSRANGE_M64_192C                   0x40
#define RFM2X_TSRANGE_0_128C                     0x80
#define RFM2X_TSRANGE_M40_216F                   0xc0
#define RFM2X_ENTSOFFS                           0x20
#define RFM2X_ENTSTRIM                           0x10
#define RFM2X_TSTRIM                             0x0f

// RFM2X_REG_14_WAKEUP_TIMER_PERIOD1             0x14
#define RFM2X_WTR                                0x3c
#define RFM2X_WTD                                0x03

// RFM2X_REG_1D_AFC_LOOP_GEARSHIFT_OVERRIDE      0x1d
#define RFM2X_AFBCD                              0x80
#define RFM2X_ENAFC                              0x40
#define RFM2X_AFCGEARH                           0x38
#define RFM2X_AFCGEARL                           0x07

// RFM2X_REG_1E_AFC_TIMING_CONTROL               0x1e
#define RFM2X_SWAIT_TIMER                        0xc0
#define RFM2X_SHWAIT                             0x38
#define RFM2X_ANWAIT                             0x07

// RFM2X_REG_30_DATA_ACCESS_CONTROL              0x30
#define RFM2X_ENPACRX                            0x80
#define RFM2X_MSBFRST                            0x00
#define RFM2X_LSBFRST                            0x40
#define RFM2X_CRCHDRS                            0x00
#define RFM2X_CRCDONLY                           0x20
#define RFM2X_ENPACTX                            0x08
#define RFM2X_ENCRC                              0x04
#define RFM2X_CRC                                0x03
#define RFM2X_CRC_CCITT                          0x00
#define RFM2X_CRC_CRC_16_IBM                     0x01
#define RFM2X_CRC_IEC_16                         0x02
#define RFM2X_CRC_BIACHEVA                       0x03

// RFM2X_REG_32_HEADER_CONTROL1                  0x32
#define RFM2X_BCEN                               0xf0
#define RFM2X_BCEN_NONE                          0x00
#define RFM2X_BCEN_HEADER0                       0x10
#define RFM2X_BCEN_HEADER1                       0x20
#define RFM2X_BCEN_HEADER2                       0x40
#define RFM2X_BCEN_HEADER3                       0x80
#define RFM2X_HDCH                               0x0f
#define RFM2X_HDCH_NONE                          0x00
#define RFM2X_HDCH_HEADER0                       0x01
#define RFM2X_HDCH_HEADER1                       0x02
#define RFM2X_HDCH_HEADER2                       0x04
#define RFM2X_HDCH_HEADER3                       0x08

// RFM2X_REG_33_HEADER_CONTROL2                  0x33
#define RFM2X_HDLEN                              0x70
#define RFM2X_HDLEN_0                            0x00
#define RFM2X_HDLEN_1                            0x10
#define RFM2X_HDLEN_2                            0x20
#define RFM2X_HDLEN_3                            0x30
#define RFM2X_HDLEN_4                            0x40
#define RFM2X_VARPKLEN                           0x00
#define RFM2X_FIXPKLEN                           0x08
#define RFM2X_SYNCLEN                            0x06
#define RFM2X_SYNCLEN_1                          0x00
#define RFM2X_SYNCLEN_2                          0x02
#define RFM2X_SYNCLEN_3                          0x04
#define RFM2X_SYNCLEN_4                          0x06
#define RFM2X_PREALEN8                           0x01

// RFM2X_REG_6D_TX_POWER                         0x6d
#define RFM2X_TXPOW                              0x07
#define RFM2X_TXPOW_4X31                         0x08 // Not used in RFM22B
#define RFM2X_TXPOW_1DBM                         0x00
#define RFM2X_TXPOW_2DBM                         0x01
#define RFM2X_TXPOW_5DBM                         0x02
#define RFM2X_TXPOW_8DBM                         0x03
#define RFM2X_TXPOW_11DBM                        0x04
#define RFM2X_TXPOW_14DBM                        0x05
#define RFM2X_TXPOW_17DBM                        0x06
#define RFM2X_TXPOW_20DBM                        0x07
// IN RFM2XB
#define RFM2X_TXPOW_LNA_SW                       0x08

// RFM2X_REG_71_MODULATION_CONTROL2              0x71
#define RFM2X_TRCLK                              0xc0
#define RFM2X_TRCLK_NONE                         0x00
#define RFM2X_TRCLK_GPIO                         0x40
#define RFM2X_TRCLK_SDO                          0x80
#define RFM2X_TRCLK_NIRQ                         0xc0
#define RFM2X_DTMOD                              0x30
#define RFM2X_DTMOD_DIRECT_GPIO                  0x00
#define RFM2X_DTMOD_DIRECT_SDI                   0x10
#define RFM2X_DTMOD_FIFO                         0x20
#define RFM2X_DTMOD_PN9                          0x30
#define RFM2X_ENINV                              0x08
#define RFM2X_FD8                                0x04
#define RFM2X_MODTYP                             0x30
#define RFM2X_MODTYP_UNMODULATED                 0x00
#define RFM2X_MODTYP_OOK                         0x01
#define RFM2X_MODTYP_FSK                         0x02
#define RFM2X_MODTYP_GFSK                        0x03

// RFM2X_REG_75_FREQUENCY_BAND_SELECT            0x75
#define RFM2X_SBSEL                              0x40
#define RFM2X_HBSEL                              0x20
#define RFM2X_FB                                 0x1f

typedef union
{

    struct
    {
        unsigned char CRCError : 1;
        unsigned char ValidPacketReceived : 1;
        unsigned char PacketSentInterrupt : 1;
        unsigned char ExternalInterrupt : 1;
        unsigned char RXFIFOAlmostFull : 1;
        unsigned char TXFIFOAlmostEmpty : 1;
        unsigned char TXFIFOAlmostFull : 1;
        unsigned char FIFOUnderflowOverflowError : 1;
    } bits;
    unsigned char IRQReg;
} tInterruptStatus1;

typedef union
{

    struct
    {
        unsigned char PowerONReset : 1;
        unsigned char ChipReady : 1;
        unsigned char LowBatteryDetect : 1;
        unsigned char WakeUpTimer : 1;
        unsigned char RSSI : 1;
        unsigned char InvalidPreambleDetected : 1;
        unsigned char ValidPreambleDetected : 1;
        unsigned char SyncDetected : 1;
    } bits;
    unsigned char IRQReg;
} tInterruptStatus2;

typedef struct
{
    unsigned char Preamble[6];
    unsigned char SyncWord[2];
    unsigned char CommandID;
    unsigned char DestinationID[4];
    unsigned char SourceID[4];
    unsigned char TransmissionCodeID;
    unsigned char SendTriesNumber;
    unsigned char PacketInfo;
    unsigned char PayloadLength;
    unsigned char Payload[32];
    unsigned char BatteryLevel[2];
    unsigned char RTC[4];
    unsigned char SignalStrength;
    unsigned char CRC[4];
} tPackageFormat;

extern volatile tPackageFormat RFM2xRXPacket;
extern volatile tPackageFormat RFM2xTXPacket;
extern volatile tInterruptStatus1 RFM2xITStatus1;
extern volatile tInterruptStatus2 RFM2xITStatus2;

extern volatile bool NewPacketReceived;

unsigned char RFM2xReadByte(unsigned char reg);
void RFM2xWriteByte(unsigned char reg, unsigned char val);
void RFM2xBurstReadByte(unsigned char reg, unsigned char *dest, unsigned char len);
void RFM2xBurstWriteByte(unsigned char reg, unsigned char *src, unsigned char len);
void RFM2xInterruptHandler(void);
unsigned char RFM2xInit(void);
unsigned char RFM2xStatusRead(void);
void RFM2xSetMode(unsigned char mode);
void RFM2xSetModeStandby(void);
void RFM2xSetModeIdle(void);
void RFM2xSetModeRX(void);
void RFM2xSetModeTX(void);
void RFM2xSendData(void);
void RFM2xReceiveData(void);
void RFM2xResetRXFIFO(void);
void RFM2xResetTXFIFO(void);
void RFM2xResetAllFIFO(void);
unsigned char RFM2xReadRSSI(void);
bool RFM2xSetFrequency(float centre, float afcPullInRange);
#ifdef	__cplusplus
}
#endif

#endif	/* RFM2X_H */

