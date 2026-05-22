#include "GPS.h"

GPS_DataTypedef gps_data = {0};



void GPS_Init(void)
{
	Init_GPS_IO();
	memset(&gps_data, 0, sizeof(GPS_DataTypedef));
	gps_data.is_data_ready = false;
	gps_data.is_valid = false;
	
}


void ParseNMEA_Data(const char* buffer)
{
	char temp_buf[GPS_BUF_MAX_LEN];
	strncpy(temp_buf, buffer, GPS_BUF_MAX_LEN - 1);
	temp_buf[GPS_BUF_MAX_LEN - 1] = '\0';		//Add a end symbol, Avoid crossing the boundary.
	
	char* token;
	char* fields[15] = {0};
	uint8_t idx = 0;
	
	token = strtok(temp_buf, ",");
	while (token != NULL && idx < 15)
	{
		fields[idx++] = token;
		token = strtok(NULL, ",");
	}
	
	//Reset RAM
//	memset(gps_data.utc_time, 0, sizeof(gps_data.utc_time));
//	memset(gps_data.latitude, 0, LATITUDE_MAX_LEN);
//	memset(gps_data.longitude, 0, LONGITUDE_MAX_LEN);
//	memset(gps_data.ns_hemisphere, 0, HEMISPHERE_MAX_LEN);
//	memset(gps_data.ew_hemisphere, 0, HEMISPHERE_MAX_LEN);
//	gps_data.is_valid = false;
	
	//		0	1	   2      3      4      5 6 7   8     9   10 11 1213 14  15
	//$GPGGA,060826.00,2236.91284,N,11403.24705,E,2,08,1.03,107.8,M,-2.4,M,,0000*4A\r\n
	if(strstr(buffer, "GGA"))
	{
		if(fields[6] && fields[6][0] != '0')
		{
			gps_data.is_valid = true;
			
			if(fields[1]) strncpy(gps_data.utc_time, 	fields[1], sizeof(gps_data.utc_time) - 1);
			if(fields[2]) strncpy(gps_data.latitude, 	fields[2], LATITUDE_MAX_LEN 	- 1);
			if(fields[3]) strncpy(gps_data.ns_hemisphere,fields[3], HEMISPHERE_MAX_LEN - 1);
			if(fields[4]) strncpy(gps_data.longitude, 	fields[4], LONGITUDE_MAX_LEN 	- 1);
			if(fields[5]) strncpy(gps_data.ew_hemisphere,fields[5], HEMISPHERE_MAX_LEN - 1);
			
			if(fields[7]) gps_data.satellite_count = atoi(fields[7]);
			
			if(fields[9]) strncpy(gps_data.altitude,	fields[9], 6);
			
			
		}
	}
	if(strstr(buffer, "RMC"))
	{
		if(fields[2] && fields[2][0] == 'A')
		{
			gps_data.is_valid = true;
			
			if(fields[8]) strncpy(gps_data.utc_today,	fields[8], 6);
		}
	}
	if(strstr(buffer, "VTG"))
	{
		gps_data.is_valid = true;
		
		float speed_temp = atof(fields[5]);
		
		sprintf(gps_data.now_speed,"%05.2f",speed_temp);
	}
}



