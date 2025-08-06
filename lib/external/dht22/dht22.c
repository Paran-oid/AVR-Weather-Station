#include "dht22.h"

#include <util/delay.h>

#define DHT22_DDR DDRB
#define DHT22_PRT PORTB
#define DHT22_PIN PINB

#define DHT22_DATA PB3


// TODO: fix this crap

static void request_dht(void) {
    DHT22_DDR |= (1 << DHT22_DATA);
    DHT22_PRT &= ~(1 << DHT22_DATA);
    _delay_ms(2);

    DHT22_PRT |= (1 << DHT22_DATA);
    DHT22_DDR &= ~(1 << DHT22_DATA);
    _delay_us(40);
    while (DHT22_PIN & (1 << DHT22_DATA));
}
static uint8_t read_bit(void) {
    while (!(DHT22_PIN & (1 << DHT22_DATA)));
    _delay_us(28);
    return (DHT22_PIN & (1 << DHT22_DATA)) ? 1 : 0;
}
static uint8_t read_byte(void) {
    uint8_t res = 0;

    for (uint8_t i = 0; i < 8; i++) {
        res <<= 1;
        res |= read_bit();
        _delay_us(50);
    }
    return res;
}

void dht22_read(struct dht22_temp* d) {
    request_dht();

    _delay_us(80);
    if (!(DHT22_PIN & (1 << DHT22_DATA))) return;

    _delay_us(80);
    if (DHT22_PIN & (1 << DHT22_DATA)) return;

    d->hum_high = read_byte();
    d->hum_low = read_byte();
    d->temp_high = read_byte();
    d->temp_low = read_byte();
    d->checksum = read_byte();
}

float dht22_get_temp(struct dht22_temp* d) {
    int16_t raw_temp = ((int16_t)d->temp_high << 8) | d->temp_low;
    if (raw_temp & 0x8000) {
        raw_temp = -(raw_temp & 0x7FFF);
    }
    return raw_temp / 10.0f;
}
float dht22_get_humidity(struct dht22_temp* d) {
    uint16_t raw_hum = ((uint16_t)d->hum_high << 8) | d->hum_low;
    return raw_hum / 10.0f;
}