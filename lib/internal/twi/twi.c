#include "twi.h"

#include <util/delay.h>

void twi_init(void) {
    // SCL_FREQ = F_CPU / (16 + 2TWBR * prescaler)
    TWBR = 72;
    TWCR |= (1 << TWEN);
}

void twi_wait_for_completion(void) { while (!(TWCR & (1 << TWINT))); }

void twi_start(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    twi_wait_for_completion();
}

void twi_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    // No need to wait for TWSTO to clear for stop condition
}

uint8_t twi_read_no_ack() {
    TWCR |= (1 << TWINT) | (1 << TWEN);
    twi_wait_for_completion();

    return TWDR;
}

uint8_t twi_read_ack() {
    TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    twi_wait_for_completion();
    return TWDR;
}

uint8_t twi_get_status(void) { return TWSR & 0xF8; }

uint8_t twi_send_data(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    twi_wait_for_completion();

    uint8_t status = twi_get_status();
    // Return 0 if no ACK received (error)
    if (status != 0x28 && status != 0x18 && status != 0x40) {
        return 0;
    }
    return 1;
}