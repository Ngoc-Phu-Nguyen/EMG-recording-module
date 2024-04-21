/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include<stm8s.h>
#include<stm8s103f3p.h>
#include<mcp3421a.h>



struct i2c_struct {
	char	SlaveAddress;
	char*	masterBuffer;
	char	masterTransactionLength;
	char	buffer_index;
	char  reg;
	char	readwrite;
	char  slave_config;
}i2c;

struct MCP3421_conf{
	char mode;
	char SPS;
	char GAIN;
}conf;

void config_MCP3421(MCP3421_MODE_TypeDef mode,
										MCP3421_SPS_TypeDef SPS, 
										MCP3421_GAIN_TypeDef GAIN)
{
	conf.mode = mode;
	conf.SPS = SPS;
	conf.GAIN = GAIN;
}

uint8_t data[3];
char index = 0;
char MCP3421_Conf;
unsigned short value;
char check,i;


void UART_TX(unsigned char val){
	while (!(UART1->SR & UART1_SR_TXE));
	UART1->DR = val;
}

unsigned char UART_RX(void){
	while (!(UART1->SR & UART1_SR_RXNE));
	return UART1->DR;
}

void delay(unsigned long count){
	while (count--)
	nop();
}

void i2c_Init(void){
  /*------------------------- I2C FREQ Configuration ------------------------*/
  /* Clear frequency bits */
  I2C->FREQR &= (uint8_t)(~I2C_FREQR_FREQ);
  /* Write new value */
  I2C->FREQR |= 10;//InputClockFrequencyMHz
	
  /*--------------------------- I2C CCR Configuration ------------------------*/
	I2C->TRISER = 11;

	/* Write CCR with new calculated value */
  I2C->CCRL = 0x32;
  I2C->CCRH = 0x00;
	
	I2C->OARH = 0b01000000;               // This bit must be set by software
	I2C->ITR |= I2C_ITR_ITEVTEN;           //Event  Enables :SB, ADDR, ADD10, STOPF, BTF, WUFH
	I2C->ITR |= I2C_ITR_ITBUFEN; 
	_asm("rim");
	/*Enable I2C peripheral*/
	I2C->CR1 |= I2C_CR1_PE;
	
}


void I2C_Read(char slaveAddress, char* buffer, char count)
{
	i2c.readwrite |= 0x01;
	i2c.SlaveAddress = slaveAddress;
	i2c.buffer_index = 0;
	i2c.masterBuffer = buffer;
	i2c.masterTransactionLength = count;	
	//wait for the Bus to get Free to avoid collisions
	while((I2C->SR3 & I2C_SR3_BUSY) == I2C_SR3_BUSY);
	
	I2C->CR2 |= I2C_CR2_ACK;	//Acknowledge Enable : Acknowledge returned after a byte is received (matched address or data)

	I2C->CR2 |= I2C_CR2_START;	//Launch the process
	delay(100);
}

void I2C_Write(char slaveAddress, char config)
{
	i2c.readwrite &= 0x00;
	i2c.SlaveAddress = slaveAddress;
	i2c.slave_config = config;
	
	//wait for the Bus to get Free to avoid collisions
	while((I2C->SR3 & I2C_SR3_BUSY) == I2C_SR3_BUSY);
	
	I2C->CR2 |= I2C_CR2_ACK;	//Acknowledge Enable : Acknowledge returned after a byte is received (matched address or data)
	//The start will enter the Master Mode (when the Busy bit is cleared)
	//If already in Master Mode, then ReStart will be generated at the end of the current transfer
	I2C->CR2 |= I2C_CR2_START;	//Launch the process
	delay(100);
}

void auto_GAIN(unsigned short check){
	if((conf.SPS == SPS240_12bit) &&(conf.GAIN > 0)){
		if (check >= (2047)){
			conf.GAIN--;
			I2C_Write(MCP3421,(conf.mode|conf.SPS|conf.GAIN));
		}
	}
}

main()
{
	CLK_CKDIVR = 0x00;  //set clock 16MHZ
	CLK_PCKENR1 = 0xFF; // set all peripherals
	

	GPIO_Init(GPIOB , GPIO_PIN_4 , GPIO_MODE_IN_PU_NO_IT);   
	GPIO_Init(GPIOB , GPIO_PIN_5 , GPIO_MODE_IN_PU_NO_IT);
	//ITC_SPR5 = 0b01000000;
	
	UART1->CR3 |= UART1_STOPBITS_1; //stop bit
	UART1->BRR2 = 0x03; UART1->BRR1 = 0x68; // 0x0683 coded funky way (see ref manual) 9600
	UART1->CR2 |= (uint8_t)UART1_CR2_TEN;//enable TX 
	
	I2C_DeInit();
	i2c_Init();
	config_MCP3421(CONTI_CONVERTION_MODE, SPS240_12bit, GAIN1);
	check = conf.mode|conf.SPS|conf.GAIN;
	I2C_Write(MCP3421,(conf.mode|conf.SPS|conf.GAIN));
	//delay(100);
while (1){
	
	I2C_Read(MCP3421,data,3);
	value = ((data[0]<<8) | data[1]);
	//auto_GAIN(value);
	UART_TX(data[0]);
	delay(300);
	UART_TX(data[1]);
	delay(300);
	UART_TX(data[2]);
	delay(300);
}
}

@far @interrupt void EXTI_I2C(void){
	if (I2C->SR1 & I2C_SR1_SB){
		i2c.reg = I2C->SR1;
		I2C->DR = (i2c.SlaveAddress) | i2c.readwrite;
	}	
	/*else if(I2C->SR1&I2C_SR1_ADDR){
		i2c.reg = I2C->SR1;
		i2c.reg = I2C->SR3;
		i2c.buffer_index = 0;
	}*/
	else if (I2C->SR3 & I2C_SR3_TRA)
	{
		if (I2C->SR1 & I2C_SR1_TXE)
		{
			I2C->DR = i2c.slave_config;
			//while(!(I2C->SR1&I2C_SR1_BTF == I2C_SR1_BTF));
			I2C->CR2 |= I2C_CR2_STOP; //generate STOP
		}
	}
	else 
	{
		if(I2C->SR1&I2C_SR1_RXNE)
		{
			char Reg = I2C->DR;
			if(i2c.buffer_index < i2c.masterTransactionLength)
				{
					i2c.masterBuffer[i2c.buffer_index] = Reg;
				}
			if(i2c.buffer_index == (i2c.masterTransactionLength - 1))//The last byte is received
				{
						I2C->CR2 &= (~I2C_CR2_ACK);//Clear ACK 
						I2C->CR2 |= I2C_CR2_STOP; //generate STOP
				}
			i2c.buffer_index++;
		}
	}
	//change one more time
}
