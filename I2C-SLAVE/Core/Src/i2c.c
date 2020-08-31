#include "i2c.h"

//the time delay function
void delay()            
{
  uint32_t i;
  for(i=0; i<100000; ++i);
}
int i = 0;
u8 is_i2c_Start(){
	for(i=0; i<50000; ++i);
	if(I2C_SCL_READ()){
		for(i=0; i<50250; ++i);
		if(!I2C_SCL_READ()){
			return GPIO_PIN_SET;
		}
	}
	return GPIO_PIN_RESET;
}

u8 is_i2c_Stop(){
	if(I2C_SCL_READ()){
		delay();
		return GPIO_PIN_SET;
	}
	return GPIO_PIN_RESET;
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
	while(len--){
		I2C_SendByte(&pdata[len]);
		delay();
	}
	return 0;
}
u32 I2C_Read(u8 slave_addr, u8 *buff, u8 numByteToRead){
	return 0;
}

void test(){
	I2C_SDA_1();
//	if(I2C_SCL_READ()){
//		LED_ON;
//	delay();
//	LED_OFF;
//	}
	I2C_SDA_0();
//	if(!I2C_SCL_READ()){
//		LED_ON;
//	delay();
//	LED_OFF;
//	}
	
}

void callback(){
	LED_ON;
	for(i=0; i<30000; ++i);
	LED_OFF;
}


