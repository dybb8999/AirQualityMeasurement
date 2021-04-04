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
	HAL_SPI_Transmit(&hspi1, &command, sizeof(command), -1);
	//HAL_SPI_Transmit_IT(&hspi1, &command, sizeof(command));
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
	uint32_t u32End = HAL_GetTick() + delaytime * 1000;
	while(u32End > HAL_GetTick());
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

void EPD_init_4Gray()
{
	uint8_t HRES_byte1 = 0x00;			//176
	uint8_t HRES_byte2=0xb0;
	uint8_t VRES_byte1=0x01;			//264
	uint8_t	VRES_byte2=0x08;

	EPD_W21_Init();
	EPD_W21_WriteCMD(0x01);			//POWER SETTING
	EPD_W21_WriteDATA (0x03);
	EPD_W21_WriteDATA (0x00);
	EPD_W21_WriteDATA (0x2b);
	EPD_W21_WriteDATA (0x2b);


	EPD_W21_WriteCMD(0x06);         //booster soft start
	EPD_W21_WriteDATA (0x07);		//A
	EPD_W21_WriteDATA (0x07);		//B
	EPD_W21_WriteDATA (0x17);		//C

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0x60);
	EPD_W21_WriteDATA (0xA5);

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0x89);
	EPD_W21_WriteDATA (0xA5);

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0x90);
	EPD_W21_WriteDATA (0x00);

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0x93);
	EPD_W21_WriteDATA (0x2A);

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0xa0);
	EPD_W21_WriteDATA (0xa5);

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0xa1);
	EPD_W21_WriteDATA (0x00);

	EPD_W21_WriteCMD(0xF8);         //boost??
	EPD_W21_WriteDATA (0x73);
	EPD_W21_WriteDATA (0x41);

	EPD_W21_WriteCMD(0x16);
	EPD_W21_WriteDATA(0x00);

	EPD_W21_WriteCMD(0x04);
	lcd_chkstatus();

	EPD_W21_WriteCMD(0x00);			//panel setting
	EPD_W21_WriteDATA(0xbf);		//KW-BF   KWR-AF	BWROTP 0f

	EPD_W21_WriteCMD(0x30);			//PLL setting
	EPD_W21_WriteDATA (0x90);      	//100hz

	EPD_W21_WriteCMD(0x61);			//resolution setting
	EPD_W21_WriteDATA (HRES_byte1);		//176
	EPD_W21_WriteDATA (HRES_byte2);
	EPD_W21_WriteDATA (VRES_byte1);		//264
	EPD_W21_WriteDATA (VRES_byte2);

	EPD_W21_WriteCMD(0x82);			//vcom_DC setting
	EPD_W21_WriteDATA (0x12);

	EPD_W21_WriteCMD(0X50);			//VCOM AND DATA INTERVAL SETTING
	EPD_W21_WriteDATA(0x97);

}

void EPD_Run()
{

	//2 gray picture(OTP)
	EPD_init(); //EPD init
	PIC_display(gImage_1); //Clear screen
	EPD_refresh();//EPD_refresh
	EPD_sleep();//EPD_sleep`
	DELAY_S(5);

	//Clear screen (OTP)
	EPD_init(); //EPD init
	PIC_display_Clean(); //Clear screen
	EPD_refresh();//EPD_refresh
	EPD_sleep();//EPD_sleep`
	//4 gray picture
	EPD_init_4Gray(); //EPD init 4 Gray
	EPD_full(pic_display_4bit); //pic1
	EPD_sleep(); //Enter deep sleep mode
	DELAY_S(5);
	return;
	//Clear screen (OTP)
	EPD_init(); //EPD init
	PIC_display_Clean(); //Clear screen
	EPD_refresh();//EPD_refresh
	EPD_sleep();//EPD_sleep`
	DELAY_S(5);


	EPD_init_4Gray(); //EPD init 4 Gray
	//Demo1
	EPD_partial_display(0,0,gImage_ta5,gImage_ta5,176,264,0); //partial display
	EPD_partial_display(0,0,gImage_ta5,gImage_ta4,176,264,1); //partial display
	EPD_partial_display(0,0,gImage_ta4,gImage_ta3,176,264,1); //partial display
	EPD_partial_display(0,0,gImage_ta3,gImage_ta2,176,264,1); //partial display
	EPD_partial_display(0,0,gImage_ta2,gImage_ta1,176,264,1); //partial display
	DELAY_S(5);
	//Clear screen (OTP)
	EPD_init(); //EPD init
	PIC_display_Clean(); //Clear screen
	EPD_refresh();//EPD_refresh
	EPD_sleep();//EPD_sleep
	DELAY_S(5);

}

