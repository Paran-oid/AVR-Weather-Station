#ifndef CORE_H
#define CORE_H

#include <avr/io.h>

#define STR_SIZE 120
#define MAX_DECIMALS 6

#define debug_init() (DDRB |= (1 << DDB4))

uint32_t str_len(const char str[]);
void str_prepend(char dst[], const char src[]);
void str_append(char dst[], const char src[]);
void str_reverse(char str[]);

void ftostr(char dst[], float num, uint8_t decimal_places);

uint8_t _log10(uint32_t n);
int64_t _pow(int64_t base, int64_t exp);
void _itoa(int value, char str[], int base);
void _memset(void* buf, int32_t c, uint32_t size);

#endif