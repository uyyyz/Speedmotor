#ifndef __TFT_H
#define __TFT_H

#include "Header.h"

#define X_MAX_PIXEL             240
#define Y_MAX_PIXEL             320

#define TFT_PORT                GPIOA

#define TFT_CS_PIN              GPIO_Pin_8
#define TFT_DC_PIN              GPIO_Pin_9
#define TFT_RST_PIN             GPIO_Pin_10
#define TFT_SDA_PIN             GPIO_Pin_11
#define TFT_SCK_PIN             GPIO_Pin_12



//Init funcions
void TFT_Init(void);
void TFT_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end);

//Use in emergency
void TFT_WriteData(uint8_t data);
void TFT_WriteCommand(uint8_t command);

//User functions
void TFT_FullScreen(uint16_t color);
void TFT_DrawChar32(uint16_t x, uint16_t y,const unsigned char *font,uint16_t fg_color, uint16_t bg_color);
void TFT_DrawChar16(uint16_t x, uint16_t y,const unsigned char *font,uint16_t fg_color, uint16_t bg_color);
void TFT_ShowImage(uint16_t x, uint16_t y,uint8_t height , uint8_t width,const unsigned char *image);

//color define and ABC
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
#define COLOR_PURPLE  0x780F
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000
#define COLOR_YELLOW  0xFFE0
#define COLOR_CYAN    0x07FF



#endif
