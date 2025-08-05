#include <avr/io.h>
#include <util/delay.h>

#include "core.h"
#include "ds1302.h"
#include "lcd.h"
#include "twi.h"
#include "usart.h"

int main(void) {
    char time_buf[16];
    char date_buf[16];

    debug_init();

    twi_init();
    usart_init();

    lcd_init();
    ds1302_init();

    while (1) {
        struct ds1302_time_t t;
        ds1302_get_time(&t);

        ds1302_date_extract(date_buf, &t);
        ds1302_time_extract(time_buf, &t);

        lcd_display_string(time_buf, 0);
        lcd_display_string(date_buf, 1);
        lcd_cursor_off();

        _delay_ms(1000);
    }

    return 0;
}
