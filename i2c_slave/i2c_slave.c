/*
i2c_slave.c
QQ:		957165610
Ver 1.0
2014-05-11				by			xulifeng
Ver 1.1
2014-05-11				by			xulifeng
Ver 1.2
2014-05-13				by			xulifeng
*/

#include "i2c_slave.h"

typedef enum
{
		I2C_SLAVE_IDLE,
		I2C_SLAVE_ADD,//write iic add
		I2C_SLAVE_REG,//write the register add
		I2C_SLAVE_WRITE,//master write and slave read
		I2C_SLAVE_READ,//master read and slave write
		I2C_SLAVE_BUSY
}e_I2C_SLAVE_MODE;

e_I2C_SLAVE_MODE		i2c_slave_mode = I2C_SLAVE_IDLE;
unsigned char I2C_DATA_TEMP = 0;
unsigned char i2c_slave_reg_p = 0;//the register add
unsigned char i2c_slave_data_p = 0;//


void i2c_slave_scl_h( void )
{
	I2C_SDA_INT_EN();
	switch( i2c_slave_mode )
	{
		case I2C_SLAVE_ADD:
		case I2C_SLAVE_REG:
		case I2C_SLAVE_WRITE:
			I2C_DATA_TEMP <<= 1;
			if( I2C_SDA_IN() )
				I2C_DATA_TEMP ++;
			i2c_slave_data_p++;
			break;
		
		case I2C_SLAVE_READ:
			i2c_slave_data_p++;
			if( i2c_slave_data_p > 8 )
			{
				if( I2C_SDA_IN() )
				{
					i2c_slave_mode = I2C_SLAVE_BUSY;
				}
			}
		default:break;
	}
}

void i2c_slave_scl_l( void )
{
	I2C_SDA_INT_DIS();
	
	I2C_SCL_L();//slow the i2c speed
	
	if( i2c_slave_data_p > 8 )
	{
		i2c_slave_data_p = 0;
		if( i2c_slave_mode == I2C_SLAVE_READ )
		{
			I2C_SLAVE_READ_BYTE(i2c_slave_reg_p++, &I2C_DATA_TEMP);
			
			if( I2C_DATA_TEMP & 0x80 )//output a bit
				I2C_SDA_H();
			else
				I2C_SDA_L();
			I2C_DATA_TEMP <<= 1;
		}
		else
			I2C_SDA_H();//end ack;
		
		I2C_SCL_H();
		return;
	}
	
	switch( i2c_slave_mode )
	{
		case I2C_SLAVE_ADD:
			if( i2c_slave_data_p == 8 )
			{
					if( I2C_DATA_TEMP >> 1 == I2C_ADD )
					{
						I2C_SDA_L();//ack
						if( I2C_DATA_TEMP & 0x01 )//read
						{
							i2c_slave_mode = I2C_SLAVE_READ;
						}
						else
						{
							i2c_slave_mode = I2C_SLAVE_REG;//write regster add
						}
					}
					else
						i2c_slave_mode = I2C_SLAVE_BUSY;//nack
			}
			break;
		case I2C_SLAVE_REG:
			if( i2c_slave_data_p == 8 )
			{
					I2C_SDA_L();//ack
					i2c_slave_reg_p = I2C_DATA_TEMP;
					i2c_slave_mode = I2C_SLAVE_WRITE;
			}
			break;
		case I2C_SLAVE_WRITE:
			if( i2c_slave_data_p == 8 )
			{//
					if( I2C_SLAVE_WRITE_BYTE( i2c_slave_reg_p++, I2C_DATA_TEMP ) )
					{
							I2C_SDA_L();
					}
					else
					{
							i2c_slave_mode = I2C_SLAVE_BUSY;
					}
			}
			break;
		case I2C_SLAVE_READ:
			if( i2c_slave_data_p < 8 )
			{
					if( I2C_DATA_TEMP & 0x80 )//output a bit
						I2C_SDA_H();
					else
						I2C_SDA_L();
					I2C_DATA_TEMP <<= 1;
			}
			if( i2c_slave_data_p == 8 )
			{
					I2C_SDA_H();
			}
			default:break;
	}
	I2C_SCL_H();
}

void i2c_slave_sda_h( void )
{
		if( I2C_SCL_IN() )
		{
			I2C_SCL_INT_DIS();
			i2c_slave_mode = I2C_SLAVE_IDLE;
		}
}

void i2c_slave_sda_l( void )
{
	if( I2C_SCL_IN() )
	{
		I2C_SCL_INT_EN();
		i2c_slave_mode = I2C_SLAVE_ADD;
		i2c_slave_data_p = 0;
	}
}


