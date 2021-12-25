/*
 * WiFi.c
 *
 *  Created on: Jan 3, 2021
 *      Author: dybb8
 */


#include "WiFi.h"
#include <string.h>
#include "WifiCommand.h"
#include "../PM25/PM25Control.h"
#include "../CO2/CO2.h"

//用于接收数据
RecvData g_WifiRecvData;

WiFiStatus g_WifiStatus;

WifiCommandRoute g_WifiCommandRoute[] = {
		{"ready", pfnReadyStatus},
		{"WIFI CONNECTED", pfnConnectedStatus},
		{"WIFI GOT IP", pfnWifiGotIP},
		{"+CWJAP:", pfnWifiConnectInfo},
		{"+CIFSR:", pfnAddress},
		{"+CWSAP:", pfnAPInfo},
};

const char g_CommandReset[] = "AT+RST";
const char g_CommandConnectStatus[] = "AT+CWJAP?\r\n";
const char g_CommandQueryAddress[] = "AT+CIFSR\r\n";

const char g_Ready[] = "\r\n\r\n";

void InitWiFiModule()
{
	g_WifiStatus.IsReady = 0;
	g_WifiStatus.IsConnected = 0;
	g_WifiStatus.IsGotIP = 0;

	g_WifiRecvData.MaxSize = 1024;
	g_WifiRecvData.UsedSize = 0;

	//先断电
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

	//HAL_UART_RX_HALFCOMPLETE_CB_ID
	//HAL_UART_RegisterCallback(&huart2, HAL_UART_RX_HALFCOMPLETE_CB_ID, pfnWiFiHalfRecvCallback);
	HAL_UART_RegisterCallback(&huart2, HAL_UART_RX_COMPLETE_CB_ID, pfnWiFiRecvCallback);
	HAL_UART_RegisterCallback(&huart2, HAL_UART_ERROR_CB_ID, pfnWiFiErrorCallback);

	//HAL_UART_Transmit(&huart2, g_CommandReset, sizeof(g_CommandReset), -1);
	//HAL_UART_Receive_IT(&huart2, g_wifiRecvBuff, 1024);

	//启动
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	StartWaitWifiData();
}

void StartWaitWifiData()
{
	g_WifiRecvData.MaxSize = 1024;
	g_WifiRecvData.UsedSize = 0;
	memset(g_WifiRecvData.buff,0,1024);

	ProcessWifiData();
}

int IsWiFiReady()
{
	return g_WifiStatus.IsReady;
}

int IsWiFiConnected()
{
	return g_WifiStatus.IsConnected;
}

int IsGotIP()
{
	return g_WifiStatus.IsGotIP;
}

void ProcessWifiData()
{
	while(1)
	{
		HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff, 1024, 5000);
		if(status != HAL_TIMEOUT && status != HAL_OK)
		{
			break;
		}

		uint16_t count = huart2.RxXferSize - huart2.RxXferCount;
		if(count == 0)
		{
			break;
		}

		g_WifiRecvData.buff[count] = 0;

		pfnProcessEvent(g_WifiRecvData.buff);
		if(g_WifiStatus.IsReady == 1)
		{
			pfnClientRecv(g_WifiRecvData.buff);
		}
	}
}

void GetConnectedWiFiName()
{
	char addressBuff[128];
	memset(addressBuff, 0, 128);

	HAL_UART_Transmit(&huart2, (uint8_t*)g_CommandConnectStatus, sizeof(g_CommandConnectStatus) - 1, -1);
	HAL_UART_Receive(&huart2, (uint8_t*)addressBuff, 128, 1000);

	uint16_t count = huart2.RxXferSize - huart2.RxXferCount;
	addressBuff[count] = 0;
	pfnProcessEvent(addressBuff);
}

void GetNetworkAddress()
{
	char addressBuff[128];
	memset(addressBuff, 0, 128);
	HAL_UART_Transmit(&huart2, (uint8_t*)g_CommandQueryAddress, sizeof(g_CommandQueryAddress) - 1, -1);
	HAL_UART_Receive(&huart2, (uint8_t*)addressBuff, 128, 1000);

	uint16_t count = huart2.RxXferSize - huart2.RxXferCount;
	addressBuff[count] = 0;
	pfnProcessEvent(addressBuff);
}

