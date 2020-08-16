/*
 * ImageCreator.c
 *
 *  Created on: Aug 16, 2020
 *      Author: dybb8
 */

#include "ImageCreator.h"

void ConvertTo2Color(PHDC hdc, uint8_t* image)
{
	//从上到下从右到左
	int nIndex = 0;
	int nCount = 0;
	int tmp = 0;
	for(int i = HDC_WIDTH - 1; i >= 0; --i)
	{
		for(int j = 0; j < HDC_HEIGH; ++j)
		{
			if(hdc->Data[i][j] != 0)
			{
				tmp = tmp | 0x1;
			}
			++nCount;

			if(nCount == 8)
			{
				image[nIndex] = tmp;
				++nIndex;
				tmp = 0;
				nCount = 0;
			}
			tmp = tmp << 1;
		}
	}

}

void FillColor(PHDC hdc, int x, int y, int w, int h, uint8_t color)
{
	for(int i = x; i < w; ++i)
	{
		for(int j = y; j < h; ++j)
		{
			hdc->Data[i][j] = color;
		}
	}
}

uint32_t GetNextChat(const char* str, uint8_t* len)
{
	uint32_t u32Ret = str[0];
	uint32_t ch1 = str[0];
	uint32_t ch2 = str[1];
	uint32_t ch3 = str[2];
	if(str[0] >= 0x80)
	{
		u32Ret = ch1 << 16 | ch2 << 8 | ch3;
		if(len != 0)
		{
			*len = 3;
		}
	}
	else
	{
		if(len != 0)
		{
			*len = 1;
		}
	}
	return u32Ret;
}

int PutChar(PHDC hdc, int x, int y, uint32_t ch, uint32_t fontsize)
{
	FontInfo fontInfo = GetFontInfo(ch, fontsize);
	if(fontInfo.data == 0)
	{
		return fontInfo.w;
	}

	for(int j = 0; j < fontInfo.h; j += 1)
	{
		if(y + j >= HDC_HEIGH)
		{
			break;
		}

		for(int i = 0; i < fontInfo.w; ++i)
		{
			uint8_t tmp = 1 << (i%8);
			tmp = tmp & fontInfo.data[j * fontInfo.w / 8 + i / 8];

			if(x + i >= HDC_WIDTH)
			{
				break;
			}

			hdc->Data[x + i][y + j] = tmp;
		}
	}

	return fontInfo.w;
}

int PutStr(PHDC hdc, int x, int y, char* str, uint32_t fontsize)
{
	int xOffset = x;
	int len = 0;
	for(int i = 0; str[i] != 0;)
	{
		uint32_t u32ch = GetNextChat(&str[i], &len);
		i += len;
		xOffset += PutChar(hdc, xOffset, y, u32ch, fontsize);
	}

	return xOffset;
}

const FontInfo GetFontInfo(uint32_t ch, uint32_t size)
{
	//https://www.23bei.com/tool-229.html
	//从左到右从上到下
	//横向八点位右高位
	//取反
	FontInfo p = {0};
	switch(size)
	{
	case 8:
		p = GetFontInfo_8Byte(ch);
		break;
	case 16:
		p = GetFontInfo_16Byte(ch);
		break;

	case 24:
		p = GetFontInfo_24Byte(ch);
		break;
	}

	return p;
}

const FontInfo GetFontInfo_8Byte(uint32_t ch)
{
	FontInfo info = {0};
	switch(ch)
	{
	case '2':
		info.data = (uint8_t*)"\xF1\xEE\xEF\xF3\xFD\xFE\xE0\xFF";
		info.w = 8;
		info.h = 8;
		break;
	}
	return info;
}

const FontInfo GetFontInfo_16Byte(uint32_t ch)
{
	FontInfo info = {0};
	switch(ch)
	{
	case '0':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9C\x8C\x94\x94\x98\x9C\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '1':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xE7\xE3\xE1\xE7\xE7\xE7\xE7\xE7\xE7\x81\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '2':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9F\xCF\xE7\xF3\xF9\xFC\x9C\x80\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '3':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9F\x9F\xC3\x9F\x9F\x9F\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '4':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xCF\xC7\xC3\xC9\xCC\x80\xCF\xCF\xCF\x87\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '5':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x80\xFC\xFC\xFC\xC0\x8F\x9F\x9F\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '6':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xE3\xF9\xFC\xFC\xC0\x9C\x9C\x9C\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '7':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x80\x9C\x9F\x9F\xCF\xE7\xF3\xF3\xF3\xF3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '8':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9C\x9C\xC1\x9C\x9C\x9C\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '9':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9C\x9C\x81\x9F\x9F\x9F\xCF\xE1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'C':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC3\x99\xBC\xFC\xFC\xFC\xFC\xBC\x99\xC3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'M':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x88\x80\x80\x94\x9C\x9C\x9C\x9C\x9C\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'O':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xE3\xC9\x9C\x9C\x9C\x9C\x9C\x9C\xC9\xE3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'P':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC0\x99\x99\x99\xC1\xF9\xF9\xF9\xF9\xF0\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'T':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x81\x81\xA5\xE7\xE7\xE7\xE7\xE7\xE7\xC3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'V':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x9C\x9C\x9C\x9C\x9C\x9C\xC9\xE3\xF7\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '.':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xE7\xE7\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case ':':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xE7\xE7\xFF\xFF\xFF\xE7\xE7\xFF\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;
	default:
		break;
	}

	return info;
}

const FontInfo GetFontInfo_24Byte(uint32_t ch)
{
	FontInfo info = {0};
	switch(ch)
	{
	case (uint32_t)'温':
		info.data = (uint8_t*)"\x0D\xFC\xEB\xFD\x0F\xFC\xEE\xFD\x0D\xFC\xEB\xFD\xFD\xFF\x04\xF8\xD5\xFA\xD5\xFA\xD5\xFA\x01\xF0";
		info.w = 16;
		info.h = 12;
		break;

	case (uint32_t)'度':
		info.data = (uint8_t*)"\xBF\xFF\x03\xF8\xDB\xFE\x03\xF8\xDB\xFE\x1B\xFE\xFB\xFF\x0B\xFE\x5B\xFF\xBD\xFF\x4E\xFE\xF2\xF1";
		info.w = 16;
		info.h = 12;
		break;

	case (uint32_t)'湿':
		info.data = (uint8_t*)"\x0D\xF8\xEB\xFB\x0F\xF8\xEE\xFB\x0D\xF8\xEB\xFB\xBD\xF6\xB4\xFA\xAD\xFC\xAD\xFE\xBD\xFE\x01\xF0";
		info.w = 16;
		info.h = 12;
		break;

	default:
		break;
	}

	return info;
}