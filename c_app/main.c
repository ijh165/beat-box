#include "devices/display.h"
#include "devices/accelerometer.h"
#include "network/udp_interface.h"
#include "audio/audio_mixer.h"
#include "audio/beat_maker.h"
#include "lib/util.h"

#define HI_HAT_CLOSED "beatbox-wav-files/100054__menegass__gui-drum-ch.wav"

int main()
{
	BeatMaker_init();
	UdpInterface_start();

	UdpInterface_waitUntilStop();
	BeatMaker_cleanup();
	return 0;
}
