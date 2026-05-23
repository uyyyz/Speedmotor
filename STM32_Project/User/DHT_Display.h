#ifndef __DHT_Display_H
#define __DHT_Display_H

#include "Header.h"
#include "DHT11.h"

/* DHT11 显示数据结构体 */
typedef struct
{
	uint8_t temperature;
	uint8_t humidity;
	uint8_t status;  /* 0: 成功, 1: 校验失败, 2: 超时 */
} DHT11_Display_Data;

/* 函数声明 */
void Init_TIM2(void);
void DHT11_Display_Update(void);
DHT11_Display_Data* DHT11_Display_GetData(void);

#endif

