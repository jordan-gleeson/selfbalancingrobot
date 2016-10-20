#include <ez8.h>

void motorInit(void);
void motor1Direction(char direction);
void motor2Direction(char direction);
void motor1Speed(int s);
void motor2Speed(int s);
void motorsSet(signed int t, int r, int offsetL, int offsetR);

#define MOTOR_FORWARD	1
#define MOTOR_BACKWARD 2
#define MOTOR_STOP 3
#define MOTOR_DRIFT	4

int RELOAD = 1000;
