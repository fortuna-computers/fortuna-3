PROJECT=blink
OBJECTS=main.o

CC=arm-none-eabi-gcc
COMMON_FLAGS=-mcpu=cortex-m3 -mfix-cortex-m3-ldrd -mthumb -msoft-float
CPPFLAGS=-std=c11 -Wall -Wextra ${COMMON_FLAGS} -Os -I../libopencm3/include -DSTM32F1
# CPPFLAGS+=-g -O0
LDSCRIPT=stm32f103.ld 
LDFLAGS=--static -nostartfiles -T${LDSCRIPT} -Wl,--gc-sections ${COMMON_FLAGS} -L../libopencm3/lib -Wl,-Map=main.map -Wl,--cref -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group -specs=nano.specs -specs=nosys.specs

all: ${PROJECT}.elf

${PROJECT}.elf: ${OBJECTS}
	${CC} ${LDFLAGS} -o $@ $^ -lopencm3_stm32f1 
	arm-none-eabi-size ${PROJECT}.elf

test-connection:
	sudo openocd -f openocd.cfg -c "init; targets; exit"

upload: ${PROJECT}.elf
	sudo openocd -f openocd.cfg -c "program $^ verify reset exit"

clean:
	rm -f *.elf *.o *.map

# vim: set ts=8 sts=8 sw=8 noexpandtab:
