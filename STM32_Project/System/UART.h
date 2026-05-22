#ifndef __UART_H
#define __UART_H


#include "Header.h"


#define USART_REC_LEN   200


void Serial_printf(char *format,...);
void Serial_SendArray(uint8_t *Array,uint16_t Lenght);

void Init_GPS_IO(void);



#endif

