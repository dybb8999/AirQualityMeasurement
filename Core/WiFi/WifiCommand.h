/*
 * WifiCommand.h
 *
 *  Created on: Jan 3, 2021
 *      Author: dybb8
 */

#ifndef WIFI_WIFICOMMAND_H_
#define WIFI_WIFICOMMAND_H_


void pfnReadyStatus(char* buff);

void pfnConnectedStatus(char* buff);

void pfnDisconnectedStatus(char* buff);

void pfnWifiGotIP(char* buff);

void pfnWifiConnectInfo(char* buff);

void pfnAddress(char* buff);

#endif /* WIFI_WIFICOMMAND_H_ */
