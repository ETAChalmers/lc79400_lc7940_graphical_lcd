CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

SOURCES=\
	main.c \
	image.c \
	lcd.c \
	support/gcc_startup_nrf52.S \
	support/startup.c \
	support/system_nrf52.c

LD_SCRIPT=support/nrf52832_xxaa.ld

CFLAGS=\
	-Isupport \
	-DBOARD_PCA10040 \
	-DCONFIG_GPIO_AS_PINRESET \
	-DNRF52 \
	-DNRF52832_XXAA \
	-DSTARTUP_CONFIG \
	-D__HEAP_SIZE=512 \
	-mcpu=cortex-m4 \
	-mlittle-endian \
	-mthumb \
	-mabi=aapcs \
	-Wall \
	-Werror \
	-ffunction-sections \
	-fdata-sections \
	-fno-strict-aliasing \
	-fno-builtin \
	-fshort-enums \
	-g3 \
	-T$(LD_SCRIPT) \
	-Wl,--gc-sections \
	-nostartfiles

all: app.elf

app.elf: $(SOURCES) $(LD_SCRIPT)
	$(CC) $(CFLAGS) $(SOURCES) -o $@

app.hex: app.elf
	$(OBJCOPY) -O ihex $< $@

flash: app.hex
	nrfjprog -f nrf52 --eraseall
	nrfjprog -f nrf52 --program $< --verify $<
	nrfjprog -f nrf52 --reset

clean:
	rm -f app.elf app.hex

.PHONY: clean all flash
