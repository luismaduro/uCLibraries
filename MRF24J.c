#include <p18cxxx.h>
#include <spi.h>
#include "MRF24J.h"
#include <delays.h>

unsigned char read_short(unsigned char address)
{
    unsigned char data;

    address &= 0x3f; /* Trim address to 6 bits */
    address <<= 1; /* Shift into the right position */
    address &= ~0x01; /* Clear write bit */

    MIWI_CS_LOW();
    WriteSPI2(address);
    data = ReadSPI2();
    MIWI_CS_HIGH();

    return data;
}

unsigned char read_long(unsigned int address)
{
    unsigned char data = 0, msb = 0, lsb = 0;

    address &= 0x03FF; //Trim address to 10 bits
    address <<= 5; //Shift the address to the msb's
    address &= ~0x0010; //Clear write bit (read)
    address |= 0x8000; //Set long addr bit

    msb = (unsigned char) ((address & 0xFF00) >> 8); //Get the 8 msb and shift it into position
    lsb = (unsigned char) (address & 0x00FF); //Get the 8 lsb

    MIWI_CS_LOW();
    WriteSPI2(msb);
    WriteSPI2(lsb);
    data = ReadSPI2();
    MIWI_CS_HIGH();

    return data;
}

void write_short(unsigned char address, unsigned char data)
{
    address &= 0x3f; /* Trim address to 6 bits */
    address <<= 1; /* Shift into the right position */
    address |= 0x01; /* Set R/W bit to write */

    MIWI_CS_LOW();
    WriteSPI2(address);
    WriteSPI2(data);
    MIWI_CS_HIGH();
}

void write_long(unsigned int address, unsigned char data)
{
    unsigned char msb = 0, lsb = 0;

    address &= 0x03FF; //Trim address to 10 bits
    address <<= 5; //Shift the address to the msb's
    address |= 0x0010; //Clear write bit (write)
    address |= 0x8000; //Set long addr bit

    msb = (unsigned char) ((address & 0xFF00) >> 8);
    lsb = (unsigned char) (address & 0x00FF);

    MIWI_CS_LOW();
    WriteSPI2(msb);
    WriteSPI2(lsb);
    WriteSPI2(data);
    MIWI_CS_HIGH();
}

void MRF24J40_Reset(void)
{
    MIWI_RESET_LOW();
    Delay1KTCYx(1);
    MIWI_RESET_HIGH();
    Delay1KTCYx(1); // from manual
}

unsigned int MRF24J40_Get_Pan(void)
{
    unsigned int panh = read_short(MRF_PANIDH);
    return (panh << 8 | read_short(MRF_PANIDL));
}

void MRF24J40_Set_Pan(unsigned int panid)
{
    write_short(MRF_PANIDH, panid >> 8);
    write_short(MRF_PANIDL, panid & 0xff);
}

void MRF24J40_Short_Address_Write(unsigned int address)
{
    write_short(MRF_SADRH, address >> 8);
    write_short(MRF_SADRL, address & 0xff);
}

unsigned int MRF24J40_Short_Address_Read(void)
{
    unsigned int a16h = read_short(MRF_SADRH);
    return (a16h << 8 | read_short(MRF_SADRL));
}

void MRF24J40_Long_Address_Write(unsigned char * address)
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        write_short(MRF_EADR0 + i, address[i]); // 0x05 address of EADR0
    }
}

void MRF24J40_Long_Address_Read(unsigned char * address)
{
    unsigned char i = 0;

    for (i = 0; i < 8; i++)
    {
        address[i] = read_short(MRF_EADR0 + i); // 0x05 address of EADR0
    }
}

/**
 * Simple send 16, with acks, not much of anything.. assumes src16 and local pan only.
 * @param data
 */
void MRF24J40_Packet_Send(unsigned int dest, unsigned int len, char* packet)
{
    int i = 0, q = 0;
    unsigned int panid = MRF24J40_Get_Pan();
    unsigned int src16 = MRF24J40_Short_Address_Read();

    write_long(i++, 11); // header length
    write_long(i++, 11 + len);

    // 0 | pan compression | ack | no security | no data pending | data frame[3 bits]
    write_long(i++, 0b01100001); // first byte of Frame Control

    // 16 bit source, 802.15.4 (2003), 16 bit dest
    write_long(i++, 0b10001000); // second byte of frame control

    write_long(i++, 1); // sequence number 1

    write_long(i++, panid & 0xff); // dest panid
    write_long(i++, panid >> 8);

    write_long(i++, dest & 0xff); // dest16 low
    write_long(i++, dest >> 8); // dest16 high

    write_long(i++, panid & 0xff); // dest panid
    write_long(i++, panid >> 8);

    write_long(i++, src16 & 0xff); // src16 low
    write_long(i++, src16 >> 8); // src16 high

    for (q = 0; q < len; q++)
    {
        write_long(i++, packet[q]);
    }

    // ack on, and go!
    write_short(MRF_TXNCON, 0b00000101);

    HOST_INTERRUPT_ENABLE = 0;

    while (MRF24J40_Get_Interrupts() != MRF_I_TXNIF);

    HOST_INTERRUPT_ENABLE = 1;
}

