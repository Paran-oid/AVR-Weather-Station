#include <avr/io.h>
#include <util/delay.h>

#include "core.h"
#include "dht22.h"
#include "ds1302.h"
#include "lcd.h"
#include "twi.h"
#include "usart.h"

// int main(void) {
//     char time_buf[16];
//     char date_buf[16];

//     debug_init();

//     twi_init();
//     usart_init();

//     lcd_init();
//     ds1302_init();

//     while (1) {
//         struct ds1302_time_t t;
//         ds1302_get_time(&t);

//         ds1302_date_extract(date_buf, &t);
//         ds1302_time_extract(time_buf, &t);

//         lcd_display_string(time_buf, 0);
//         lcd_display_string(date_buf, 1);
//         lcd_cursor_off();

//         _delay_ms(1000);
//     }

//     return 0;
// }

int main(void) {
    usart_init();
    struct dht22_temp d;

    char buf[16];

    while (1) {
        dht22_read(&d);

        int16_t temperature = (float)(dht22_get_temp(&d)) * 10;
        int16_t humidity = (float)(dht22_get_humidity(&d)) * 10;

        usart_print_string("Temp: ");
        _itoa(temperature, buf, 10);
        usart_print_string(buf);
        usart_print_string("C");
        usart_print_string("\r\n");

        usart_print_string("Humidity: ");
        _itoa(humidity, buf, 10);
        usart_print_string(buf);
        usart_print_string("%");
        usart_print_string("\r\n\r\n");

        _delay_ms(2000);
    }
    return 0;
}