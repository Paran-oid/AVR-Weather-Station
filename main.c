#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "buttons.h"
#include "core.h"
#include "dht22.h"
#include "ds1302.h"
#include "lcd.h"
#include "twi.h"
#include "usart.h"

uint8_t current_mode = 1;

ISR(PCINT0_vect) {
    static uint8_t previous = 1;
    uint8_t current = (PINB & (1 << PB5)) ? 1 : 0;
    if (previous && !current) {
        current_mode ^= 1;
    }

    previous = current;
}

int main(void) {
    // basics
    debug_init();
    button_program_init();

    // functionalities
    twi_init();
    usart_init();
    sei();

    // peripherals
    lcd_init();
    ds1302_init();

    char buf1[16];
    char buf2[16];

    while (1) {
        _memset(buf1, 0, 16);
        _memset(buf2, 0, 16);

        if (current_mode) {
            // TODO: fix this crap not showing up ffs
            struct ds1302_time_t t;
            ds1302_get_time(&t);

            ds1302_date_extract(buf1, &t);
            ds1302_time_extract(buf2, &t);

            lcd_display_string(buf2, 0);
            lcd_display_string(buf1, 1);

            lcd_cursor_off();
            _delay_ms(1000);

        } else {
            struct dht22_temp d;
            dht22_read(&d);

            dht22_temperature_extract(buf2, &d);
            dht22_humidity_extract(buf1, &d);

            // TODO: after debug enable me
            lcd_display_string(buf2, 0);
            lcd_display_string(buf1, 1);

            lcd_cursor_off();
            _delay_ms(1000);
        }
        lcd_clear();
    }

    return 0;
}
