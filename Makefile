CC = arm-none-eabi-gcc
CFLAGS = -I ./includes -O2 -Wall -mthumb -mcpu=cortex-m0plus
LDFLAGS = -O2 -Wall -mthumb -mcpu=cortex-m0plus --specs=nano.specs -Wl,--gc-sections,-Map=$(TARGET).map -T link.ld
OPENOCD = openocd -f openocd.cfg -c "program $(TARGET).elf verify reset exit"

TARGET = main
OBJS = main.o startup.o

all: $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

flash: $(TARGET).elf
	$(OPENOCD)

clean:
	rm -f $(OBJS)

cleanall: clean
	rm -f $(TARGET).elf
