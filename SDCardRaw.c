/**
 *  @file       SDCardRaw.h
 *  @brief      Library to write in raw mode to sdcard
 *  @author     Luis Maduro
 *  @version    1.00
 *  @date       11 de Outubro de 2012
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
#include "SDCardRaw.h"

#if !SD_RAW_SAVE_RAM
#pragma udata sdfat
/* static data buffer for acceleration */
static unsigned char raw_block[512];
#pragma udata
/* offset where the data within raw_block lies on the card */
static offset_t raw_block_address;
#if SD_RAW_WRITE_BUFFERING
/* flag to remember if raw_block was written to the card */
static unsigned char raw_block_written;
#endif
#endif

/* card type state */
static unsigned char sd_raw_card_type;

static unsigned char sd_raw_send_command(unsigned char command, unsigned long arg);
unsigned char sd_raw_available(void);
unsigned char sd_raw_sync(void);

/**
 * \ingroup sd_raw
 * Initializes memory card communication.
 *
 * \returns 0 on failure, 1 on success.
 */
unsigned char sd_raw_init(void)
{
    unsigned char i;
    unsigned char response;
    /* enable inputs for reading card status */
    //    configure_pin_available();
    //    configure_pin_locked();

    /* enable outputs for MOSI, SCK, SS, input for MISO */
    configure_pin_mosi();
    configure_pin_sck();
    configure_pin_ss();
    configure_pin_miso();

    unselect_card();

    SPIInit(0x00, 0);

    /* initialization procedure */
    sd_raw_card_type = 0;

    if (!sd_raw_available())
        return 0;

    /* card needs 74 cycles minimum to start up */
    for (i = 0; i < 10; ++i)
    {
        /* wait 8 clock cycles */
        SPIRead();
    }

    /* address card */
    select_card();

    /* reset card */
    for (i = 0;; ++i)
    {
        response = sd_raw_send_command(CMD_GO_IDLE_STATE, 0);
        if (response == (1 << R1_IDLE_STATE))
            break;

        if (i == 0x1ff)
        {
            unselect_card();
            return 0;
        }
    }

#if SD_RAW_SDHC
    /* check for version of SD card specification */
    response = sd_raw_send_command(CMD_SEND_IF_COND, 0x100 /* 2.7V - 3.6V */ | 0xaa /* test pattern */);
    if ((response & (1 << R1_ILL_COMMAND)) == 0)
    {
        SPIRead();
        SPIRead();
        if ((SPIRead() & 0x01) == 0)
            return 0; /* card operation voltage range doesn't match */
        if (SPIRead() != 0xaa)
            return 0; /* wrong test pattern */

        /* card conforms to SD 2 card specification */
        sd_raw_card_type |= (1 << SD_RAW_SPEC_2);
    }
    else
#endif
    {
        /* determine SD/MMC card type */
        sd_raw_send_command(CMD_APP, 0);
        response = sd_raw_send_command(CMD_SD_SEND_OP_COND, 0);
        if ((response & (1 << R1_ILL_COMMAND)) == 0)
        {
            /* card conforms to SD 1 card specification */
            sd_raw_card_type |= (1 << SD_RAW_SPEC_1);
        }
        else
        {
            /* MMC card */
        }
    }

    /* wait for card to get ready */
    for (i = 0;; ++i)
    {
        if (sd_raw_card_type & ((1 << SD_RAW_SPEC_1) | (1 << SD_RAW_SPEC_2)))
        {
            unsigned long arg = 0;
#if SD_RAW_SDHC
            if (sd_raw_card_type & (1 << SD_RAW_SPEC_2))
                arg = 0x40000000;
#endif
            sd_raw_send_command(CMD_APP, 0);
            response = sd_raw_send_command(CMD_SD_SEND_OP_COND, arg);
        }
        else
        {
            response = sd_raw_send_command(CMD_SEND_OP_COND, 0);
        }

        if ((response & (1 << R1_IDLE_STATE)) == 0)
            break;

        if (i == 0x7fff)
        {
            unselect_card();
            return 0;
        }
    }

#if SD_RAW_SDHC
    if (sd_raw_card_type & (1 << SD_RAW_SPEC_2))
    {
        if (sd_raw_send_command(CMD_READ_OCR, 0))
        {
            unselect_card();
            return 0;
        }

        if (SPIRead() & 0x40)
            sd_raw_card_type |= (1 << SD_RAW_SPEC_SDHC);

        SPIRead();
        SPIRead();
        SPIRead();
    }
#endif

    /* set block size to 512 bytes */
    if (sd_raw_send_command(CMD_SET_BLOCKLEN, 512))
    {
        unselect_card();
        return 0;
    }

    /* deaddress card */
    unselect_card();

#if !SD_RAW_SAVE_RAM
    /* the first block is likely to be accessed first, so precache it here */
    raw_block_address = (offset_t) - 1;
#if SD_RAW_WRITE_BUFFERING
    raw_block_written = 1;
#endif
    if (!sd_raw_read(0, raw_block, sizeof (raw_block)))
        return 0;
#endif

    return 1;
}

