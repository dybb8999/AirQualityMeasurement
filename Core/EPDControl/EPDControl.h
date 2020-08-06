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

void PIC_display(const uint8_t* picData_new);
void PIC_display_Clean();

void EPD_refresh();
void EPD_sleep();

void DELAY_S(uint32_t delaytime);

void EPD_Run();

#endif /* EPDCONTROL_H_ */
