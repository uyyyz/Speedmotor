#ifndef __GPS_H
#define __GPS_H

#include "Header.h"

#define GPS_BUF_MAX_LEN    128    // GPS接收缓冲区
#define UTC_TIME_MAX_LEN   10     // UTC时间缓冲区
#define LATITUDE_MAX_LEN   10     // 纬度缓冲区
#define LONGITUDE_MAX_LEN  11     // 经度缓冲区
#define HEMISPHERE_MAX_LEN 2      // 半球缓冲区
#define MIN(a, b)          ((a) < (b) ? (a) : (b)) // 安全取最小值宏

typedef struct{
	char buf[GPS_BUF_MAX_LEN];            	// 原始NMEA语句
	char utc_today[6];					// 日期
	char utc_time[UTC_TIME_MAX_LEN];      	// UTC时间
	char latitude[LATITUDE_MAX_LEN];       	// 纬度
	char longitude[LONGITUDE_MAX_LEN];     	// 经度
	char ns_hemisphere[HEMISPHERE_MAX_LEN];	// N/S
	char ew_hemisphere[HEMISPHERE_MAX_LEN];	// E/W
	char altitude[6];					// 海拔高度
	char now_speed[6];					// 瞬时速度
	bool is_data_ready;                    	// 原始数据已接收完毕
	bool is_valid;                         	// 定位是否有效
	uint8_t satellite_count;               	// 卫星数量（从GGA获取）
}GPS_DataTypedef;

extern GPS_DataTypedef gps_data;


void ParseNMEA_Data(const char* buffer);

void GPS_Init(void);


#endif

