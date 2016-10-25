#include <ez8.h>
#include <delay.h>
#include <STRING.H>
#include <LCD.H>
#include <STDIO.H>
#include <motor.h>
#include <uart.h>
#include <i2c.h>
#include <main.h>


//The main function that ran on the robot
void main(void){
	char inChar[25];
	char tmp;
	int i;

	//Initialise all of the different components
	i2cInit();
	init_LCD();
	//Confirm on the LCD that LCD initialisation was successful
	signon();
	init_coms();
	motorInit();
	//Global Enable interrupts
	EI();
	//Set the name of the bluetooth module
	sendString("AT+NAMErJordan");

	//Set values for global variables
	rxFlag = 0;
	command = 0;





	while(1){
		i++;

		//This switching code recieves a command from serial and will change what the output on the LCD is accordingly and either drive or not drive the motors
		command = commandF();
		if(command != 0){
			sprintf(inChar, "Command: %c \r\n", command);
			sendString(inChar);
		}
		if(command == 'w'){
			speed = speed + 1;
			command = 0;
			motorsSet(speed, 0, loffset, roffset);
		} else if(command == 's'){
			speed = speed - 1;
			command = 0;
			motorsSet(speed, 0, loffset, roffset);
		} else if(command == 'm'){
			tmp = mode(1);
		}

		//Only poll the sensors every so often
		if(i>15000){
			tmp = mode(0);
			i=0;
		}
	}
}

//Tells the other files and functions what needs to be displayed and read
char mode(char change){
	static char m = 1;
	char inChar[25];
	static char tmpSpeed = 0;

	if(m){
		if(tmpSpeed != speed){
			sprintf(inChar, "%d", speed);
			printMessage(inChar, 1);
		}
		tmpSpeed = speed;
		printI2Cq = 0;
	} else {
		printI2Cq = 1;
		mems_read();
	}

	if(change){
		m ^= 0x01;
		return m;
	} else {
		return m;
	}

}


char printI2CqF(void){
	return printI2Cq;
}
