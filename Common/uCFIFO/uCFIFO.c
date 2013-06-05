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

bool uFIFOisFull(uFIFO *f)
{
    return (f->used >= f->size);
}

bool uFIFOisEmpty(uFIFO *f)
{
    return (f->used == 0);
}

unsigned char uFIFOGet(uFIFO *f)
{
    unsigned char c;

    if (f->used == 0)
    {
        f->status = FIFO_UNDERFLOW;

        return 0;
    }
    else
    {
        f->status = FIFO_GOOD;

        c = f->data[f->getIndex];

        if (f->getIndex == 0xFFFF)
            f->getIndex = 0;
        else
            f->getIndex++;

        f->used--;

        return c;
    }
}

void uFIFOPut(uFIFO *f, unsigned char c)
{
    if (f->used >= f->size)
        f->status = FIFO_OVERFLOW;
    else
    {
        f->status = FIFO_GOOD;

        f->data[f->putIndex] = c;

        if (f->putIndex == 0xFFFF)
            f->putIndex = 0;
        else
            f->putIndex++;

        f->used++;
    }
}

unsigned char uFIFOPeek(uFIFO *f)
{
    return f->data[f->getIndex];
}

unsigned int uFIFOSpaceOcupied(uFIFO *f)
{
    return f->used;
}

void uFIFOClear(uFIFO *f)
{
    f->status = FIFO_GOOD;
    f->putIndex = 0;
    f->getIndex = 0;
    f->used = 0;
}

tFIFOStatus uFIFOStatus(uFIFO *f)
{
    return f->status;
}