// Code by Brian Bienvenu; Jim Cocks; Ben Giacoppo

#include <eZ8.h>
#include <STDIO.H>
#include <uart.h>
#include "i2c.h"
#include <LCD.h>

#define FAIL (0)
#define PASS (1)
#define AUTO_INC (0x80)
#define READ_BIT (0x01)

//IC2 Interrupt Enable
#define IC2_EI (0x04) //0000 0100

//Z8 definitions I2CCTL
#define I2CEN_MASK	(0x80)
#define START_MASK 	(0x40)
#define STOP_MASK  	(0x20)
#define TXI_MASK	(0x08)
#define NAK_MASK	(0x04)
#define FLUSH_MASK 	(0x02)
//I2CSTAT
#define TDRE_MASK  	(0x80)
#define RDRF_MASK	(0x40)
#define NCKI_MASK	(0x01)


typedef char enum
{
	FAILED = -1,
	BUSY = 0,
	DONE = 1
} readState_t;

typedef char enum
{
	DEV_ADDESS_WRITE,
	DEV_SUB_ADDRESS,
	START_READ,
	DEV_ADDESS_READ,
	READ_BYTE,
	READ_LAST,
	END_CYCLE
} cycle_t;

typedef struct
{
	readState_t reading;
	cycle_t cycle;
	unsigned char deviceAddress;
	unsigned char registerAddress;
	unsigned char bytesToRead;
	unsigned char* storePtr;
} i2cState_t;

static i2cState_t state;
//prototypes
void Si2c_Isr(void);

/********************************************************************************************
Description:
	Initalises the i2c: ports, interrupt, baud rate, and internal state
Notes:
	doesn't perform and transactions
Returns:
	0 <- fail, 1 pass
********************************************************************************************/
void i2cInit(void)
{
	//PA6 and PA7
	//Set Pins to I2C function
	PAAF |= 0xC0; 	//1100 0000

	//set baud rate
	I2CBRH = (I2C_BAUD_DIV >> 8);
	I2CBRL = (I2C_BAUD_DIV & 0xFF);

	//clear txi
	I2CCTL &= ~TXI_MASK;
	//disable I2C control register
	I2CCTL &= ~I2CEN_MASK;
	DI();
	//set isr for I2C
	SET_VECTOR(I2C,Si2c_Isr);
	//set I2C isr to highest priority
	IRQ0ENH |= IC2_EI;
	IRQ0ENL |= IC2_EI;

	//Port D int1_g and int1_A interrupts
	IRQ1ENH |= 0x03;
	IRQ1ENL |= 0x03;
	IRQPS |= 0x03;
	EI();
	//init internal state so it doesn't init to busy or failed
	state.reading = DONE;
}

/********************************************************************************************
Description:
	Performs single byte i2c write
Notes:
	Is blocking
	Doesn't use interrupts
	Don't use while performing a read
Returns:
	1 <= successful, 0 <= failed
********************************************************************************************/
unsigned char i2cWrite(unsigned char deviceAddress, unsigned char registerAddress, unsigned char dataByte)
{
	unsigned char ret;
	ret = FAIL;
	//disable interrupts
	DI();
	//enable I2C control register
	I2CCTL |= I2CEN_MASK;

	//wait till TDRE = 1
	while((I2CSTAT & TDRE_MASK) == 0x00);

	//write device address to I2C data reg
	I2CDATA = deviceAddress;

	//assert start
	I2CCTL |= START_MASK;

	//wait till TDRE = 1
	while((I2CSTAT & TDRE_MASK) == 0x00);

	//write device reg address to I2C data reg
	I2CDATA = registerAddress;

	//wait till TDRE = 1 or error if NCKI bit set
	while((I2CSTAT & (NCKI_MASK | TDRE_MASK)) == 0x00);
	if((I2CSTAT & NCKI_MASK) == 0x00)
	{
		//write dataByte to I2C data reg
		I2CDATA = dataByte;

		//wait till TDRE = 1 or error if NCKI bit set
		while((I2CSTAT & (NCKI_MASK | TDRE_MASK)) == 0x00);
		if((I2CSTAT & NCKI_MASK) == 0x00)
		{
			//set stop bit (if got to here with out error succesful) eg PASS
			I2CCTL |= STOP_MASK;
			ret = PASS;
		}
	}

	if(ret == FAIL)
	{
		//set stop and flush bits
		I2CCTL |= (STOP_MASK | FLUSH_MASK);
	}
	//disable I2C control register
	//I2CCTL &= ~I2CEN_MASK;
	//enable interrupts
	EI();
}

/********************************************************************************************
Description:
	Initiates numToRead i2c reads
Notes:
	uses interrupts
	use i2cIsDataReady() to check when read is complete
Returns:
	none
********************************************************************************************/
void i2cStartRead(unsigned char deviceAddress, unsigned char registerAddress, unsigned char numToRead, unsigned char * dataLoc)
{
	//set reading flag
	state.reading = BUSY;
	//set internals
	state.deviceAddress = deviceAddress;
	state.registerAddress = registerAddress;
	state.bytesToRead = numToRead;
	state.storePtr = dataLoc;
	//set state
	state.cycle = DEV_ADDESS_WRITE;
	//assert IEN
	I2CCTL |= I2CEN_MASK;
	//assert TXI bit
	I2CCTL |= TXI_MASK;
}

