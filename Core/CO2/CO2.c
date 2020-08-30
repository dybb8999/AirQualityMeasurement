/*
 * CO2.c
 *
 *  Created on: Aug 30, 2020
 *      Author: dybb8
 */


#include "CO2.h"
#include "../Inc/main.h"
#include "../PM25/PM25Control.h"
#include <string.h>

Co2ModuleRecvData g_Co2Data;

Co2ModuleSendData g_Co2SendData;

uint32_t g_u32Co2Value;

void InitCo2Module()
{
	g_u32Co2Value = 0;

	uint8_t moduleChange[] = {0xff, 0x01, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0xfb};
	memset(&g_Co2Data, 0, sizeof(g_Co2Data));
	memset(&g_Co2SendData, 0, sizeof(g_Co2SendData));

	//HAL_UART_Receive(&huart4, recvBuff, sizeof(recvBuff), -1);

	HAL_UART_Transmit(&huart4, moduleChange, sizeof(moduleChange), -1);
	//HAL_UART_Receive(&huart4, recvBuff, sizeof(recvBuff), -1);
	HAL_UART_RegisterCallback(&huart4, HAL_UART_RX_COMPLETE_CB_ID, pfnCo2RecvCallback);
	HAL_UART_RegisterCallback(&huart4, HAL_UART_ERROR_CB_ID, pfnCo2ErrorCallback);


	g_Co2SendData.Info.AddressCode = 1;
	g_Co2SendData.Info.FunctionCode = 3;
	g_Co2SendData.Info.StartAddress = htons(5);
	g_Co2SendData.Info.DataLength = htons(1);
	g_Co2SendData.Info.VerifyCodeLow = 0x94;
	g_Co2SendData.Info.VerifyCodeHigh = 0xB;

	HAL_UART_Transmit(&huart4, (uint8_t *)&g_Co2SendData, sizeof(g_Co2SendData), -1);
	//HAL_UART_Receive(&huart4, (uint8_t*)&g_Co2Data, sizeof(g_Co2Data), -1);
	HAL_UART_Receive_IT(&huart4, (uint8_t*)&g_Co2Data, sizeof(g_Co2Data));
}

void GetCo2Value()
{
	HAL_UART_Transmit(&huart4, (uint8_t *)&g_Co2SendData, sizeof(g_Co2SendData), -1);
	HAL_UART_Receive_IT(&huart4, (uint8_t*)&g_Co2Data, sizeof(g_Co2Data)-1);
}

void pfnCo2RecvCallback(struct __UART_HandleTypeDef *huart)
{
	g_u32Co2Value = htons(g_Co2Data.Info.Concentration);
}

void pfnCo2ErrorCallback(struct __UART_HandleTypeDef *huart)
{
	GetCo2Value();
}
