#ifndef TWI_H
#define TWI_H

#include <avr/io.h>

#define SDA PC4
#define SCL PC5

#define SLA_READ(addr) ((addr << 1) | 1)
#define SLA_WRITE(addr) ((addr << 1) | 0)

void twi_init(void);

void twi_wait_for_completion(void);
void twi_start(void);
void twi_stop(void);

uint8_t twi_read_no_ack();
uint8_t twi_read_ack();

uint8_t twi_send_data(uint8_t data);
uint8_t twi_get_status(void);

#endif