#include "Header.h"


static char Time_display[12] = "00:00:00.00"; 
static char Today_display[11] = "2000/00/00";
static char Temp_display[6] = "00.00";
static char Humi_display[6] = "00.00";

const uint8_t hot_reset[]  = {0xB5,0x62,0x06,0x04,0x04,0x00,0x00,0x00,0x02,0x00,0x10,0x68};
const uint8_t cold_reset[] = {0xB5,0x62,0x06,0x04,0x04,0x00,0xFF,0xFF,0x02,0x00,0x0E,0x61};

int main(void)
{
	LCD_Init();
	
	
	LCD_FillScreen(COLOR_WHITE);
	LCD_SetRotation(LCD_ROTATION_270);
	LCD_DrawString(80, 150, "Initializing...", COLOR_BLACK, COLOR_WHITE, 2);
	LCD_DrawString(250,230,"Version:1.0",COLOR_BLACK,COLOR_WHITE,1);
	LCD_DrawImage(130,80,60,60,(const uint16_t *)gImage_1);

	
	GPS_Init();
//	Serial_SendArray(hot_reset,sizeof(cold_reset));
	Init_DHT11_IO();
	Init_TIM2();
	

	Delay_ms(1000);
	
	//Temp UI design
	UI_Init_Display();
	
	while(1)
	{
		ParseNMEA_Data(gps_data.buf);

		//GPS data display
		UI_Time_Alti_Display(Time_display,Today_display);
		
		//DHT11 data display	
		UI_Temp_Humi_Display(Temp_display,Humi_display);
		UI_GPS_Power();

	}
}



