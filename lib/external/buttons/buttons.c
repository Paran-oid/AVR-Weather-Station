#include "buttons.h"

#include <avr/interrupt.h>
#include <util/delay.h>

#define BUTTONS_DDR DDRB
#define BUTTONS_PRT PORTB
#define BUTTONS_PIN PINB

#define BUTTONP PB5

void button_program_init(void) {
    BUTTONS_DDR &= ~(1 << BUTTONP);
    BUTTONS_PRT |= (1 << BUTTONP);

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT5);
}
