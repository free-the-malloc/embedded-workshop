
# -Os optimises the binary size
# -D is to define a macro, F_APU to equal 16000000UL
# -m defines which AVR architecture we're using

CFLAGS = -Os -DF_APU=16000000UL -mmcu=atmega328p
LDFLAGS = -mmcu=atmega328p
FFLAGS = -c arduino -p ATmega328p
BUILD = build
SRC = src
TARGET = blink

$(TARGET).hex: $(TARGET)
	avr-objcopy -O ihex $(BUILD)/$< $(BUILD)/$@

$(TARGET): $(TARGET).o
	avr-gcc $(LDFLAGS) -o $(BUILD)/$@ $(BUILD)/$<

$(TARGET).o: $(SRC)/$(TARGET).c
	mkdir -p $(BUILD)
	avr-gcc $(CFLAGS) -c -o $(BUILD)/$@ $<

deploy: $(TARGET).hex
	mkdir -p $(BUILD)
	avrdude $(FFLAGS) -P $(PORT) -U flash:w:$(BUILD)/$(TARGET).hex

clean:
	rm -r $(BUILD)