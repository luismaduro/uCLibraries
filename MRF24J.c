#include <p18cxxx.h>
#include "MRF24J.h"
#include "SPIDevice.h"
#include <delays.h>

unsigned char MRF24J40ReadShort(unsigned char address);
unsigned char MRF24J40ReadLong(unsigned int address);
void MRF24J40WriteShort(unsigned char address, unsigned char data);
void MRF24J40WriteLong(unsigned int address, unsigned char data);

unsigned char MRF24J40ReadShort(unsigned char address)
{
    unsigned char data;

    address &= 0x3f; /* Trim address to 6 bits */
    address <<= 1; /* Shift into the right position */
    address &= ~0x01; /* Clear write bit */

    MIWI_SELECT();
    SPIWrite(address);
    data = SPIRead();
    MIWI_DESELECT();

    return data;
}

unsigned char MRF24J40ReadLong(unsigned int address)
{
    unsigned char data = 0, msb = 0, lsb = 0;

    address &= 0x03FF; //Trim address to 10 bits
    address <<= 5; //Shift the address to the msb's
    address &= ~0x0010; //Clear write bit (read)
    address |= 0x8000; //Set long addr bit

    msb = (unsigned char) ((address & 0xFF00) >> 8); //Get the 8 msb and shift it into position
    lsb = (unsigned char) (address & 0x00FF); //Get the 8 lsb

    MIWI_SELECT();
    SPIWrite(msb);
    SPIWrite(lsb);
    data = SPIRead();
    MIWI_DESELECT();

    return data;
}

void MRF24J40WriteShort(unsigned char address, unsigned char data)
{
    address &= 0x3f; /* Trim address to 6 bits */
    address <<= 1; /* Shift into the right position */
    address |= 0x01; /* Set R/W bit to write */

    MIWI_SELECT();
    SPIWrite(address);
    SPIWrite(data);
    MIWI_DESELECT();
}

void MRF24J40WriteLong(unsigned int address, unsigned char data)
{
    unsigned char msb = 0, lsb = 0;

    address &= 0x03FF; //Trim address to 10 bits
    address <<= 5; //Shift the address to the msb's
    address |= 0x0010; //Clear write bit (write)
    address |= 0x8000; //Set long addr bit

    msb = (unsigned char) ((address & 0xFF00) >> 8);
    lsb = (unsigned char) (address & 0x00FF);

    MIWI_SELECT();
    SPIWrite(msb);
    SPIWrite(lsb);
    SPIWrite(data);
    MIWI_DESELECT();
}

void MRF24J40Reset(void)
{
    MIWI_RESET_LOW();
    Delay1KTCYx(1);
    MIWI_RESET_HIGH();
    Delay1KTCYx(1); // from manual
}

unsigned int MRF24J40GetPanID(void)
{
    unsigned int panh = MRF24J40ReadShort(MRF_PANIDH);
    return (panh << 8 | MRF24J40ReadShort(MRF_PANIDL));
}

void MRF24J40SetPanID(unsigned int panid)
{
    MRF24J40WriteShort(MRF_PANIDH, panid >> 8);
    MRF24J40WriteShort(MRF_PANIDL, panid & 0xff);
}

void MRF24J40ShortAddressWrite(unsigned int address)
{
    MRF24J40WriteShort(MRF_SADRH, address >> 8);
    MRF24J40WriteShort(MRF_SADRL, address & 0xff);
}

unsigned int MRF24J40ShortAddressRead(void)
{
    unsigned int a16h = MRF24J40ReadShort(MRF_SADRH);
    return (a16h << 8 | MRF24J40ReadShort(MRF_SADRL));
}

void MRF24J40LongAddressWrite(unsigned char * address)
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        MRF24J40WriteShort(MRF_EADR0 + i, address[i]); // 0x05 address of EADR0
    }
}

void MRF24J40LongAddressRead(unsigned char * address)
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        address[i] = MRF24J40ReadShort(MRF_EADR0 + i); // 0x05 address of EADR0
    }
}

/**
 * Simple send 16, with acks, not much of anything.. assumes src16 and local pan only.
 * @param data
 */
