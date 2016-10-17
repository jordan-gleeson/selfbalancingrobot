void init_LCD(void);
void LCD_data_disp(char data);
void LCD_command(char data);
void signon(void);
void LCD_clear(void);
void errorMessage(void);
void successMessage(void);
void printMessage(char message[], char clear);
void LCD_data_push(char data);
// void *LCD_write_string(char *str);

//LCD state machine control definitions
#define LCD_INACTIVE 0x00
#define LCD_DATA 0x01
#define LCD_CONTROL 0x02
#define LCD_READ 0x03
#define LCD_WRITE 0x04
#define LCD_ENABLE_HIGH 0x05
#define LCD_ENABLE_LOW 0x06
#define LCD_CONTRAST_BIT_HIGH 0x07
#define LCD_CONTRAST_BIT_LOW 0x08

#define LCD_D0 0x01 // goes to Port B:1
#define LCD_D1 0x02 // goes to Port B:4
#define LCD_D2 0x04 // goes to Port B:5
#define LCD_D3 0x08 // goes to Port B:6
#define LCD_D4 0x10 // goes to Port F:0
#define LCD_D5 0x20 // goes to Port B:3
#define LCD_D6 0x40 // goes to Port B:2
#define LCD_D7 0x80 // goes to Port H:2
