button.h: 

led.h: 

lab1-ex4.c: button.h led.h

lab1-ex4.o: lab1-ex4.c

../../drivers/avr/pio.h: 

button.c: ../../drivers/avr/pio.h button.h

button.o: button.c

../../drivers/avr/pio.c: ../../drivers/avr/pio.h

../../drivers/avr/pio.o: ../../drivers/avr/pio.c

led.c: ../../drivers/avr/pio.h led.h

led.o: led.c

../../drivers/avr/system.c: 

../../drivers/avr/system.o: ../../drivers/avr/system.c

lab1-ex4.out: lab1-ex4.o button.o ../../drivers/avr/pio.o led.o ../../drivers/avr/system.o

