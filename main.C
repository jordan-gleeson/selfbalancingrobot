#include <ez8.h>
#include <delay.h>
#include <STRING.H>
#include <LCD.H>
#include <STDIO.H>

void init_IO(void);
void isr_U1_RX(void);
void isr_U1_TX(void);
void init_coms(void);
void sendString(char stringSend[25]);


char inChar = 0;
char string[25];
// char current = 0;
char txFinished = 1;
char command = 0;


void main(void){
	char reser[];
	char test[5];
	init_IO();
	init_LCD();
	signon();
	// printMessage("This is a test", 1);
	init_coms();
	EI();

	while(1){
    if(command != 0){
      // txFinished = 1;
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

void init_coms(void){
	U0BRH = 0x00;
	U0BRL = 0x82;

  //CHANGE PA4&5 are BT RX< TX
	PAAF |= 0x30;
	PADD &= ~0x20;
	PADD |= 0x10;
	PAADDR = 0x00;

	DI();

	SET_VECTOR(UART0_RX, isr_U1_RX);
	SET_VECTOR(UART0_TX, isr_U1_TX);

	IRQ0 |= 0x04;
	IRQ0ENH &= ~0x18;
	IRQ0ENL |= 0x18;


	U0CTL0 = 0xC0;
	U0CTL1 = 0x00;

	EI();

	sendString("AT+NAMErJordan");
}

void sendString(char stringSend[25]){
	char current = 0;
	char size = 0;
	size = strlen(stringSend);
	while(1){
		if(txFinished==1)
			{
			if(current < size){
				U0TXD = stringSend[current];
				current++;
				txFinished = 0;
			} else {
				current = 0;
				txFinished = 0;
				break;
			}
		}
	}

}

#pragma interrupt
void isr_U1_RX(void){
	DI();
	command = U0RXD;
	IRQ0 &= ~0x40;
	EI();
}

#pragma interrupt
void isr_U1_TX(void){
	txFinished = 1;
}
