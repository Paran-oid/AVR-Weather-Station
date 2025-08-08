#ifndef USART_H
#define USART_H

#include <avr/io.h>

#define BAUD 9600

void usart_init(void);
void usart_transmit_byte(uint8_t data);
uint8_t usart_receive_byte(void);

void usart_print_string(const char str[]);
void usart_print_number(uint32_t num);
void usart_print_hex(uint32_t num);
void usart_print_binary(uint32_t num);
void usart_print_float(float num, uint8_t decimal_places);

uint16_t usart_string_get(char str[], uint16_t str_size);

#endif