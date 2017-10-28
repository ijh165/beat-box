#include "devices/display.h"
#include "devices/accelerometer.h"
#include "network/udp_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "audio/audio_mixer.h"

#define SOURCE_FILE "beatbox-wav-files/100060__menegass__gui-drum-splash-hard.wav"

int main()
{
	Accelerometer_init();
	AudioMixer_init();

	wavedata_t* sampleFile = malloc(sizeof(*sampleFile));
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE, sampleFile);

	AudioMixer_queueSound(sampleFile);

	Accelerometer_startMotionDetection();
	UdpInterface_start();

	UdpInterface_waitUntilStop();
	Accelerometer_stopMotionDetection();

	AudioMixer_cleanup();
	AudioMixer_freeWaveFileData(sampleFile);

	return 0;
}
