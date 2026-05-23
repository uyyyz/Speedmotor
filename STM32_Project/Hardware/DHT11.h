#ifndef __DHT11_H
#define __DHT11_H

#include "Header.h"

#define DHT11_GPIO_PORT	GPIOA
#define DHT11_inout		GPIO_Pin_12

/* DHT11 数据结构体 */
typedef struct
{
	uint8_t humidity_int;		/* 湿度整数部分 */
	uint8_t humidity_dec;		/* 湿度小数部分 */
	uint8_t temperature_int;	/* 温度整数部分 */
	uint8_t temperature_dec;	/* 温度小数部分 */
	uint8_t checksum;			/* 校验和 */
} DHT11_Data;

extern DHT11_Data DHT11_LastData;

/* 函数声明 */
void Init_DHT11_IO(void);
uint8_t DHT11_Read(DHT11_Data *data);
uint8_t DHT11_GetTemperature(void);
uint8_t DHT11_GetHumidity(void);





#endif


