#ifndef DS1302_H
#define DS1302_H

#include <avr/io.h>
#include <avr/pgmspace.h>

static const char* const WeekDays[] PROGMEM = {
    "Monday", "Tuesday",  "Wednesday", "Thursday",
    "Friday", "Saturday", "Sunday"};

struct ds1302_time_t {
    uint8_t second, minute, hour;
    uint8_t day, month, dow, year;
};

enum ds1302_command {
    // Write commands (LSB = 0)
    DS1302_CMD_WRITE_SECONDS = 0x80,
    DS1302_CMD_WRITE_MINUTES = 0x82,
    DS1302_CMD_WRITE_HOURS = 0x84,
    DS1302_CMD_WRITE_DATE = 0x86,
    DS1302_CMD_WRITE_MONTH = 0x88,
    DS1302_CMD_WRITE_DAY = 0x8A,
    DS1302_CMD_WRITE_YEAR = 0x8C,
    DS1302_CMD_WRITE_CONTROL = 0x8E,
    DS1302_CMD_WRITE_CHARGER = 0x90,
    DS1302_CMD_WRITE_CLOCK_BURST = 0xBE,

    // Read commands (LSB = 1)
    DS1302_CMD_READ_SECONDS = 0x81,
    DS1302_CMD_READ_MINUTES = 0x83,
    DS1302_CMD_READ_HOURS = 0x85,
    DS1302_CMD_READ_DATE = 0x87,
    DS1302_CMD_READ_MONTH = 0x89,
    DS1302_CMD_READ_DAY = 0x8B,
    DS1302_CMD_READ_YEAR = 0x8D,
    DS1302_CMD_READ_CONTROL = 0x8F,
    DS1302_CMD_READ_CHARGER = 0x91,
    DS1302_CMD_READ_CLOCK_BURST = 0xBF

};

void ds1302_init();
void ds1302_write_byte(uint8_t data);

uint8_t ds1302_read_byte(void);
void ds1302_write_register(uint8_t reg, uint8_t data);
uint8_t ds1302_read_register(uint8_t reg);

// TODO: implement set time(currently using arduino IDE ready-made code from a
// TODO: library)
void ds1302_set_time(struct ds1302_time_t* t);
void ds1302_get_time(struct ds1302_time_t* t);

void ds1302_time_to_str(char buffer[], const struct ds1302_time_t* t);

void ds1302_date_extract(char buffer[], const struct ds1302_time_t* t);
void ds1302_time_extract(char buffer[], const struct ds1302_time_t* t);

#endif