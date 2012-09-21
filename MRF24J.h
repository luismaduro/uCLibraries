#ifndef LIB_MRF24J_H
#define LIB_MRF24J_H

#include <p18cxxx.h>
#include "stdbool.h"

#ifdef NO

#define MIWI_COM_EN()       (PORTEbits.RE6 = 1)
#define MIWI_WAKE_LOW()     (PORTEbits.RE5 = 0)
#define MIWI_WAKE_HIGH()    (PORTEbits.RE5 = 1)
#define MIWI_CS_HIGH()      (PORTEbits.RE0 = 1)
#define MIWI_CS_LOW()       (PORTEbits.RE0 = 0)
#define MIWI_RESET_HIGH()   (PORTEbits.RE4 = 1)
#define MIWI_RESET_LOW()    (PORTEbits.RE4 = 0)

#define SDO_HIGH()          (PORTDbits.RD4 = 1)
#define SDO_LOW()           (PORTDbits.RD4 = 0)
#define SCK_HIGH()          (PORTDbits.RD6 = 1)
#define SCK_LOW()           (PORTDbits.RD6 = 0)

#define HOST_INTERRUPT_ENABLE (INTCONbits.INT0IE)

#endif

#ifdef GATEWAY

#define MIWI_COM_EN()       (PORTAbits.RA4 = 1)
#define MIWI_WAKE_LOW()     (PORTAbits.RA3 = 0)
#define MIWI_WAKE_HIGH()    (PORTAbits.RA3 = 1)
#define MIWI_CS_HIGH()      (PORTAbits.RA0 = 1)
#define MIWI_CS_LOW()       (PORTAbits.RA0 = 0)
#define MIWI_RESET_HIGH()   (PORTAbits.RA2 = 1)
#define MIWI_RESET_LOW()    (PORTAbits.RA2 = 0)

#define SDO_HIGH()			(PORTDbits.RD4 = 1)
#define SDO_LOW()			(PORTDbits.RD4 = 0)
#define SCK_HIGH()			(PORTDbits.RD6 = 1)
#define SCK_LOW()			(PORTDbits.RD6 = 0)

#define HOST_INTERRUPT_ENABLE (INTCONbits.INT0IE)

#endif

#define MRF_RXMCR 0x00
#define MRF_PANIDL 0x01
#define MRF_PANIDH 0x02
#define MRF_SADRL 0x03
#define MRF_SADRH 0x04
#define MRF_EADR0 0x05
#define MRF_EADR1 0x06
#define MRF_EADR2 0x07
#define MRF_EADR3 0x08
#define MRF_EADR4 0x09
#define MRF_EADR5 0x0A
#define MRF_EADR6 0x0B
#define MRF_EADR7 0x0C
#define MRF_RXFLUSH 0x0D
//#define MRF_Reserved 0x0E
//#define MRF_Reserved 0x0F
#define MRF_ORDER 0x10
#define MRF_TXMCR 0x11
#define MRF_ACKTMOUT 0x12
#define MRF_ESLOTG1 0x13
#define MRF_SYMTICKL 0x14
#define MRF_SYMTICKH 0x15
#define MRF_PACON0 0x16
#define MRF_PACON1 0x17
#define MRF_PACON2 0x18
//#define MRF_Reserved 0x19
#define MRF_TXBCON0 0x1A

// TXNCON: TRANSMIT NORMAL FIFO CONTROL REGISTER (ADDRESS: 0x1B)
#define MRF_TXNCON      0x1B
#define MRF_TXNTRIG     0
#define MRF_TXNSECEN    1
#define MRF_TXNACKREQ   2
#define MRF_INDIRECT    3
#define MRF_FPSTAT      4

#define MRF_TXG1CON 0x1C
#define MRF_TXG2CON 0x1D
#define MRF_ESLOTG23 0x1E
#define MRF_ESLOTG45 0x1F
#define MRF_ESLOTG67 0x20
#define MRF_TXPEND 0x21
#define MRF_WAKECON 0x22
#define MRF_FRMOFFSET 0x23

