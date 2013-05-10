CC = msp430-gcc
UPLOAD = mspdebug

MCU = msp430g2553

COMMON_SRC = common.c
COMMON_HDR = common.h

CFLAGS = -mmcu=$(MCU)

all: main

%: %.elf
	$(UPLOAD) rf2500 "prog $<"

%.elf: %.c morse.o nokia_lcd.o key.o common.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.c %.h common.o
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

common.o: common.c common.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $<

clean:
	rm *.elf *.o

