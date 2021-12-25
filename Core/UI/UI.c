/*
 * UI.c
 *
 *  Created on: Aug 16, 2020
 *      Author: dybb8
 */


#include "UI.h"
#include <string.h>
#include "../PM25/PM25Control.h"
#include "../CO2/CO2.h"
#include <stdlib.h>
#include "../WiFi/WiFi.h"

void UIForAirQuality(PHDC hdc)
{
	//左边label宽36像素
	int xOffset = 0;
	int yOffset = 0;
	char numBuff[16];

	//清屏
	FillColor(hdc, 0, 0, HDC_WIDTH, HDC_HEIGH, 1);

	//PM1
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "PM1:", 16);
	itoa(g_AirQuality.PM1, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;

	xOffset = PutStr(hdc, xOffset, yOffset, "ug", 12);
	xOffset += PutChar(hdc, xOffset, yOffset, '/', 16);
	xOffset += PutChar(hdc, xOffset, yOffset + 8, 'm', 12);
	xOffset += PutChar(hdc, xOffset, yOffset + 4, '3', 8);
	yOffset += 16;

	//PM2.5
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "PM2.5:", 16);
	itoa(g_AirQuality.PM2_5, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;

	xOffset = PutStr(hdc, xOffset, yOffset, "ug", 12);
	xOffset += PutChar(hdc, xOffset, yOffset, '/', 16);
	xOffset += PutChar(hdc, xOffset, yOffset + 8, 'm', 12);
	xOffset += PutChar(hdc, xOffset, yOffset + 4, '3', 8);
	yOffset += 16;

	//PM10
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "PM10:", 16);
	itoa(g_AirQuality.PM10, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;

	xOffset = PutStr(hdc, xOffset, yOffset, "ug", 12);
	xOffset += PutChar(hdc, xOffset, yOffset, '/', 16);
	xOffset += PutChar(hdc, xOffset, yOffset + 8, 'm', 12);
	xOffset += PutChar(hdc, xOffset, yOffset + 4, '3', 8);
	yOffset += 16;

	//TVOC
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "TVOC:", 16);
	itoa(g_AirQuality.TVOC, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;

	xOffset = PutStr(hdc, xOffset, yOffset, "ppb", 16);

	yOffset += 16;

	//温度
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "温度", 16);
	xOffset += PutChar(hdc, xOffset, yOffset, ':', 16);
	itoa(g_AirQuality.Temperature, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;

	xOffset = PutStr(hdc, xOffset, yOffset, "度", 16);
	yOffset += 16;

	//湿度
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "湿度:", 16);
	itoa(g_AirQuality.Tumidity, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;

	xOffset = PutStr(hdc, xOffset, yOffset, "%RH", 16);

	yOffset += 16;

	//Co2
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "CO", 16);
	xOffset += PutChar(hdc, xOffset, yOffset + 6, '2', 8);
	xOffset += PutChar(hdc, xOffset, yOffset, ':', 16);
	itoa(g_u32Co2Value, numBuff, 10);
	xOffset = PutStr(hdc, 80, yOffset, numBuff, 16);
	xOffset += 2;
	xOffset = PutStr(hdc, xOffset, yOffset, "ppm", 16);
	yOffset += 16;

	//IP
	xOffset = 0;
	xOffset = PutStr(hdc, xOffset, yOffset, "IP:", 16);
	xOffset = PutStr(hdc, 80, yOffset, g_WifiStatus.Address, 16);
	yOffset += 16;

	// AP Name
	// xOffset = 0;
	// xOffset = PutStr(hdc, xOffset, yOffset, "AP:", 16);
	// xOffset = PutStr(hdc, 80, yOffset, g_WifiStatus.SSID, 16);
	// yOffset += 16;

	// AP IP
	// xOffset = 0;
	// xOffset = PutStr(hdc, xOffset, yOffset, "AP IP:", 16);
	// xOffset = PutStr(hdc, 80, yOffset, g_WifiStatus.APAddress, 16);
	// yOffset += 16;
}

void UIForStartUp(PHDC hdc, int progress)
{
	//清屏
	FillColor(hdc, 0, 0, HDC_WIDTH, HDC_HEIGH, 1);

	int xOffset = 90;
	int yOffset = 70;
	xOffset = PutStr(hdc, xOffset, yOffset, "正在启动:", 16);
	DrawLineH(hdc, 0,90,262, 0);
	DrawLineV(hdc, 0,90,20, 0);
	DrawLineV(hdc, 262,90,20, 0);
	DrawLineH(hdc, 0,110,263, 0);

	int w = HDC_WIDTH * (progress * 1.0 / 100);
	DrawRectangle(hdc, 0, 90, w, 20, 0);
}
