OUTDIR = $(HOME)/cmpt433/public/myApps
EXE = beatbox

CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -pthread

# Asound process:
# get alibsound2 lib on target:
# 	# apt-get install libasound2
# Copy target's /usr/lib/arm-linux-gnueabihf/libasound.so.2.0.0 
#      to host  ~/public/asound_lib_BBB/libasound.so
# Copy to just base library:

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB 

SRC = main.c devices/accelerometer.c devices/joy_stick.c lib/util.c lib/i2c.c \
	network/udp.c  network/udp_interface.c \
	audio/audio_mixer.c audio/beat_maker.c \
	input/audio_controller.c
	 
OBJ = $(addsuffix .o, $(basename $(SRC)))

all: wav beatbox

beatbox: $(OBJ)
	$(CC) $(CFLAGS) -o $(OUTDIR)/$(EXE) $(OBJ) $(LFLAGS) -lpthread -lasound

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

wav:
	mkdir -p $(OUTDIR)/beatbox-wav-files/
	cp -R beatbox-wav-files/* $(OUTDIR)/beatbox-wav-files/

clean:
	rm -f $(OBJ)
	rm -f $(OUTDIR)/$(EXE)
	