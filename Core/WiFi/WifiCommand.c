/*
 * WifiCommand.c
 *
 *  Created on: Jan 3, 2021
 *      Author: dybb8
 */

#include "WiFi.h"
#include "WifiCommand.h"
#include <string.h>

void pfnReadyStatus(char* buff)
{
	g_WifiStatus.IsReady = 1;
}

void pfnConnectedStatus(char* buff)
{
	g_WifiStatus.IsConnected = 1;
}

void pfnDisconnectedStatus(char* buff)
{
	g_WifiStatus.IsConnected = 0;
}

void pfnWifiGotIP(char* buff)
{
	g_WifiStatus.IsGotIP = 1;

	GetNetworkAddress();

	GetConnectedWiFiName();

	GetSSID();

	SetMUXMode();
	StartServerMode();
}

void pfnWifiConnectInfo(char* buff)
{
	char* strWifiNameBegin = strstr(buff, "\"");
	char* strWifiNameEnd = 0;
	do
	{
		if(strWifiNameBegin == 0)
		{
			break;
		}

		strWifiNameEnd = strstr(strWifiNameBegin + 1, "\"");
		if(strWifiNameEnd == 0)
		{
			break;
		}

		int i = 0;
		for(char *pOffset = strWifiNameBegin + 1; pOffset < strWifiNameEnd; ++pOffset)
		{
			g_WifiStatus.Name[i] = *pOffset;
			++i;
		}
	}while(0);
}

void pfnAddress(char* buff)
{
	char* pStart = strstr(buff, ":STAIP");
	if(pStart != 0)
	{
		//IP
		int i = 0;
		for(pStart += 8; *pStart != '\"'; ++pStart, ++i)
		{
			g_WifiStatus.Address[i] = *pStart;
		}

	}

	pStart = strstr(buff, ":STAMAC");
	if(pStart != 0)
	{
		//MAC
		int i = 0;
		for(pStart += 9; *pStart != '\"'; ++pStart, ++i)
		{
			g_WifiStatus.Mac[i] = *pStart;
		}

	}

	pStart = strstr(buff, ":APIP");
	if(pStart != 0)
	{
		int i = 0;
		for(pStart += 7; *pStart != '\"'; ++pStart, ++i)
		{
			g_WifiStatus.APAddress[i] = *pStart;
		}
	}
}

void pfnAPInfo(char* buff)
{
	char* ssidStart = strstr(buff, "\"");
	if(ssidStart != 0)
	{
		int i = 0;
		for(ssidStart += 1; *ssidStart != '\"'; ++ssidStart, ++i)
		{
			g_WifiStatus.SSID[i] = *ssidStart;
		}
	}
}

