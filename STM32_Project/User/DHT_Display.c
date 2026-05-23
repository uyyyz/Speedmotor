#include "DHT_Display.h"

/* 数据存储 */
static DHT11_Display_Data g_DHT11_Data = {0, 0, 2};

/* TIM2初始化 - 每2秒触发一次中断 */
void Init_TIM2(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 启用TIM2时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* TIM2时基配置 */
	/* STM32F103: APB1时钟 = 72MHz (系统时钟) */
	/* 预分频器：71 -> 计数频率 = 72MHz / 72 = 1MHz（1us计数一次） */
	/* 自动重装载寄存器: 2000000 -> 2秒触发一次中断 */
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1;			/* 2秒 */
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;			/* 分频系数 */
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;			/* 时钟分频 */
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* 使能TIM2的更新中断 */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	/* NVIC配置 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 启动TIM2 */
	TIM_Cmd(TIM2, ENABLE);
}

/* TIM2中断处理程序 */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		/* 每2秒读取一次DHT11 */
		DHT11_Display_Update();
	}
}

/* 更新DHT11数据 */
void DHT11_Display_Update(void)
{
	DHT11_Data data;
	uint8_t result;
	
	/* 读取DHT11数据 */
	result = DHT11_Read(&data);
	
	/* 保存数据 */
	g_DHT11_Data.temperature = data.temperature_int;
	g_DHT11_Data.humidity = data.humidity_int;
	g_DHT11_Data.status = result;
	
	/* 可以在这里添加显示更新逻辑 */
	if(result == 0)
	{
		/* 读取成功 - 可以调用显示函数 */
		/* Display_DHT11(data.humidity_int, data.temperature_int); */
	}
	else if(result == 1)
	{
		/* 校验失败 */
	}
	else if(result == 2)
	{
		/* 超时 */
	}
}

/* 获取DHT11数据 */
DHT11_Display_Data* DHT11_Display_GetData(void)
{
	return &g_DHT11_Data;
}
