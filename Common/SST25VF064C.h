#ifndef SST25VF064C_H
#define SST25VF064C_H

#include "SPIDevice.h"
#include "uwn_common.h"

uint8_t FlashGetDeviceID(void);
uint8_t FlashNormalRead(uint32_t address);
uint8_t FlashFastRead(uint32_t address);
void FlashWritePage(uint8_t writeData, uint32_t address);
void FlashSectorErase(uint32_t address);
void FlashBulkErase(void);
void FlashWriteSR(uint8_t setReg);
void FlashWriteEnable(void);
uint8_t FlashReadSR(void);
void FlashWaitForWrite(void);

#endif
