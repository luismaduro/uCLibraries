#include "SST25VF064C.h"
#include "SPIDevice.h"

uint8_t FlashGetDeviceID(void)
{

    uint8_t rcvByte[3];

    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();


    FlashMemorySelect();

    //set command  0x9F for Read ID
    SPIWrite(0x9F);

    //Get device ID
    rcvByte[0] = SPIRead();
    rcvByte[1] = SPIRead();
    rcvByte[2] = SPIRead();

    FlashMemoryDeselect();


    if (rcvByte[2] == 0x15)
    {
        return 16;
    }
    else if (rcvByte[2] == 0x18)
    {
        return 128;
    }
    else
    {
        return 0;
    }

}

uint8_t FlashNormalRead(uint32_t address)
{
    uint8_t sndByte[4] = {0, 0, 0, 0};
    uint8_t rcvByte;

    //set command  0x03 for Read at normal speed 
    sndByte[0] = 0x03;
    //set 3 byte address MSB first 
    sndByte[1] = (address >> 16);
    sndByte[2] = (address >> 8);
    sndByte[3] = address;


    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();

    FlashMemorySelect();

    SPIWrite(sndByte[0]);
    SPIWrite(sndByte[1]);
    SPIWrite(sndByte[2]);
    SPIWrite(sndByte[3]);

    //read a byte 	
    rcvByte = SPIRead();

    FlashMemoryDeselect();

    return rcvByte;

}

uint8_t FlashFastRead(uint32_t address)
{
    uint8_t sndByte[5] = {0, 0, 0, 0, 0};
    uint8_t rcvByte;

    //set command  0x0B for Read at fast speed 
    sndByte[0] = 0x0B;
    //set 3 byte address MSB first 
    sndByte[1] = (address >> 16);
    sndByte[2] = (address >> 8);
    sndByte[3] = address;
    sndByte[4] = 0x00;


    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();

    FlashMemorySelect();

    SPIWrite(sndByte[0]);
    SPIWrite(sndByte[1]);
    SPIWrite(sndByte[2]);
    SPIWrite(sndByte[3]);
    SPIWrite(sndByte[4]);

    //read a byte 	
    rcvByte = SPIRead();

    FlashMemoryDeselect();

    return rcvByte;

}

void FlashWritePage(uint8_t writeData, uint32_t address)
{
    uint8_t sndByte[5] = {0, 0, 0, 0, 0};

    //set command  0x02 for Page Program 
    sndByte[0] = 0x02;
    //set 3 byte address MSB first 
    sndByte[1] = (address >> 16);
    sndByte[2] = (address >> 8);
    sndByte[3] = (address);
    sndByte[4] = writeData;



    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();

    //enable write
    FlashWriteEnable();

    FlashMemorySelect();

    SPIWrite(sndByte[0]);
    SPIWrite(sndByte[1]);
    SPIWrite(sndByte[2]);
    SPIWrite(sndByte[3]);
    SPIWrite(sndByte[4]);

    FlashMemoryDeselect();

}

void FlashSectorErase(uint32_t address)
{
    uint8_t sndByte[4] = {0, 0, 0, 0};

    //set command  0xD8 for sector erase 
    sndByte[0] = 0xD8;
    //set 3 byte address MSB first 
    sndByte[1] = (address >> 16);
    sndByte[2] = (address >> 8);
    sndByte[3] = (address);

    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();

    //enable write
    FlashWriteEnable();

    FlashMemorySelect();

    SPIWrite(sndByte[0]);
    SPIWrite(sndByte[1]);
    SPIWrite(sndByte[2]);
    SPIWrite(sndByte[3]);

    FlashMemoryDeselect();

}

void FlashBulkErase(void)
{

    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();
    FlashWriteSR(0x00);
    //enable write
    FlashWriteEnable();

    FlashMemorySelect();

    //set command  0xC7 for bulk erase 
    SPIWrite(0xC7);

    FlashMemoryDeselect();

}

void FlashWriteSR(uint8_t setReg)
{
    // wait for any unfinished write or erase cycle
    // this is called as that command will not be ignored during a write or erase cycle
    FlashWaitForWrite();

    //enable write
    FlashWriteEnable();

    FlashMemorySelect();

    //set command  0x01 to Write to Status Register
    SPIWrite(0x01);
    SPIWrite(setReg);

    FlashMemoryDeselect();
}

void FlashWriteEnable(void)
{
    FlashMemorySelect();

    //set command to 0x06 for write enable
    SPIWrite(0x06);

    FlashMemoryDeselect();

}

uint8_t FlashReadSR(void)
{
    uint8_t SR;

    FlashMemorySelect();

    //set command  0x05 for read status register 
    SPIWrite(0x05);
    //Get status register
    SR = SPIRead();

    FlashMemoryDeselect();

    return SR;

}

void FlashWaitForWrite(void)
{
    uint8_t SR;

    SR = FlashReadSR();

    //0x1 means a write is in progess
    while (SR & 0x1)
    {
        //keep polling status register
        SR = FlashReadSR();
    }

}
