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
	AudioMixer_init();

	wavedata_t* sampleFile = malloc(sizeof(*sampleFile));
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE, sampleFile);

	AudioMixer_queueSound(sampleFile);

	//AudioMixer_freeWaveFileData(&sampleFile);


	while(true);
	AudioMixer_cleanup();


	/*Accelerometer_init();

	Accelerometer_startMotionDetection();
	UdpInterface_start();



	UdpInterface_waitUntilStop();
	Accelerometer_stopMotionDetection();*/



	return 0;
}
