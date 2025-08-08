#include "dht22.h"

#include <util/delay.h>

#include "core.h"

#define DHT22_DDR DDRB
#define DHT22_PRT PORTB
#define DHT22_PIN PINB

#define DHT22_DATA PB3

uint8_t wait_for_pin_change(uint8_t expected_value, uint16_t timeout_us) {
    uint16_t count = 0;
    while (count < timeout_us) {
        if ((DHT22_PIN & (1 << DHT22_DATA) ? 1 : 0) == expected_value) {
            return 1;
        }
        _delay_us(1);
        count++;
    }
    return 0;
}

static void request_dht(void) {
    DHT22_DDR |= (1 << DHT22_DATA);
    DHT22_PRT &= ~(1 << DHT22_DATA);
    _delay_ms(18);

    DHT22_PRT |= (1 << DHT22_DATA);
    DHT22_DDR &= ~(1 << DHT22_DATA);
    _delay_us(40);
}

static uint8_t read_bit(void) {
    if (!wait_for_pin_change(1, 100)) return 0XFF;

    _delay_us(30);

    uint8_t bit = DHT22_PIN & (1 << DHT22_DATA) ? 1 : 0;

    if (!wait_for_pin_change(0, 100)) return 0xFF;

    return bit;
}
static uint8_t read_byte(void) {
    uint8_t res = 0;

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t bit = read_bit();
        if (bit == 0XFF) return 0xFF;
        res = (res << 1) | bit;
    }

    return res;
}

void dht22_read(struct dht22_temp* d) {
    request_dht();

    if (!wait_for_pin_change(1, 80)) {
        return;
    }

    if (!wait_for_pin_change(0, 80)) {
        return;
    }

    uint8_t data[5];
    for (uint8_t i = 0; i < 5; i++) {
        data[i] = read_byte();
    }

    uint8_t sum = data[0] + data[1] + data[2] + data[3];
    if (sum != data[4]) {
        PORTB |= (1 << PB4);
        _delay_ms(1000);
        PORTB &= (1 << PB4);
    }

    d->hum_high = data[0];
    d->hum_low = data[1];
    d->temp_high = data[2];
    d->temp_low = data[3];
    d->checksum = data[4];
}

float dht22_get_temp(struct dht22_temp* d) {
    int16_t raw_temp = ((int16_t)d->temp_high << 8) | d->temp_low;
    return raw_temp / 10.0f;
}
float dht22_get_humidity(struct dht22_temp* d) {
    uint16_t raw_hum = ((uint16_t)d->hum_high << 8) | d->hum_low;
    return raw_hum / 10.0f;
}

void dht22_temperature_extract(char buf[], struct dht22_temp* d) {
    float temperature = dht22_get_temp(d);

    char temp[5];
    ftostr(temp, temperature, 1);

    str_append(buf, "Temp: ");
    str_append(buf, temp);
    str_append(buf, "C");
}

void dht22_humidity_extract(char buf[], struct dht22_temp* d) {
    float humidity = dht22_get_humidity(d);

    char temp[5];
    ftostr(temp, humidity, 1);

    str_append(buf, "Humidity: ");
    str_append(buf, temp);
    str_append(buf, "%");
}
