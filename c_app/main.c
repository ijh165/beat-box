#include "audio/audio_mixer.h"
#include "audio/beat_maker.h"
#include "input/audio_controller.h"
#include "lib/util.h"
#include "network/udp_interface.h"

#include <stdio.h>

#define HI_HAT_CLOSED "beatbox-wav-files/100054__menegass__gui-drum-ch.wav"

int main()
{
	BeatMaker_init();
	AudioController_init();
	UdpInterface_start();

	UdpInterface_waitUntilStop();
	AudioController_cleanup();
	BeatMaker_cleanup();
	return 0;
}
