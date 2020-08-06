/*
 * cpp
 *
 *  Created on: Aug 5, 2020
 *      Author: dybb8
 */

#include "../Picture/picture.h"
#include "stm32f1xx_hal.h"
#include "EPDControl.h"

void EPD_W21_Init()
{
	EPD_W21_RST(0);
	driver_delay_xms(1000);
	EPD_W21_RST(1);
	driver_delay_xms(1000);
}

void EPD_W21_RST(uint8_t signal)
{
	if(signal == 1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
	}
}

void EPD_W21_WriteCMD(uint8_t data)
{
	SPI_Delay(1);
	EPD_W21_CS(0);
	EPD_W21_DC(0);
	SPI_Write(data);
	EPD_W21_CS(1);
}

void SPI_Delay(uint8_t xrate)
{
	uint8_t i = 0;
	while(xrate)
	{
		for(i = 0; i < EPD_W21_SPI_SPEED; ++i);

		xrate -= 1;
	}
}

void EPD_W21_CS(uint8_t signal)
{
	if(signal == 1)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
	}
}

void EPD_W21_DC(uint8_t signal)
{
	if(signal == 1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
	}
}

void EPD_W21_CLK(uint8_t signal)
{
	if(signal == 1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	}
}

void SPI_Write(uint8_t command)
{
	/*
	uint8_t i;
	SPI_Delay(1);
	for(i = 0; i < 8; ++i)
	{
		EPD_W21_CLK(0);
		SPI_Delay(1);
		if(command & 0x80)
		{
			EPD_W21_MOSI(1);
		}
		else
		{
			EPD_W21_MOSI(0);
		}

		command = (command << 1);
		SPI_Delay(1);
		driver_delay_us(1);
		EPD_W21_CLK(1);
		SPI_Delay(1);
	}
	*/
	HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 10000);
}

void EPD_W21_MOSI(uint8_t signal)
{
	if(signal == 1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	}
}

void driver_delay_us(uint32_t xus)
{
	for(;xus>1;xus--);
}

void driver_delay_xms(uint32_t xms)
{
    uint32_t i = 0 , j=0;

    for(j=0;j<xms;j++)
	{
        for(i=0; i<256; i++);
    }
}

void EPD_W21_WriteDATA(uint8_t data)
{
	SPI_Delay(1);
	EPD_W21_CS(0);
	EPD_W21_DC(1);
	SPI_Write(data);
	EPD_W21_CS(1);
}

void PIC_display2(const uint8_t *picData_new, uint32_t len)
{
	EPD_W21_WriteCMD(0x10);			//Transfer old data

	uint32_t i;
	for(i=0;i<5808;i++)
	{
	  EPD_W21_WriteDATA(0xff);
	}
	EPD_W21_WriteCMD(0x13);		     //Transfer new data
	for(i=0;i<len;i++)
	{
	  EPD_W21_WriteDATA(*picData_new);
	  picData_new++;
	}
}

void PIC_display(const uint8_t *picData_new)
{
	EPD_W21_WriteCMD(0x10);			//Transfer old data

	uint32_t i;
	for(i=0;i<5808;i++)
	{
	  EPD_W21_WriteDATA(0xff);
	}
	EPD_W21_WriteCMD(0x13);		     //Transfer new data
	for(i=0;i<5808;i++)
	{
	  EPD_W21_WriteDATA(*picData_new);
	  picData_new++;
	}
}

void PIC_display_Clean()
{
	EPD_W21_WriteCMD(0x10);			//Transfer old data

	uint32_t i;
	for(i=0;i<5808;i++)
	{
	  EPD_W21_WriteDATA(0xff);
	}
	EPD_W21_WriteCMD(0x13);		     //Transfer new data
	for(i=0;i<5808;i++)
	{
	  EPD_W21_WriteDATA(0xff);
	}
}

void EPD_refresh()
{
	EPD_W21_WriteCMD(0x12);			//DISPLAY REFRESH
	driver_delay_xms(200);	        //!!!The delay here is necessary, 200uS at least!!!
	lcd_chkstatus();//waiting for the electronic paper IC to release the idle signal
}

void EPD_sleep()
{
  	EPD_W21_WriteCMD(0X02);  	//power off
	EPD_W21_WriteCMD(0X07);  	//deep sleep
	EPD_W21_WriteDATA(0xA5);
}

void lcd_chkstatus()
{
	uint8_t busy;
	do
	{
		EPD_W21_WriteCMD(0x71);
		busy = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
		busy =!(busy & 0x01);
	}
	while(busy);
	driver_delay_xms(200);
}

void DELAY_S(uint32_t delaytime)
{
	uint32_t i,j,k;
	for(i=0;i<delaytime;i++)
  {
		for(j=0;j<4000;j++)
		{
			for(k=0;k<222;k++);

		}
	}
}

void EPD_init()
{
	EPD_W21_Init();					//Electronic paper IC reset

	EPD_W21_WriteCMD(0x06);			 //boost soft start
	EPD_W21_WriteDATA(0x07);		//A
	EPD_W21_WriteDATA(0x07);		//B
	EPD_W21_WriteDATA(0x17);		//C

	EPD_W21_WriteCMD(0x04);
	lcd_chkstatus();

	EPD_W21_WriteCMD(0x00);			//panel setting
	EPD_W21_WriteDATA(0x1f);		//LUT from OTP，128x296

	EPD_W21_WriteCMD(0x16);
	EPD_W21_WriteDATA(0x00);		//KW-BF   KWR-AF	BWROTP 0f

	EPD_W21_WriteCMD(0xF8);         //boost设定
	EPD_W21_WriteDATA (0x60);
	EPD_W21_WriteDATA(0xa5);

	EPD_W21_WriteCMD(0xF8);         //boost设定
	EPD_W21_WriteDATA (0x73);
	EPD_W21_WriteDATA (0x23);

	EPD_W21_WriteCMD(0xF8);         //boost设定
	EPD_W21_WriteDATA (0x7C);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0X50);
	EPD_W21_WriteDATA(0x97);		//WBmode:VBDF 17|D7 VBDW 97 VBDB 57		WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

void EPD_Run()
{

	EPD_init();
	PIC_display2(g_Image3, sizeof(g_Image3));
	EPD_refresh();
	EPD_sleep();
	DELAY_S(10);

}
