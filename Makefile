STM_COMMON=/home/jean-lopes/stm32/STM32CubeF4

vpath %.s $(STM_COMMON)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc
vpath %.c $(STM_COMMON)/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates
vpath %.c $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src

CFLAGS  = -g -O2 -Wall -std=gnu99
CFLAGS += -DUSE_FULL_LL_DRIVER -DSTM32F401xC -Tstm32f401ccux.ld
CFLAGS += --specs=nosys.specs -mcpu=cortex-m4 -mlittle-endian
CFLAGS += -mthumb -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -I$(STM_COMMON)/Drivers/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I.

STM_SRCS = 					\
	stm32f4xx_ll_gpio.c 	\
	stm32f4xx_ll_rcc.c	 	\
	stm32f4xx_ll_usart.c 	\
	system_stm32f4xx.c   	\
	startup_stm32f401xc.s

.PHONY: clean

clean:
	rm -f *.o main.elf main.bin

main.elf: $(wildcard *.c) $(STM_SRCS)
	arm-none-eabi-gcc $(CFLAGS) $^ -o $@

main.bin: main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin

burn: main.bin
	st-flash write main.bin 0x08000000

build: main.elf

ocd:
	openocd \
		--file /usr/share/openocd/scripts/interface/stlink-v2-1.cfg \
		--file /usr/share/openocd/scripts/target/stm32f4x.cfg -c init
