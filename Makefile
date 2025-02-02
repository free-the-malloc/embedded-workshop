
# -Os optimises the binary size
# -D is to define a macro, F_APU to equal 16000000UL
# -m defines which AVR architecture we're using

CC := avr-gcc

CFLAGS = -Os -DF_APU=16000000UL -mmcu=atmega328p -Iinclude
LDFLAGS = -mmcu=atmega328p -Iinclude
FFLAGS = -c arduino -p ATmega328p

BUILD_DIR := build
SRC_DIR := src
IDIR := include

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(shell find $(IDIR) -name '*.h')


$(TARGET).hex: $(BUILD_DIR)/$(TARGET)
	avr-objcopy -O ihex $< $(BUILD_DIR)/$@

$(BUILD_DIR)/$(TARGET): $(OBJS)
	avr-gcc $(LDFLAGS) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.c.o: %.c $(DEPS)
	mkdir -p $(dir $@)
	avr-gcc $(CFLAGS) -c -o $@ $<

deploy: $(TARGET).hex
	avrdude $(FFLAGS) -P $(PORT) -U flash:w:$(BUILD_DIR)/$(TARGET).hex


.PHONY: clean

clean:
	rm -r $(BUILD_DIR)
