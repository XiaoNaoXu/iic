#ifndef __I2C_SLAVE_H__
#define __I2C_SLAVE_H__

#define I2C_ADD			0x34

#include "i2c_slave_port.h"

#define I2C_SCL_H()				GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define I2C_SCL_L()				GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define I2C_SDA_H()				GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define I2C_SDA_L()				GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define I2C_SCL_IN()			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define I2C_SDA_IN()			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)

#define I2C_SDA_INT_EN()								i2c_slave_sda_int_en()
#define I2C_SDA_INT_DIS()								i2c_slave_sda_int_dis()
#define I2C_SCL_INT_EN()								i2c_slave_scl_int_en()
#define I2C_SCL_INT_DIS()								i2c_slave_scl_int_dis()
#define I2C_SLAVE_WRITE_BYTE(reg,x)			i2c_slave_write_byte(reg,x)
#define I2C_SLAVE_READ_BYTE(reg,x)			i2c_slave_read_byte(reg,x)

void i2c_slave_scl_h( void );
void i2c_slave_scl_l( void );

void i2c_slave_sda_h( void );
void i2c_slave_sda_l( void );

#endif
