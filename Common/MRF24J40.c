/**
 *  @file       MRF24J40.c
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
#include "MRF24J40.h"

unsigned char MACCurrentChannel;
tMACInitParameters MACInitParams;
volatile tMRF24J40Status MRF24J40Status;
WORD_VAL MACPANID;
WORD_VAL myNetworkAddress;
unsigned char BankIndex = 0xFF;
volatile unsigned int failureCounter = 0;

unsigned char MRF24J40GetLongRAMAddr(unsigned char address)
{
    unsigned char toReturn;
    volatile unsigned char tmpRFIE = HOST_INTERRUPT;

    HOST_INTERRUPT = 0;
    MRF24J40_SELECT_DEVICE();
    SPIWrite(address);
    toReturn = SPIRead();
    MRF24J40_DESELECT_DEVICE();
    HOST_INTERRUPT = tmpRFIE;

    return toReturn;
}

unsigned char MRF24J40GetShortRAMAddr(unsigned int address)
{
    unsigned char toReturn;
    volatile unsigned char tmpRFIE = HOST_INTERRUPT;

    HOST_INTERRUPT = 0;
    MRF24J40_SELECT_DEVICE();
    SPIWrite(((address >> 3)&0x7F) | 0x80);
    SPIWrite(((address << 5)&0xE0));
    toReturn = SPIRead();
    MRF24J40_DESELECT_DEVICE();

    return toReturn;
}

void MRF24J40SetShortRAMAddr(unsigned char address, unsigned char data)
{
    volatile unsigned char tmpRFIE = HOST_INTERRUPT;

    HOST_INTERRUPT = 0;
    MRF24J40_SELECT_DEVICE();
    SPIWrite(address);
    SPIWrite(data);
    MRF24J40_DESELECT_DEVICE();
    HOST_INTERRUPT = tmpRFIE;
}

void MRF24J40SetLongRAMAddr(unsigned int address, unsigned char data)
{
    volatile unsigned char tmpRFIE = HOST_INTERRUPT;

    HOST_INTERRUPT = 0;
    MRF24J40_SELECT_DEVICE();
    SPIWrite((((unsigned char) (address >> 3))&0x7F) | 0x80);
    SPIWrite((((unsigned char) (address << 5))&0xE0) | 0x10);
    SPIWrite(data);
    MRF24J40_DESELECT_DEVICE();
    HOST_INTERRUPT = tmpRFIE;
}

void InitMRF24J40(void)
{
    unsigned char i;
    unsigned int j;

    // first perform a hardware reset
    MRF24J40_RESET();
    for (j = 0; j < (unsigned int) 300; j++)
    {
    }

    MRF24J40_RUN();
    for (j = 0; j < (unsigned int) 300; j++)
    {
    }

    /* do a soft reset */
    MRF24J40SetShortRAMAddr(WRITE_SOFTRST, 0x07);
    do
    {
        i = MRF24J40GetShortRAMAddr(READ_SOFTRST);
    }
    while ((i & 0x07) != (unsigned char) 0x00);

    for (j = 0; j < (unsigned int) 1000; j++)
    {
    }

    /* flush the RX fifo */
    MRF24J40SetShortRAMAddr(WRITE_RXFLUSH, 0x01);

    /* Program the short MAC Address, 0xffff */
    MRF24J40SetShortRAMAddr(WRITE_SADRL, 0xFF);
    MRF24J40SetShortRAMAddr(WRITE_SADRH, 0xFF);
    MRF24J40SetShortRAMAddr(WRITE_PANIDL, 0xFF);
    MRF24J40SetShortRAMAddr(WRITE_PANIDH, 0xFF);

    /* Program Long MAC Address*/
    for (i = 0; i < (unsigned char) 8; i++)
    {
        MRF24J40SetShortRAMAddr(WRITE_EADR0 + i * 2,
                                MACInitParams.PAddress[i]);
    }

    /* setup */
    MRF24J40SetLongRAMAddr(RFCTRL2, 0x80);

#if defined(ENABLE_PA_LNA)
#if defined(MRF24J40MB)
    //Power set to 0dB
    MRF24J40SetLongRAMAddr(RFCTRL3, 0x00);
#endif

    /* program RSSI ADC with 2.5 MHz clock */
    MRF24J40SetLongRAMAddr(RFCTRL6, 0x90);

    MRF24J40SetLongRAMAddr(RFCTRL7, 0x80);

    MRF24J40SetLongRAMAddr(RFCTRL8, 0x10);

    MRF24J40SetLongRAMAddr(SCLKDIV, 0x21);

    /* Program CCA mode using RSSI */
    MRF24J40SetShortRAMAddr(WRITE_BBREG2, 0x80);
    /* Enable the packet RSSI */
    MRF24J40SetShortRAMAddr(WRITE_BBREG6, 0x40);
    /* Program CCA, RSSI threshold values */
    MRF24J40SetShortRAMAddr(WRITE_RSSITHCCA, 0x60);

