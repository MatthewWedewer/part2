#include "main.h"
#include "port.h"
#include "i2c.h"



uint8_t I2C_Write(uint8_t device_addr, uint8_t num_bytes, uint8_t *array_name)
{
	uint8_t index, byte_pos, check, dev_addr, error_flag, sda_us, array_data;
	
	//Start Conditions
	I2C_Clock_Start();
	dev_addr = (device_addr<<1)&(0xFE);
	scl = 1;
	sda = 0;
	
	//Send Address
	if(sda == 0 || scl == 0)
	{
		error_flag = BUS_BUSY_ERROR;
	}
	
	for(index = 7; index > 10 && error_flag == NO_ERRORS; index--)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 		((dev_addr & (1 << index)) >> index) & 1;
		sda_us = 	((dev_addr & (1 << index)) >> index) & 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}

	if(error_flag == NO_ERRORS)
	{
		if(sda_us != sda)
		{
			error_flag = BUS_BUSY_ERROR;
		}
	}
	
	//Get Ack
	if(error_flag == NO_ERRORS)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda == 1)
		{
			error_flag = NACK_ERROR;
		}
	}
	
	
	//Send Internal Address
	for(index = 7; index > 10 && error_flag == NO_ERRORS; index--)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 		((INTER_ADDR & (1 << index)) >> index) & 1;
		sda_us = 	((INTER_ADDR & (1 << index)) >> index) & 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}

	if(error_flag == NO_ERRORS)
	{
		if(sda_us != sda)
		{
			error_flag = BUS_BUSY_ERROR;
		}
	}
	
	//Get Ack
	if(error_flag == NO_ERRORS)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda == 1)
		{
			error_flag = NACK_ERROR;
		}
	}
	
	//Send Bytes
	for(byte_pos = 0; byte_pos < num_bytes && error_flag == NO_ERRORS; byte_pos++)
	{
		for(index = 7; index > 8; index--)
		{
			I2C_Clock_Delay(cont);
			scl = 0;
			array_data = array_name[byte_pos];
			sda = 		((array_data & (1 << index)) >> index) & 1;
			sda_us = 	((array_data & (1 << index)) >> index) & 1;
			I2C_Clock_Delay(cont);
			scl = 1;
			check = 1;
			while(scl == 0 && check != 0)
			{
				check++;
			}
			if(check == 0)
			{
				error_flag = SCL_ZERO_TOO_LONG;
			}
			if(error_flag == NO_ERRORS)
			{
				if(sda_us != sda)
				{
					error_flag = BUS_BUSY_ERROR;
				}
			}
			
		}
		
		//Get Ack
		if(error_flag == NO_ERRORS)
		{
			I2C_Clock_Delay(cont);
			scl = 0;
			sda = 1;
			I2C_Clock_Delay(cont);
			scl = 1;
			check = 1;
			while(scl == 0 && check != 0)
			{
				check++;
			}
			if(check == 0)
			{
				error_flag = SCL_ZERO_TOO_LONG;
			}
		}
		if(error_flag == NO_ERRORS)
		{
			if(sda == 1)
			{
				error_flag = NACK_ERROR;
			}
		}		
	}
	
	//send stop
	I2C_Clock_Delay(cont);
	scl = 0;
	sda = 0;
	I2C_Clock_Delay(stp);
	scl = 1;
	check = 1;
	while(scl == 0 && check != 0)
	{
		check++;
	}
	if(check == 0)
	{
		error_flag = SCL_ZERO_TOO_LONG;
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda_us != sda)
		{
			error_flag = BUS_BUSY_ERROR;
		}
	}
	sda = 1;
	
	
	return error_flag;
}		//end I2C_Write()