/**
 * \ingroup sd_raw
 * Checks wether a memory card is located in the slot.
 *
 * \returns 1 if the card is available, 0 if it is not.
 */
unsigned char sd_raw_available(void)
{
    return get_pin_available() == 0x00;
}

/**
 * \ingroup sd_raw
 * Checks wether the memory card is locked for write access.
 *
 * \returns 1 if the card is locked, 0 if it is not.
 */
unsigned char sd_raw_locked(void)
{
    return get_pin_locked() == 0x00;
}

/**
 * \ingroup sd_raw
 * Send a command to the memory card which responses with a R1 response (and possibly others).
 *
 * \param[in] command The command to send.
 * \param[in] arg The argument for command.
 * \returns The command answer.
 */
unsigned char sd_raw_send_command(unsigned char command, unsigned long arg)
{
    unsigned char response;
    unsigned char i;

    /* wait some clock cycles */
    SPIRead();

    /* send command via SPI */
    SPIWrite(0x40 | command);
    SPIWrite((arg >> 24) & 0xff);
    SPIWrite((arg >> 16) & 0xff);
    SPIWrite((arg >> 8) & 0xff);
    SPIWrite((arg >> 0) & 0xff);
    switch (command)
    {
    case CMD_GO_IDLE_STATE:
        SPIWrite(0x95);
        break;
    case CMD_SEND_IF_COND:
        SPIWrite(0x87);
        break;
    default:
        SPIWrite(0xff);
        break;
    }

    /* receive response */
    for (i = 0; i < 10; ++i)
    {
        response = SPIRead();
        if (response != 0xff)
            break;
    }

    return response;
}

/**
 * \ingroup sd_raw
 * Reads raw data from the card.
 *
 * \param[in] offset The offset from which to read.
 * \param[out] buffer The buffer into which to write the data.
 * \param[in] length The number of bytes to read.
 * \returns 0 on failure, 1 on success.
 * \see sd_raw_read_interval, sd_raw_write, sd_raw_write_interval
 */
unsigned char sd_raw_read(offset_t offset, unsigned char* buffer, uintptr_t length)
{
    unsigned int i;
    unsigned char *cache;
    offset_t block_address;
    unsigned int block_offset;
    unsigned int read_length;
    while (length > 0)
    {
        /* determine byte count to read at once */
        block_offset = offset & 0x01ff;
        block_address = offset - block_offset;
        read_length = 512 - block_offset; /* read up to block border */
        if (read_length > length)
            read_length = length;

#if !SD_RAW_SAVE_RAM
        /* check if the requested data is cached */
        if (block_address != raw_block_address)
#endif
        {
#if SD_RAW_WRITE_BUFFERING
            if (!sd_raw_sync())
                return 0;
#endif

            /* address card */
            select_card();

            /* send single block request */
#if SD_RAW_SDHC
            if (sd_raw_send_command(CMD_READ_SINGLE_BLOCK, (sd_raw_card_type & (1 << SD_RAW_SPEC_SDHC) ? block_address / 512 : block_address)))
#else
            if (sd_raw_send_command(CMD_READ_SINGLE_BLOCK, block_address))
#endif
            {
                unselect_card();
                return 0;
            }

            /* wait for data block (start byte 0xfe) */
            while (SPIRead() != 0xfe);

#if SD_RAW_SAVE_RAM
            /* read byte block */
            unsigned int read_to = block_offset + read_length;
            for (unsigned int i = 0; i < 512; ++i)
            {
                unsigned char b = SPIRead();
                if (i >= block_offset && i < read_to)
                    *buffer++ = b;
            }
#else
            /* read byte block */
            cache = raw_block;
            for (i = 0; i < 512; ++i)
                *cache++ = SPIRead();
            raw_block_address = block_address;

            memcpy(buffer, raw_block + block_offset, read_length);
            buffer += read_length;
#endif

            /* read crc16 */
            SPIRead();
            SPIRead();

            /* deaddress card */
            unselect_card();

            /* let card some time to finish */
            SPIRead();
        }
#if !SD_RAW_SAVE_RAM
        else
        {
            /* use cached data */
            memcpy(buffer, raw_block + block_offset, read_length);
            buffer += read_length;
        }
#endif

        length -= read_length;
        offset += read_length;
    }

    return 1;
}