#if defined(ENABLE_PA_LNA)

#if defined(MRF24J40MC)
    MRF24J40SetShortRAMAddr(WRITE_GPIODIR, 0x08);
    MRF24J40SetShortRAMAddr(WRITE_GPIO, 0x08);
#endif
    MRF24J40SetLongRAMAddr(TESTMODE, 0x0F);
#endif

    MRF24J40SetShortRAMAddr(WRITE_FFOEN, 0x98);
    MRF24J40SetShortRAMAddr(WRITE_TXPEMISP, 0x95);

    // wait until the MRF24J40 in receive mode
    do
    {
        i = MRF24J40GetLongRAMAddr(RFSTATE);
    }
    while ((i & 0xA0) != 0xA0);

    MRF24J40SetShortRAMAddr(WRITE_INTMSK, 0xE6);

#ifdef ENABLE_INDIRECT_MESSAGE
    MRF24J40SetShortRAMAddr(WRITE_ACKTMOUT, 0xB9);
#endif

    // Make RF communication stable under extreme temperatures
    MRF24J40SetLongRAMAddr(RFCTRL0, 0x03);
    MRF24J40SetLongRAMAddr(RFCTRL1, 0x02);

    MRF24J40SetChannel(MACCurrentChannel, 0);

    // Define TURBO_MODE if more bandwidth is required
    // to enable radio to operate to TX/RX maximum
    // 625Kbps
#ifdef TURBO_MODE

    MRF24J40SetShortRAMAddr(WRITE_BBREG0, 0x01);
    MRF24J40SetShortRAMAddr(WRITE_BBREG3, 0x38);
    MRF24J40SetShortRAMAddr(WRITE_BBREG4, 0x5C);

    MRF24J40SetShortRAMAddr(WRITE_RFCTL, 0x04);
    MRF24J40SetShortRAMAddr(WRITE_RFCTL, 0x00);

#endif

}

/**
 * This is the primary MiMAC interface for the protocol layer to
 * set the operating frequency of the RF transceiver. Valid channel
 * number are from 0 to 31. For different frequency band, data rate
 * and other RF settings, some channels from 0 to 31 might be
 * unavailable. Paramater offsetFreq is used to fine tune the center
 * frequency across the frequency band. For transceivers that follow
 * strict definition of channels, this parameter may be discarded.
 * The center frequency is calculated as
 * (LowestFrequency + Channel * ChannelGap + offsetFreq)
 *
 * @param channel Channel number. Range from 0 to 31. Not all channels
 *                are available under all conditions.
 *
 * @param offsetFreq Offset frequency used to fine tune the center
 *                   frequency. May not apply to all RF transceivers
 *
 * @return A boolean to indicates if channel setting is successful.
 *
 * @code
 *  // Set center frequency to be exactly channel 12
 *  MRF24J40MACSetChannel(12, 0);
 * @endcode
 */
boolean MRF24J40MACSetChannel(unsigned char channel,
                              unsigned char offsetFreq)
{
    if (channel < 11 || channel > 26)
    {
        return false;
    }

#if defined(ENABLE_PA_LNA) && (defined(MRF24J40MB) || defined(MRF24J40MC))
    if (channel == 26)
    {
        return false;
    }
#endif

    MACCurrentChannel = channel;
    MRF24J40SetLongRAMAddr(RFCTRL0, ((channel - 11) << 4) | 0x03);
    MRF24J40SetShortRAMAddr(WRITE_RFCTL, 0x04);
    MRF24J40SetShortRAMAddr(WRITE_RFCTL, 0x00);

    return true;
}

/**
 * This is the primary MiMAC interface for the protocol layer to
 * set the output power for the RF transceiver. Whether the RF
 * transceiver can adjust output power depends on the hardware
 * implementation.
 *
 * @param outputPower RF transceiver output power.
 * @return A boolean to indicates if setting output power is successful.
 *
 * @code
 * // Set output power to be 0dBm
 * MRF24J40MACSetPower(TX_POWER_0_DB);
 * @endcode
 *
 */
