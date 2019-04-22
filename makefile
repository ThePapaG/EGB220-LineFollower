
MAIN = 220

FLASH.bin : $(MAIN).elf
	avr-objcopy -O binary $(MAIN).elf FLASH.bin

$(MAIN).elf : $(MAIN).o ISR.o Hardware.o
	avr-gcc -mmcu=atmega32u4 $(MAIN).o ISR.o Hardware.o -o $(MAIN).elf

$(MAIN).o : $(MAIN).c ISR.h Hardware.h
	avr-gcc -O1 -mmcu=atmega32u4 -c $(MAIN).c -o $(MAIN).o

ISR.o : ISR.c
	avr-gcc -O1 -mmcu=atmega32u4 -c ISR.c -o ISR.o

Hardware.o : Hardware.c
	avr-gcc -O1 -mmcu=atmega32u4 -c Hardware.c -o Hardware.o		
