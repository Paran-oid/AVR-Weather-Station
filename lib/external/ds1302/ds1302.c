#include "ds1302.h"

#include <util/delay.h>

#include "core.h"

#define DS1302_DDR DDRB
#define DS1302_PRT PORTB
#define DS1302_PIN PINB

#define DS1302_CLK PB0
#define DS1302_DAT PB1
#define DS1302_RST PB2

static uint8_t bin_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}
static uint8_t bcd_to_bin(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

void ds1302_init() {
    DS1302_DDR |= (1 << DS1302_CLK) | (1 << DS1302_DAT) | (1 << DS1302_RST);

    // struct ds1302_time_t t {
    //     .second = 0, .minute = 45,
    //     .hour = 15,      // 3 PM
    //         .day = 7,    // Sunday (1=Mon, 7=Sun)
    //         .month = 8,  // August
    //         .date = 4,   // 4th of the month
    //         .year = 25   // 2025
    // };
    // __attribute__((unused)) ds1302_set_time(&t);
}
void ds1302_write_byte(uint8_t data) {
    DS1302_DDR |= (1 << DS1302_DAT);

    for (uint8_t i = 0; i < 8; i++) {
        if (data & (1 << i)) {
            DS1302_PRT |= (1 << DS1302_DAT);
        } else {
            DS1302_PRT &= ~(1 << DS1302_DAT);
        }
        DS1302_PRT |= (1 << DS1302_CLK);
        _delay_us(1);
        DS1302_PRT &= ~(1 << DS1302_CLK);
        _delay_us(1);
    }

    _delay_us(1);
}
uint8_t ds1302_read_byte(void) {
    DS1302_DDR &= ~(1 << DS1302_DAT);

    uint8_t res = 0x00, i = 0;
    while (i < 8) {
        uint8_t bit = (DS1302_PIN & (1 << DS1302_DAT)) ? 1 : 0;
        res |= (bit << i);

        DS1302_PRT |= (1 << DS1302_CLK);
        _delay_us(1);
        DS1302_PRT &= ~(1 << DS1302_CLK);
        _delay_us(1);
        i++;
    }

    return res;
}

void ds1302_write_register(uint8_t reg, uint8_t data) {
    DS1302_PRT |= (1 << DS1302_RST);
    _delay_us(1);

    ds1302_write_byte(reg);
    ds1302_write_byte(data);

    DS1302_PRT &= ~(1 << DS1302_RST);
    _delay_us(1);
}

uint8_t ds1302_read_register(uint8_t reg) {
    DS1302_PRT |= (1 << DS1302_RST);
    _delay_us(1);

    ds1302_write_byte(reg);
    uint8_t res = ds1302_read_byte();

    DS1302_PRT &= ~(1 << DS1302_RST);
    _delay_us(1);

    return res;
}

// TODO: implement me and make sure you are able to set time
void ds1302_set_time(struct ds1302_time_t* t) {
    // Disable write protection
    ds1302_write_register(0x8E, 0x00);

    // Write time values (all in BCD)
    ds1302_write_register(DS1302_CMD_WRITE_SECONDS,
                          bin_to_bcd(t->second) & 0x7F);  // Clear CH
    ds1302_write_register(DS1302_CMD_WRITE_MINUTES, bin_to_bcd(t->minute));
    ds1302_write_register(DS1302_CMD_WRITE_HOURS,
                          bin_to_bcd(t->hour) & 0x3F);  // 24h format
    ds1302_write_register(DS1302_CMD_WRITE_DATE, bin_to_bcd(t->day));
    ds1302_write_register(DS1302_CMD_WRITE_MONTH, bin_to_bcd(t->month));
    ds1302_write_register(DS1302_CMD_WRITE_DAY, bin_to_bcd(t->dow));
    ds1302_write_register(DS1302_CMD_WRITE_YEAR, bin_to_bcd(t->year));

    // Re-enable write protection
    ds1302_write_register(0x8E, 0x80);
}

void ds1302_get_time(struct ds1302_time_t* t) {
    ds1302_write_register(0x8E, 0x00);

    // Read each register using your read_register function
    uint8_t raw_seconds = ds1302_read_register(DS1302_CMD_READ_SECONDS);
    uint8_t raw_minutes = ds1302_read_register(DS1302_CMD_READ_MINUTES);
    uint8_t raw_hours = ds1302_read_register(DS1302_CMD_READ_HOURS);
    uint8_t raw_date = ds1302_read_register(DS1302_CMD_READ_DATE);
    uint8_t raw_month = ds1302_read_register(DS1302_CMD_READ_MONTH);
    uint8_t raw_day = ds1302_read_register(DS1302_CMD_READ_DAY);
    uint8_t raw_year = ds1302_read_register(DS1302_CMD_READ_YEAR);

    // Convert from BCD to binary (decimal) using your bcd_to_bin function
    t->second = bcd_to_bin(raw_seconds &
                           0x7F);  // bit 7 is CH (clock halt), mask it out
    t->minute = bcd_to_bin(raw_minutes);
    t->hour =
        bcd_to_bin(raw_hours & 0x3F);  // 24-hour mode assumed, mask upper bits
    t->day = bcd_to_bin(raw_date);
    t->month = bcd_to_bin(raw_month);
    t->dow = bcd_to_bin(raw_day);
    t->year = bcd_to_bin(raw_year);

    ds1302_write_register(0x8E, 0x80);
}

void ds1302_time_to_str(char buffer[], const struct ds1302_time_t* t) {
    buffer[0] = '\0';

    char time_buf[16];
    char date_buf[16];

    ds1302_date_extract(date_buf, t);
    ds1302_time_extract(time_buf, t);

    str_append(buffer, date_buf);
    str_append(buffer, " ");
    str_append(buffer, time_buf);
}

void ds1302_date_extract(char buffer[], const struct ds1302_time_t* t) {
    buffer[0] = '\0';
    char temp[20];

    // Year "20xx"
    str_append(buffer, "20");
    _itoa(t->year, temp, 10);
    str_append(buffer, temp);
    temp[0] = '\0';
    str_append(buffer, "-");

    // Month with leading zero if needed
    if (t->month < 10) str_append(buffer, "0");
    _itoa(t->month, temp, 10);
    str_append(buffer, temp);
    temp[0] = '\0';
    str_append(buffer, "-");

    // Day with leading zero if needed
    if (t->day < 10) str_append(buffer, "0");
    _itoa(t->day, temp, 10);
    str_append(buffer, temp);
    temp[0] = '\0';
    str_append(buffer, " ");

    // !Weekday name: DISABLED FOR LACK OF BITS
    // const char* dow_str =
    //     (t->dow >= 1 && t->dow <= 7) ? WeekDays[t->dow - 1] : "Unknown";
    // str_append(buffer, dow_str);
    // str_append(buffer, " ");
}

void ds1302_time_extract(char buffer[], const struct ds1302_time_t* t) {
    buffer[0] = '\0';
    char temp[20];

    // Hour with leading zero
    if (t->hour < 10) str_append(buffer, "0");
    _itoa(t->hour, temp, 10);
    str_append(buffer, temp);
    temp[0] = '\0';
    str_append(buffer, ":");

    // Minute with leading zero
    if (t->minute < 10) str_append(buffer, "0");
    _itoa(t->minute, temp, 10);
    str_append(buffer, temp);
    temp[0] = '\0';
    str_append(buffer, ":");

    // Second with leading zero
    if (t->second < 10) str_append(buffer, "0");
    _itoa(t->second, temp, 10);
    str_append(buffer, temp);
}
