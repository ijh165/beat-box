#ifndef JOY_STICK_H_
#define JOY_STICK_H_

#include <stdbool.h>

typedef struct {
	int JOY_STICK_RIGHT;
	int JOY_STICK_LEFT;
	int JOY_STICK_UP;
	int JOY_STICK_DOWN;
	int JOY_STICK_CENTER;
} joyStickState_t;

void JoyStick_init(void);
joyStickState_t JoyStick_getState(void);
void JoyStick_cleanup(void);

#endif /* JOY_STICK_H_ */
