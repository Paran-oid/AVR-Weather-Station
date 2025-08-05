#include "lcd.h"

#include <util/delay.h>

#include "twi.h"

#define BACKLIGHT 0X08
#define EN 0x04
#define RW 0x00
#define RS_COMMAND 0X00
#define RS_DATA 0X01

static int16_t lcd_address = 0x00;

void lcd_init(void) {
    if (!lcd_find()) {
        while (1) {
        }
    }

    _delay_ms(50);  // wait for LCD to boot

    // Send reset sequence (3x 0x30, then 0x20 to switch to 4-bit mode)
	lcd_write_nibble(0x30, RS_COMMAND);
    _delay_ms(5);
    lcd_write_nibble(0x30, RS_COMMAND);
    _delay_us(150);
    lcd_write_nibble(0x30, RS_COMMAND);
    _delay_us(150);
    lcd_write_nibble(0x20, RS_COMMAND);  // 4-bit mode

    lcd_write_command(0x28);  // Function set: 4-bit, 2 lines, 5x8 dots
    lcd_write_command(0x0F);  // Display ON, cursor ON, blinking ON
    lcd_write_command(0x06);  // Entry mode set: cursor right
    lcd_clear();              // Clear display
}

uint8_t lcd_find(void) {
    for (uint16_t addr = 0x03; addr < 0x78; addr++) {
        twi_start();

        twi_send_data(addr);
        uint8_t status = twi_get_status();

        twi_stop();

        if (status == 0x18 || status == 0x40) {
            lcd_address = addr;
            break;
        }
    }

    if (!lcd_address) {
        PORTB |= (1 << PB4);
        return 0;
    }
    return 1;
}

void lcd_clear(void) {
    lcd_write_command(0x01);
    _delay_ms(2);
}

void lcd_home(void) {
    lcd_write_command(0x02);
    _delay_ms(2);
}

void lcd_display_char(char c, uint8_t row, uint8_t col) {
    lcd_set_cursor(col, row);
    lcd_write_data(c);
}

void lcd_display_string(const char str[], uint8_t row) {
    int i = 0;
    while (*str) {
        lcd_display_char(*str++, row, i++);
    }
}
void lcd_set_cursor(uint8_t col, uint8_t row) {
    if (row > 1) row = 1;
    uint8_t address = col + (row == 0 ? 0x00 : 0x40);
    lcd_write_command(0x80 | address);
}

void lcd_cursor_on() { lcd_write_command(0x0E); }
void lcd_cursor_off() { lcd_write_command(0x0C); }
void lcd_cursor_blink() { lcd_write_command(0x0F); }

void lcd_write_nibble(uint8_t nibble, uint8_t rs_bit) {
    uint8_t data = (nibble & 0xF0) | BACKLIGHT | rs_bit | RW;

    twi_start();
    twi_send_data(lcd_address);
    twi_send_data(data | EN);
    _delay_us(1);
    twi_stop();

    twi_start();
    twi_send_data(lcd_address);
    twi_send_data(data & ~EN);
    _delay_us(50);
    twi_stop();
}

void lcd_write_command(uint8_t cmd) {
    uint8_t high_nibble = cmd & 0xF0;
    uint8_t low_nibble = (cmd << 4) & 0xF0;

    lcd_write_nibble(high_nibble, RS_COMMAND);
    lcd_write_nibble(low_nibble, RS_COMMAND);

    // clear/home takes 1.53 ms
    if (cmd == 0x01 || cmd == 0x02) {
        _delay_ms(2);
    }
}
void lcd_write_data(uint8_t data) {
    uint8_t high_nibble = data & 0xF0;
    uint8_t low_nibble = (data << 4) & 0xF0;

    lcd_write_nibble(high_nibble, RS_DATA);
    lcd_write_nibble(low_nibble, RS_DATA);

    // clear/home takes 1.53 ms
    if (data == 0x01 || data == 0x02) {
        _delay_ms(2);
    }
}
