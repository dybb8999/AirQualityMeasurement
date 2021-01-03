/*
 * CO2.h
 *
 *  Created on: Aug 30, 2020
 *      Author: dybb8
 */

#ifndef CO2_CO2_H_
#define CO2_CO2_H_

#include <stdint.h>
#include "../Inc/main.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_spi.h"
//#include "stm32f1xx_hal_usart.h"
#include "stm32f1xx_hal_uart.h"

typedef union _Co2ModuleSendData
{
	uint64_t u64Data;
	struct
	{
		uint64_t AddressCode : 8;
		uint64_t FunctionCode : 8;
		uint64_t StartAddress : 16;
		uint64_t DataLength : 16;
		uint64_t VerifyCodeLow : 8;
		uint64_t VerifyCodeHigh : 8;
	}Info;
}Co2ModuleSendData, *PCo2ModuleSendData;

typedef union _Co2ModuleRecvData
{
	uint64_t u64Data;
	struct
	{
		uint64_t StartPoint : 8;
		uint64_t AddressCode : 8;
		uint64_t FunctionCode : 8;
		uint64_t VerifyCount : 8;
		uint64_t Concentration : 16;
		uint64_t VerifyCodeLow : 8;
		uint64_t VerifyCodeHigh : 8;
	}Info;
}Co2ModuleRecvData, *PCo2ModuleRecvData;

extern uint32_t g_u32Co2Value;

void InitCo2Module();

void GetCo2Value();

void pfnCo2RecvCallback(struct __UART_HandleTypeDef *huart);

void pfnCo2ErrorCallback(struct __UART_HandleTypeDef *huart);

#endif /* CO2_CO2_H_ */
