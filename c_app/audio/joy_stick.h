/*
 * joy_stick.h
 *
 *  Created on: Oct 28, 2017
 *      Author: zyzhou
 */

#ifndef AUDIO_JOY_STICK_H_
#define AUDIO_JOY_STICK_H_

#define GPIO_EXPORT "/sys/class/gpio/export"


typedef struct {
	int JOY_STICK_RIGHT;
	int JOY_STICK_LEFT;
	int JOY_STICK_UP;
	int JOY_STICK_DOWN;
	int JOY_STICK_CENTER;
} joystick;

void JoyStick_init();
void JoyStick_changeTempo();
void JoyStick_changeBeat();
void JoyStick_changeVolume(int change);

#endif /* AUDIO_JOY_STICK_H_ */
