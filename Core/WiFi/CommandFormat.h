#ifndef _COMMAND_FORMAT_H
#define _COMMAND_FORMAT_H

#include <stdint.h>
#include <stddef.h>

#define COMMAND_HEADER ((uint32_t)'lleH')

enum
{
    COMMAND_TYPE_UNKNOWN = 0,
    COMMAND_TYPE_WiFi_CONNECTED,
    COMMAND_TYPE_WiFi_GET_IP,
    COMMAND_TYPE_GET_AIR_QUALITY,
    COMMAND_TYPE_UPDATE_AIR_QUALITY,
    COMMAND_TYPE_COMMAND_INVALID,
};
typedef uint32_t CmdType;

typedef struct _CommandHeader
{
    uint32_t Flag;
    CmdType Type;
    // 不包含struct _CommandHeader的大小
    uint32_t Size;
}CommandHeader;

// Command Data

// ESP8266 -> UART
typedef struct _CommandWifiConnected
{
    // Nothing
}CommandWifiConnected;

typedef struct _CommandWiFiGetIp
{
    uint32_t IsIPv4;
}CommandWiFiGetIp;

/*
    ESP8266 -> 
*/
typedef struct _CommandWiFiGetIpReply
{
    // 1: Ipv4, 0: Ipv6
    int IsIpv4;
    union _IpAddress
    {
        char Ipv4[16];
        char Ipv6[40];
    }IpAddress;
}CommandWiFiGetIpReply;

// ESP8266 <-
typedef struct _CommandGetAirQuality
{
    // Nothing
}CommandGetAirQuality;

// ESP8266 ->
typedef struct _CommandGetAirQualityReply
{
    // char count
    uint32_t Len;
    // char data[0];
}CommandGetAirQualityReply;

typedef struct _CommandUpdateAirQuality
{
    uint32_t PM1;
	uint32_t PM2_5;
	uint32_t PM10;
	uint32_t TVOC;			//室内挥发有机物
	uint32_t Temperature;	//温度
	uint32_t Humidity;		//湿度
    uint32_t CO2;
}CommandUpdateAirQuality;

typedef struct _CommandInvalidReply
{
    uint32_t Len;
    // char buff[];
}CommandInvalidReply;

#endif