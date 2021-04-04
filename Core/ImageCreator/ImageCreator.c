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

uint32_t GetNextChat(const char* str, uint32_t* len)
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
	uint32_t len = 0;
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

	case 12:
		p = GetFontInfo_12Byte(ch);
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

	case '3':
		info.data = (uint8_t*)"\xE0\xEF\xF7\xF3\xEF\xEE\xF1\xFF";
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

	case 'b':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xF8\xF9\xF9\xE1\xC9\x99\x99\x99\x99\xC4\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'p':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xFF\xC4\x99\x99\x99\x99\x99\xC1\xF9\xF9\xF0";
		info.w = 8;
		info.h = 16;
		break;

	case 'm':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xFF\xC8\x80\x94\x94\x94\x94\x94\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'A':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xF7\xE3\xC9\x9C\x9C\x80\x9C\x9C\x9C\x9C\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'B':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC0\x99\x99\x99\xC1\x99\x99\x99\x99\xC0\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'C':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC3\x99\xBC\xFC\xFC\xFC\xFC\xBC\x99\xC3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'D':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xE0\xC9\x99\x99\x99\x99\x99\x99\xC9\xE0\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'E':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x80\x99\xB9\xE9\xE1\xE9\xF9\xB9\x99\x80\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'F':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x80\x99\xB9\xE9\xE1\xE9\xF9\xF9\xF9\xF0\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'G':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC3\x99\xBC\xFC\xFC\x84\x9C\x9C\x99\xA3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'H':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x9C\x9C\x9C\x80\x9C\x9C\x9C\x9C\x9C\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'I':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC3\xE7\xE7\xE7\xE7\xE7\xE7\xE7\xE7\xC3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'J':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x87\xCF\xCF\xCF\xCF\xCF\xCC\xCC\xCC\xE1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'K':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x98\x99\xC9\xC9\xE1\xE1\xC9\x99\x99\x98\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'L':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xF0\xF9\xF9\xF9\xF9\xF9\xF9\xB9\x99\x80\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'M':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x88\x80\x80\x94\x9C\x9C\x9C\x9C\x9C\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'N':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x98\x90\x80\x84\x8C\x9C\x9C\x9C\x9C\xFF\xFF\xFF";
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

	case 'Q':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9C\x9C\x9C\x9C\x9C\x94\x84\xC1\xCF\x8F\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'R':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC0\x99\x99\x99\xC1\xC9\x99\x99\x99\x98\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'S':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xC1\x9C\x9C\xF9\xE3\xCF\x9F\x9C\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'T':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x81\x81\xA5\xE7\xE7\xE7\xE7\xE7\xE7\xC3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'U':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x9C\x9C\x9C\x9C\x9C\x9C\x9C\x9C\xC1\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'V':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x9C\x9C\x9C\x9C\x9C\x9C\xC9\xE3\xF7\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'W':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x9C\x9C\x9C\x9C\x94\x94\x80\xC9\xC9\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'X':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x9C\x9C\xC9\xC9\xE3\xE3\xC9\xC9\x9C\x9C\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'Y':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x99\x99\x99\x99\xC3\xE7\xE7\xE7\xE7\xC3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case 'Z':
		info.data = (uint8_t*)"\xFF\xFF\xFF\x80\x9C\x9E\xCF\xE7\xF3\xF9\xBC\x9C\x80\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '-':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x80\xFF\xFF\xFF\xFF\xFF\xFF\xFF";
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

	case '/':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xBF\x9F\xCF\xE7\xF3\xF9\xFC\xFE\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case '%':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xFF\xFF\xBC\x9C\xCF\xE7\xF3\xF9\x9C\x9E\xFF\xFF\xFF";
		info.w = 8;
		info.h = 16;
		break;

	case (uint32_t)'温':
		info.data = (uint8_t*)"\xFF\xEF\x3D\xC0\xB3\xEF\xB7\xEF\x3E\xE0\xB9\xEF\xBB\xEF\x2F\xE0\xF7\xFF\x1B\xC0\xD8\xDA\xDB\xDA\xDB\xDA\xDB\xDA\x0B\x80\xFB\xFF";
		info.w = 16;
		info.h = 16;
		break;

	case (uint32_t)'度':
		info.data = (uint8_t*)"\x7F\xFF\xFF\xDE\x03\x80\xBB\xFB\xBB\xEB\x03\xC0\xBB\xFB\x3B\xF8\xFB\xFF\x0B\xF0\xBB\xFB\x7B\xFD\xFB\xFE\x3D\xF9\xCE\x87\xF3\xDF";
		info.w = 16;
		info.h = 16;
		break;

	case (uint32_t)'湿':
		info.data = (uint8_t*)"\xFF\xEF\x1D\xC0\xD3\xEF\xD7\xEF\x1E\xE0\xD9\xEF\xDB\xEF\x0F\xE0\x77\xFB\x7B\xFB\x68\xDB\x5B\xEB\x3B\xF3\x7B\xDB\x03\x80\xFB\xFF";
		info.w = 16;
		info.h = 16;
		break;
	default:
		break;
	}

	return info;
}

const FontInfo GetFontInfo_12Byte(uint32_t ch)
{
	FontInfo info = {0};
	switch(ch)
	{
	case 'u':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xCC\xDD\xDD\xDD\xCD\xD3\xFF\xFF\xFF";
		info.w = 8;
		info.h = 12;
		break;

	case 'g':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xD3\xCD\xDD\xDD\xDD\xC3\xDF\xE3\xFF";
		info.w = 8;
		info.h = 12;
		break;

	case 'm':
		info.data = (uint8_t*)"\xFF\xFF\xFF\xE8\xD5\xD5\xD5\xD5\xC0\xFF\xFF\xFF";
		info.w = 8;
		info.h = 12;
		break;
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
