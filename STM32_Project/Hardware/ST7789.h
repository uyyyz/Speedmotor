#ifndef __ST7789_H
#define __ST7789_H

#include "Header.h"

/* ═══════════════════════════════════════════════════════════════════════
 *  屏幕分辨率（2.8 寸 ST7789，240×320）
 * ═══════════════════════════════════════════════════════════════════════ */
#define LCD_WIDTH   240
#define LCD_HEIGHT  320

/*
 * ST7789 驱动有时存在 20 像素的行偏移（取决于模组厂商）
 * 如果显示有偏移，取消注释并调整以下宏
 */
/* #define LCD_COL_OFFSET  0  */
/* #define LCD_ROW_OFFSET  0  */

/* ═══════════════════════════════════════════════════════════════════════
 *  ST7789 寄存器地址
 * ═══════════════════════════════════════════════════════════════════════ */
#define ST7789_NOP       0x00
#define ST7789_SWRESET   0x01  /* 软件复位 */
#define ST7789_RDDID     0x04
#define ST7789_RDDST     0x09
#define ST7789_SLPIN     0x10  /* 进入睡眠 */
#define ST7789_SLPOUT    0x11  /* 退出睡眠 */
#define ST7789_PTLON     0x12
#define ST7789_NORON     0x13  /* 正常显示 */
#define ST7789_INVOFF    0x20  /* 关闭反色 */
#define ST7789_INVON     0x21  /* 开启反色 */
#define ST7789_GAMSET    0x26
#define ST7789_DISPOFF   0x28  /* 关闭显示 */
#define ST7789_DISPON    0x29  /* 开启显示 */
#define ST7789_CASET     0x2A  /* 列地址设置 */
#define ST7789_RASET     0x2B  /* 行地址设置 */
#define ST7789_RAMWR     0x2C  /* 写显存 */
#define ST7789_RAMRD     0x2E  /* 读显存 */
#define ST7789_PTLAR     0x30
#define ST7789_VSCRDEF   0x33
#define ST7789_MADCTL    0x36  /* 内存访问控制（旋转/镜像） */
#define ST7789_VSCSAD    0x37  /* 垂直卷动起始地址 */
#define ST7789_COLMOD    0x3A  /* 像素格式 */
#define ST7789_PORCTRL   0xB2  /* Porch 控制 */
#define ST7789_GCTRL     0xB7  /* Gate 控制 */
#define ST7789_VCOMS     0xBB
#define ST7789_LCMCTRL   0xC0
#define ST7789_VDVVRHEN  0xC2
#define ST7789_VRHS      0xC3
#define ST7789_VDVS      0xC4
#define ST7789_FRCTRL2   0xC6
#define ST7789_PWCTRL1   0xD0
#define ST7789_PVGAMCTRL 0xE0  /* 正极性伽马 */
#define ST7789_NVGAMCTRL 0xE1  /* 负极性伽马 */

/* ═══════════════════════════════════════════════════════════════════════
 *  MADCTL 方向位（可组合）
 * ═══════════════════════════════════════════════════════════════════════ */
#define MADCTL_MY   0x80  /* 行地址顺序 */
#define MADCTL_MX   0x40  /* 列地址顺序 */
#define MADCTL_MV   0x20  /* 行/列互换 */
#define MADCTL_ML   0x10  /* 垂直刷新顺序 */
#define MADCTL_RGB  0x00  /* RGB 像素顺序 */
#define MADCTL_BGR  0x08  /* BGR 像素顺序 */

/* 常用方向预设（竖屏 0°为默认）
 *
 *  ST7789 MADCTL 位对 X/Y 轴的影响：
 *    MX=1 → 列地址从右往左（水平镜像）
 *    MY=1 → 行地址从下往上（垂直镜像）
 *    MV=1 → 行/列互换（转置，即旋转 90°）
 *
 *  0°  竖屏正向：不翻转，不互换
 *  90° 横屏：     MV + MX（转置后再水平翻转，等效顺时针 90°）
 * 180° 竖屏倒置：MX + MY（水平 + 垂直同时翻转）
 * 270° 横屏倒置：MV + MY（转置后再垂直翻转，等效逆时针 90°）
 */
#define LCD_ROTATION_0    (MADCTL_BGR)                               /* 0°   竖屏 */
#define LCD_ROTATION_90   (MADCTL_MX | MADCTL_MV | MADCTL_BGR)      /* 90°  横屏 */
#define LCD_ROTATION_180  (MADCTL_MX | MADCTL_MY | MADCTL_BGR)      /* 180° 竖屏 */
#define LCD_ROTATION_270  (MADCTL_MY | MADCTL_MV | MADCTL_BGR)      /* 270° 横屏 */

/* ═══════════════════════════════════════════════════════════════════════
 *  RGB565 颜色宏（大端格式，高字节先发）
 * ═══════════════════════════════════════════════════════════════════════ */
#define RGB565(r, g, b)  ((uint16_t)(((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

#define COLOR_BLACK    0x0000
#define COLOR_WHITE    0xFFFF
#define COLOR_RED      RGB565(255,   0,   0)
#define COLOR_GREEN    RGB565(  0, 255,   0)
#define COLOR_BLUE     RGB565(  0,   0, 255)
#define COLOR_YELLOW   RGB565(255, 255,   0)
#define COLOR_CYAN     RGB565(  0, 255, 255)
#define COLOR_MAGENTA  RGB565(255,   0, 255)
#define COLOR_ORANGE   RGB565(255, 165,   0)
#define COLOR_GRAY     RGB565(128, 128, 128)

/* ═══════════════════════════════════════════════════════════════════════
 *  公共 API
 * ═══════════════════════════════════════════════════════════════════════ */

/* 初始化与配置 */
void LCD_Init(void);
void LCD_SetRotation(uint8_t madctl);
void LCD_SetBacklight(uint8_t on);

/* 基本绘图（均使用 DMA） */
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_FillScreen(uint16_t color);
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_DrawCircle(int16_t cx, int16_t cy, int16_t r, uint16_t color);
void LCD_FillCircle(int16_t cx, int16_t cy, int16_t r, uint16_t color);

/* 图像（像素数组须为 RGB565 大端格式） */
void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                   const uint16_t *img);

/* 字符 / 字符串（内置 ASCII 5×7 点阵，可自行替换更大字库） */
void LCD_DrawChar(uint16_t x, uint16_t y, char c,
                  uint16_t fg, uint16_t bg, uint8_t size);
void LCD_DrawString(uint16_t x, uint16_t y, const char *str,
                    uint16_t fg, uint16_t bg, uint8_t size);

#endif /* __ST7789_H */