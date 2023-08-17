PREFIX = arm-none-eabi
CC = $(PREFIX)-gcc
OBJCOPY = $(PREFIX)-objcopy

STM_COMMON = /home/jean-lopes/stm32/STM32F4xx_DSP_StdPeriph_Lib_V1.9.0

MCU=STM32F401xx

LDFILE=stm32f401ccux.ld

#search for startup ASM (.s) files in STM Library
vpath %.s $(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates/TrueSTUDIO
# search for '.h' files in STM Library
vpath %.h $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
# search for '.c' files in STM Library
vpath %.c $(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/src
vpath %.c $(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F4xx/Source/Templates

CFLAGS  = -g -O2 -Wall -std=gnu99
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -D$(MCU)
CFLAGS += -T$(LDFILE)
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include
CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Device/ST/STM32F4xx/Include
CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc
CFLAGS += -I.

C_HEADERS = stm32f4xx_conf.h

C_SOURCES = main.c

# STM Library sources
C_SOURCES += system_stm32f4xx.c
C_SOURCES += stm32f4xx_rcc.c
C_SOURCES += stm32f4xx_gpio.c
C_SOURCES += stm32f4xx_usart.c

ASM_SOURCES = startup_stm32f401xx.s

SRCS= $(C_SOURCES) $(ASM_SOURCES)

OBJS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

.PHONY: all build clean burn

all: clean build

clean:
	rm -f $(OBJS) main.elf main.hex main.bin

main.elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@

main.bin: main.elf
	$(OBJCOPY) -O binary main.elf main.bin

build: main.bin

burn: main.bin
	st-flash write main.bin 0x08000000
