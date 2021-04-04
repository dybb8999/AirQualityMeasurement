#ifndef PM25CONTROL_H_
#define PM25CONTROL_H_

#include <stdint.h>
#include "../Inc/main.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_spi.h"
//#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_uart.h"
typedef struct _AirQuality
{
	uint32_t PM1;
	uint32_t PM2_5;
	uint32_t PM10;
	uint32_t TVOC;			//室内挥发有机物
	uint32_t Temperature;	//温度
	uint32_t Tumidity;		//湿度
}AirQuality, *PAirQuality;

typedef struct _AirQualityRecvData
{
	//Must 0x42
	uint8_t Head1;
	//Must 0x4D
	uint8_t Head2;

	union _FrameSize
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}FrameSize;

	union _PM1Value
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}PM1Value;

	union _PM2_5Value
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}PM2_5Value;

	union _PM10Value
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}PM10Value;

	union _NumOfParticulatesBiggerThan0_3
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}NumOfParticulatesBiggerThan0_3;

	union _NumOfParticulatesBiggerThan0_5
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}NumOfParticulatesBiggerThan0_5;

	union _NumOfParticulatesBiggerThan1
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}NumOfParticulatesBiggerThan1;

	union _NumOfParticulatesBiggerThan2_5
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}NumOfParticulatesBiggerThan2_5;

	union _NumOfParticulatesBiggerThan5
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}NumOfParticulatesBiggerThan5;

	union _NumOfParticulatesBiggerThan10
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}NumOfParticulatesBiggerThan10;

	uint16_t reserve1;
	uint16_t reserve2;
	uint16_t reserve3;

	//Must 0x1E
	uint16_t Flag;

	union _Tumidity
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}Tumidity;

	union _Temperature
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}Temperature;

	union _TOVC
	{
		struct
		{
			uint8_t High;
			uint8_t Low;
		}HighAndLow;
		uint16_t data;
	}TOVC;

	//Not use
	uint16_t eCo2;

	uint16_t SumCheck;
}AirQualityRecvData, *PAirQualityRecvData;

//存放当前空气质量数据
extern AirQuality g_AirQuality;

//初始化PM2.5
void InitAirQuality();

//获取数据回调
void pfnAirQualityRecvCallback(struct __UART_HandleTypeDef *huart);

void pfnAirQualityErrorCallback(struct __UART_HandleTypeDef *huart);

uint16_t htons(uint16_t data);

void UpdataPM25Data();

#endif