unsigned char MRF24J40_Packet_Receive(char* packet)
{
    unsigned char interruptWasEnabled = HOST_INTERRUPT_ENABLE;
    unsigned char frameLength = 0;
    unsigned char header[12] = {0};
    unsigned char i = 0x00;
    unsigned char lqi = 0, rssi = 0;

    // We got a packet.
    // Disable host microcontroller interrupt.

    // Disable receiving packets off air, set RXDECINV = 1
    MRF24J40_RX_Disable();

    if (interruptWasEnabled)
    {
        HOST_INTERRUPT_ENABLE = 0;
    }

    frameLength = read_long(MRF_RXFIFO);

    for (i = 0; i < 12; i++)
        header[i] = read_long(MRF_RXFIFO + i);

    for (i = 12; i < frameLength - 1; i++)
    {
        packet[i - 12] = read_long(MRF_RXFIFO + i);
    }

    lqi = read_long(MRF_RXFIFO + frameLength + 1);
    rssi = read_long(MRF_RXFIFO + frameLength + 2);

    // Flush rx fifo
    MRF24J40_RX_Flush();

    // Enable receiving packets off air, set RXDECINV = 0
    MRF24J40_RX_Enable();

    // Enable host microcontroller interrupt.
    if (interruptWasEnabled)
    {
        HOST_INTERRUPT_ENABLE = 1;
    }

    return frameLength;
}

void MRF24J40_Set_Interrupts(void)
{
    // interrupts for rx and tx normal complete

    write_short(MRF_INTCON, 0b11110110); //rx int and tx int
    write_long(MRF_SLPCON0, 0b00000010); //set int rising edge and sleep clock enable
}

unsigned char MRF24J40_Get_Interrupts(void)
{
    return (read_short(MRF_INTSTAT));
}

/** use the 802.15.4 channel numbers..
 */
void MRF24J40_Set_Channel(unsigned char channel)
{
    write_long(MRF_RFCON0, (((channel) << 4) | 0x03));
    write_short(MRF_RFCTL, 0x04); //  â€“ Reset RF state machine.
    write_short(MRF_RFCTL, 0x00); // part 2
}

unsigned char MRF24J40_Get_Channel(void)
{
    return (11 + (read_long(MRF_RFCON0) >> 4));
}

void MRF24J40_Set_Power(unsigned char power)
{
    write_long(MRF_RFCON3, (power << 3) & 0b11111000);
}

void MRF24J40_Init(unsigned char channel, unsigned char power, unsigned int pan, unsigned int short_address)
{

    // Seems a bit ridiculous when I use reset pin anyway
    //write_short(MRF_SOFTRST, 0x7); // from manual
    //while (read_short(MRF_SOFTRST) & 0x7 != 0);         //wait until reset finish

    MRF24J40_Reset();

    // wait for mrf to be in receive mode
    while (read_long(MRF_RFSTATE) & 0xa0 != 0xa0);

    MRF24J40_RX_Flush();

    // Setup Sleep - Enable Immediate Wake-up mode, wake pin polarity to active high
    MIWI_WAKE_LOW();
    write_short(MRF_RXFLUSH, 0x60);
    write_short(MRF_WAKECON, 0x80);

    //Perform a power management reset
    write_short(MRF_SOFTRST, 0x40);

    MIWI_WAKE_HIGH();

    write_long(MRF_TESTMODE, 0x0f); //enable PA/LNA

    write_short(MRF_RFCTL, 0x04); // Reset RF state machine.
    write_short(MRF_RFCTL, 0x00); // part 2

    write_long(MRF_RFCON0, 0x03); //Initialize RFOPT = 0x03.
    write_long(MRF_RFCON1, 0x01); //Initialize VCOOPT = 0x02.
    write_long(MRF_RFCON2, 0x80); //Enable PLL (PLLEN = 1).
    MRF24J40_Set_Power(power);
    write_long(MRF_RFCON6, 0x90); //Initialize TXFIL = 1 and 20MRECVR = 1.
    write_long(MRF_RFCON7, 0x80); //Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
    write_long(MRF_RFCON8, 0x10); //Initialize RFVCO = 1.
    write_long(MRF_SLPCON1, 0x21); //Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.

    //  Configuration for nonbeacon-enabled devices (see Section 3.8,"Beacon-Enabled and Nonbeacon-Enabled Networks"):

    write_short(MRF_BBREG2, 0x80); // Set CCA mode to ED
    write_short(MRF_CCAEDTH, 0x60); // Set CCA ED threshold.
    write_short(MRF_BBREG6, 0x40); // Set appended RSSI value to RXFIFO.

    write_short(MRF_PACON2, 0x98); //Initialize FIFOEN = 1 and TXONTS = 0x6.
    write_short(MRF_TXSTBL, 0x95); //Initialize RFSTBL = 0x9.

    MRF24J40_Set_Interrupts();
    MRF24J40_Set_Channel(channel);

    MRF24J40_Set_Pan(pan);
    MRF24J40_Short_Address_Write(short_address);

    // wait for mrf to be in receive mode
    while (read_long(MRF_RFSTATE) & 0xa0 != 0xa0);

    write_short(MRF_RFCTL, 0x04); // Reset RF state machine.
    write_short(MRF_RFCTL, 0x00); // part 2

    Delay1KTCYx(1); // delay at least 192usec
}

void MRF24J40_Set_Promiscuous(bool enabled)
{
    if (enabled == true)
    {
        write_short(MRF_RXMCR, 0x01);
    }
    else
    {
        write_short(MRF_RXMCR, 0x00);
    }
}

void MRF24J40_RX_Flush(void)
{
    write_short(MRF_RXFLUSH, read_short(MRF_RXFLUSH) | 0x01);
}

void MRF24J40_RX_Disable(void)
{
    write_short(MRF_BBREG1, 0x04); // RXDECINV - disable receiver
}

void MRF24J40_RX_Enable(void)
{
    write_short(MRF_BBREG1, 0x00); // RXDECINV - enable receiver
}