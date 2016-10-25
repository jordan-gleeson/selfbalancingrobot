#include <ez8.h>
#include <LCD.H>
#include <delay.h>
#include <STRING.H>

//Code by myself from EMB
//Altered for new pins
void init_LCD(void){
	dly(40);

	//LCD pins
	//PB1 = 0
	//PB4 = 1
	//PB5 = 2
	//PB6 = 3
	//PB7 = 4
	//PB3 = 5
	//PB2 = 6
	//PH2 = 7

	//Initialse data bit ports
	PBAF &= ~0xFE;
	PBDD &= ~0xFE;
	PBADDR &= ~0xFE;
	PBOUT &= ~0xFE;
	PHAF &= ~0x04;
	PHDD &= ~0x04;
	PHADDR &= ~0x04;
	PHOUT &= ~0x04;

	//PB0 = LCD_E
	//PH0 = LCD_RS
	//PH1 = LCD_RW
	//PC1 = Backlight

	//Initialise Port D
	PBAF &= ~0x01;
	PBDD &= ~0x01;
	PBADDR &= ~0x01;
	PBOUT &= ~0x01;
	PCAF &= ~0x02;
	PCDD &= ~0x02;
	PCADDR &= ~0x02;
	PHAF &= ~0x03;
	PHDD &= ~0x03;
	PHADDR &= ~0x03;
	PHOUT &= ~0x03;

	//LCD Initialisation sequence
	LCD_command(0x3C);
	dly(1);
	LCD_command(0x0F);
	dly(1);
	LCD_command(0x01);
	dly(3);
	LCD_command(0x06);

	//Turn on the backlight
	PCOUT |= 0x02;

	LCD_clear();

}

//Code by myself from EMB
void set_LCD_control_bus(char data){
	char temp;
	switch(data){
		case LCD_DATA:
			PHOUT |= 0x01;
		break;//set RS bit to 1
		case LCD_CONTROL:
			PHOUT &= ~0x01;
		break; //set RS bit to 0;
		case LCD_READ:
			PHOUT |= 0x02;
		break; //set RW bit to 1
		case LCD_WRITE:
			PHOUT &= ~0x02;
		break; //set RW bit to 0;
		case LCD_ENABLE_HIGH:
			PBOUT |= 0x01;
		break; //set E clk bit to 1
		case LCD_ENABLE_LOW:
			PBOUT &= ~0x01;
		break; //set E clk bit to 0
		case LCD_INACTIVE:

			//Set RS and RW to 1
			PHOUT |= 0x03;

			//This code is from John Hodder
			PHOUT |= 0x01;
			PHOUT &= ~0x02;
		break;
		default:

		break;
	}
}

//Push variable data to the pins of the LCD
void LCD_data_push(char data){
		//This code is from John Hodder
			PBOUT =
		 ((data & LCD_D0) << 1) |
		 ((data & LCD_D1) << 3) |
		 ((data & LCD_D2) << 3) |
		 ((data & LCD_D3) << 3) |
		 ((data & LCD_D4) << 3) |
		 ((data & LCD_D5) >> 2) |
		 ((data & LCD_D6) >> 4) |
		 (PBOUT & 0x01); //Mask Enable bit

		PHOUT = ((data & LCD_D7) >> 5) | (PHOUT & ~0x04); //Shifting data bit 7 down to bit 2 for PHOUT to send PH2 correctly. Masking bit the lower bits so they are unchanged.
}

//Used for sending commands to the LCD_D
//Code by myself from EMB
void LCD_command(char data) {
// Set_LCD_Control (LCD_INACTIVE); // set everything to initial state
	set_LCD_control_bus(LCD_INACTIVE);
// Set_LCD_Control (???); // set R/W to the write mode
	set_LCD_control_bus(LCD_WRITE);
// Set_LCD_Control (???); // set RS for LCD data
	set_LCD_control_bus(LCD_CONTROL);
//  ??? ; // wait for enable pulse width (250ns) or more (1ms)
	dly(1);
//  ??? ; // set enable to high
	set_LCD_control_bus(LCD_ENABLE_HIGH);
//  LCD_data_push = data; // make a valid data
	LCD_data_push(data);
//  ??? ; // wait for enable pulse width (250ns) or more (1ms)
	dly(1);
//  ??? ; // set enable to low to latch the data
	set_LCD_control_bus(LCD_ENABLE_LOW);
//  ??? ; // A delay of ??? after each LCD write ensures that LCD executes
	dly(1);
//  required instruction before the next is sent (can also check busy flag)
}

//Display data variable on the LCD
//Code by myself from EMB
void LCD_data_disp(char data) {
// Set_LCD_Control (LCD_INACTIVE); // set everything to initial state
	set_LCD_control_bus(LCD_INACTIVE);
// Set_LCD_Control (???); // set R/W to the write mode
	set_LCD_control_bus(LCD_WRITE);
// Set_LCD_Control (???); // set RS for LCD data
	set_LCD_control_bus(LCD_DATA);
//  ??? ; // wait for enable pulse width (250ns) or more (1ms)
	dly(1);
//  ??? ; // set enable to high
	set_LCD_control_bus(LCD_ENABLE_HIGH);
//  PAOUT = data; // make a valid data
	LCD_data_push(data);
//  ??? ; // wait for enable pulse width (250ns) or more (1ms)
	dly(1);
//  ??? ; // set enable to low to latch the data
	set_LCD_control_bus(LCD_ENABLE_LOW);
//  ??? ; // A delay of ??? after each LCD write ensures that LCD executes
	dly(1);
//  required instruction before the next is sent (can also check busy flag)
}

//print that the LCD has initialised correctly
//Code by myself from EMB
void signon(void){
	char i;

	//Define the string to be printed
	char init_string[] = {"LCD Initialised!"};

	//Loop through each element in the given string and write it to the LCD
	for(i=0;i<strlen(init_string);i++){
		LCD_data_disp(init_string[i]);
	}

	//Delay for a second
	dly(1000);

}

//Clears the LCD
//Code by myself from EMB
void LCD_clear(void){
	LCD_command(0x01);
	dly(2);
}

//Print the passed string to the LCD
//Code by myself from EMB
void printMessage(char message[], char clear){
	char i;

	if(clear){
		LCD_clear();
	}

	//Loop through each element in the given string and write it to the LCD
	for(i=0;i<strlen(message);i++){
		LCD_data_disp(message[i]);
	}

	//Delay for a bit
	dly(2);
}
