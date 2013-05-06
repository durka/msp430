CC = msp430-gcc
UPLOAD = mspdebug

MCU = msp430g2553

all: hello_world

%: %.elf
	$(UPLOAD) rf2500 "prog $<"

%.elf: %.c
	$(CC) -mmcu=$(MCU) -o $@ $<

clean:
	rm *.elf