/**
 * \ingroup sd_raw
 * Continuously reads units of \c interval bytes and calls a callback function.
 *
 * This function starts reading at the specified offset. Every \c interval bytes,
 * it calls the callback function with the associated data buffer.
 *
 * By returning zero, the callback may stop reading.
 *
 * \note Within the callback function, you can not start another read or
 *       write operation.
 * \note This function only works if the following conditions are met:
 *       - (offset - (offset % 512)) % interval == 0
 *       - length % interval == 0
 *
 * \param[in] offset Offset from which to start reading.
 * \param[in] buffer Pointer to a buffer which is at least interval bytes in size.
 * \param[in] interval Number of bytes to read before calling the callback function.
 * \param[in] length Number of bytes to read altogether.
 * \param[in] callback The function to call every interval bytes.
 * \param[in] p An opaque pointer directly passed to the callback function.
 * \returns 0 on failure, 1 on success
 * \see sd_raw_write_interval, sd_raw_read, sd_raw_write
 */
unsigned char sd_raw_read_interval(offset_t offset, unsigned char* buffer, uintptr_t interval, uintptr_t length, sd_raw_read_interval_handler_t callback, void* p)
{
    if (!buffer || interval == 0 || length < interval || !callback)
        return 0;

#if !SD_RAW_SAVE_RAM
    while (length >= interval)
    {
        /* as reading is now buffered, we directly
         * hand over the request to sd_raw_read()
         */
        if (!sd_raw_read(offset, buffer, interval))
            return 0;
        if (!callback(buffer, offset, p))
            break;
        offset += interval;
        length -= interval;
    }

    return 1;
#else
    /* address card */
    select_card();

    unsigned int block_offset;
    unsigned int read_length;
    unsigned char* buffer_cur;
    unsigned char finished = 0;
    do
    {
        /* determine byte count to read at once */
        block_offset = offset & 0x01ff;
        read_length = 512 - block_offset;

        /* send single block request */
#if SD_RAW_SDHC
        if (sd_raw_send_command(CMD_READ_SINGLE_BLOCK, (sd_raw_card_type & (1 << SD_RAW_SPEC_SDHC) ? offset / 512 : offset - block_offset)))
#else
        if (sd_raw_send_command(CMD_READ_SINGLE_BLOCK, offset - block_offset))
#endif
        {
            unselect_card();
            return 0;
        }

        /* wait for data block (start byte 0xfe) */
        while (SPIRead() != 0xfe);

        /* read up to the data of interest */
        for (unsigned int i = 0; i < block_offset; ++i)
            SPIRead();

        /* read interval bytes of data and execute the callback */
        do
        {
            if (read_length < interval || length < interval)
                break;

            buffer_cur = buffer;
            for (unsigned int i = 0; i < interval; ++i)
                *buffer_cur++ = SPIRead();

            if (!callback(buffer, offset + (512 - read_length), p))
            {
                finished = 1;
                break;
            }

            read_length -= interval;
            length -= interval;

        }
        while (read_length > 0 && length > 0);

        /* read rest of data block */
        while (read_length-- > 0)
            SPIRead();

        /* read crc16 */
        SPIRead();
        SPIRead();

        if (length < interval)
            break;

        offset = offset - block_offset + 512;

    }
    while (!finished);

    /* deaddress card */
    unselect_card();

    /* let card some time to finish */
    SPIRead();

    return 1;
#endif
}

#if DOXYGEN || SD_RAW_WRITE_SUPPORT

/**
 * \ingroup sd_raw
 * Writes raw data to the card.
 *
 * \note If write buffering is enabled, you might have to
 *       call sd_raw_sync() before disconnecting the card
 *       to ensure all remaining data has been written.
 *
 * \param[in] offset The offset where to start writing.
 * \param[in] buffer The buffer containing the data to be written.
 * \param[in] length The number of bytes to write.
 * \returns 0 on failure, 1 on success.
 * \see sd_raw_write_interval, sd_raw_read, sd_raw_read_interval
 */