void MRF24J40SendPacket(unsigned int dest, unsigned int len, char* packet)
{
    int i = 0, q = 0;
    unsigned int panid = MRF24J40GetPanID();
    unsigned int src16 = MRF24J40ShortAddressRead();

    MRF24J40WriteLong(i++, 11); // header length
    MRF24J40WriteLong(i++, 11 + len);

    // 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
    MRF24J40WriteLong(i++, 0b01100001); // first byte of Frame Control

    // 16 bit source, 802.15.4 (2003), 16 bit dest
    MRF24J40WriteLong(i++, 0b10001000); // second byte of frame control

    MRF24J40WriteLong(i++, 1); // sequence number 1

    MRF24J40WriteLong(i++, panid & 0xff); // dest panid
    MRF24J40WriteLong(i++, panid >> 8);

    MRF24J40WriteLong(i++, dest & 0xff); // dest16 low
    MRF24J40WriteLong(i++, dest >> 8); // dest16 high

    MRF24J40WriteLong(i++, panid & 0xff); // dest panid
    MRF24J40WriteLong(i++, panid >> 8);

    MRF24J40WriteLong(i++, src16 & 0xff); // src16 low
    MRF24J40WriteLong(i++, src16 >> 8); // src16 high

    for (q = 0; q < len; q++)
    {
        MRF24J40WriteLong(i++, packet[q]);
    }

    // ack on, and go!
    MRF24J40WriteShort(MRF_TXNCON, 0b00000101);

    HOST_INTERRUPT_ENABLE = 0;

    while (MRF24J40GetInterrupts() != MRF_I_TXNIF);

    HOST_INTERRUPT_ENABLE = 1;
}

unsigned char MRF24J40ReceivePacket(char* packet)
{
    unsigned char interruptWasEnabled = HOST_INTERRUPT_ENABLE;
    unsigned char frameLength = 0;
    unsigned char header[12] = {0};
    unsigned char i = 0x00;
    unsigned char lqi = 0, rssi = 0;

    // We got a packet.
    // Disable host microcontroller interrupt.

    // Disable receiving packets off air, set RXDECINV = 1
    MRF24J40RXDisable();

    if (interruptWasEnabled)
    {
        HOST_INTERRUPT_ENABLE = 0;
    }

    frameLength = MRF24J40ReadLong(MRF_RXFIFO);

    for (i = 0; i < 12; i++)
        header[i] = MRF24J40ReadLong(MRF_RXFIFO + i);

    for (i = 12; i < frameLength - 1; i++)
    {
        packet[i - 12] = MRF24J40ReadLong(MRF_RXFIFO + i);
    }

    lqi = MRF24J40ReadLong(MRF_RXFIFO + frameLength + 1);
    rssi = MRF24J40ReadLong(MRF_RXFIFO + frameLength + 2);

    // Flush rx fifo
    MRF24J40RXFlush();

    // Enable receiving packets off air, set RXDECINV = 0
    MRF24J40RXEnable();

    // Enable host microcontroller interrupt.
    if (interruptWasEnabled)
    {
        HOST_INTERRUPT_ENABLE = 1;
    }

    return frameLength;
}

void MRF24J40SetInterrupts(void)
{
    // interrupts for rx and tx normal complete

    MRF24J40WriteShort(MRF_INTCON, 0b11110110); //rx int and tx int
    MRF24J40WriteLong(MRF_SLPCON0, 0b00000010); //set int rising edge and sleep clock enable
}

unsigned char MRF24J40GetInterrupts(void)
{
    return (MRF24J40ReadShort(MRF_INTSTAT));
}

/** use the 802.15.4 channel numbers..
 */
void MRF24J40SetChannel(unsigned char channel)
{
    MRF24J40WriteLong(MRF_RFCON0, (((channel) << 4) | 0x03));
    MRF24J40WriteShort(MRF_RFCTL, 0x04); //  â€“ Reset RF state machine.
    MRF24J40WriteShort(MRF_RFCTL, 0x00); // part 2
}

unsigned char MRF24J40GetChannel(void)
{
    return (11 + (MRF24J40ReadLong(MRF_RFCON0) >> 4));
}

void MRF24J40SetPower(unsigned char power)
{
    MRF24J40WriteLong(MRF_RFCON3, (power << 3) & 0b11111000);
}

