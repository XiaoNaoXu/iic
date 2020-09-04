#include "i2c.h"

//the time delay function

/**
* @brief sleep function
* @param[in] us: sleep time(microsecond)
* @retval void
*/
void delay_us(uint32_t us)
{
	__IO int i = us;
	while(i--);
}


/**
* @brief sleep function
* @param[in] us: sleep time(millisecond)
* @retval void
*/
void delay_ms(uint32_t ms)
{
	while(ms--) {
		delay_us(909);
	}
}


/**
* @brief Start signal function
* @retval void
*/
void i2c_Start(){
	I2C_SCL_1();
	I2C_SDA_1();
	delay_us(2);
	I2C_SDA_0();
	I2C_SCL_0();
	delay_us(2);
}

/**
* @brief Start stop function
* @retval void
*/
void i2c_Stop(){
	I2C_SDA_0();
	I2C_SCL_1();
	I2C_SDA_1();
}

/**
* @brief Send a acknowledge
* @retval void
*/
void i2c_SendAck(void){
	I2C_SDA_0();
	I2C_SCL_1();
	delay_us(2);
	I2C_SCL_0();
	I2C_SDA_1();
}

/**
* @brief Send a not acknowledge
* @retval void
*/
void i2c_SendNAck(void){
	I2C_SDA_1();
	I2C_SCL_1();
	delay_us(2);
	I2C_SCL_0();
}

/**
* @brief wiat slave or master return ack
* @param[in] re_value: A ack or a Nack
* @retval uint8_t:acknowledge
*/
u8 i2c_WaitAck(){
	u8 re_value;
	I2C_SCL_0();
	delay_us(2);
	I2C_SCL_1();
	re_value = I2C_SDA_READ();
	delay_us(2);
	I2C_SCL_0();
	return re_value;
}

/**
* @brief send a byte
* @param[in] i: Control loop variable
* @retval void
*/
void I2C_SendByte(u8 data_byte){
	__IO u8 i = 0, j = 0;
	for(i = 0; i < 8; ++i){
		//
		j = (data_byte) & 0x80;
		(j) ? (I2C_SDA_1()):(I2C_SDA_0());
		I2C_SCL_1();
		delay_us(2);
		I2C_SCL_0();
		I2C_SDA_0();
		data_byte <<= 1U;
	}
}

u8 I2C_ReadByte(){
	__IO u8 i, value = 0;
	for(i = 0; i < 8; ++i){
		value <<= 1U;
		if(I2C_SCL_READ()){
			delay_us(2);
		  if(I2C_SCL_READ())
			{	
				delay_us(2);
				value |= I2C_SDA_READ();
			}
		}
	}
	return value;
}

u32 I2C_Write(u8 slave_addr, u8 *data, u32 data_length){
//	u8 *pdata = data;
//	u32 len = data_length;
	i2c_Start();
	I2C_SendByte(slave_addr);
//	if(!i2c_WaitAck()){
//		//	while(len--){
////		I2C_SendByte(&pdata[len]);
////		delay_us(2);
////	}
//	}
	i2c_Stop();		
	return 0;
}
u32 I2C_Read(u8 slave_addr, u8 *buff, u8 numByteToRead){
	return 0;
}

void test(void){
	I2C_SCL_1();
	__IO u8 i = I2C_SCL_READ();
	if(I2C_SCL_READ()){
		LED_ON;
		delay_us(2);
	}
	delay_us(2);
	I2C_SCL_0();
	if(!I2C_SCL_READ()){
		LED_OFF;
		delay_us(2);
	}
}

void LED(u32 up_time){
	LED_ON;
	delay_us(up_time);
	LED_OFF;
}