unsigned char sd_raw_write(offset_t offset, const unsigned char* buffer, uintptr_t length)
{
    unsigned char *cache;
    unsigned int i;
    offset_t block_address;
    unsigned int block_offset;
    unsigned int write_length;

    if (sd_raw_locked())
        return 0;


    while (length > 0)
    {
        /* determine byte count to write at once */
        block_offset = offset & 0x01ff;
        block_address = offset - block_offset;
        write_length = 512 - block_offset; /* write up to block border */
        if (write_length > length)
            write_length = length;

        /* Merge the data to write with the content of the block.
         * Use the cached block if available.
         */
        if (block_address != raw_block_address)
        {
#if SD_RAW_WRITE_BUFFERING
            if (!sd_raw_sync())
                return 0;
#endif

            if (block_offset || write_length < 512)
            {
                if (!sd_raw_read(block_address, raw_block, sizeof (raw_block)))
                    return 0;
            }
            raw_block_address = block_address;
        }

        if (buffer != raw_block)
        {
            memcpy(raw_block + block_offset, buffer, write_length);

#if SD_RAW_WRITE_BUFFERING
            raw_block_written = 0;

            if (length == write_length)
                return 1;
#endif
        }

        /* address card */
        select_card();

        /* send single block request */
#if SD_RAW_SDHC
        if (sd_raw_send_command(CMD_WRITE_SINGLE_BLOCK, (sd_raw_card_type & (1 << SD_RAW_SPEC_SDHC) ? block_address / 512 : block_address)))
#else
        if (sd_raw_send_command(CMD_WRITE_SINGLE_BLOCK, block_address))
#endif
        {
            unselect_card();
            return 0;
        }

        /* send start byte */
        SPIWrite(0xfe);

        /* write byte block */
        cache = raw_block;
        for (i = 0; i < 512; ++i)
            SPIWrite(*cache++);

        /* write dummy crc16 */
        SPIWrite(0xff);
        SPIWrite(0xff);

        /* wait while card is busy */
        while (SPIRead() != 0xff);
        SPIRead();

        /* deaddress card */
        unselect_card();

        buffer += write_length;
        offset += write_length;
        length -= write_length;

#if SD_RAW_WRITE_BUFFERING
        raw_block_written = 1;
#endif
    }

    return 1;
}
#endif

#if DOXYGEN || SD_RAW_WRITE_SUPPORT

/**
 * \ingroup sd_raw
 * Writes a continuous data stream obtained from a callback function.
 *
 * This function starts writing at the specified offset. To obtain the
 * next bytes to write, it calls the callback function. The callback fills the
 * provided data buffer and returns the number of bytes it has put into the buffer.
 *
 * By returning zero, the callback may stop writing.
 *
 * \param[in] offset Offset where to start writing.
 * \param[in] buffer Pointer to a buffer which is used for the callback function.
 * \param[in] length Number of bytes to write in total. May be zero for endless writes.
 * \param[in] callback The function used to obtain the bytes to write.
 * \param[in] p An opaque pointer directly passed to the callback function.
 * \returns 0 on failure, 1 on success
 * \see sd_raw_read_interval, sd_raw_write, sd_raw_read
 */
unsigned char sd_raw_write_interval(offset_t offset, unsigned char* buffer, uintptr_t length, sd_raw_write_interval_handler_t callback, void* p)
{
    unsigned char endless = (length == 0);
#if SD_RAW_SAVE_RAM
#error "SD_RAW_WRITE_SUPPORT is not supported together with SD_RAW_SAVE_RAM"
#endif

    if (!buffer || !callback)
        return 0;

    while (endless || length > 0)
    {
        unsigned int bytes_to_write = callback(buffer, offset, p);
        if (!bytes_to_write)
            break;
        if (!endless && bytes_to_write > length)
            return 0;

        /* as writing is always buffered, we directly
         * hand over the request to sd_raw_write()
         */
        if (!sd_raw_write(offset, buffer, bytes_to_write))
            return 0;

        offset += bytes_to_write;
        length -= bytes_to_write;
    }

    return 1;
}
#endif

#if DOXYGEN || SD_RAW_WRITE_SUPPORT

