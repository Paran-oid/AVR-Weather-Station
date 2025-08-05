#ifndef LCD_H
#define LCD_H

#include <avr/io.h>

void lcd_init(void);
uint8_t lcd_find(void);
void lcd_clear(void);
void lcd_home(void);

void lcd_display_char(char c, uint8_t row, uint8_t col);
void lcd_display_string(const char str[], uint8_t row);

void lcd_set_cursor(uint8_t cx, uint8_t cy);
void lcd_cursor_on();
void lcd_cursor_off();
void lcd_cursor_blink();

void lcd_write_nibble(uint8_t nibble, uint8_t rs_bit);
void lcd_write_command(uint8_t cmd);
void lcd_write_data(uint8_t data);

#endif