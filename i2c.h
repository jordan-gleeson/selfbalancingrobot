// Code by Brian Bienvenu; Jim Cocks; Ben Giacoppo

#ifndef I2C__H
#define I2C__H

//Initalises I2C
void i2cInit(void);

void gyro_read(void);

//Write a single byte must not use while performing read
unsigned char i2cWrite(unsigned char dev_adrs, unsigned char reg_adrs, unsigned char w_data);

//read set umber of bytes, must not be used seperatly from Write
//use i2cIsDataReady to check when read is complete
void i2cStartRead(unsigned char deviceAddress, unsigned char registerAddress, unsigned char numToRead, unsigned char * dataLoc);

char i2cIsDataReady(void);   //returns 1 when read complete, -1 fail, 0 still doing

//Acclerometer Address
#define ACCEL (0x30) //0011 0000 **** CHANGED
//Gyro Address
#define GYRO  (0xD4) //1101 0100 **** CHANGED

//Baud Rate
#define SYS_CLK 20000000
#define I2C_BAUD (400000) //Note:20mhz clk, max baud 400k (need fast mode) or 100k for normal
#define I2C_BAUD_DIV (SYS_CLK / (4* I2C_BAUD))

/**********************************
Acc Settings
	1.3khz output data rate
	Normal Mode
	X enabled
	Y enabled
	Big Endian
	+- 2G range (00)
	High Resolution Mode Enabled

Note: rest default
************************************/
#define REG1_A_DATA		(0x97) // 1001 0111
#define REG1_A_ADDRESS 	(0x20)

#define REG4_A_DATA		(0x48) // 0100 1000
#define REG4_A_ADDRESS 	(0x23)

#define X_LOW_ADDRESS		(0x28)
#define X_HIGH_ADDRESS		(0x29)
#define Y_LOW_ADDRESS		(0x2A)
#define Y_HIGH_ADDRESS		(0x2B)


/**********************************
Gyro Settings
	output data rate (760hz) 100hz cut off
	normal mode enabled
	Yaxis enabled
	continuous update
	Big Endian
	250dps range (00)
	disbale spi
Note: rest default
************************************/
#define REG1_G_DATA		(0xFF) //1111 1010
#define REG1_G_ADDRESS (0x20)

#define REG4_G_DATA 	(0x40)//0100 0000
#define REG4_G_ADDRESS 	(0x23)

#define REG5_G_DATA 	(0x10)//0001 0000
#define REG5_G_ADDRESS 	(0x24)

//#define RATE_LOW_ADDRESS	(0x2A)
//#define RATE_HIGH_ADDRESS	(0x2B)
#define RATE_LOW_ADDRESS	(0x28)
#define RATE_HIGH_ADDRESS	(0x29)

#endif //I2C__H


unsigned char * acc_data_xl;
unsigned char * acc_data_xh;

unsigned char * acc_data_yl;
unsigned char * acc_data_yh;

unsigned char * gyro_data_l;
unsigned char * gyro_data_h;

char acc_xh;
char acc_xl;
char acc_yh;
char acc_yl;
char acc_y;
char acc_x;
char gyro_h;
char gyro_l;
int gyro_val;
