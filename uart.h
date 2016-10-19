#include <ez8.h>
#include <STRING.H>

void isr_U1_RX(void);
void isr_U1_TX(void);
void init_coms(void);
void sendString(char stringSend[25]);
char commandF(void);

char inChar = 0;
char string[25];
// char current = 0;
char txFinished = 1;
char command = 0;
char rxFlag = 0;