#define MRF_I_TXNIF     0b00000001
#define MRF_I_RXIF      0b00001000
#define MRF_I_WAKEIF    0b01000000
#define MRF_I_SLEEPIF   0b10000000
// TXSTAT: TX MAC STATUS REGISTER (ADDRESS: 0x24)
#define MRF_TXSTAT 0x24

#define TXNRETRY1       7
#define TXNRETRY0       6
#define CCAFAIL         5
#define TXG2FNT         4
#define TXG1FNT         3
#define TXG2STAT        2
#define TXG1STAT        1
#define TXNSTAT         0

#define MRF_TXBCON1 0x25
#define MRF_GATECLK 0x26
#define MRF_TXTIME 0x27
#define MRF_HSYMTMRL 0x28
#define MRF_HSYMTMRH 0x29
#define MRF_SOFTRST 0x2A
//#define MRF_Reserved 0x2B
#define MRF_SECCON0 0x2C
#define MRF_SECCON1 0x2D
#define MRF_TXSTBL 0x2E
//#define MRF_Reserved 0x2F
#define MRF_RXSR 0x30
#define MRF_INTSTAT 0x31
#define MRF_INTCON 0x32
#define MRF_GPIO 0x33
#define MRF_TRISGPIO 0x34
#define MRF_SLPACK 0x35
#define MRF_RFCTL 0x36
#define MRF_SECCR2 0x37
#define MRF_BBREG0 0x38
#define MRF_BBREG1 0x39
#define MRF_BBREG2 0x3A
#define MRF_BBREG3 0x3B
#define MRF_BBREG4 0x3C
//#define MRF_Reserved 0x3D
#define MRF_BBREG6 0x3E
#define MRF_CCAEDTH 0x3F

#define MRF_RFCON0 0x200
#define MRF_RFCON1 0x201
#define MRF_RFCON2 0x202
#define MRF_RFCON3 0x203
#define MRF_RFCON5 0x205
#define MRF_RFCON6 0x206
#define MRF_RFCON7 0x207
#define MRF_RFCON8 0x208
#define MRF_SLPCAL0 0x209
#define MRF_SLPCAL1 0x20A
#define MRF_SLPCAL2 0x20B
#define MRF_RFSTATE 0x20F
#define MRF_RSSI 0x210
#define MRF_SLPCON0 0x211
#define MRF_SLPCON1 0x220
#define MRF_WAKETIMEL 0x222
#define MRF_WAKETIMEH 0x223
#define MRF_REMCNTL 0x224
#define MRF_REMCNTH 0x225
#define MRF_MAINCNT0 0x226
#define MRF_MAINCNT1 0x227
#define MRF_MAINCNT2 0x228
#define MRF_MAINCNT3 0x229
#define MRF_TESTMODE 0x22F
#define MRF_ASSOEADR1 0x231
#define MRF_ASSOEADR2 0x232
#define MRF_ASSOEADR3 0x233
#define MRF_ASSOEADR4 0x234
#define MRF_ASSOEADR5 0x235
#define MRF_ASSOEADR6 0x236
#define MRF_ASSOEADR7 0x237
#define MRF_ASSOSADR0 0x238
#define MRF_ASSOSADR1 0x239
#define MRF_UPNONCE0 0x240
#define MRF_UPNONCE1 0x241
#define MRF_UPNONCE2 0x242
#define MRF_UPNONCE3 0x243
#define MRF_UPNONCE4 0x244
#define MRF_UPNONCE5 0x245
#define MRF_UPNONCE6 0x246
#define MRF_UPNONCE7 0x247
#define MRF_UPNONCE8 0x248
#define MRF_UPNONCE9 0x249
#define MRF_UPNONCE10 0x24A
#define MRF_UPNONCE11 0x24B
#define MRF_UPNONCE12 0x24C

