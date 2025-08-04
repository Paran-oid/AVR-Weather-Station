#ifndef CORE_H
#define CORE_H

#include <avr/io.h>

#define BUF_SIZE 120

uint32_t str_len(const char str[]);
void str_prepend(char dst[], const char src[]);
void str_append(char dst[], const char src[]);
void str_reverse(char str[]);

void _itoa(int value, char str[], int base);

#endif