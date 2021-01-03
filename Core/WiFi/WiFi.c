/*
 * WiFi.c
 *
 *  Created on: Jan 3, 2021
 *      Author: dybb8
 */


#include "WiFi.h"
#include <string.h>
#include "WifiCommand.h"

//用于接收数据
RecvData g_WifiRecvData;

WiFiStatus g_WifiStatus;

WifiCommandRoute g_WifiCommandRoute[] = {
		{"ready", pfnReadyStatus},
		{"WIFI CONNECTED", pfnConnectedStatus},
		{"WIFI DISCONNECT", pfnDisconnectedStatus},
		{"WIFI GOT IP", pfnWifiGotIP},
		{"+CWJAP:", pfnWifiConnectInfo},
		{"+CIFSR:",pfnAddress},
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
	//等待网卡启动
	while(1)
	{
		HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff, 1024, 100);
		if(status == HAL_TIMEOUT)
		{
			break;
		}
	}

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


void GetConnectedWiFiName()
{
	HAL_UART_Transmit(&huart2, g_CommandConnectStatus, sizeof(g_CommandConnectStatus) - 1, -1);
}

void GetNetworkAddress()
{
	memset(g_WifiRecvData.buff,0,1024);
	HAL_UART_Transmit(&huart2, g_CommandQueryAddress, sizeof(g_CommandQueryAddress) - 1, -1);

	HAL_UART_Receive(&huart2, (uint8_t*)g_WifiRecvData.buff, 1024, 2000);
	g_WifiRecvData.buff[huart2.RxXferCount] = 0;
	pfnProcessEvent(g_WifiRecvData.buff);
}

void pfnWiFiRecvCallback(struct __UART_HandleTypeDef *huart)
{
	//new commond
	if(g_WifiRecvData.buff[g_WifiRecvData.UsedSize] == '\n')
	{
		g_WifiRecvData.buff[g_WifiRecvData.UsedSize] = 0;
		g_WifiRecvData.UsedSize = 0;

		pfnProcessEvent(g_WifiRecvData.buff);
	}
	else if(g_WifiRecvData.buff[g_WifiRecvData.UsedSize] == '\r')
	{
		//什么也不做，去除\r
	}
	else
	{
		g_WifiRecvData.UsedSize++;
		if(g_WifiRecvData.UsedSize >= g_WifiRecvData.MaxSize)
		{
			g_WifiRecvData.UsedSize = 0;
		}
	}
	HAL_UART_Receive_IT(&huart2, g_WifiRecvData.buff + g_WifiRecvData.UsedSize, 1);
}

void pfnWiFiHalfRecvCallback(struct __UART_HandleTypeDef *huart)
{

}

void pfnWiFiErrorCallback(struct __UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(&huart2, g_WifiRecvData.buff + g_WifiRecvData.UsedSize, 1);
}

void pfnProcessEvent(uint8_t* buff)
{
	for(int i = 0; i < sizeof(g_WifiCommandRoute)/sizeof(WifiCommandRoute); ++i)
	{
		if(strstr(g_WifiRecvData.buff, g_WifiCommandRoute[i].Command) != 0)
		{
			g_WifiCommandRoute[i].pfnCallback(g_WifiRecvData.buff);
		}
	}
}