void EPD_DrawImage(uint8_t* pImage)
{
	//2 gray picture(OTP)
	EPD_init(); //EPD init
	PIC_display(pImage); //Clear screen
	EPD_refresh();//EPD_refresh
	EPD_sleep();//EPD_sleep`
}

//4 grayscale demo function
/********Color display description
      white  gray1  gray2  black
0x10|  01     01     00     00
0x13|  01     00     01     00
                                   ****************/
void pic_display_4bit(void)
{
	uint32_t i, j;
	uint8_t temp1, temp2, temp3;

	//old  data
	EPD_W21_WriteCMD(0x10);

	for (i=0;i<5808;i++)	               //5808*4  264*176
	{
		temp3=0;
		for (j=0;j<4;j++)
		{
			temp1 = gImage_11[i*4+j];
			temp2 = temp1&0xF0;
			if (temp2 == 0xF0)
				temp3 |= 0x01;//white
			else if (temp2 == 0x00)
				temp3 |= 0x00;  //black
			else if ((temp2>0xA0)&&(temp2<0xF0))
				temp3 |= 0x01;  //gray1
			else
				temp3 |= 0x00; //gray2
			temp3 <<= 1;
			temp1 <<= 4;
			temp2 = temp1&0xF0;
			if (temp2 == 0xF0)  //white
				temp3 |= 0x01;
			else if (temp2 == 0x00) //black
				temp3 |= 0x00;
			else if ((temp2>0xA0)&&(temp2<0xF0))
				temp3 |= 0x01; //gray1
			else
				temp3 |= 0x00;	//gray2
			if (j!=3)
				temp3 <<= 1;


		}
		EPD_W21_WriteDATA(temp3);
	}
	//new  data
	EPD_W21_WriteCMD(0x13);

	for (i=0;i<5808;i++)	               //5808*4   264*176
	{
		temp3=0;
		for (j=0;j<4;j++)
		{
			temp1 = gImage_11[i*4+j];
			temp2 = temp1&0xF0;
			if (temp2 == 0xF0)
				temp3 |= 0x01;//white
			else if (temp2 == 0x00)
				temp3 |= 0x00;  //black
			else if ((temp2>0xA0)&&(temp2<0xF0))
				temp3 |= 0x00;  //gray1
			else
				temp3 |= 0x01; //gray2
			temp3 <<= 1;
			temp1 <<= 4;
			temp2 = temp1&0xF0;
			if (temp2 == 0xF0)  //white
				temp3 |= 0x01;
			else if (temp2 == 0x00) //black
				temp3 |= 0x00;
			else if ((temp2>0xA0)&&(temp2<0xF0))
				temp3 |= 0x00;//gray1
			else
				temp3 |= 0x01;	//gray2
			if (j!=3)
				temp3 <<= 1;

		}
		EPD_W21_WriteDATA(temp3);
	}
}

void lut()
{
	unsigned int count;
	EPD_W21_WriteCMD(0x20);
	for(count=0;count<44;count++)
		{EPD_W21_WriteDATA(lut_vcomDC[count]);}

	EPD_W21_WriteCMD(0x21);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_ww[count]);}

	EPD_W21_WriteCMD(0x22);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_bw[count]);}

	EPD_W21_WriteCMD(0x23);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_wb[count]);}

	EPD_W21_WriteCMD(0x24);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_bb[count]);}
}

void lut1()
{
	unsigned int count;
	EPD_W21_WriteCMD(0x20);
	for(count=0;count<44;count++)
		{EPD_W21_WriteDATA(lut_vcom1[count]);}

	EPD_W21_WriteCMD(0x21);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_ww1[count]);}

	EPD_W21_WriteCMD(0x22);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_bw1[count]);}

	EPD_W21_WriteCMD(0x23);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_wb1[count]);}

	EPD_W21_WriteCMD(0x24);
	for(count=0;count<42;count++)
		{EPD_W21_WriteDATA(lut_bb1[count]);}
}

