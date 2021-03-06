#ifndef I2C_H
#define I2C_H

#include "stm32g0xx_hal.h"
#include <stdlib.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

//LED
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOF
#define LED_GREEN_Pin GPIO_PIN_5
#define LED_GREEN_GPIO_Port GPIOA

//slave address
#define I2C_WRITE_ADDRESS 0xA0
#define I2C_READ_ADDRESS 0xA1
#define self_addr_read 0xA0
#define self_addr_write 0xA1

//I2C port select
#define I2C_PORT_GPIO GPIOC
#define I2C_SDA_PORT GPIOC
#define I2C_SCL_PORT GPIOC
#define I2C_SDA_PIN GPIO_PIN_4
#define I2C_SCL_PIN GPIO_PIN_5

//SCL
//#define I2C_SCL_1() I2C_PORT_GPIO->BSRR = I2C_SCL_PIN											  // SCL = 1 
//#define I2C_SCL_0() I2C_PORT_GPIO->BSRR = (uint32_t)I2C_SCL_PIN << 16U  		// SCL = 0 
#define I2C_SCL_1() HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET)
#define I2C_SCL_0() HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_RESET)

//SDA
//#define I2C_SDA_1() I2C_PORT_GPIO->BSRR = I2C_SDA_PIN												// SDA = 1
//#define I2C_SDA_0() I2C_PORT_GPIO->BSRR = (uint32_t)I2C_SDA_PIN << 16U			// SDA = 0
#define I2C_SDA_1() HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET)
#define I2C_SDA_0() HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_RESET)

//#define I2C_SDA_READ() (I2C_PORT_GPIO->IDR & I2C_SDA_PIN)									// read SDA status
#define I2C_SDA_READ() HAL_GPIO_ReadPin(I2C_SDA_PORT, I2C_SDA_PIN)
//#define I2C_SCL_READ() (I2C_PORT_GPIO->IDR & I2C_SCL_PIN)									// read SCL status
#define I2C_SCL_READ() HAL_GPIO_ReadPin(I2C_SCL_PORT, I2C_SCL_PIN)

//LED Control
#define LED_ON HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET)        // LED4 UP, PA5 = 1
#define LED_OFF HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET)       // LED4 DOWN, PA5 = 0
#define LED_ON_() HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET)        // LED4 UP, PA5 = 1
#define LED_OFF_() HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET)       // LED4 DOWN, PA5 = 0

//the time delay function
void delay_us(u32);
void delay_ms(u32);

//GPIO init
void i2c_slave_SCL_Falling_Exti_Enable(void);
void i2c_slave_SCL_Falling_Exti_Disable(void);
void i2c_slave_SDA_GPIO_Init(void);
//void i2c_slave_SDA_GPIO_Init(void);

//start and stop signal
u8 is_i2c_Start(void);
u8 is_i2c_Stop(void);

//acknowledge
void i2c_SendAck(void);
void i2c_SendNAck(void);
u8 i2c_WaitAck(void);

//Send and receive data
void i2c_SendByte(u8 *data_byte);
u32 i2c_ReadByte( void);
u32 I2C_Write(u8 *data, u32 data_length);
u32 I2C_Read(void);

void test(void);
void callback(void);
void LED(u32);


#endif



