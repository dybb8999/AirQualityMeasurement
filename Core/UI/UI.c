/*
 * UI.c
 *
 *  Created on: Aug 16, 2020
 *      Author: dybb8
 */


#include "UI.h"
#include <string.h>
#include "../PM25/PM25Control.h"
#include <stdlib.h>

void UIForAirQuality(PHDC hdc)
{
	//左边label宽36像素
	int xOffset = 0;
	int yOffset = 0;
	char numBuff[16];

	//清屏
	FillColor(hdc, 0, 0, HDC_WIDTH, HDC_HEIGH, 1);

	//PM1
	PutStr(hdc, xOffset, yOffset, "PM1:", 16);
	itoa(g_AirQuality.PM1, numBuff, 16);
	PutStr(hdc, 80, yOffset, numBuff, 16);
	yOffset += 16;

	//PM2.5
	xOffset = 0;
	PutStr(hdc, xOffset, yOffset, "PM2.5:", 16);
	itoa(g_AirQuality.PM2_5, numBuff, 16);
	PutStr(hdc, 80, yOffset, numBuff, 16);
	yOffset += 16;

	//PM10
	xOffset = 0;
	PutStr(hdc, xOffset, yOffset, "PM10:", 16);
	itoa(g_AirQuality.PM10, numBuff, 16);
	PutStr(hdc, 80, yOffset, numBuff, 16);
	yOffset += 16;

	//TVOC
	xOffset = 0;
	PutStr(hdc, xOffset, yOffset, "TVOC:", 16);
	itoa(g_AirQuality.TVOC, numBuff, 16);
	PutStr(hdc, 80, yOffset, numBuff, 16);
	yOffset += 16;

	//温度
	xOffset = 0;
	xOffset += PutStr(hdc, xOffset, yOffset, "温度", 24);
	xOffset += PutChar(hdc, xOffset, yOffset, ':', 16);
	itoa(g_AirQuality.Temperature, numBuff, 16);
	PutStr(hdc, 80, yOffset, numBuff, 16);
	yOffset += 16;

	//湿度
	xOffset = 0;
	xOffset += PutStr(hdc, xOffset, yOffset, "湿度", 24);
	xOffset += PutChar(hdc, xOffset, yOffset, ':', 16);
	itoa(g_AirQuality.Tumidity, numBuff, 16);
	PutStr(hdc, 80, yOffset, numBuff, 16);
	yOffset += 16;
}
