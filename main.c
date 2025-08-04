#include <avr/io.h>
#include <util/delay.h>

#include "ds1302.h"
#include "usart.h"

int main(void) {
    char buf[100];

    usart_init();
    ds1302_init();

    while (1) {
        struct ds1302_time_t t;
        ds1302_get_time(&t);

        ds1302_time_to_str(buf, &t);

        usart_print_string(buf);
        usart_print_string("\r\n");

        _delay_ms(1000);
    }

    return 0;
}
