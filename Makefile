CC_FLAGS=-std=c99 -Wall -O0 -g3 -iquote ./ -I ./ -I ./STM32CubeF3/Drivers/CMSIS/Device/ST/STM32F3xx/Include/\
 -I ./STM32CubeF3/Drivers/CMSIS/Core/Include/ -mfloat-abi=soft -mcpu=cortex-m4 -mthumb -gdwarf-5 -D=STM32F303xE 
#CC=D:\ARM\ver12_2\arm-gnu-toolchain-12.2.rel1-mingw-w64-i686-arm-none-eabi\bin\arm-none-eabi-gcc.exe
CC=arm-none-eabi-gcc

all: f.elf

main.o: main.c Makefile
	${CC} -c main.c ${CC_FLAGS}

startup.o: startup.c Makefile
	${CC} -c startup.c ${CC_FLAGS}

adc_dma.o: adc_dma.c Makefile
	${CC}  -c adc_dma.c ${CC_FLAGS}

uart_diag.o: uart_diag.c Makefile
	${CC}  -c uart_diag.c ${CC_FLAGS}

f.elf: startup.o main.o adc_dma.o uart_diag.o Makefile linker_script.ld
	${CC} ${CC_FLAGS} -o f.elf main.o startup.o adc_dma.o uart_diag.o -specs=nosys.specs -u _print_float -nostartfiles -T linker_script.ld 
	# arm-none-eabi-objdump -D f.elf > f.elf.dump
	# arm-none-eabi-objcopy -O ihex $@ f.elf.ihex
	# arm-none-eabi-objcopy -O srec $@ f.elf.srec
	# arm-none-eabi-size $@

.PHONY: clean
clean:
	-rm ./*.o ./*.elf ./*.ihex ./*.dump ./*.srec
