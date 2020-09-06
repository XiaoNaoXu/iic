#include "i2c.h"


u32 buff_size = 10U;


//the time delay function

/**
* @brief sleep function
* @param[in] us: sleep time(microsecond)
* @retval void
*/
void delay_us(uint32_t us)
{
	__IO int i = us * 12;
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

u8 is_i2c_Start(){
	//LED(2);
	if(!I2C_SDA_READ()){
		//LED(2);
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
	delay_us(2);
}
void i2c_SendNAck(void){
	I2C_SDA_1();
	delay_us(2);
	I2C_SDA_0();
}

u8 i2c_WaitAck(){
	__IO u8 re_value;
	if(I2C_SDA_READ() & I2C_SCL_READ()){
		delay_us(2);
		re_value = I2C_SDA_READ();
	}
	return re_value;
}

void I2C_SendByte(u8 *data_byte){
	__IO u8 i = 0, j = 0;
	for(i = 0; i < 8; ++i){
		//
		j = (*data_byte) & 0x80;
		(j) ? (I2C_SDA_1()):(I2C_SDA_0());
		delay_us(2);
		I2C_SCL_1();
		delay_us(2);
		I2C_SCL_0();
		delay_us(2);
		*data_byte <<= 1U;
	}
}

u8 I2C_ReadByte(){
	__IO u8 i, value = 0, j;
	for(i = 0; i < 8; ++i){
		delay_us(2);
		value <<= 1U;
		while(1){
			if(I2C_SCL_READ()){
				LED(0);
				j = I2C_SDA_READ();
			  value |= j;
			  break;
			}
		}
	}
	return value;
}

u32 I2C_Write(u8 *data, u32 data_length){
	u8 *pdata = data;
	__IO u32 len = data_length;
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
	__IO u8 slave_addr = I2C_ReadByte();
	LED(0);
	i2c_SendNAck();
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
	i2c_slave_SCL_Falling_Exti_Enable();
	return 0;
}

void i2c_slave_SCL_Falling_Exti_Enable(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// I2C SCL - PC5
  GPIO_InitStruct.Pin = I2C_SCL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(I2C_SCL_PORT, &GPIO_InitStruct);
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void i2c_slave_SCL_Falling_Exti_Disable(){
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
	HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
	//__HAL_GPIO_EXTI_CLEAR_FALLING_IT(GPIO_PIN_10);
}

void i2c_slave_SDA_GPIO_Init(){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// I2C SDA - PC4
  GPIO_InitStruct.Pin = I2C_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(I2C_SDA_PORT, &GPIO_InitStruct);
}



void LED(u32 up_time){
	LED_ON;
	delay_us(up_time);
	LED_OFF;
}

void callback(){
	LED_ON;
	delay_us(2);
	LED_OFF;
}

