/**
 *  @file       MRF24J40.h
 *  @brief      IEEE802.15.4 Radio Transciever
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       06/12/2012
 *
 *  Copyright (C) 2012  Luis Maduro
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MRF24J40_H
#define	MRF24J40_H

#ifdef	__cplusplus
extern "C"
{
#endif

#include <p18cxxx.h>
#include "SPIDevice.h"
#include "stdboolean.h"

#ifdef NO

#define MRF24J40_LEVEL_CONVERTER_ENABLE()   (PORTEbits.RE6 = 1)
#define MRF24J40_AWAKE()                    (PORTEbits.RE5 = 0)
#define MRF24J40_SLEEP()                    (PORTEbits.RE5 = 1)
#define MRF24J40_DESELECT_DEVICE()          (PORTEbits.RE0 = 1)
#define MRF24J40_SELECT_DEVICE()            (PORTEbits.RE0 = 0)
#define MRF24J40_RUN()                      (PORTEbits.RE4 = 1)
#define MRF24J40_RESET()                    (PORTEbits.RE4 = 0)

#define HOST_INTERRUPT (INTCONbits.INT0IE)

#endif

#ifdef GATEWAY

#define MRF24J40_LEVEL_CONVERTER_ENABLE()   (PORTAbits.RA4 = 1)
#define MRF24J40_AWAKE()                    (PORTAbits.RA3 = 0)
#define MRF24J40_SLEEP()                    (PORTAbits.RA3 = 1)
#define MRF24J40_DESELECT_DEVICE()          (PORTAbits.RA0 = 1)
#define MRF24J40_SELECT_DEVICE()            (PORTAbits.RA0 = 0)
#define MRF24J40_RUN()                      (PORTAbits.RA2 = 1)
#define MRF24J40_RESET()                    (PORTAbits.RA2 = 0)

#define HOST_INTERRUPT (INTCONbits.INT0IE)

#endif

#define ENABLE_PA_LNA
#define TURBO_MODE
#define MRF24J40MB
#define ENABLE_SECURITY

extern unsigned char MACCurrentChannel;

//long address registers
#define RFCTRL0 (0x200)
#define RFCTRL1 (0x201)
#define RFCTRL2 (0x202)
#define RFCTRL3 (0x203)
#define RFCTRL4 (0x204)
#define RFCTRL5 (0x205)
#define RFCTRL6 (0x206)
#define RFCTRL7 (0x207)
#define RFCTRL8 (0x208)
#define CAL1 (0x209)
#define CAL2 (0x20a)
#define CAL3 (0x20b)
#define SFCNTRH (0x20c)
#define SFCNTRM (0x20d)
#define SFCNTRL (0x20e)
#define RFSTATE (0x20f)
#define RSSI (0x210)
#define CLKIRQCR (0x211)
#define SRCADRMODE (0x212)
#define SRCADDR0 (0x213)
#define SRCADDR1 (0x214)
#define SRCADDR2 (0x215)
#define SRCADDR3 (0x216)
#define SRCADDR4 (0x217)
#define SRCADDR5 (0x218)
#define SRCADDR6 (0x219)
#define SRCADDR7 (0x21a)
#define RXFRAMESTATE (0x21b)
#define SECSTATUS (0x21c)
#define STCCMP (0x21d)
#define HLEN (0x21e)
#define FLEN (0x21f)
#define SCLKDIV (0x220)
//#define reserved (0x221)
#define WAKETIMEL (0x222)
#define WAKETIMEH (0x223)
#define TXREMCNTL (0x224)
#define TXREMCNTH (0x225)
#define TXMAINCNTL (0x226)
#define TXMAINCNTM (0x227)
#define TXMAINCNTH0 (0x228)
#define TXMAINCNTH1 (0x229)
#define RFMANUALCTRLEN (0x22a)
#define RFMANUALCTRL (0x22b)
#define RFRXCTRL RFMANUALCTRL
#define TxDACMANUALCTRL (0x22c)
#define RFMANUALCTRL2 (0x22d)
#define TESTRSSI (0x22e)
#define TESTMODE (0x22f)

#define NORMAL_TX_FIFO  (0x000)
#define BEACON_TX_FIFO  (0x080)
#define GTS1_TX_FIFO    (0x100)
#define GTS2_TX_FIFO    (0x180)

#define RX_FIFO         (0x300)

#define SECURITY_FIFO   (0x280)

//short address registers for reading
#define READ_RXMCR (0x00)
#define READ_PANIDL (0x02)
#define READ_PANIDH (0x04)
#define READ_SADRL (0x06)
#define READ_SADRH (0x08)
#define READ_EADR0 (0x0A)
#define READ_EADR1 (0x0C)
#define READ_EADR2 (0x0E)
#define READ_EADR3 (0x10)
#define READ_EADR4 (0x12)
#define READ_EADR5 (0x14)
#define READ_EADR6 (0x16)
#define READ_EADR7 (0x18)
#define READ_RXFLUSH (0x1a)
#define READ_TXSTATE0 (0x1c)
#define READ_TXSTATE1 (0x1e)
#define READ_ORDER (0x20)
#define READ_TXMCR (0x22)
#define READ_ACKTMOUT (0x24)
#define READ_SLALLOC (0x26)
#define READ_SYMTICKL (0x28)
#define READ_SYMTICKH (0x2A)
#define READ_PAONTIME (0x2C)
#define READ_PAONSETUP (0x2E)
#define READ_FFOEN (0x30)
#define READ_CSMACR (0x32)
#define READ_TXBCNTRIG (0x34)
#define READ_TXNMTRIG (0x36)
#define READ_TXG1TRIG (0x38)
#define READ_TXG2TRIG (0x3A)
#define READ_ESLOTG23 (0x3C)
#define READ_ESLOTG45 (0x3E)
#define READ_ESLOTG67 (0x40)
#define READ_TXPEND (0x42)
#define READ_TXBCNINTL (0x44)
#define READ_FRMOFFSET (0x46)
#define READ_TXSR (0x48)
#define READ_TXLERR (0x4A)
#define READ_GATE_CLK (0x4C)
#define READ_TXOFFSET (0x4E)
#define READ_HSYMTMR0 (0x50)
#define READ_HSYMTMR1 (0x52)
#define READ_SOFTRST (0x54)
#define READ_BISTCR (0x56)
#define READ_SECCR0 (0x58)
#define READ_SECCR1 (0x5A)
#define READ_TXPEMISP (0x5C)
#define READ_SECISR (0x5E)
#define READ_RXSR (0x60)
#define READ_ISRSTS (0x62)
#define READ_INTMSK (0x64)
#define READ_GPIO (0x66)
#define READ_GPIODIR (0x68)
#define READ_SLPACK (0x6A)
#define READ_RFCTL (0x6C)
#define READ_SECCR2 (0x6E)
#define READ_BBREG0 (0x70)
#define READ_BBREG1 (0x72)
#define READ_BBREG2 (0x74)
#define READ_BBREG3 (0x76)
#define READ_BBREG4 (0x78)
#define READ_BBREG5 (0x7A)
#define READ_BBREG6 (0x7C)
#define READ_RSSITHCCA (0x7E)

//short address registers for writing
//short address registers for reading
#define WRITE_RXMCR (0x01)
#define WRITE_PANIDL (0x03)
#define WRITE_PANIDH (0x05)
#define WRITE_SADRL (0x07)
#define WRITE_SADRH (0x09)
#define WRITE_EADR0 (0x0B)
#define WRITE_EADR1 (0x0D)
#define WRITE_EADR2 (0x0F)
#define WRITE_EADR3 (0x11)
#define WRITE_EADR4 (0x13)
#define WRITE_EADR5 (0x15)
#define WRITE_EADR6 (0x17)
#define WRITE_EADR7 (0x19)
#define WRITE_RXFLUSH (0x1B)
#define WRITE_TXSTATE0 (0x1D)
#define WRITE_TXSTATE1 (0x1F)
#define WRITE_ORDER (0x21)
#define WRITE_TXMCR (0x23)
#define WRITE_ACKTMOUT (0x25)
#define WRITE_SLALLOC (0x27)
#define WRITE_SYMTICKL (0x29)
#define WRITE_SYMTICKH (0x2B)
#define WRITE_PAONTIME (0x2D)
#define WRITE_PAONSETUP (0x2F)
#define WRITE_FFOEN (0x31)
#define WRITE_CSMACR (0x33)
#define WRITE_TXBCNTRIG (0x35)
#define WRITE_TXNMTRIG (0x37)
#define WRITE_TXG1TRIG (0x39)
#define WRITE_TXG2TRIG (0x3B)
#define WRITE_ESLOTG23 (0x3D)
#define WRITE_ESLOTG45 (0x3F)
#define WRITE_ESLOTG67 (0x41)
#define WRITE_TXPEND (0x43)
#define WRITE_TXBCNINTL (0x45)
#define WRITE_FRMOFFSET (0x47)
#define WRITE_TXSR (0x49)
#define WRITE_TXLERR (0x4B)
#define WRITE_GATE_CLK (0x4D)
#define WRITE_TXOFFSET (0x4F)
#define WRITE_HSYMTMR0 (0x51)
#define WRITE_HSYMTMR1 (0x53)
#define WRITE_SOFTRST (0x55)
#define WRITE_BISTCR (0x57)
#define WRITE_SECCR0 (0x59)
#define WRITE_SECCR1 (0x5B)
#define WRITE_TXPEMISP (0x5D)
#define WRITE_SECISR (0x5F)
#define WRITE_RXSR (0x61)
#define WRITE_ISRSTS (0x63)
#define WRITE_INTMSK (0x65)
#define WRITE_GPIO (0x67)
#define WRITE_GPIODIR (0x69)
#define WRITE_SLPACK (0x6B)
#define WRITE_RFCTL (0x6D)
#define WRITE_SECCR2 (0x6F)
#define WRITE_BBREG0 (0x71)
#define WRITE_BBREG1 (0x73)
#define WRITE_BBREG2 (0x75)
#define WRITE_BBREG3 (0x77)
#define WRITE_BBREG4 (0x79)
#define WRITE_BBREG5 (0x7B)
#define WRITE_BBREG6 (0x7D)
#define WRITE_RSSITHCCA (0x7F)

#define CHANNEL_11 0x00
#define CHANNEL_12 0x10
#define CHANNEL_13 0x20
#define CHANNEL_14 0x30
#define CHANNEL_15 0x40
#define CHANNEL_16 0x50
#define CHANNEL_17 0x60
#define CHANNEL_18 0x70
#define CHANNEL_19 0x80
#define CHANNEL_20 0x90
#define CHANNEL_21 0xa0
#define CHANNEL_22 0xb0
#define CHANNEL_23 0xc0
#define CHANNEL_24 0xd0
#define CHANNEL_25 0xe0
#define CHANNEL_26 0xf0

#if defined(ENABLE_PA_LNA) && (defined(MRF24J40MB) || defined(MRF24J40MC))
#define FULL_CHANNEL_MAP        0x03FFF800
#else
#define FULL_CHANNEL_MAP        0x07FFF800
#endif

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

typedef union
{
    unsigned char Val; // value of interrupts

    struct
    {
        unsigned char RF_TXIF : 1; // transmission finish interrupt
        unsigned char : 2;
        unsigned char RF_RXIF : 1; // receiving a packet interrupt
        unsigned char SECIF : 1; // receiving a secured packet interrupt
        unsigned char : 4;
    } bits; // bit map of interrupts
} tMRF24J40IFReg;

typedef union
{
    unsigned char Val;

    struct
    {
        unsigned char TX_BUSY : 1;
        unsigned char TX_PENDING_ACK : 1;
        unsigned char TX_FAIL : 1;
        unsigned char RX_SECURITY : 1;
        unsigned char RX_IGNORE_SECURITY : 1;
        unsigned char RX_BUFFERED : 1;
    } bits;
} tMRF24J40Status;

/***************************************************************************
 * Parameters to Transmit a Packet
 *
 *      This structure contains configurations to transmit a packet
 **************************************************************************/
