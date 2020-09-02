#include "i2c.h"

//the time delay function

#define ack_TimeOut (u32)100000;

void delay()            
{
  uint32_t i;
  for(i=0; i<100000; ++i);
}

void i2c_Start(){
	I2C_SCL_1();
	I2C_SDA_1();
	delay();
	I2C_SDA_0();
	delay();
	I2C_SCL_0();
	delay();
}

void i2c_Stop(){
	I2C_SDA_0();
	I2C_SCL_1();
	delay();
	I2C_SDA_1();
}

void i2c_SendAck(void){
	I2C_SDA_0();
	delay();
	I2C_SCL_1();
	delay();
	I2C_SCL_0();
	delay();
	I2C_SDA_1();
}
void i2c_SendNAck(void){
	I2C_SDA_1();
	delay();
	I2C_SCL_1();
	delay();
	I2C_SCL_0();
}

u8 i2c_WaitAck(){
	u32 timeout = ack_TimeOut;
	u8 re_value;
	I2C_SCL_0();
	delay();
	I2C_SCL_1();
	re_value = I2C_SDA_READ();
	delay();
	I2C_SCL_0();
	return re_value;
}

void I2C_SendByte(u8 data_byte){
	u8 i = 0, j = 0;
	for(i = 0; i < 8; ++i){
		//
		j = (data_byte) & 0x80;
		(j) ? (I2C_SDA_1()):(I2C_SDA_0());
		delay();
		I2C_SCL_1();
		delay();
		I2C_SCL_0();
		delay();
		data_byte <<= 1U;
	}
	I2C_SDA_1();
}

u8 I2C_ReadByte(){
	u8 i, value = 0;
	for(i = 0; i < 8; ++i){
		value <<= 1U;
		if(I2C_SCL_READ()){
			delay();
		  if(I2C_SCL_READ())
			{	
				delay();
				value |= I2C_SDA_READ();
			}
		}
	}
	return value;
}

u32 I2C_Write(u8 slave_addr, u8 *data, u32 data_length){
	u8 *pdata = data;
	u32 len = data_length;
	i2c_Start();
	I2C_SendByte(slave_addr);
//	if(!i2c_WaitAck()){
//		//	while(len--){
////		I2C_SendByte(&pdata[len]);
////		delay();
////	}
//	}
//	i2c_Stop();		
	return 0;
}
u32 I2C_Read(u8 slave_addr, u8 *buff, u8 numByteToRead){
	return 0;
}

void test(void){
	I2C_SCL_1();
	u8 i = I2C_SCL_READ();
	if(I2C_SCL_READ()){
		LED_ON;
		delay();
	}
	delay();
	I2C_SCL_0();
	if(!I2C_SCL_READ()){
		LED_OFF;
		delay();
	}
}
