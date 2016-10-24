#include <balancing.h>
#include <i2c.h>
#include <MATH.H>

float complementary(void){
	float thetaGyro = 0;
	float thetaAcc = 0;
	float theta = 0;
	
	//Integrate gyro
	thetaGyro += (float) gyroXValueF() * 0.04;
	
	//Angle from accelerometer based on gravity
	thetaAcc = atan2((float) accXValueF(), (float) (accZValueF()) * (57.32));
	
	theta = 0.98*(thetaGyro + theta) + 0.02 * thetaAcc;
	
	return theta;
}