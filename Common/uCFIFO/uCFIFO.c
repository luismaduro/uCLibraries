/*************************************************************************
 Theory of Operation:
   This FIFO implementation provides a memory safe 'First In First Out'
   circular buffer.  If the operating conditions of a FIFO causes it
   to 'underflow' or 'overflow' the FIFO will not corrupt memory other
   than its own data buffer.  However, memory accesses into the buffer
   will be invalid.  If a FIFO 'underflows' or 'overflows', it should
   be re-initialized or cleared.

   Example Usage:
      volatile unsigned char fifo_buf[128];
      FIFO fifo;
      FIFOinit(&fifo, 128, &fifo_buf[0]);

 ************************************************************************/

#include "uCFIFO.h"

void FIFOInit(FIFO *f, unsigned int size, unsigned char *data)
{
    f->size = size;
    f->data = data;
    f->status = FIFO_GOOD;
    f->putIndex = 0;
    f->getIndex = 0;
    f->used = 0;
}

bool FIFOisFull(FIFO *f)
{
    return (f->used >= f->size);
}

bool FIFOisEmpty(FIFO *f)
{
    return (f->used == 0);
}

unsigned char FIFOGet(FIFO *f)
{
    unsigned char c;
    
    if (f->used > 0)
    {
        c = f->data[f->getIndex];
        f->getIndex = (f->getIndex + 1) % f->size;
        f->used--;
        
        return c;
    }
    else
    {
        f->status = FIFO_UNDERFLOW;
        return 0;
    }
}

void FIFOPut(FIFO *f, unsigned char c)
{
    if (f->used >= f->size)
        f->status = FIFO_OVERFLOW;
    else
    {
        f->data[f->putIndex] = c;
        f->putIndex = (f->putIndex + 1) % f->size;
        f->used++;
    }
}

unsigned char FIFOPeek(FIFO *f)
{
    return f->data[f->getIndex];
}

unsigned int FIFOSpaceOcupied(FIFO *f)
{
    return f->used;
}

void FIFOClear(FIFO *f)
{
    f->status = FIFO_GOOD;
    f->putIndex = 0;
    f->getIndex = 0;
    f->used = 0;
}

tFIFOStatus FIFOStatus(FIFO *f)
{
    return f->status;
}