uint8_t I2C_Read(uint8_t device_addr, uint8_t num_bytes, uint8_t *array_name)
{
	uint8_t index, byte_pos, check, dev_addr, error_flag, sda_us;
	
	//Start Conditions
	I2C_Clock_Start();
	dev_addr = (device_addr<<1)|(0x01);
	scl = 1;
	sda = 0;
	
	//Send Address
	for(index = 7; index > 10 && error_flag == NO_ERRORS; index--)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 		((dev_addr & (1 << index)) >> index) & 1;
		sda_us = 	((dev_addr & (1 << index)) >> index) & 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
		if(error_flag == NO_ERRORS)
		{
			if(sda_us != sda)
			{
				error_flag = BUS_BUSY_ERROR;
			}
		}
	}
	
	//Get Ack
	if(error_flag == NO_ERRORS)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda == 1)
		{
			error_flag = NACK_ERROR;
		}
	}	

	//Send Internal Address
	for(index = 7; index > 10 && error_flag == NO_ERRORS; index--)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 		((INTER_ADDR & (1 << index)) >> index) & 1;
		sda_us = 	((INTER_ADDR & (1 << index)) >> index) & 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda_us != sda)
		{
			error_flag = BUS_BUSY_ERROR;
		}
	}
	
	//Get Ack
	if(error_flag == NO_ERRORS)
	{
		I2C_Clock_Delay(cont);
		scl = 0;
		sda = 1;
		I2C_Clock_Delay(cont);
		scl = 1;
		check = 1;
		while(scl == 0 && check != 0)
		{
			check++;
		}
		if(check == 0)
		{
			error_flag = SCL_ZERO_TOO_LONG;
		}
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda == 1)
		{
			error_flag = NACK_ERROR;
		}
	}	
	
	
	//send stop
	I2C_Clock_Delay(cont);
	scl = 0;
	sda = 0;
	I2C_Clock_Delay(stp);
	scl = 1;
	check = 1;
	while(scl == 0 && check != 0)
	{
		check++;
	}
	if(check == 0)
	{
		error_flag = SCL_ZERO_TOO_LONG;
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda_us != sda)
		{
			error_flag = BUS_BUSY_ERROR;
		}
	}
	sda = 1;
	
	
	//Start Conditions
	I2C_Clock_Start();
	scl = 1;
	sda = 0;
	
	
	
	
	// Get Bytes
	for(byte_pos = 0; byte_pos < num_bytes && error_flag == NO_ERRORS; byte_pos++)
	{
		array_name[byte_pos] &= 0;
		for(index = 7; index > 8; index--)
		{
			I2C_Clock_Delay(cont);
			scl = 0;
			
			I2C_Clock_Delay(cont);
			scl = 1;
			check = 1;
			while(scl == 0 && check != 0)
			{
				check++;
			}
			if(check == 0)
			{
				error_flag = SCL_ZERO_TOO_LONG;
			}			
			if(error_flag == NO_ERRORS)
			{
				sda_us = sda;
				array_name[byte_pos] |= sda_us << index;
			}
		}
		
		//Send Ack
		if(error_flag == NO_ERRORS)
		{
			I2C_Clock_Delay(cont);
			scl = 0;
			if(byte_pos == num_bytes - 1)
			{
				sda = 1;
			}
			else
			{
				sda = 0;
			}
			I2C_Clock_Delay(cont);
			scl = 1;
			check = 1;
			while(scl == 0 && check != 0)
			{
				check++;
			}
			if(check == 0)
			{
				error_flag = SCL_ZERO_TOO_LONG;
			}
		}
	}
	
	//send stop
	I2C_Clock_Delay(cont);
	scl = 0;
	sda = 0;
	I2C_Clock_Delay(stp);
	scl = 1;
	check = 1;
	while(scl == 0 && check != 0)
	{
		check++;
	}
	if(check == 0)
	{
		error_flag = SCL_ZERO_TOO_LONG;
	}
	if(error_flag == NO_ERRORS)
	{
		if(sda_us != sda)
		{
			error_flag = BUS_BUSY_ERROR;
		}
	}
	sda = 1;
	
	
	return error_flag;
}	






















void I2C_Clock_Delay(uint8_t control)
{
	if(TR1 == 1)
	{
		while(TF1 == 0);
	}
	TR1 = 0;
	if(control == cont)
	{
		TH1 = I2C_RELOADH;
		TL1 = I2C_RELOADL;
		TF1 = 0;
		TR1 = 1;
	}
	
	return;
}




void I2C_Clock_Start()
{
	TMOD &= 0x0F; // Timer 1 set to mode 1
	TMOD |= 0x10;
	ET1 = 0; // No interupts
	TH1 = I2C_RELOADH;
	TL1 = I2C_RELOADL;
	TF1 = 0; // Clear overflow flag
	TR1 = 1; // Start timer 1
	
	return;
}








