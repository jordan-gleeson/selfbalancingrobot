#include <ez8.h>
#include <delay.h>

void init_IO(void);
void isr_U1_RX(void);
void isr_U1_TX(void);
void sendString(char stringSend[25]);


char inChar = 0;
char string[25];
// char current = 0;
char txFinished = 1;
char command = 0;


void main(void){
	init_IO();
  init_coms();

	while(1){
    if(command != 0){

      txFinished = 1;
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
	U1BRH = 0x00;
	U1BRL = 0x78;

  //CHANGE PA4&5 are BT RX< TX
	PADD &= ~0x20;
	PADD |= 0x10;
	PAAF |= 0x30;
	PAADDR = 0x00;

	DI();

	SET_VECTOR(UART1_RX, isr_U1_RX);
	SET_VECTOR(UART1_TX, isr_U1_TX);

	IRQ2 = IRQ2 & 0xDF;
	IRQ2ENH &= ~0x60;
	IRQ2ENL |= 0x60;


	U1CTL0 = 0xC0;
	U1CTL1 = 0x00;

	EI();

	sendString("Hello Latrobe ");
}

void sendString(char stringSend[25]){
	char current = 0;
	char size = 0;
	size = strlen(stringSend);
	while(1){
		if(txFinished==1)
			{
			if(current < size){
				U1TXD = stringSend[current];
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
	command = U1RXD;
	IRQ2 &= ~0x40;
	EI();
}

#pragma interrupt
void isr_U1_TX(void){
	txFinished = 1;
}
