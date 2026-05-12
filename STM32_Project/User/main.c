#include "Header.h"


int main(void)
{
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	
	LCD_Init();
	LCD_FillScreen(COLOR_BLACK);
	LCD_DrawString(30, 130, "Initializing...", COLOR_WHITE, COLOR_BLACK, 1);
	Delay_ms(2000);
	
	while(1)
	{
		LCD_FillScreen(COLOR_BLUE);
		LCD_FillScreen(COLOR_BLACK);
		LCD_FillScreen(COLOR_CYAN);
		LCD_FillScreen(COLOR_GRAY);
		LCD_FillScreen(COLOR_GREEN);
	}
}