void GetSSID()
{
	char addressBuff[128];
	memset(addressBuff, 0, 128);
	char getAPName[] = "AT+CWSAP?\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*)getAPName, sizeof(getAPName) - 1, -1);
	HAL_UART_Receive(&huart2, (uint8_t*)addressBuff, 128, 1000);

	uint16_t count = huart2.RxXferSize - huart2.RxXferCount;
	addressBuff[count] = 0;
	pfnProcessEvent(addressBuff);
}

void SetMUXMode()
{
	char setMuxMode[] = "AT+CIPMUX=1\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*)setMuxMode, sizeof(setMuxMode) - 1, -1);
	HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff, 1024, 1000);
}

void StartServerMode()
{
	char setServerMode[] = "AT+CIPSERVER=1,80\r\n";
	HAL_UART_Transmit(&huart2, (uint8_t*)setServerMode, sizeof(setServerMode) - 1, -1);
	HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff, 1024, 1000);
}

void pfnProcessEvent(char* buff)
{
	char* pOffset = buff;
	for(int i = 0; i < sizeof(g_WifiCommandRoute)/sizeof(WifiCommandRoute); ++i)
	{
		char *p = strstr(pOffset, g_WifiCommandRoute[i].Command);
		if(p != 0)
		{
			g_WifiCommandRoute[i].pfnCallback(p);
		}
	}
}

int pfnClientRecv(char* buff)
{
	int id = -1;

	if(strstr(buff, "CLOSED") != 0)
	{
		return -1;
	}

	if(strstr(buff, "FAIL") != 0)
	{
		return -1;
	}

	sscanf(buff, "%d,CONNECT", &id);

	do
	{
		if(id < 0)
		{
			break;
		}

		char retBuff[1024] = {0};
		sprintf(retBuff,
				"{\"pm1\":%ld,\"pm2.5\":%ld,\"pm10\":%ld,\"tvoc\":%ld,\"Temperature\":%ld,\"Tumidity\":%ld,\"co2\":%lu}",
				g_AirQuality.PM1,
				g_AirQuality.PM2_5,
				g_AirQuality.PM10,
				g_AirQuality.TVOC,
				g_AirQuality.Temperature,
				g_AirQuality.Tumidity,
				g_u32Co2Value);

		char sendCommond[256] = {0};
		sprintf(sendCommond,"AT+CIPSEND=%d,%d\r\n", id, strlen(retBuff));
		HAL_UART_Transmit(&huart2, (uint8_t*)sendCommond, strlen(sendCommond), -1);

		g_WifiRecvData.MaxSize = 1024;
		g_WifiRecvData.UsedSize = 0;
		while(1)
		{
			HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff + g_WifiRecvData.UsedSize, 1024 - g_WifiRecvData.UsedSize, 1000);
			uint32_t count = huart2.RxXferSize - huart2.RxXferCount;
			if(count == 0)
			{
				break;
			}
			g_WifiRecvData.UsedSize += count;

			g_WifiRecvData.buff[g_WifiRecvData.UsedSize] = 0;
			char* pOffset = strstr(g_WifiRecvData.buff, ">");
			if(pOffset != 0)
			{
				break;
			}
		}

		HAL_UART_Transmit(&huart2, (uint8_t*)retBuff, strlen(retBuff), -1);
		while(1)
		{
			HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff, 1024, 1000);
			uint32_t count = huart2.RxXferSize - huart2.RxXferCount;
			if(count == 0)
			{
				break;
			}

			g_WifiRecvData.buff[count] = 0;
			char* pOffset = strstr(g_WifiRecvData.buff, "SEND OK");
			if(pOffset != 0)
			{
				break;
			}
		}
	}while(0);
	return id;
}

void pfnWiFiRecvCallback(struct __UART_HandleTypeDef *huart)
{
	uint16_t count = huart2.RxXferSize - huart2.RxXferCount;
	do {
		if(count == 0)
		{		
			break;
		}

		g_WifiRecvData.buff[count + 1] = 0;

		pfnProcessEvent(g_WifiRecvData.buff);
		pfnClientRecv(g_WifiRecvData.buff);
	} while (0);

	StartWaitWifiData();
}

void pfnWiFiErrorCallback(struct __UART_HandleTypeDef *huart)
{
	StartWaitWifiData();
}
