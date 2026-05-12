#ifndef __SPI_DMA_H
#define __SPI_DMA_H

#include "Header.h"

/*
 * 硬件连接定义 (SPI1)
 * ─────────────────────────────────────────
 *  ST7789 引脚   STM32F103 引脚   说明
 *  SCL/SCK    → PA5             SPI1_SCK
 *  SDA/MOSI   → PA7             SPI1_MOSI
 *  CS         → PA4             片选 (低有效)
 *  DC/RS      → PA3             数据/命令选择
 *  RST        → PA2             硬件复位 (低有效)
 *  BLK        → PA1             背光 (高有效, 可接PWM)
 *  VCC        → 3.3V
 *  GND        → GND
 * ─────────────────────────────────────────
 *
 * DMA 通道:
 *   SPI1_TX → DMA1 Channel3
 */

/* GPIO 端口 */
#define LCD_GPIO_PORT     GPIOA
#define LCD_GPIO_CLK      RCC_APB2Periph_GPIOA

/* 引脚定义 */
#define LCD_SCK_PIN       GPIO_Pin_5   /* PA5 - SPI1_SCK  */
#define LCD_MOSI_PIN      GPIO_Pin_7   /* PA7 - SPI1_MOSI */
#define LCD_CS_PIN        GPIO_Pin_4   /* PA4 - CS        */
#define LCD_DC_PIN        GPIO_Pin_3   /* PA3 - DC        */
#define LCD_RST_PIN       GPIO_Pin_2   /* PA2 - RST       */
#define LCD_BLK_PIN       GPIO_Pin_1   /* PA1 - BLK       */

/* 控制宏 */
#define LCD_CS_LOW()      GPIO_ResetBits(LCD_GPIO_PORT, LCD_CS_PIN)
#define LCD_CS_HIGH()     GPIO_SetBits(LCD_GPIO_PORT, LCD_CS_PIN)
#define LCD_DC_LOW()      GPIO_ResetBits(LCD_GPIO_PORT, LCD_DC_PIN)   /* 命令 */
#define LCD_DC_HIGH()     GPIO_SetBits(LCD_GPIO_PORT, LCD_DC_PIN)     /* 数据 */
#define LCD_RST_LOW()     GPIO_ResetBits(LCD_GPIO_PORT, LCD_RST_PIN)
#define LCD_RST_HIGH()    GPIO_SetBits(LCD_GPIO_PORT, LCD_RST_PIN)
#define LCD_BLK_ON()      GPIO_SetBits(LCD_GPIO_PORT, LCD_BLK_PIN)
#define LCD_BLK_OFF()     GPIO_ResetBits(LCD_GPIO_PORT, LCD_BLK_PIN)

/* SPI / DMA */
#define LCD_SPI           SPI1
#define LCD_SPI_CLK       RCC_APB2Periph_SPI1
#define LCD_DMA           DMA1
#define LCD_DMA_CLK       RCC_AHBPeriph_DMA1
#define LCD_DMA_CHANNEL   DMA1_Channel3   /* SPI1_TX */

/* DMA 传输完成标志 */
#define LCD_DMA_TC_FLAG   DMA1_FLAG_TC3

/* 公共 API */
void SPI_DMA_Init(void);
void SPI_DMA_SendByte(uint8_t data);
void SPI_DMA_SendBuffer(const uint8_t *buf, uint32_t len);
void SPI_DMA_SendBuffer16(const uint16_t *buf, uint32_t len);
void SPI_DMA_WaitDone(void);

/* 用于填充纯色时的辅助：重复发送同一字节 */
void SPI_DMA_FillColor(uint16_t color, uint32_t count);

#endif /* __SPI_DMA_H */