void gyro_read(void){
	char display[25];
	i2cStartRead(ACCEL, X_LOW_ADDRESS, 1, acc_data_xl);
	i2cStartRead(ACCEL, X_HIGH_ADDRESS, 1, acc_data_xh);

	i2cStartRead(ACCEL, Y_LOW_ADDRESS, 1, acc_data_yl);
	i2cStartRead(ACCEL, Y_HIGH_ADDRESS, 1, acc_data_yh);

	i2cStartRead(GYRO, RATE_LOW_ADDRESS, 1, gyro_data_l);
	i2cStartRead(GYRO, RATE_HIGH_ADDRESS, 1, gyro_data_h);

	acc_xh = *acc_data_xh;
	acc_xl = *acc_data_xl;
	acc_x = (acc_xl & ((acc_xh<<8) & 0xFF00));

	acc_yh = *acc_data_yh;
	acc_yl = *acc_data_yl;
	acc_y = (acc_yl & ((acc_yh<<8) & 0xFF00));


	gyro_h = *gyro_data_h;
	gyro_l = *gyro_data_l;
	gyro_val = (((gyro_h << 8) & 0xFF00) & gyro_l);

	sprintf(display, "%d", acc_xl);
	sendString(display);
// 	printMessage(display, 1);
	//LCD Display

}



/********************************************************************************************
Description: State machine handles
		Nack interrupt
		Empty Interrupt
		Recieve interrupt

********************************************************************************************/
#pragma interrupt
void Si2c_Isr(void)
{
	//Disable Interrupts
	if(I2CSTAT & NCKI_MASK) //not ack interr
	{
		//in read last byte statwe
		if(state.cycle == READ_LAST)
		{
			//read i2c data
			*(state.storePtr) = I2CDATA;
			//assert stop
			I2CCTL |= STOP_MASK;
			//read cycle completed
			state.cycle = END_CYCLE;
			//clear reading ass samples are now avaliable
			state.reading  = DONE;
			//disable i2c
			I2CCTL &= ~I2CEN_MASK;
		}
		else
		{
			//error
			//set stop and flush bits
			I2CCTL |= (STOP_MASK | FLUSH_MASK);
			//clear txi
			I2CCTL &= ~TXI_MASK;
			//reset state
			state.cycle = END_CYCLE;
			//report failure
			state.reading  = FAILED;
			//disable i2c
			I2CCTL &= ~I2CEN_MASK;
		}
	}
	else if(I2CSTAT & (TDRE_MASK| RDRF_MASK)) //transmit empty interr | receive interrupt (RDRF)
	{
		switch (state.cycle)
		{
			case DEV_ADDESS_WRITE:
				//write device address to data reg
				I2CDATA = state.deviceAddress & ~READ_BIT; //remove read LSB bit here
				//assert start
				I2CCTL |= START_MASK;
				//set to next state
				state.cycle = DEV_SUB_ADDRESS;
				break;
			case DEV_SUB_ADDRESS:
				if(state.bytesToRead > 1)
				{
					//set first bit to 1 to get addres to auto inc
					I2CDATA = AUTO_INC | state.registerAddress;
				}
				else
				{
					//write device sub register to data reg
					I2CDATA = state.registerAddress;
				}
				//need to wait till i2cdata empty
				//set to next state
				state.cycle = START_READ;
				break;
			case START_READ:
				//commence read cycle
				//write device address to data reg
				I2CDATA = state.deviceAddress | READ_BIT;
				//assert start
				I2CCTL |= START_MASK;
				//clear TXI (stops transmit empty interrupts)
				I2CCTL &= ~TXI_MASK;
				if(state.bytesToRead == 1)
				{
					//set state to read last
					state.cycle = READ_LAST;
					//set NACK bit will generate nack interrupt
					I2CCTL |= NAK_MASK;
				}
				else
				{
					//set state to read
					state.cycle = READ_BYTE;
				}

				break;
			case READ_BYTE:
				//read i2c data
				*(state.storePtr) = I2CDATA;
				//move next location
				state.storePtr++;
				state.bytesToRead--;
				if(state.bytesToRead == 1)
				{
					//set NACK bit will generate nack interrupt to complete
					I2CCTL |= NAK_MASK;
					//set state to last read
					state.cycle = READ_LAST;
				}
				break;
			default:
				//should not happen
				break;
		}
	}
}

/********************************************************************************************
Description:
	gives the state of a i2c read
Notes:
	is not valid until a i2cStartRead has not been called
Returns:
	read;
	1 <= done, 0 <= busy , -1 <= failed
********************************************************************************************/
char i2cIsDataReady(void)
{
	return (char) state.reading;
}
