#ifndef JOY_STICK_H_
#define JOY_STICK_H_

#define GPIO_EXPORT "/sys/class/gpio/export"
#define JOY_STICK_CENTER_FILE "/sys/class/gpio/gpio27/value"
#define JOY_STICK_RIGHT_FILE "/sys/class/gpio/gpio47/value"
#define JOY_STICK_LEFT_FILE "/sys/class/gpio/gpio65/value"
#define JOY_STICK_UP_FILE "/sys/class/gpio/gpio26/value"
#define JOY_STICK_DOWN_FILE "/sys/class/gpio/gpio46/value"

typedef struct {
	int JOY_STICK_RIGHT;
	int JOY_STICK_LEFT;
	int JOY_STICK_UP;
	int JOY_STICK_DOWN;
	int JOY_STICK_CENTER;
} joystick;

joystick joyStick;
void JoyStick_init(void);
void JoyStick_cleanup(void);

#endif /* AUDIO_JOY_STICK_H_ */
