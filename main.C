#include <ez8.h>
#include <delay.h>
#include <STRING.H>
#include <LCD.H>
#include <STDIO.H>
#include <motor.h>
#include <uart.h>
#include <i2c.h>

void init_IO(void);

void main(void){
	char reser[];
	char test[5];
	init_IO();
	init_LCD();
	signon();
	// printMessage("This is a test", 1);
	init_coms();
	i2cInit();
	EI();

	rxFlag = 0;
	command = 0;

	// motorsSet(500, 0, 0, 0);
	// motor1Direction(MOTOR_STOP);
	// motor1Speed(1);
	//
	// motor2Direction(MOTOR_STOP);
	// motor2Speed(1);

	while(1){
		// motor1Direction(MOTOR_STOP);
		// motor1Speed(1);
		//
		// motor2Direction(MOTOR_STOP);
		// motor2Speed(1);
		gyro_read();
// 		dly(500);
		// char inChar[25];
		command = commandF();
		if(command != 0){
			//  sprintf(inChar, "%c", command);
			//  sendString(inChar);
			//  printMessage(inChar, 1);
			 command = 0;
		}
	}
}


void init_IO(void){

	//Initialise Port G to be entirely output and with no alternate functions
	// PGAF = 0x00;
	// PGDD = 0x00;
	// PGADDR = 0x00;
	// PGOUT = 0x00;

}
