#include "i2c.h"


u32 buff_size = 10U;
u8 self_addr_read = 0xA0;
u8 self_addr_write = 0xA1;
int i = 0;

//the time delay function
void delay()            
{
  uint32_t i;
  for(i=0; i<100000; ++i);
}

u8 is_i2c_Start(){
	for(i=0; i<300; ++i);
	if(I2C_SCL_READ() && (!I2C_SDA_READ())){
		return GPIO_PIN_SET;
	}
	return GPIO_PIN_RESET;
}

u8 is_i2c_Stop(){
	if(I2C_SCL_READ()){
		return GPIO_PIN_SET;
	}
	return GPIO_PIN_RESET;
}

void i2c_SendAck(void){
	I2C_SDA_0();
	delay();
}
void i2c_SendNAck(void){
	I2C_SDA_1();
	delay();
}

u8 i2c_WaitAck(){
	u8 re_value;
	if(I2C_SDA_READ() & I2C_SCL_READ()){
		delay();
		re_value = I2C_SDA_READ();
	}
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
}

u8 I2C_ReadByte(){
	u8 i, value = 0;
	for(i = 0; i < 8; ++i){
		value <<= 1U;
		delay();
		if(I2C_SCL_READ()){
			if(I2C_SDA_READ()){
				value |= 1U;
			}		
		}
	}
	return value;
}

u32 I2C_Write(u8 *data, u32 data_length){
	u8 *pdata = data;
	u32 len = data_length;
	while(len--){
		I2C_SendByte(&pdata[len]);
		if(i2c_WaitAck()){
			continue;
		}
		else{
			break;
		}
	}
	return 0;
}
u32 I2C_Read(){
	u8 *rdata = (u8 *)malloc(sizeof(u8) * buff_size);
	u8 wdata[] = {0, 1, 2, 3};
	u32 i = 0;
	u8 slave_addr = I2C_ReadByte();
	LED_con(slave_addr);
	if(slave_addr == self_addr_write){
		I2C_Write(wdata, 4);
	}
	else if(slave_addr == self_addr_read){
		while(!is_i2c_Stop()){
			rdata[++i] = I2C_ReadByte();
			if(i == buff_size){
				i2c_SendNAck();
				break;
			}
			i2c_SendAck();
		}
	}
	free(rdata);
	LED_con(0x44);
	return 0;
}

void i2c_Gpio10_Falling_Exti_Enable(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void i2c_Gpio10_Falling_Exti_Disable(){
	//HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
	//__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
	//__HAL_GPIO_EXTI_CLEAR_FALLING_IT(GPIO_PIN_10);
	
}

void i2c_Gpio10_Rising_Exti_Enable(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void i2c_Gpio10_Rising_Exti_Disable(){
	
  //__HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_PIN_10);
	
}

void callback(){
	LED_ON;
	for(i = 0; i < 3000; ++i);
	LED_OFF;
}

void LED_con(u8 var){
	u32 i = 0, j;
	for(i = 0; i < 8; ++i){
		j = ((0x80) & var);
		if(j == 0x80){
			callback();
		}
		var <<= 1U;
	}
}

