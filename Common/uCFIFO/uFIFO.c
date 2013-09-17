/*************************************************************************
 Theory of Operation:
   This FIFO implementation provides a memory safe 'First In First Out'
   circular buffer.  If the operating conditions of a FIFO causes it
   to 'underflow' or 'overflow' the FIFO will not corrupt memory other
   than its own data buffer.  However, memory accesses into the buffer
   will be invalid.  If a FIFO 'underflows' or 'overflows', it should
   be re-initialized or cleared.

   Example Usage:
      unsigned char fifo_buf[128];
      tFIFO fifo;
      FIFOInit(&fifo,&fifo_buf[0], 128);

 ************************************************************************/

#include "uFIFO.h"

void uFIFOInit(tFIFO *f, uint8_t *data, uint8_t size)
{
    f->Size = size;
    f->DataPtr = data;
    f->Status = UFIFO_GOOD;
    f->PutIndex = 0;
    f->GetIndex = 0;
    f->Used = 0;
}

bool uFIFOisFull(tFIFO* f)
{
    return (f->Used >= f->Size);
}

bool uFIFOisEmpty(tFIFO* f)
{
    return (f->Used == 0);
}

uint8_t uFIFOGet(tFIFO* f)
{
    uint8_t c;
    if (f->Used > 0)
    {
        c = f->DataPtr[f->GetIndex];
        f->GetIndex = (f->GetIndex + 1) % f->Size;
        f->Used--;
        return c;
    }
    else
    {
        f->Status = UFIFO_UNDERFLOW;
        return 0;
    }
}

void uFIFOPut(tFIFO* f, uint8_t c)
{
    if (f->Used >= f->Size)
        f->Status = UFIFO_OVERFLOW;
    else
    {
        f->DataPtr[f->PutIndex] = c;
        f->PutIndex = (f->PutIndex + 1) % f->Size;
        f->Used++;
    }
}

uint8_t uFIFOPeek(tFIFO* f)
{
    return f->DataPtr[f->GetIndex];
}

uint8_t uFIFOBytesInQueue(tFIFO* f)
{
    return f->Used;
}

void uFIFOClear(tFIFO* f)
{
    f->Status = UFIFO_GOOD;
    f->PutIndex = 0;
    f->GetIndex = 0;
    f->Used = 0;
}

tFIFOStatus uFIFOStatus(tFIFO* f)
{
    return f->Status;
}