boolean MRF24J40MACSetPower(unsigned char outputPower)
{
    unsigned char reg;
    unsigned char tmp = outputPower % 10;

    reg = (outputPower / 10) << 6;

    if (tmp > 5)
    {
        reg += 0x38;
    }
    else if (tmp > 4)
    {
        reg += 0x30;
    }
    else if (tmp > 3)
    {
        reg += 0x28;
    }
    else if (tmp > 2)
    {
        reg += 0x20;
    }
    else if (tmp > 1)
    {
        reg += 0x18;
    }
    else if (tmp > 0)
    {
        reg += 0x10;
    }

    MRF24J40SetLongRAMAddr(RFCTRL3, reg);

    return true;
}

/**
 * This is the primary MAC interface for the protocol layer to
 * set alternative network address and/or PAN identifier. This function
 * call applies to only IEEE 802.15.4 compliant RF transceivers. In case
 * alternative network address is not supported, this function will return
 * false.
 *
 * @param Address The alternative network address of the host device.
 *
 * @param PANID The PAN identifier of the host device
 * 
 * @return A boolean to indicates if setting alternative network address
 * is successful.
 *
 * @code
 * unsigned int NetworkAddress = 0x0000;
 * unsigned int PANID = 0x1234;
 * MiMAC_SetAltAddress(&NetworkAddress, &PANID);
 * @endcode
 */
boolean MRF24J40MACSetAltAddress(unsigned int *Address,
                                 unsigned int *PANID)
{
    myNetworkAddress.v[0] = Address[0];
    myNetworkAddress.v[1] = Address[1];
    MACPANID.v[0] = PANID[0];
    MACPANID.v[1] = PANID[1];

    MRF24J40SetShortRAMAddr(WRITE_SADRL, myNetworkAddress.v[0]);
    MRF24J40SetShortRAMAddr(WRITE_SADRH, myNetworkAddress.v[1]);
    MRF24J40SetShortRAMAddr(WRITE_PANIDL, MACPANID.v[0]);
    MRF24J40SetShortRAMAddr(WRITE_PANIDH, MACPANID.v[1]);

    return true;
}

boolean MRF24J40MACInit(tMACInitParameters initValue)
{
    unsigned char i;

    MACInitParams = initValue;

    MACCurrentChannel = 11;

    InitMRF24J40();

    MRF24J40Status.Val = 0;

    for (i = 0; i < BANK_SIZE; i++)
    {
        RxBuffer[i].PayloadLen = 0;
    }
    return TRUE;
}

boolean MRF24J40MACReceivedPacket(void)
{
    BYTE i;

    //If the stack TX has been busy for a long time then
    //time out the TX because we may have missed the interrupt
    //and don't want to lock up the stack forever
    if (MRF24J40Status.bits.TX_BUSY)
    {
        if (failureCounter >= FAILURE_COUNTER)
        {
            failureCounter = 0;
            MRF24J40Status.bits.TX_BUSY = 0;
        }
        else
        {
            failureCounter++;
        }
    }

    BankIndex = 0xFF;

    for (i = 0; i < BANK_SIZE; i++)
    {
        if (RxBuffer[i].PayloadLen > 0)
        {
            BankIndex = i;
            break;
        }
    }

    //if(MRF24J40Status.bits.RX_BUFFERED == 1)
    if (BankIndex < BANK_SIZE)
    {
        BYTE addrMode;
#ifndef TARGET_SMALL
        BOOL bIntraPAN = TRUE;

        if ((RxBuffer[BankIndex].Payload[0] & 0x40) == 0)
        {
            bIntraPAN = FALSE;
        }
#endif
        MACRxPacket.flags.Val = 0;
        MACRxPacket.altSourceAddress = FALSE;

        //Determine the start of the MAC payload
        addrMode = RxBuffer[BankIndex].Payload[1] & 0xCC;
        switch (addrMode)
        {
        case 0xC8: //short dest, long source
            // for P2P only broadcast allows short destination address
            if (RxBuffer[BankIndex].Payload[5] == 0xFF && RxBuffer[BankIndex].Payload[6] == 0xFF)
            {
                MACRxPacket.flags.bits.broadcast = 1;
            }
            MACRxPacket.flags.bits.sourcePrsnt = 1;

#ifndef TARGET_SMALL
            if (bIntraPAN) // check if it is intraPAN
#endif
            {
#ifndef TARGET_SMALL
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
#endif
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[7]);

                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 19;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[15]);
            }
#ifndef TARGET_SMALL
            else
            {
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[7];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[8];
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[9]);
                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 21;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[17]);
            }
#endif

            break;

        case 0xCC: // long dest, long source
            MACRxPacket.flags.bits.sourcePrsnt = 1;
#ifndef TARGET_SMALL
            if (bIntraPAN) // check if it is intraPAN