void EPD_partial_display(uint16_t x,uint16_t y,const unsigned char *old_data,const unsigned char *new_data,unsigned int w,unsigned int l,unsigned char mode) //partial display
{
	unsigned int i,count;
	EPD_W21_WriteCMD(0x82);			//vcom_DC setting
	EPD_W21_WriteDATA (0x08);
	EPD_W21_WriteCMD(0X50);
	EPD_W21_WriteDATA(0x47);
	lut1();
	EPD_W21_WriteCMD(0x91);		//This command makes the display enter partial mode
	EPD_W21_WriteCMD(0x90);		//resolution setting

	EPD_W21_WriteCMD(0x14); //write old data
	EPD_W21_WriteDATA(0);	    //x-start
	EPD_W21_WriteDATA(x);     //x-end
	EPD_W21_WriteDATA(0);	    //y-start
	EPD_W21_WriteDATA(y);	    //y-end
	EPD_W21_WriteDATA(0);	    //w-start
	EPD_W21_WriteDATA(w);	    //w-end
	EPD_W21_WriteDATA(0);	    //l-start
	EPD_W21_WriteDATA(l);	    //l-end

	count=w*l/8;
	if(mode==0)
	for(i=0;i<count;i++)
	{
		EPD_W21_WriteDATA(0x00);
	}
	else
	for(i=0;i<count;i++)
	{
		EPD_W21_WriteDATA(~old_data[i]);
	}

	EPD_W21_WriteCMD(0x15); //write new data
	EPD_W21_WriteDATA(0);	    //x-start
	EPD_W21_WriteDATA(x);     //x-end
	EPD_W21_WriteDATA(0);	    //y-start
	EPD_W21_WriteDATA(y);	    //y-end
	EPD_W21_WriteDATA(0);	    //w-start
	EPD_W21_WriteDATA(w);	    //w-end
	EPD_W21_WriteDATA(0);	    //l-start
	EPD_W21_WriteDATA(l);	    //l-end

	if(mode!=2) //new  datas
	{
		for(i=0;i<count;i++)
		{
			EPD_W21_WriteDATA(~new_data[i]);
			//old_data[i]=new_data[i];
		}
	}
	else  //white
	{
		for(i=0;i<count;i++)
		{
		 EPD_W21_WriteDATA(0x00);
		}
	}

	EPD_W21_WriteCMD(0x16); //write display position
	EPD_W21_WriteDATA(0);	    //x-start
	EPD_W21_WriteDATA(x);     //x-end
	EPD_W21_WriteDATA(0);	    //y-start
	EPD_W21_WriteDATA(y);	    //y-end
	EPD_W21_WriteDATA(0);	    //w-start
	EPD_W21_WriteDATA(w);	    //w-end
	EPD_W21_WriteDATA(0);	    //l-start
	EPD_W21_WriteDATA(l);	    //l-end
	lcd_chkstatus();

	EPD_W21_WriteCMD(0x12);		 //DISPLAY REFRESH
	driver_delay_xms(1);     //!!!The delay here is necessary, 200uS at least!!!
	lcd_chkstatus();

}

void EPD_full(void pic_display(void))
{
	pic_display(); //picture
	lut11(); //Power settings
	EPD_W21_WriteCMD(0x12);			//DISPLAY REFRESH
	driver_delay_xms(100);	    //!!!The delay here is necessary, 200uS at least!!!
	lcd_chkstatus();
}

//LUT download
void lut11(void)
{
	unsigned int count;
	{
		EPD_W21_WriteCMD(0x20);							//vcom
		for (count=0;count<44;count++)
		{
			EPD_W21_WriteDATA(lut_vcom11[count]);
		}

		EPD_W21_WriteCMD(0x21);							//red not use
		for (count=0;count<42;count++)
		{
			EPD_W21_WriteDATA(lut_ww11[count]);
		}

		EPD_W21_WriteCMD(0x22);							//bw r
		for (count=0;count<42;count++)
		{
			EPD_W21_WriteDATA(lut_bw11[count]);
		}

		EPD_W21_WriteCMD(0x23);							//wb w
		for (count=0;count<42;count++)
		{
			EPD_W21_WriteDATA(lut_wb11[count]);
		}

		EPD_W21_WriteCMD(0x24);							//bb b
		for (count=0;count<42;count++)
		{
			EPD_W21_WriteDATA(lut_bb11[count]);
		}

		EPD_W21_WriteCMD(0x25);							//vcom
		for (count=0;count<42;count++)
		{
			EPD_W21_WriteDATA(lut_ww11[count]);
		}
	}
}
