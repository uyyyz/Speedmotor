#include "GPS_Display.h"



void UI_Init_Display(void)
{
	LCD_FillScreen(COLOR_BLACK);
	
	LCD_DrawString(10,10,	"NOW:",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(65,10,	"00",	COLOR_WHITE,COLOR_BLACK,4);			//Now Time
	LCD_DrawString(120,25,	"km/h",	COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(10,70,	"AVG:",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(65,70,	"00",	COLOR_WHITE,COLOR_BLACK,4);			//AVG Time
	LCD_DrawString(120,85,	"km/h",	COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(210,10,	"Total:",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(205,35,	"000",	COLOR_WHITE,COLOR_BLACK,4);		//Total Speed
	LCD_DrawString(280,55,	"km",	COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(185,75,	"00:00",	COLOR_WHITE,COLOR_BLACK,4);		//Total Time
	
	LCD_DrawString(10,125,	"Temp:",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,125,	"00.00",	COLOR_WHITE,COLOR_BLACK,2);		//temperature
	LCD_DrawString(130,125,	"C",		COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(10,150,	"Humi:",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,150,	"00.00",	COLOR_WHITE,COLOR_BLACK,2);		//humidity
	LCD_DrawString(130,150,	"%",		COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(10,175,	"Alti:",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,175,	"000.0",	COLOR_WHITE,COLOR_BLACK,2);		//Altitude
	LCD_DrawString(130,175,	"m",		COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(105,200,	"-T I M E-",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(20,220,	"2000/00/00",	COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(160,220,	"00:00:00.00",	COLOR_WHITE,COLOR_BLACK,2);			//Real-Time
	
	LCD_FillRect(155,12,3,7,COLOR_RED);
	LCD_FillRect(160,7,3,12,COLOR_RED);
	LCD_FillRect(165,2,3,17,COLOR_RED);
}


void UI_Time_Alti_Display(char *Time_display, char *Today_display)
{
	sprintf(Time_display,"%c%c:%c%c:%c%c.%c%c",gps_data.utc_time[0],gps_data.utc_time[1],gps_data.utc_time[2],gps_data.utc_time[3],gps_data.utc_time[4],gps_data.utc_time[5],gps_data.utc_time[7],gps_data.utc_time[8]);
	LCD_DrawString(160,220,Time_display,COLOR_WHITE,COLOR_BLACK,2);
	sprintf(Today_display,"20%c%c/%c%c/%c%c",gps_data.utc_today[4],gps_data.utc_today[5],gps_data.utc_today[2],gps_data.utc_today[3],gps_data.utc_today[0],gps_data.utc_today[1]);
	LCD_DrawString(20,220,Today_display,COLOR_WHITE,COLOR_BLACK,2);
	
	LCD_DrawString(70,175,gps_data.altitude,COLOR_WHITE,COLOR_BLACK,2);
}


void UI_Temp_Humi_Display(char *Temp, char *Humi)
{
	snprintf(Temp, sizeof(Temp), "%02d.%02d", DHT11_LastData.temperature_int, DHT11_LastData.temperature_dec);
	snprintf(Humi, sizeof(Humi), "%02d.%02d", DHT11_LastData.humidity_int, DHT11_LastData.humidity_dec);
	LCD_DrawString(70,125,Temp,COLOR_WHITE,COLOR_BLACK,2);
	LCD_DrawString(70,150,Humi,COLOR_WHITE,COLOR_BLACK,2);
}


void UI_GPS_Power(void)
{
	if(gps_data.satellite_count >= 10)
	{
		LCD_FillRect(155,12,3,7,COLOR_RED);
		LCD_FillRect(160,7,3,12,COLOR_RED);
		LCD_FillRect(165,2,3,17,COLOR_RED);
	}
	else if(gps_data.satellite_count >= 5)
	{
		LCD_FillRect(155,12,3,7,COLOR_RED);
		LCD_FillRect(160,7,3,12,COLOR_RED);
		LCD_FillRect(165,2,3,17,COLOR_BLACK);
	}
	else if(gps_data.satellite_count > 0)
	{
		LCD_FillRect(155,12,3,7,COLOR_RED);
		LCD_FillRect(160,7,3,12,COLOR_BLACK);
		LCD_FillRect(165,2,3,17,COLOR_BLACK);
	}
	else if(gps_data.satellite_count == 0)
	{
		LCD_FillRect(155,12,3,7,COLOR_BLACK);
		LCD_FillRect(160,7,3,12,COLOR_BLACK);
		LCD_FillRect(165,2,3,17,COLOR_BLACK);
	}
}


void UI_Now_Speed(void)
{
	
}










