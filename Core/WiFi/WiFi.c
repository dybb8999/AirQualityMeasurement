/*
 * WiFi.c
 *
 *  Created on: Jan 3, 2021
 *      Author: dybb8
 */


#include "WiFi.h"
#include <string.h>
#include <stdio.h>
#include "../PM25/PM25Control.h"
#include "../CO2/CO2.h"
#include "CommandFormat.h"

WiFiStatus g_WifiStatus;

void InitWiFiModule()
{
	//先断电
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

	//HAL_UART_RX_HALFCOMPLETE_CB_ID
	//HAL_UART_RegisterCallback(&huart2, HAL_UART_RX_HALFCOMPLETE_CB_ID, pfnWiFiHalfRecvCallback);
	// HAL_UART_RegisterCallback(&huart2, HAL_UART_RX_COMPLETE_CB_ID, pfnWiFiRecvCallback);
	// HAL_UART_RegisterCallback(&huart2, HAL_UART_ERROR_CB_ID, pfnWiFiErrorCallback);

	//HAL_UART_Transmit(&huart2, g_CommandReset, sizeof(g_CommandReset), -1);
	//HAL_UART_Receive_IT(&huart2, g_wifiRecvBuff, 1024);
	HAL_Delay(100);
	//启动
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	// 等待初始化完毕
	uint8_t buff[1024];
	HAL_UART_Receive(&huart2, buff, sizeof(buff), 5000);
	// StartWaitWifiData();
	GetIp();
}

void GetIp()
{

	uint8_t buff[128] = {0};
	CommandHeader* cmd_header = (CommandHeader*)buff;
	cmd_header->Flag = COMMAND_HEADER;
	cmd_header->Type = COMMAND_TYPE_WiFi_GET_IP;
	cmd_header->Size = sizeof(CommandWiFiGetIp);

	CommandWiFiGetIp* get_ip = (CommandWiFiGetIp*)(cmd_header+1);
	get_ip->IsIPv4 = 1;
	HAL_UART_Transmit(&huart2, buff, sizeof(CommandHeader) + sizeof(CommandWiFiGetIp), -1);
	HAL_UART_Receive(&huart2, buff, sizeof(CommandHeader) + sizeof(CommandWiFiGetIpReply), 1000);

	do
	{
		if(cmd_header->Flag != COMMAND_HEADER)
		{
			break;
		}

		CommandWiFiGetIpReply* reply = (CommandWiFiGetIpReply*)(cmd_header+1);
		strcpy(g_WifiStatus.Address, reply->IpAddress.Ipv4);
	}while(0);
}

void UpdateAirQualityInfo(uint32_t pm1, uint32_t pm2_5, uint32_t pm10,uint32_t tvoc, uint32_t temperature, uint32_t humidity, uint32_t co2)
{
	uint8_t buff[sizeof(CommandHeader) + sizeof(CommandUpdateAirQuality)] = {0};
	CommandHeader* cmd_header = (CommandHeader*)buff;
	cmd_header->Flag = COMMAND_HEADER;
	cmd_header->Type = COMMAND_TYPE_UPDATE_AIR_QUALITY;
	cmd_header->Size = sizeof(CommandUpdateAirQuality);

	CommandUpdateAirQuality* air_quality = (CommandUpdateAirQuality*)(cmd_header+1);
	air_quality->PM1 = pm1;
	air_quality->PM2_5 = pm2_5;
	air_quality->PM10 = pm10;
	air_quality->TVOC = tvoc;
	air_quality->Temperature = temperature;
	air_quality->Humidity = humidity;
	air_quality->CO2 = co2;

	HAL_UART_Transmit(&huart2, buff, sizeof(buff), -1);
}
