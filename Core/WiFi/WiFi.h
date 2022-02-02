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

typedef struct _WiFiStatus
{
	//IP Address
	char Address[32];
}WiFiStatus, *PWiFiStatus;

extern WiFiStatus g_WifiStatus;

void InitWiFiModule();

void GetIp();

void UpdateAirQualityInfo(uint32_t pm1, uint32_t pm2_5, uint32_t pm10,uint32_t tvoc, uint32_t temperature, uint32_t humidity, uint32_t co2);

#endif /* WIFI_WIFI_H_ */