typedef struct
{

    union
    {
        unsigned char Val;

        struct
        {
            unsigned char packetType : 2; // type of packet. Possible types are
            // * PACKET_TYPE_DATA - Data type
            // * PACKET_TYPE_COMMAND -  Command type
            // * PACKET_TYPE_ACK -  Acknowledgement type
            // * PACKET_TYPE_RESERVE - Reserved type
            unsigned char broadcast : 1; // 1: broadcast, 0: unicast
            unsigned char secEn : 1; // 1: secure the MAC payload, 0: send plain text
            unsigned char repeat : 1; // 1: allow repeaters to forward the message, 0: send message directly
            unsigned char ackReq : 1; // 1: acknowledgement required, 0: no acknowldgement
            unsigned char destPrsnt : 1; // 1: destination address in the packet, 0: destination address not in the packet
            unsigned char sourcePrsnt : 1; // 1: source address in the packet, 0: source address not in the packet
        } bits;
    } flags;

    unsigned char *DestAddress; // destination address
    boolean altDestAddr; // use the alternative network address as destination in the packet
    boolean altSrcAddr; // use the alternative network address as source in the packet
    unsigned int DestPANID; // PAN identifier of the destination

} MAC_TRANS_PARAM;

/***************************************************************************
 * Content of the Received Message
 *
 *      This structure contains all information of the received message
 **************************************************************************/
