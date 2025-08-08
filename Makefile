PROGRAMMER = arduino
PORT = /dev/ttyACM0 9600
BAUDRATE = 115200

MCU = atmega328p
F_CPU = 16000000UL
TARGET = build/main

SRC = main.c

CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Ilib/internal/twi -Ilib/internal/pwm -Ilib/internal/usart -Ilib/internal/adc -Ilib/external/ds1302 -Iutils -Ilib/external/lcd -Ilib/external/dht22 -Ilib/external/buttons
LDFLAGS = -mmcu=$(MCU)

all:$(TARGET).hex

$(TARGET).elf: $(SRC) lib/internal/twi/twi.c lib/internal/usart/usart.c utils/core.c lib/external/ds1302/ds1302.c lib/external/lcd/lcd.c lib/external/dht22/dht22.c lib/external/buttons/buttons.c
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

upload: $(TARGET).hex
	avrdude -c $(PROGRAMMER) -P $(PORT) -b $(BAUDRATE) -p $(MCU) -U flash:w:$(TARGET).hex:i

clean:
	rm -f $(OBJ) $(ELF) $(HEX)