PREFIX 			= arm-none-eabi 
CC					= $(PREFIX)-gcc

CFLAGS			= -mpcu=cortex-m4 -mthumb -g
LDFLAGS			= -nostdlib -T linker_script.ld

EXECUTABLE 	= base.elf


all: build link

build: base.o startup_stm32f303vctx.o

base.o: base.c 
	arm-none-eabi-gcc -g -mcpu=cortex-m4 -mthumb -c base.c

startup_stm32f303vctx.o: startup_stm32f303vctx.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -c startup_stm32f303vctx.s

link: base.o startup_stm32f303vctx.o STM32F303VCTX_FLASH.ld
	arm-none-eabi-gcc base.o startup_stm32f303vctx.o -mcpu=cortex-m4 -mthumb --specs=nosys.specs -T STM32F303VCTX_FLASH.ld -lc -o base.elf

clean:
	rm *.o *.elf
