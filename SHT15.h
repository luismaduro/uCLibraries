#ifndef SHT15_H
#define SHT15_H

#include <p18cxxx.h>

#define DATA_TRIS TRISEbits.TRISE2
#define SCK_TRIS TRISEbits.TRISE3

#define DATA PORTEbits.RE2
#define SCK PORTEbits.RE3

#define ACK 1
#define NOT_ACK 0
//  adr     cmd     r/w
#define READ_STATUS_REG     0b00000111      //  000     0011    1
#define WRITE_STATUS_REG    0b00000110      //  000     0011    0
#define MEASURE_TEMPERATURE 0b00000011      //  000     0001    1
#define MEASURE_HUMIDITY    0b00000101      //  000     0010    1
#define RESET               0b00011110      //  000     1111    0

#define HEATER_ON           0b00000111
#define HEATER_OFF          0b00000011

#define OTP_RELOAD          0b00000101
#define OTP_NORELOAD        0b00000111

#define LOW_RESOLUTION      0b00000111
#define HIGH_RESOLUTION     0b00000110

//-----------------------FUNCOES--------------------//
unsigned char sht15_wait_measure_complete(void);
void sht15_connection_reset(void);
void sht15_transmition_start(void);
unsigned char sht15_send_byte(unsigned char);
unsigned char sht15_read_byte(unsigned char);
unsigned char sht15_read_status_reg(unsigned char *, unsigned char *);
unsigned char sht15_write_status_reg(unsigned char);
unsigned int measure_temperature(void);
unsigned int measure_humidity(void);

#endif