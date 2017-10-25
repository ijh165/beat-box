OUTDIR = ~/cmpt433/public/myApps
EXE = retrofit-kit

CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread

SRC = main.c devices/accelerometer.c lib/util.c lib/i2c.c \
	network/udp.c  network/udp_interface.c
	 
OBJ = $(addsuffix .o, $(basename $(SRC)))

all: beatbox

beatbox: $(OBJ)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(EXE) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	rm -f $(OUTDIR)/$(EXE)
	rm -f $(OUTDIR)/noworky