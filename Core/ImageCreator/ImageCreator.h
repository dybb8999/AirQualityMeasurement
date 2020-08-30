/*
 * ImageCreator.h
 *
 *  Created on: Aug 16, 2020
 *      Author: dybb8
 */

#ifndef IMAGECREATOR_H_
#define IMAGECREATOR_H_
#include <stdint.h>

#define HDC_WIDTH (264)

#define HDC_HEIGH (176)

typedef struct _HDC
{
	// 264*176
	uint8_t Data[HDC_WIDTH][HDC_HEIGH];
}HDC, *PHDC;

typedef struct _FontInfo
{
	uint8_t* data;
	uint8_t w;
	uint8_t h;
}FontInfo, *PFontInfo;

void ConvertTo2Color(PHDC hdc, uint8_t* image);

void FillColor(PHDC hdc, int x, int y, int w, int h, uint8_t color);

uint32_t GetNextChat(const char* str, uint8_t* len);

int PutChar(PHDC hdc, int x, int y, uint32_t ch, uint32_t fontsize);

int PutStr(PHDC hdc, int x, int y, char* str, uint32_t fontsize);

const FontInfo GetFontInfo(uint32_t ch, uint32_t size);

const FontInfo GetFontInfo_8Byte(uint32_t ch);

const FontInfo GetFontInfo_12Byte(uint32_t ch);

const FontInfo GetFontInfo_16Byte(uint32_t ch);

#endif /* IMAGECREATOR_H_ */