#endif
            {
                //rxFrame.flags.bits.intraPAN = 1;
#ifndef TARGET_SMALL
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
#endif
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[13]);
                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 25;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[21]);
            }
#ifndef TARGET_SMALL
            else
            {
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[13];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[14];
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[15]);
                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 27;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[23]);
            }
#endif
            break;

        case 0x80: // short source only. used in beacon
        {
            MACRxPacket.flags.bits.broadcast = 1;
            MACRxPacket.flags.bits.sourcePrsnt = 1;
            MACRxPacket.altSourceAddress = TRUE;
            MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
            MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
            MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[5]);
            MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 11;
            MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[7]);
        }
            break;

        case 0x88: // short dest, short source
        {
            if (RxBuffer[BankIndex].Payload[5] == 0xFF && RxBuffer[BankIndex].Payload[6] == 0xFF)
            {
                MACRxPacket.flags.bits.broadcast = 1;
            }
            MACRxPacket.flags.bits.sourcePrsnt = 1;
            MACRxPacket.altSourceAddress = TRUE;
#ifndef TARGET_SMALL
            if (bIntraPAN == FALSE)
            {
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[7];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[8];
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[9]);
                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 15;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[11]);
            }
            else
#endif
            {
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[7]);
                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 13;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[9]);
            }
        }
            break;

        case 0x8C: // long dest, short source
        {
            MACRxPacket.flags.bits.sourcePrsnt = 1;
            MACRxPacket.altSourceAddress = TRUE;
#ifndef TARGET_SMALL
            if (bIntraPAN) // check if it is intraPAN
#endif
            {
#ifndef TARGET_SMALL
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[3];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[4];
#endif
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[12]);

                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 19;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[15]);
            }
#ifndef TARGET_SMALL
            else
            {
                MACRxPacket.SourcePANID.v[0] = RxBuffer[BankIndex].Payload[12];
                MACRxPacket.SourcePANID.v[1] = RxBuffer[BankIndex].Payload[13];
                MACRxPacket.SourceAddress = &(RxBuffer[BankIndex].Payload[14]);
                MACRxPacket.PayloadLen = RxBuffer[BankIndex].PayloadLen - 21;
                MACRxPacket.Payload = &(RxBuffer[BankIndex].Payload[17]);
            }
#endif
        }
            break;


        case 0x08: //dest-short, source-none
            // all other addressing mode will not be supported in P2P
        default:
            // not valid addressing mode or no addressing info
            MiMAC_DiscardPacket();
            return FALSE;
        }

#ifdef ENABLE_SECURITY
        if (RxBuffer[BankIndex].Payload[0] & 0x08)
        {
            // if security is used, remove the security header and MIC from
            // the payload size
            switch (mySecurityLevel)
            {
            case 0x02:
            case 0x05:
                MACRxPacket.PayloadLen -= 21;
                break;
            case 0x03:
            case 0x06:
                MACRxPacket.PayloadLen -= 13;
                break;
            case 0x04:
            case 0x07:
                MACRxPacket.PayloadLen -= 9;
                break;
            case 0x01:
                MACRxPacket.PayloadLen -= 5;
                break;
            default:
                break;
            }
            // remove the security header from the payload
            MACRxPacket.Payload = &(MACRxPacket.Payload[5]);
            MACRxPacket.flags.bits.secEn = 1;
        }
#else
        if (RxBuffer[BankIndex].Payload[0] & 0x08)
        {
            MiMAC_DiscardPacket();
            return FALSE;
        }
#endif

        // check the frame type. Only the data and command frame type
        // are supported. Acknowledgement frame type is handled in
        // MRF24J40 transceiver hardware.
        switch (RxBuffer[BankIndex].Payload[0] & 0x07) // check frame type
        {
        case 0x01: // data frame
            MACRxPacket.flags.bits.packetType = PACKET_TYPE_DATA;
            break;
        case 0x03: // command frame
            MACRxPacket.flags.bits.packetType = PACKET_TYPE_COMMAND;
            break;
        case 0x00:
            // use reserved packet type to represent beacon packet
            MACRxPacket.flags.bits.packetType = PACKET_TYPE_RESERVE;
            break;
        default: // not support frame type
            MiMAC_DiscardPacket();
            return FALSE;
        }
#ifndef TARGET_SMALL
        MACRxPacket.LQIValue = RxBuffer[BankIndex].Payload[RxBuffer[BankIndex].PayloadLen - 2];
        MACRxPacket.RSSIValue = RxBuffer[BankIndex].Payload[RxBuffer[BankIndex].PayloadLen - 1];
#endif

        return TRUE;
    }
    return FALSE;
}