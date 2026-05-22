#include "UART.h"


char USART_RX_BUF[USART_REC_LEN];
uint8_t Rx_Buf_Point;


void Init_GPS_IO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	// TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 	// RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


//nomarl fanction to send data.

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array,uint16_t Lenght)
{
	uint16_t i;
	for(i=0;i<Lenght;i++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for(i=0;String[i]!=0;i++)
	{
		Serial_SendByte(String[i]);
	}
}


int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}



void Serial_printf(char *format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_SendString(String);
}





void USART1_IRQHandler(void)
{
	uint8_t Rx_Buf;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Rx_Buf = USART_ReceiveData(USART1);
		
		if(Rx_Buf == '$')
		{
			Rx_Buf_Point = 0;
		}
		else if(Rx_Buf_Point < USART_REC_LEN - 1)
		{
			USART_RX_BUF[Rx_Buf_Point] = Rx_Buf;
			Rx_Buf_Point++;
		}
		if(Rx_Buf == '*')	//Recive over
		{
			strncpy(gps_data.buf, (char*)USART_RX_BUF, MIN(Rx_Buf_Point, GPS_BUF_MAX_LEN - 1));
			gps_data.is_data_ready = true;
		}
		
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}




