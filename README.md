# AVR Weather Station

A digital weather station built using an AVR microcontroller that displays time, temperature, and humidity on a 16x2 LCD display.

## Features

- Real-time clock using DS1302 RTC module
- Temperature and humidity monitoring using DHT22 sensor
- 16x2 LCD display interface
- Mode switching between time/date and weather information
- UART debugging capabilities
- I2C (TWI) communication support

## Hardware Requirements

- AVR microcontroller (ATmega328P/ATmega32)
- 16x2 LCD Display
- DHT22 Temperature & Humidity Sensor
- DS1302 Real-Time Clock Module
- Push button for mode switching
- 16MHz crystal oscillator
- Power supply (5V)

## Project Structure

```
AVR Weather Station/
├── lib/
│   ├── external/          # Third-party libraries
│   │   ├── dht22/        # DHT22 sensor driver
│   │   └── ds1302/       # RTC module driver
│   └── internal/         # Project-specific libraries
│       ├── lcd/          # LCD display driver
│       ├── twi/          # I2C communication
│       └── usart/        # Serial communication
├── utils/
│   └── core.c           # Utility functions
├── include/             # Header files
├── docs/               # Documentation and datasheets
└── Makefile            # Build configuration
```

## Building and Flashing

Make sure you have the AVR-GCC toolchain installed. Then:

```bash
# Build the project
make

# Flash to microcontroller
make flash
```

## Features Implementation

- **Time Display Mode**: Shows current time and date from the DS1302 RTC
- **Weather Mode**: Displays current temperature (°C) and humidity (%)
- **Mode Switching**: Using external interrupt (PCINT0) on button press
- **Serial Debug**: UART communication for debugging purposes

## Pin Configuration

- LCD: PORT[D4-D7] for data, PORT[RS,EN] for control
- DHT22: Single wire on PORTB
- DS1302: CLK, DAT, RST pins on designated ports
- Button: PORTB5 with internal pull-up

## Dependencies

- avr-gcc
- avrdude
- make

## License

MIT License

Copyright (c) 2025 Aziz Hmidi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Author

Aziz Hmidi
