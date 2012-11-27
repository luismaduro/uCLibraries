
#include <p18cxxx.h>
#include <delays.h>
#include "SHT15.h"

unsigned char sht15_wait_measure_complete(void)
{
    unsigned long i = 0;

    DATA_TRIS = 1;

    for (i = 0; i < 1000000; i++)
        if (DATA == 0)
            return 0;

    if (DATA)
        return 1;
}

void sht15_connection_reset(void)
{
    unsigned char i = 0;

    DATA_TRIS = 0;
    SCK_TRIS = 0;

    Delay1KTCYx(1);
    DATA = 1;
    Delay1KTCYx(1);
    SCK = 0;
    Delay1KTCYx(1);
    for (i = 0; i < 9; i++)
    {
        SCK = 1;
        Delay1KTCYx(1);
        SCK = 0;
        Delay1KTCYx(1);
    }
}

void sht15_transmition_start(void)
{
    DATA_TRIS = 0;
    SCK_TRIS = 0;

    DATA = 1;
    Delay1KTCYx(1);
    SCK = 1;
    Delay1KTCYx(1);
    DATA = 0;
    Delay1KTCYx(1);
    SCK = 0;
    Delay1KTCYx(1);
    SCK = 1;
    Delay1KTCYx(1);
    DATA = 1;
    Delay1KTCYx(1);
    SCK = 0;
    Delay1KTCYx(1);
}

unsigned char sht15_send_byte(unsigned char data)
{
    unsigned char i = 0, error = 0;

    DATA_TRIS = 0;
    SCK_TRIS = 0;

    DATA = 1;

    Delay1KTCYx(1);

    for (i = 0x80; i > 0; i /= 2)
    {
        if (i & data)
            DATA = 1;
        else
            DATA = 0;
        Delay1KTCYx(1);
        SCK = 1;
        Delay1KTCYx(1);
        SCK = 0;
        Delay1KTCYx(1);
    }

    DATA = 1; //Libertar o bus
    DATA_TRIS = 1;

    SCK = 1;
    Delay1KTCYx(1);
    error = DATA;
    Delay1KTCYx(1);
    SCK = 0;
    Delay1KTCYx(1);

    return error;
}

unsigned char sht15_read_byte(unsigned char ack)
{
    unsigned char i = 0, data = 0;

    DATA_TRIS = 1;
    SCK_TRIS = 0;

    SCK = 0;
    Delay1KTCYx(1);

    for (i = 0x80; i > 0; i /= 2)
    {
        SCK = 1;
        Delay1KTCYx(1);
        if (DATA == 1)
            data = (data | i);
        else
            data = (data & ~i);
        Delay1KTCYx(1);
        SCK = 0;
        Delay1KTCYx(1);
    }

    DATA_TRIS = 0;
    Delay1KTCYx(1);
    DATA = !ack;
    Delay1KTCYx(1);
    SCK = 1;
    Delay1KTCYx(1);
    SCK = 0;
    Delay1KTCYx(1);
    DATA = 1;
    Delay1KTCYx(1);

    return data;
}

unsigned char sht15_read_status_reg(unsigned char *value, unsigned char *checksum)
{
    unsigned char error = 0;

    sht15_connection_reset();
    sht15_transmition_start();
    error = sht15_send_byte(READ_STATUS_REG);
    *value = sht15_read_byte(ACK);
    *checksum = sht15_read_byte(NOT_ACK);

    return error;
}

unsigned char sht15_write_status_reg(unsigned char data)
{
    unsigned char error = 0;

    sht15_connection_reset();
    sht15_transmition_start();

    error += sht15_send_byte(WRITE_STATUS_REG);
    error += sht15_send_byte(data);

    return error;
}

unsigned int measure_temperature(void)
{
    unsigned int error = 0, msb = 0, lsb = 0, checksum = 0;
    unsigned int temperature = 0;

    sht15_connection_reset();

    sht15_transmition_start();

    error += sht15_send_byte(MEASURE_TEMPERATURE);

    error += sht15_wait_measure_complete();

    msb = sht15_read_byte(ACK);
    lsb = sht15_read_byte(ACK);
    checksum = sht15_read_byte(NOT_ACK);

    if (error != 0)
        sht15_connection_reset();
    else
    {
        temperature = (msb << 8) + lsb;
        return temperature;
    }
}

unsigned int measure_humidity(void)
{
    unsigned int error = 0, msb = 0, lsb = 0, checksum = 0;
    unsigned int humidity = 0;

    sht15_connection_reset();

    sht15_transmition_start();

    error += sht15_send_byte(MEASURE_HUMIDITY);

    error += sht15_wait_measure_complete();

    msb = sht15_read_byte(ACK);
    lsb = sht15_read_byte(ACK);
    checksum = sht15_read_byte(NOT_ACK);

    if (error != 0)
        sht15_connection_reset();
    else
    {
        humidity = (msb << 8) + lsb;
        return humidity;
    }
}