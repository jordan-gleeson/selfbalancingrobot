#include <ez8.h>
#include <delay.h>
#include <STRING.H>
#include <LCD.H>
#include <STDIO.H>
#include <motor.h>
#include <uart.h>
#include <i2c.h>

#define loffset 405
#define roffset 407

void init_IO(void);

void main(void){
	char reser[];
	char test[5];
	char inChar[25];
	int speed = 0;
	DI();
	init_IO();
	i2cInit();
	init_LCD();
	signon();
	// printMessage("This is a test", 1);
	init_coms();

	motorInit();
	EI();
	sendString("AT+NAMErJordan");

	rxFlag = 0;
	command = 0;
	
	sendString("got here");
	
	printMessage("This is a test", 1);

	while(1){

		mems_read();
		dly(50);
		
		
// 		command = commandF();
// 		if(command == 'w'){
// 			//  sprintf(inChar, "%c", command);
// 			//  sendString(inChar);
// 			//  printMessage(inChar, 1);
// 			speed = speed + 1;
// 			command = 0;
// 			sprintf(inChar, "%d", speed);
// 			printMessage(inChar, 1);
// 			motorsSet(speed, 0, loffset, roffset);
// 		} else if(command == 's'){
// 			speed = speed - 1;
// 			command = 0;
// 			sprintf(inChar, "%d", speed);
// 			printMessage(inChar, 1);
// 			motorsSet(speed, 0, loffset, roffset);
// 		}
		
	}
}


void init_IO(void){

	//Initialise Port G to be entirely output and with no alternate functions
	// PGAF = 0x00;
	// PGDD = 0x00;
	// PGADDR = 0x00;
	// PGOUT = 0x00;

}
