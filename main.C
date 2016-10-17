#include <ez8.h>
#include <delay.h>
#include <STRING.H>
#include <LCD.H>

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
	init_IO();
	init_LCD();
	signon();
	// init_coms();

	while(1){
    // if(command != 0){
    //
    //   txFinished = 1;
    // }
	}
}


void init_IO(void){

	//Initialise Port G to be entirely output and with no alternate functions
	// PGAF = 0x00;
	// PGDD = 0x00;
	// PGADDR = 0x00;
	// PGOUT = 0x00;

}