void MRF24J40Init(unsigned char channel, unsigned char power, unsigned int pan, unsigned int short_address)
{

    // Seems a bit ridiculous when I use reset pin anyway
    //write_short(MRF_SOFTRST, 0x7); // from manual
    //while (read_short(MRF_SOFTRST) & 0x7 != 0);         //wait until reset finish

    MRF24J40Reset();

    // wait for mrf to be in receive mode
    while (MRF24J40ReadLong(MRF_RFSTATE) & 0xa0 != 0xa0);

    MRF24J40RXFlush();

    // Setup Sleep - Enable Immediate Wake-up mode, wake pin polarity to active high
    MIWI_WAKE_LOW();
    MRF24J40WriteShort(MRF_RXFLUSH, 0x60);
    MRF24J40WriteShort(MRF_WAKECON, 0x80);

    //Perform a power management reset
    MRF24J40WriteShort(MRF_SOFTRST, 0x40);

    MIWI_WAKE_HIGH();

    MRF24J40WriteLong(MRF_TESTMODE, 0x0f); //enable PA/LNA

    MRF24J40WriteShort(MRF_RFCTL, 0x04); // Reset RF state machine.
    MRF24J40WriteShort(MRF_RFCTL, 0x00); // part 2

    MRF24J40WriteLong(MRF_RFCON0, 0x03); //Initialize RFOPT = 0x03.
    MRF24J40WriteLong(MRF_RFCON1, 0x01); //Initialize VCOOPT = 0x02.
    MRF24J40WriteLong(MRF_RFCON2, 0x80); //Enable PLL (PLLEN = 1).
    MRF24J40SetPower(power);
    MRF24J40WriteLong(MRF_RFCON6, 0x90); //Initialize TXFIL = 1 and 20MRECVR = 1.
    MRF24J40WriteLong(MRF_RFCON7, 0x80); //Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
    MRF24J40WriteLong(MRF_RFCON8, 0x10); //Initialize RFVCO = 1.
    MRF24J40WriteLong(MRF_SLPCON1, 0x21); //Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

    //  Configuration for nonbeacon-enabled devices (see Section 3.8,"Beacon-Enabled and Nonbeacon-Enabled Networks"):

    MRF24J40WriteShort(MRF_BBREG2, 0x80); // Set CCA mode to ED
    MRF24J40WriteShort(MRF_CCAEDTH, 0x60); // Set CCA ED threshold.
    MRF24J40WriteShort(MRF_BBREG6, 0x40); // Set appended RSSI value to RXFIFO.

    MRF24J40WriteShort(MRF_PACON2, 0x98); //Initialize FIFOEN = 1 and TXONTS = 0x6.
    MRF24J40WriteShort(MRF_TXSTBL, 0x95); //Initialize RFSTBL = 0x9.

    MRF24J40SetInterrupts();
    MRF24J40SetChannel(channel);

    MRF24J40SetPanID(pan);
    MRF24J40ShortAddressWrite(short_address);

    // wait for mrf to be in receive mode
    while (MRF24J40ReadLong(MRF_RFSTATE) & 0xa0 != 0xa0);

    MRF24J40WriteShort(MRF_RFCTL, 0x04); // Reset RF state machine.
    MRF24J40WriteShort(MRF_RFCTL, 0x00); // part 2

    Delay1KTCYx(1); // delay at least 192usec
}

void MRF24J40SetPromiscuous(boolean enabled)
{
    if (enabled == true)
    {
        MRF24J40WriteShort(MRF_RXMCR, 0x01);
    }
    else
    {
        MRF24J40WriteShort(MRF_RXMCR, 0x00);
    }
}

void MRF24J40RXFlush(void)
{
    MRF24J40WriteShort(MRF_RXFLUSH, MRF24J40ReadShort(MRF_RXFLUSH) | 0x01);
}

void MRF24J40RXDisable(void)
{
    MRF24J40WriteShort(MRF_BBREG1, 0x04); // RXDECINV - disable receiver
}

void MRF24J40RXEnable(void)
{
    MRF24J40WriteShort(MRF_BBREG1, 0x00); // RXDECINV - enable receiver
}