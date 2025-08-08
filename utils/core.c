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
    char buf[STR_SIZE];

    if (total_size >= STR_SIZE) return;

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
    char buf[STR_SIZE];

    if (total_size >= STR_SIZE) return;

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

void ftostr(char dst[], float num, uint8_t decimal_places) {
    if (decimal_places > MAX_DECIMALS) return;

    dst[0] = '\0';

    char int_str[10];
    char fraction_str[10];

    if (num < 0) {
        str_append(dst, "-");
        num = -num;
    }

    int32_t int_part = (int32_t)num;
    float fraction = num - int_part;

    _itoa(int_part, int_str, 10);
    str_append(dst, int_str);

    if (decimal_places > 0) {
        str_append(dst, ".");

        uint32_t scaled_fraction =
            (uint32_t)(fraction * _pow(10, decimal_places));

        int zeros_needed =
            decimal_places -
            (scaled_fraction == 0 ? 0 : (uint8_t)_log10(scaled_fraction) + 1);
        while (--zeros_needed > 0) {
            str_append(dst, "0");
        }

        if (scaled_fraction > 0) {
            _itoa(scaled_fraction, fraction_str, 10);
            str_append(dst, fraction_str);
        } else {
            str_append(dst, "0");
        }
    }
}

uint8_t _log10(uint32_t n) {
    if (n == 0) return 0;

    uint8_t digits = 0;
    while (n != 0) {
        n /= 10;
        digits++;
    }
    return digits - 1;
}

int64_t _pow(int64_t base, int64_t exp) {
    int64_t result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

void _itoa(int value, char str[], int base) {
    if (value == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    const char map[] = "0123456789ABCDEF";
    str[0] = '\0';

    while (value) {
        char temp[2] = {map[value % base], '\0'};
        str_append(str, temp);
        value = value / base;
    }

    str_reverse(str);
}

void _memset(void* buf, int32_t c, uint32_t size) {
    char* p = (char*)buf;
    for (uint32_t i = 0; i < size; i++) {
        p[i] = c;
    }
}