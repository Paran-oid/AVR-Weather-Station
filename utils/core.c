#include "core.h"

#include "usart.h"

uint32_t str_len(const char str[]) {
    uint32_t res = 0;
    while (*str++) res++;
    return res;
}
void str_prepend(char dst[], const char src[]) {
    uint32_t src_size = str_len(src);
    uint32_t dst_size = str_len(dst);

    uint32_t total_size = dst_size + src_size;
    char buf[BUF_SIZE];

    if (total_size >= BUF_SIZE) return;

    uint32_t i = 0;
    while (i < src_size) {
        buf[i] = src[i];
        i++;
    }

    uint32_t j = 0;
    while (j < dst_size) {
        buf[i + j] = dst[j];
        j++;
    }

    buf[total_size] = '\0';

    for (uint32_t k = 0; k <= i + j; k++) {
        dst[k] = buf[k];
    }
}

void str_append(char dst[], const char src[]) {
    uint32_t src_size = str_len(src);
    uint32_t dst_size = str_len(dst);

    uint32_t total_size = dst_size + src_size;
    char buf[BUF_SIZE];

    if (total_size >= BUF_SIZE) return;

    uint32_t i = 0;
    while (i < dst_size) {
        buf[i] = dst[i];
        i++;
    }

    uint32_t j = 0;
    while (j < src_size) {
        buf[i + j] = src[j];
        j++;
    }

    buf[total_size] = '\0';

    for (uint32_t k = 0; k <= i + j; k++) {
        dst[k] = buf[k];
    }
}

void str_reverse(char str[]) {
    uint32_t len = str_len(str);

    for (uint32_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

void _itoa(int value, char str[], int base) {
    const char map[] = "0123456789ABCDEF";
    str[0] = '\0';

    while (value) {
        char temp[2] = {map[value % base], '\0'};
        str_append(str, temp);
        value = value / base;
    }

    str_reverse(str);
}