typedef struct
{

    union
    {
        unsigned char Val;

        struct
        {
            unsigned char packetType : 2; // type of packet. Possible types are
            // * PACKET_TYPE_DATA - Data type
            // * PACKET_TYPE_COMMAND -  Command type
            // * PACKET_TYPE_ACK -  Acknowledgement type
            // * PACKET_TYPE_RESERVE - Reserved type
            unsigned char broadcast : 1; // 1: broadcast, 0: unicast
            unsigned char secEn : 1; // 1: secure the MAC payload, 0: send plain text
            unsigned char repeat : 1; // 1: allow repeaters to forward the message, 0: send message directly
            unsigned char ackReq : 1; // 1: acknowledgement required, 0: no acknowldgement
            unsigned char destPrsnt : 1; // 1: destination address in the packet, 0: destination address not in the packet
            unsigned char sourcePrsnt : 1; // 1: source address in the packet, 0: source address not in the packet
        } bits;
    } flags;

    unsigned char * SourceAddress; // Address of the Sender
    unsigned char * Payload; // Pointer to the payload
    unsigned char PayloadLen; // Payload size
    unsigned char RSSIValue; // RSSI value for the received packet
    unsigned char LQIValue; // LQI value for the received packet
    boolean altSourceAddress; // Source address is the alternative network address
    unsigned int SourcePANID; // PAN ID of the sender

} tMACReceivedPacket;

/***************************************************************************
 * Initialization Parameters for MAC
 *
 *      This structure contains parameters in the initialization
 **************************************************************************/
typedef struct
{

    union
    {
        unsigned char Val;

        struct
        {
            unsigned char RepeaterMode : 1; // 1: Allow to act as repeater, 0: disable repeater function
            unsigned char CCAEnable : 1; // 1: Enable Clear Channel Assessment, 0: Disable CCA
            unsigned char NetworkFreezer : 1;
            unsigned char PAddrLength : 4; // Length of the permanent address, range from 1 to 8.
        } bits;
    } actionFlags;

    unsigned char *PAddress; // Permenet transceiver address

} tMACInitParameters;

extern tMACReceivedPacket MACRxPacket;

struct
{
    unsigned char PayloadLen;
    unsigned char Payload[144];
} RxBuffer[2];

#ifdef	__cplusplus
}
#endif

#endif	/* MRF24J40_H */

