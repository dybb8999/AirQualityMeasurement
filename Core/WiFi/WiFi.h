/*
 * WiFi.h
 *
 *  Created on: Jan 3, 2021
 *      Author: dybb8
 */

#ifndef WIFI_WIFI_H_
#define WIFI_WIFI_H_

#include "../Inc/main.h"
#include <stdint.h>

typedef struct _RecvData
{
	char buff[1024];
	uint32_t MaxSize;
	uint32_t UsedSize;
}RecvData, *PRecvData;

typedef struct _WiFiStatus
{
	int IsReady;
	int IsConnected;
	int IsGotIP;
	//WIFI Name
	char Name[32];

	//AP 热点的IP
	char APAddress[32];

	//WIFI Info
	char SSID[32];
	//IP Address
	char Address[32];
	//MAC Address
	char Mac[32];
}WiFiStatus, *PWiFiStatus;

typedef struct _WifiCommandRoute
{
	char *Command;
	void(*pfnCallback)(char* buff);
}WifiCommandRoute, *PWifiCommandRoute;

extern WiFiStatus g_WifiStatus;

void InitWiFiModule();

int IsWiFiReady();
int IsWiFiConnected();
int IsGotIP();

void ProcessWifiData();

void pfnWiFiRecvCallback(struct __UART_HandleTypeDef *huart);
void pfnWiFiHalfRecvCallback(struct __UART_HandleTypeDef *huart);
void pfnWiFiErrorCallback(struct __UART_HandleTypeDef *huart);

void pfnProcessEvent(char* buff);
int pfnClientRecv(char* buff);

void StartWaitWifiData();

void GetConnectedWiFiName();
void GetNetworkAddress();
void GetSSID();
void SetMUXMode();
void StartServerMode();

#endif /* WIFI_WIFI_H_ */
