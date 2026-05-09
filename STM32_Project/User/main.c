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
	

	TFT_Init();
	TFT_FullScreen(COLOR_RED);
	TFT_FullScreen(COLOR_BLUE);
	TFT_FullScreen(COLOR_BLACK);

	
	TFT_ShowImage(60, 150, 50, 50, gImage_1);

	while(1)
	{
		TFT_DrawChar16(10, 300, tfont16, COLOR_WHITE, COLOR_BLACK);
	}
}



