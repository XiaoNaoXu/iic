#include "i2c.h"

//the time delay function
void delay()            
{
  uint32_t i;
  for(i=0; i<10000; ++i);
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

u8 i2c_WaitAck(){
	u8 re_value;
	I2C_SDA_1();
	I2C_SCL_1();
	delay();
	re_value = I2C_SDA_READ();
	I2C_SCL_0();
	delay();
	return re_value;
}

void I2C_SendByte(u8 *data_byte){
	u8 i = 0, j = 0;
	for(i = 0; i < 8; ++i){
		//
		j = (*data_byte) & 0x80;
		(j) ? (I2C_SDA_1()):(I2C_SDA_0());
//		if(j){
//			I2C_SDA_1();
//		}
//		else{
//			I2C_SDA_0();
//		}
		delay();
		I2C_SCL_1();
		delay();
		I2C_SCL_0();
		delay();
		*data_byte <<= 1U;
	}
	I2C_SDA_1();
	delay();
}

u8 I2C_ReadByte(){
	u8 i, value = 0;
	for(i = 0; i < 8; ++i){
		value <<= 1U;
		I2C_SCL_1();
		delay();
		value |= I2C_SDA_READ();
		I2C_SCL_0();
		delay();
	}
	return value;
}

u32 I2C_Write(u8 slave_addr, u8 *data, u32 data_length){
	u8 *pdata = data;
	u32 len = data_length;
	i2c_Start();
	I2C_SendByte(&slave_addr);
	while(len--){
		I2C_SendByte(&pdata[len]);
		delay();
	}
	i2c_Stop();
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
