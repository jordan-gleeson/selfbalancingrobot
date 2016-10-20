#include <motor.h>

void motorInit(void){
	//Setup PWM output ports
	PADD &= ~(0x02); //ENA for motor 1, left, Timer 0
	PCDD &= ~(0x80); //ENB for motor 2, right, Timer 1
	PAAF |= (0x02); //turn on Alternate Function
	PCAF |= (0x80);

	//Setup data direction as output
	PGDD &= ~(0x02); //IN1A, motor 1 A PG1
	PGDD &= ~(0x01); //IN2A, motor 1 B PG0
	PGDD &= ~(0x80); //IN1B, motor 2 A PG7
	PGDD &= ~(0x40); //IN2B, motor 2 B PG6

	//set Timers to PWM mode, no prescaler
	T0CTL1 = 0x83; //PWM_MODE | PRESCALAR_1
	T2CTL1 = 0x83; //PWM_MODE | PRESCALAR_1
	//set reset count value to 0
	T0H = 0;
	T0L = 0;
	T2H = 0;
	T2L = 0;
	//set reload value
	T0RH = (RELOAD >> 8);
	T0RL = (RELOAD & 0x00FF);
	T2RH = (RELOAD >> 8);
	T2RL = (RELOAD & 0x00FF);
	//set PWM duty cycle to 0%
	T0PWMH = 0;
	T0PWML = 0;
	T2PWMH = 0;
	T2PWML = 0;
}


void motor1Direction(char direction)
{
	switch (direction)
	{
		case MOTOR_FORWARD:
			PGOUT &= ~(0x02); //IN1A set to 0
			PGOUT |= 0x01;    //IN2A set to 1
			T0CTL1 |= (1<<7); //enable Timer 0
			break;

		case MOTOR_BACKWARD:
			PGOUT |= 0x02;	  //IN1A set to 1
			PGOUT &= ~(0x01); //IN2A set to 0
			T0CTL1 |= (1<<7); //enable Timer 0
			break;

		case MOTOR_STOP: //active braking
			PGOUT &= ~(0x02); //IN1A set to 0
			PGOUT &= ~(0x01); //IN2A set to 0
			T0CTL1 |= (1<<7); //enable Timer 0
			break;

		case MOTOR_DRIFT: //passive braking
			T0CTL1 &= ~(1<<7); //disable Timer 0
			break;
	}
}

void motor2Direction(char direction){
	switch (direction){
		case MOTOR_FORWARD:
			PGOUT |= 0x80; //IN1B set to 1
			PGOUT &= ~(0x40);  //IN2B set to 0
			T2CTL1 |= (1<<7); //enable Timer 2
			break;

		case MOTOR_BACKWARD:
			PGOUT &= ~(0x80);//IN1B set to 0
			PGOUT |= 0x40; 	//IN2B set to 1
			T2CTL1 |= (1<<7); //enable Timer 2
			break;

		case MOTOR_STOP: //active braking
			PGOUT &= ~(0x80); //IN1A set to 0
			PGOUT &= ~(0x40); //IN2A set to 0
			T2CTL1 |= (1<<7); //enable Timer 2
			break;

		case MOTOR_DRIFT: //passive braking
			T2CTL1 &= ~(1<<7); //disable Timer 2
			break;
	}
}

void motor1Speed(int s){
	if (s>=RELOAD){
		s = RELOAD-1;
	}
	s = RELOAD - s;
	T0PWMH = (s >> 8);
	T0PWML = (s & 0x00FF);
}

void motor2Speed(int s){
	if (s>=RELOAD){
		s = RELOAD-1;
	}
	s = RELOAD - s;
	T2PWMH = (s >> 8);
	T2PWML = (s & 0x00FF);
}

void motorsSet(signed int t, int r, int offsetL, int offsetR){
	int m1;
	int m2;
	m1 = t + r;
	m2 = t - r;
	if(t==0){
		motor1Direction(MOTOR_STOP);
		motor2Direction(MOTOR_STOP);
	} else {
	if(m2 > 0)
	{
		motor2Direction(MOTOR_BACKWARD);
	}
	else
	{
		m2 = - m2;
		motor2Direction(MOTOR_FORWARD);
	}
	if(m1 > 0)
	{
		motor1Direction(MOTOR_BACKWARD);
	}
	else
	{
		m1 = - m1;
		motor1Direction(MOTOR_FORWARD);
	}
	motor2Speed(m2+offsetL);
	motor1Speed(m1+offsetR);
	}
}
