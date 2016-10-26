#include <ez8.h>
#include <delay.h>
#include <STRING.H>
#include <LCD.H>
#include <STDIO.H>
#include <motor.h>
#include <uart.h>
#include <i2c.h>
#include <main.h>

void main(void){
	char reser[];
	char test[5];
	char inChar[25];
	char tmp;

	int i = 0;
// 	init_IO();
	i2cInit();
	init_LCD();
	signon();
// 	printMessage("This is a test", 1);
	init_coms();
// printMessage("This is a test02", 1);
	motorInit();
// 	printMessage("This is a test03", 1);
	EI();
	sendString("AT+NAMErJordan");
// 	printMessage("This is a test04", 1);
	rxFlag = 0;
	command = 0;

// 	sendString("got here");

// 	printMessage("This is a test05", 1);

	while(1){
		i++;
		// speed = (float) accXValueF() * 0.03;
		// motorsSet(speed, 0, loffset, roffset);
// 		mems_read();
// 		dly(50);
//
		//
		command = commandF();
		if(command != 0){
			sprintf(inChar, "Command: %c \r\n", command);
			sendString(inChar);
		}
		if(command == 'w'){
			//  sprintf(inChar, "%c", command);
			//  sendString(inChar);
			//  printMessage(inChar, 1);
			speed = speed + 1;
			command = 0;
			motorsSet(speed, r, roffset, loffset);
		} else if(command == 's'){
			speed = speed - 1;
			command = 0;
			motorsSet(speed, r, roffset, loffset);
		} else if(command == 'm'){
			tmp = mode(1);
		}
		//
		if(i>30000){
			// sprintf(inChar, "%d", accXValueF());
			// printMessage(inChar, 1);
			tmp = mode(0);
			i=0;
		}
// 		dly(100);

	}
}


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
