#include "DHT11.h"

/* 静态数据存储最后一次读取的数据 */
DHT11_Data DHT11_LastData;

/* 辅助函数：设置引脚为输出模式 */
static void DHT11_SetOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = DHT11_inout;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/* 辅助函数：设置引脚为输入模式 */
static void DHT11_SetInput(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = DHT11_inout;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/* 辅助函数：读取引脚电平 */
static uint8_t DHT11_ReadPin(void)
{
	return GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_inout);
}

/* 辅助函数：设置引脚电平 */
static void DHT11_SetPin(uint8_t level)
{
	if(level)
		GPIO_SetBits(DHT11_GPIO_PORT, DHT11_inout);
	else
		GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_inout);
}

/* 初始化DHT11 IO */
void Init_DHT11_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DHT11_inout;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
	
	DHT11_SetPin(1);
	Delay_ms(1);
}

/* 等待引脚变化 */
static uint8_t DHT11_WaitFor(uint8_t level, uint32_t timeout)
{
	uint32_t count = 0;
	
	while(DHT11_ReadPin() != level)
	{
		if(count++ > timeout)
			return 0;
		Delay_us(1);
	}
	
	return 1;
}

/* 读取一个字节的数据 */
static uint8_t DHT11_ReadByte(void)
{
	uint8_t byte = 0;
	uint32_t i;
	
	for(i = 0; i < 8; i++)
	{
		/* 等待引脚拉低 */
		if(!DHT11_WaitFor(0, 100))
			return 0;
		
		/* 等待引脚拉高 */
		if(!DHT11_WaitFor(1, 100))
			return 0;
		
		/* 延迟测量 */
		Delay_us(40);
		
		/* 读取电平 - 高于40us认为是1，否则是0 */
		byte <<= 1;
		if(DHT11_ReadPin())
			byte |= 1;
		
		/* 等待引脚拉低 */
		if(!DHT11_WaitFor(0, 100))
			return 0;
	}
	
	return byte;
}

/* DHT11读取函数 */
uint8_t DHT11_Read(DHT11_Data *data)
{
	uint8_t checksum;
	
	if(data == NULL)
		return 2;
	
	/* 设置为输出模式 */
	DHT11_SetOutput();
	
	/* 主机拉低信号至少18ms */
	DHT11_SetPin(0);
	Delay_ms(20);
	
	/* 释放引脚 */
	DHT11_SetPin(1);
	Delay_us(30);
	
	/* 设置为输入模式等待DHT11响应 */
	DHT11_SetInput();
	
	/* 等待DHT11拉低信号（60-80us） */
	if(!DHT11_WaitFor(0, 100))
		return 2;  /* 超时 */
	
	/* 等待DHT11释放信号 */
	if(!DHT11_WaitFor(1, 100))
		return 2;  /* 超时 */
	
	/* 读取5个字节数据 */
	data->humidity_int = DHT11_ReadByte();
	data->humidity_dec = DHT11_ReadByte();
	data->temperature_int = DHT11_ReadByte();
	data->temperature_dec = DHT11_ReadByte();
	data->checksum = DHT11_ReadByte();
	
	/* 设置为输出模式，释放总线 */
	DHT11_SetOutput();
	DHT11_SetPin(1);
	
	/* 校验数据 */
	checksum = data->humidity_int + data->humidity_dec + 
	           data->temperature_int + data->temperature_dec;
	
	if(checksum != data->checksum)
		return 1;  /* 校验失败 */
	
	/* 保存数据 */
	DHT11_LastData = *data;
	
	return 0;  /* 成功 */
}

/* 获取温度 */
uint8_t DHT11_GetTemperature(void)
{
	DHT11_Data data;
	
	if(DHT11_Read(&data) == 0)
		return data.temperature_int;
	
	return 0;
}

/* 获取湿度 */
uint8_t DHT11_GetHumidity(void)
{
	DHT11_Data data;
	
	if(DHT11_Read(&data) == 0)
		return data.humidity_int;
	
	return 0;
}



