#include "PM25Control.h"
#include <string.h>
#include "main.h"


AirQuality g_AirQuality;

//用于接收数据的缓冲区 不公开
AirQualityRecvData g_AirQualityRecvData;

void InitAirQuality()
{
	memset(&g_AirQuality, 0, sizeof(g_AirQuality));
	//HAL_UART_RegisterCallback(&huart5, HAL_UART_RX_COMPLETE_CB_ID, pfnAirQualityRecvCallback);
	//HAL_UART_RegisterCallback(&huart5, HAL_UART_ERROR_CB_ID, pfnAirQualityErrorCallback);

	//HAL_UART_Receive_IT(&huart5, (uint8_t*)&g_AirQualityRecvData, sizeof(g_AirQualityRecvData));
}

void pfnAirQualityRecvCallback(struct __UART_HandleTypeDef *huart)
{
	if(huart != &huart5)
	{
		return;
	}

	g_AirQuality.PM1 = htons(g_AirQualityRecvData.PM1Value.data);
	g_AirQuality.PM2_5 = htons(g_AirQualityRecvData.PM2_5Value.data);
	g_AirQuality.PM10 = htons(g_AirQualityRecvData.PM10Value.data);
	g_AirQuality.TVOC = htons(g_AirQualityRecvData.TOVC.data);
	g_AirQuality.Temperature = htons(g_AirQualityRecvData.Temperature.data) / 10;
	g_AirQuality.Tumidity = htons(g_AirQualityRecvData.Tumidity.data) / 10;

	HAL_UART_Receive_IT(&huart5, (uint8_t*)&g_AirQualityRecvData, sizeof(g_AirQualityRecvData));
}

void pfnAirQualityErrorCallback(struct __UART_HandleTypeDef *huart)
{
	if(huart != &huart5)
	{
		return;
	}
	HAL_UART_Receive_IT(&huart5, (uint8_t*)&g_AirQualityRecvData, sizeof(g_AirQualityRecvData));
}

uint16_t htons(uint16_t data)
{
	uint16_t low = data & 0xFF;
	data = data >> 8;
	low = low << 8;
	data |= low;
	return data;
}

void UpdataPM25Data()
{
	uint8_t buff[128] = {0};
	HAL_UART_Receive(&huart5, buff, sizeof(buff), 2000);

	uint32_t count = huart5.RxXferSize - huart5.RxXferCount;
	for(uint32_t i = 0; i < count; ++i)
	{
		if(buff[i] == 0x42 && buff[i+1] == 0x4D)
		{
			memcpy(&g_AirQualityRecvData, &buff[i], sizeof(g_AirQualityRecvData));

			g_AirQuality.PM1 = htons(g_AirQualityRecvData.PM1Value.data);
			g_AirQuality.PM2_5 = htons(g_AirQualityRecvData.PM2_5Value.data);
			g_AirQuality.PM10 = htons(g_AirQualityRecvData.PM10Value.data);
			g_AirQuality.TVOC = htons(g_AirQualityRecvData.TOVC.data);
			g_AirQuality.Temperature = htons(g_AirQualityRecvData.Temperature.data) / 10;
			g_AirQuality.Tumidity = htons(g_AirQualityRecvData.Tumidity.data) / 10;
			break;
		}
	}
}