/**
 * \ingroup sd_raw
 * Writes the write buffer's content to the card.
 *
 * \note When write buffering is enabled, you should
 *       call this function before disconnecting the
 *       card to ensure all remaining data has been
 *       written.
 *
 * \returns 0 on failure, 1 on success.
 * \see sd_raw_write
 */
unsigned char sd_raw_sync(void)
{
#if SD_RAW_WRITE_BUFFERING
    if (raw_block_written)
        return 1;
    if (!sd_raw_write(raw_block_address, raw_block, sizeof (raw_block)))
        return 0;
    raw_block_written = 1;
#endif
    return 1;
}
#endif

/**
 * \ingroup sd_raw
 * Reads informational data from the card.
 *
 * This function reads and returns the card's registers
 * containing manufacturing and status information.
 *
 * \note: The information retrieved by this function is
 *        not required in any way to operate on the card,
 *        but it might be nice to display some of the data
 *        to the user.
 *
 * \param[in] info A pointer to the structure into which to save the information.
 * \returns 0 on failure, 1 on success.
 */
unsigned char sd_raw_get_info(struct SDCardInfo* info)
{
    unsigned char csd_read_bl_len = 0;
    unsigned char csd_c_size_mult = 0;
#if SD_RAW_SDHC
    unsigned int csd_c_size = 0;
#else
    unsigned long csd_c_size = 0;
#endif
    unsigned char csd_structure = 0;

    unsigned char i;
    
    if (!info || !sd_raw_available())
        return 0;

    memset(info, 0, sizeof (*info));

    select_card();

    /* read cid register */
    if (sd_raw_send_command(CMD_SEND_CID, 0))
    {
        unselect_card();
        return 0;
    }
    while (SPIRead() != 0xfe);
    for (i = 0; i < 18; ++i)
    {
        unsigned char b = SPIRead();

        switch (i)
        {
        case 0:
            info->manufacturer = b;
            break;
        case 1:
        case 2:
            info->oem[i - 1] = b;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            info->product[i - 3] = b;
            break;
        case 8:
            info->revision = b;
            break;
        case 9:
        case 10:
        case 11:
        case 12:
            info->serial |= (unsigned long) b << ((12 - i) * 8);
            break;
        case 13:
            info->manufacturing_year = b << 4;
            break;
        case 14:
            info->manufacturing_year |= b >> 4;
            info->manufacturing_month = b & 0x0f;
            break;
        }
    }

    /* read csd register */
    if (sd_raw_send_command(CMD_SEND_CSD, 0))
    {
        unselect_card();
        return 0;
    }
    while (SPIRead() != 0xfe);
    for (i = 0; i < 18; ++i)
    {
        unsigned char b = SPIRead();

        if (i == 0)
        {
            csd_structure = b >> 6;
        }
        else if (i == 14)
        {
            if (b & 0x40)
                info->flag_copy = 1;
            if (b & 0x20)
                info->flag_write_protect = 1;
            if (b & 0x10)
                info->flag_write_protect_temp = 1;
            info->format = (b & 0x0c) >> 2;
        }
        else
        {
#if SD_RAW_SDHC
            if (csd_structure == 0x01)
            {
                switch (i)
                {
                case 7:
                    b &= 0x3f;
                case 8:
                case 9:
                    csd_c_size <<= 8;
                    csd_c_size |= b;
                    break;
                }
                if (i == 9)
                {
                    ++csd_c_size;
                    info->capacity = (offset_t) csd_c_size * 512 * 1024;
                }
            }
            else if (csd_structure == 0x00)
#endif
            {
                switch (i)
                {
                case 5:
                    csd_read_bl_len = b & 0x0f;
                    break;
                case 6:
                    csd_c_size = b & 0x03;
                    csd_c_size <<= 8;
                    break;
                case 7:
                    csd_c_size |= b;
                    csd_c_size <<= 2;
                    break;
                case 8:
                    csd_c_size |= b >> 6;
                    ++csd_c_size;
                    break;
                case 9:
                    csd_c_size_mult = b & 0x03;
                    csd_c_size_mult <<= 1;
                    break;
                case 10:
                    csd_c_size_mult |= b >> 7;

                    info->capacity = (unsigned long) csd_c_size << (csd_c_size_mult + csd_read_bl_len + 2);
                    break;
                }
            }
        }
    }

    unselect_card();

    return 1;
}

