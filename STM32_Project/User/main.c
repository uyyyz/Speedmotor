#include "Header.h"


static char Time_display[20] = "00:00:00.00"; 
static char Today_display[10] = "2000/00/00";

int main(void)
{
	LCD_Init();
	
	
	LCD_FillScreen(COLOR_WHITE);
	LCD_SetRotation(LCD_ROTATION_270);
	LCD_DrawString(80, 150, "Initializing...", COLOR_BLACK, COLOR_WHITE, 2);
	LCD_DrawString(250,230,"Version:1.0",COLOR_BLACK,COLOR_WHITE,1);
	LCD_DrawImage(130,80,60,60,(const uint16_t *)gImage_1);
	Delay_ms(500);
	
	GPS_Init();
	
	//Temp UI design
	LCD_FillScreen(COLOR_BLACK);
	LCD_DrawString(10,10,"NOW:",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(65,10,"00",COLOR_WHITE,COLOR_BLACK,4);			//Now Time
	LCD_DrawString(120,25,"km/h",COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(10,70,"AVG:",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(65,70,"00",COLOR_WHITE,COLOR_BLACK,4);			//AVG Time
	LCD_DrawString(120,85,"km/h",COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(210,10,"Total:",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(205,35,"000",COLOR_WHITE,COLOR_BLACK,4);		//Total Speed
	LCD_DrawString(280,55,"km",COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(185,75,"00:00",COLOR_WHITE,COLOR_BLACK,4);		//Total Time
	
	LCD_DrawString(10,125,"Temp:",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,125,"00.00",COLOR_WHITE,COLOR_BLACK,2);		//temperature
	LCD_DrawString(130,125,"C",COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(10,150,"Humi:",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,150,"00.00",COLOR_WHITE,COLOR_BLACK,2);		//humidity
	LCD_DrawString(130,150,"%",COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(10,175,"Alti:",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,175,"000.0",COLOR_WHITE,COLOR_BLACK,2);		//Altitude
	LCD_DrawString(130,175,"m",COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(105,200,"-T I M E-",COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(20,220,Today_display,COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(160,220,Time_display,COLOR_WHITE,COLOR_BLACK,2);			//Real-Time
	
	LCD_FillRect(150,15,2,3,COLOR_RED);
	
	while(1)
	{
//		LCD_FillScreen(COLOR_BLUE);
//		Delay_ms(1000);
//		LCD_FillScreen(COLOR_BLACK);
//		Delay_ms(2000);
//		LCD_FillScreen(COLOR_CYAN);
//		Delay_ms(2000);
//		LCD_FillScreen(COLOR_GRAY);
//		Delay_ms(2000);
//		LCD_FillScreen(COLOR_GREEN);
//		Delay_ms(2000);
		

		ParseNMEA_Data(gps_data.buf);
		sprintf(Time_display,"%c%c:%c%c:%c%c.%c%c",gps_data.utc_time[0],gps_data.utc_time[1],gps_data.utc_time[2],gps_data.utc_time[3],gps_data.utc_time[4],gps_data.utc_time[5],gps_data.utc_time[7],gps_data.utc_time[8]);
		LCD_DrawString(160,220,Time_display,COLOR_WHITE,COLOR_BLACK,2);
		sprintf(Today_display,"20%c%c/%c%c/%c%c",gps_data.utc_today[4],gps_data.utc_today[5],gps_data.utc_today[2],gps_data.utc_today[3],gps_data.utc_today[0],gps_data.utc_today[1]);
		LCD_DrawString(20,220,Today_display,COLOR_WHITE,COLOR_BLACK,2);
		
		LCD_DrawString(70,175,gps_data.altitude,COLOR_WHITE,COLOR_BLACK,2);
		
	}
}



