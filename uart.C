#include <uart.h>

//Code written by myself for EMB, slightly altered for this project

//Intialise the commuications
void init_coms(void){
	U0BRH = 0x00;
	U0BRL = 0x82;

  //CHANGE PA4&5 are BT RX< TX
	PAAF |= 0x30;
	PADD &= ~0x20;
	PADD |= 0x10;
	PAADDR = 0x00;

	SET_VECTOR(UART0_RX, isr_U1_RX);
	SET_VECTOR(UART0_TX, isr_U1_TX);

	IRQ0 |= 0x04;
	IRQ0ENH &= ~0x18;
	IRQ0ENL |= 0x18;


	U0CTL0 = 0xC0;
	U0CTL1 = 0x00;


}

//Send a given string to the bluetooth module
void sendString(char stringSend[25]){
	char current = 0;
	char size = 0;
	txFinished = 1;
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

//Store and pass the recieved char
char commandF(void){
	char temp = command;
	command = 0;
	return temp;
}

//Recieve
#pragma interrupt
void isr_U1_RX(void){
	DI();
	command = U0RXD;
	IRQ0 &= ~0x40;
	EI();
}

//When send finished
#pragma interrupt
void isr_U1_TX(void){
	txFinished = 1;
}