// FIFO buffers
#define MRF_TXNORMALFIFO 0x000
#define MRF_TXNORMALFIFO_MAX_LENGTH 128
#define MRF_TXBEACONFIFO 0x080
#define MRF_TXBEACONFIFO_MAX_LENGTH 128
#define MRF_TXGTS1FIFO 0x100
#define MRF_TXGTS1FIFO_MAX_LENGTH 128
#define MRF_TXGTS2FIFO 0x180
#define MRF_TXGTS2FIFO_MAX_LENGTH 128
#define MRF_SECURITYKEYFIFO 0x280
#define MRF_SECURITYKEYFIFO_MAX_LENGTH 64
#define MRF_RXFIFO 0x300
#define MRF_RXFIFO_LENGTH 144

//Defines for Output Power
#define TX_POWER_0_DB       0
#define TX_POWER_0_5_DB     1
#define TX_POWER_1_2_DB     2
#define TX_POWER_1_9_DB     3
#define TX_POWER_2_8_DB     4
#define TX_POWER_3_7_DB     5
#define TX_POWER_4_9_DB     6
#define TX_POWER_6_3_DB     7
#define TX_POWER_10_0_DB    8
#define TX_POWER_10_5_DB    9
#define TX_POWER_11_2_DB    10
#define TX_POWER_11_9_DB    11
#define TX_POWER_12_8_DB    12
#define TX_POWER_13_7_DB    13
#define TX_POWER_14_9_DB    14
#define TX_POWER_16_3_DB    15
#define TX_POWER_20_0_DB    16
#define TX_POWER_20_5_DB    17
#define TX_POWER_21_2_DB    18
#define TX_POWER_21_9_DB    19
#define TX_POWER_22_8_DB    20
#define TX_POWER_23_7_DB    21
#define TX_POWER_24_9_DB    22
#define TX_POWER_26_3_DB    23
#define TX_POWER_30_0_DB    24
#define TX_POWER_30_5_DB    25
#define TX_POWER_31_2_DB    26
#define TX_POWER_31_9_DB    27
#define TX_POWER_32_8_DB    28
#define TX_POWER_33_7_DB    29
#define TX_POWER_34_9_DB    30
#define TX_POWER_36_3_DB    31

#define CHANNEL_11 0x00
#define CHANNEL_12 0x01
#define CHANNEL_13 0x02
#define CHANNEL_14 0x03
#define CHANNEL_15 0x04
#define CHANNEL_16 0x05
#define CHANNEL_17 0x06
#define CHANNEL_18 0x07
#define CHANNEL_19 0x08
#define CHANNEL_20 0x09
#define CHANNEL_21 0x0A
#define CHANNEL_22 0x0B
#define CHANNEL_23 0x0C
#define CHANNEL_24 0x0D
#define CHANNEL_25 0x0E
#define CHANNEL_26 0x0F

// MIN/MAX/ABS macros
#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))


void MRF24J40_Reset(void);
void MRF24J40_Init(unsigned char channel, unsigned char power, unsigned int pan, unsigned int address16);

unsigned char read_short(unsigned char address);
unsigned char read_long(unsigned int address);

void write_short(unsigned char address, unsigned char data);
void write_long(unsigned int address, unsigned char data);

unsigned int MRF24J40_Get_Pan(void);
void MRF24J40_Set_Pan(unsigned int panid);

void MRF24J40_Short_Address_Write(unsigned int address);
unsigned int MRF24J40_Short_Address_Read(void);

void MRF24J40_Long_Address_Write(unsigned char * address);
void MRF24J40_Long_Address_Read(unsigned char * address);


void MRF24J40_Set_Interrupts(void);
unsigned char MRF24J40_Get_Interrupts(void);

void MRF24J40_Set_Promiscuous(bool enabled);

void MRF24J40_Set_Power(unsigned char power);

/**
 * Set the channel, using 802.15.4 channel numbers (11..26)
 */
void MRF24J40_Set_Channel(unsigned char channel);
unsigned char MRF24J40_Get_Channel(void);

void MRF24J40_RX_Enable(void);
void MRF24J40_RX_Disable(void);

/** If you want to throw away rx data */
void MRF24J40_RX_Flush(void);

void MRF24J40_Packet_Send(unsigned int dest, unsigned int len, char* packet);
unsigned char MRF24J40_Packet_Receive(char* packet);

#endif  /* LIB_MRF24J_H */
