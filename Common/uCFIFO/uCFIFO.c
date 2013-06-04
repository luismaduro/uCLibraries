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
      FIFO fifo;
      FIFOInit(&fifo, 128, &fifo_buf[0]);

 ************************************************************************/

#include "uCFIFO.h"

void uFIFOInit(uFIFO *f, unsigned int size, unsigned char *data)
{
    f->size = size;
    f->data = data;
    f->status = FIFO_GOOD;
    f->putIndex = 0;
    f->getIndex = 0;
    f->used = 0;
}

bool FIFOisFull(uFIFO *f)
{
    return (f->used >= f->size);
}

bool FIFOisEmpty(uFIFO *f)
{
    return (f->used == 0);
}

unsigned char FIFOGet(uFIFO *f)
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

void FIFOPut(uFIFO *f, unsigned char c)
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

unsigned char FIFOPeek(uFIFO *f)
{
    return f->data[f->getIndex];
}

unsigned int FIFOSpaceOcupied(uFIFO *f)
{
    return f->used;
}

void FIFOClear(uFIFO *f)
{
    f->status = FIFO_GOOD;
    f->putIndex = 0;
    f->getIndex = 0;
    f->used = 0;
}

tFIFOStatus FIFOStatus(uFIFO *f)
{
    return f->status;
}