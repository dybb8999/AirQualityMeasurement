/*
 * EPDControl.h
 *
 *  Created on: Aug 5, 2020
 *      Author: dybb8
 */

#ifndef EPDCONTROL_H_
#define EPDCONTROL_H_

#include <stdint.h>
#include "../Inc/main.h"
#include "../ImageCreator/ImageCreator.h"

#define EPD_W21_SPI_SPEED (0x2)

void EPD_W21_Init();
void EPD_W21_WriteCMD(uint8_t data);
void EPD_W21_WriteDATA(uint8_t data);

void EPD_W21_RST(uint8_t status);
void EPD_W21_CS(uint8_t signal);
void EPD_W21_DC(uint8_t signal);
void EPD_W21_CLK(uint8_t signal);
void EPD_W21_MOSI(uint8_t signal);

void SPI_Delay(uint8_t xrate);
void SPI_Write(uint8_t command);

void driver_delay_us(uint32_t xus);
void driver_delay_xms(uint32_t xms);

void lcd_chkstatus();

void EPD_init();
void EPD_init_4Gray();

void PIC_dispaly2(const uint8_t* picData_new, const uint32_t len);
void PIC_display(const uint8_t* picData_new);
void PIC_display_Clean();
//4 grayscale demo function
/********Color display description
      white  gray1  gray2  black
0x10|  01     01     00     00
0x13|  01     00     01     00
                                   ****************/
void pic_display_4bit(void);

void EPD_refresh();
void EPD_sleep();
void EPD_partial_display(uint16_t x,uint16_t y,const unsigned char *old_data,const unsigned char *new_data,unsigned int w,unsigned int l,unsigned char mode); //partial display
void EPD_full(void pic_display());

void DELAY_S(uint32_t delaytime);

void lut();
void lut1();
void lut11();

void EPD_Run();

void EPD_DrawImage(uint8_t* pImage);

#endif /* EPDCONTROL_H_ */
