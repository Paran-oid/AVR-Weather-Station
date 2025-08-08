#include "usart.h"

#include <avr/io.h>
#include <util/setbaud.h>

#include "core.h"

// UBRR: Usart baud rate register (16 bit register)

void usart_init(void) {
    // set baud rate basically
    UBRR0H = UBRRH_VALUE;  // UBRRH_VALUE is a ready to use micro
    UBRR0L = UBRRL_VALUE;  // UBRRL_VALUE is a ready to use micro

// U2X0 enables double speed mode for the USART.
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif

    // enable receiver and transmitter
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);

    // set frame format: 8data; 1stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void usart_transmit_byte(uint8_t data) {
    // wait for empty transmit buf
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data;  // send data
}

uint8_t usart_receive_byte(void) {
    loop_until_bit_is_set(UCSR0A, RXC0);  // wait for incoming data
    return UDR0;                          // return register value
}

// must pass an already defined buf
uint16_t usart_string_get(char buf[], uint16_t buf_size) {
    uint16_t i = 0;
    unsigned char byte;

    while (1) {
        if (i >= buf_size - 1) {
            return 1;
        }

        byte = usart_receive_byte();
        usart_transmit_byte(byte);

        if (byte == '\r') {
            break;
        }

        buf[i++] = byte;
    }

    buf[i] = '\0';

    return i;
}

void usart_print_string(const char s[]) {
    while (*s) {
        usart_transmit_byte(*s++);
    }
}

void usart_print_number(uint32_t num) {
    char buf[12];
    _itoa(num, buf, 10);
    usart_print_string(buf);
}
void usart_print_hex(uint32_t num) {
    char buf[5];
    _itoa(num, buf, 16);
    usart_print_string("0x");
    usart_print_string(buf);
}

void usart_print_binary(uint32_t num) {
    char buf[36];
    _itoa(num, buf, 2);
    usart_print_string("0b");
    usart_print_string(buf);
}

void usart_print_float(float num, uint8_t decimal_places) {
    if (decimal_places > MAX_DECIMALS) return;

    char int_str[10];
    char fraction_str[10];

    int_str[0] = fraction_str[0] = '\0';

    int32_t int_part = (int32_t)num;
    float fraction = num - int_part;

    if (fraction < 0) {
        fraction *= -1;
    }

    uint32_t scaled_fraciton = fraction * _pow(10, decimal_places);

    _itoa(int_part, int_str, 10);
    _itoa(scaled_fraciton, fraction_str, 10);

    usart_print_string(int_str);
    if (decimal_places != 0 && fraction_str[0] == '\0') {
        usart_print_string(".");
        usart_print_string(fraction_str);
    }
}