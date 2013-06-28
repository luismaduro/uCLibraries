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

#include "uFIFO.h"

//This initializes the FIFO structure with the given buffer and size

void uFIFOInit(tFIFO *f, unsigned char *buf, unsigned int size)
{
    f->Head = 0;
    f->Tail = 0;
    f->Size = size;
    f->SpaceOcupied = 0;
    f->bufferPointer = buf;
}

//This reads nbytes bytes from the FIFO
//The number of bytes read is returned

unsigned int uFIFOGet(tFIFO * f, unsigned char *buf, unsigned int nbytes)
{
    unsigned int i;

    for (i = 0; i < nbytes; i++)
    {
        if (f->Tail != f->Head)
        {
            //see if any data is available
            *buf = f->bufferPointer[f->Tail]; //grab a byte from the buffer

            f->Tail++; //increment the tail

            if (f->Tail == f->Size)
            {
                //check for wrap-around
                f->Tail = 0;
            }

            f->SpaceOcupied--;
        }
        else
        {
            return i; //number of bytes read
        }
    }

    return nbytes;
}

//This writes up to nbytes bytes to the FIFO
//If the head runs in to the tail, not all bytes are written
//The number of bytes written is returned

unsigned int uFIFOPut(tFIFO * f, unsigned char *buf, unsigned int nbytes)
{
    unsigned int i;

    for (i = 0; i < nbytes; i++)
    {
        //first check to see if there is space in the buffer
        if (f->Head + 1 == f->Tail)
        {
            return i; //no more room
        }
        else
        {
            f->bufferPointer[f->Head] = *buf++;

            f->Head++; //increment the head

            if (f->Head == f->Size && f->Tail != 0)
            {
                //check for wrap-around
                f->Head = 0;
            }

            f->SpaceOcupied++;
        }
    }

    return nbytes;
}
