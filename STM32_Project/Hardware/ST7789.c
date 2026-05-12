#include "st7789.h"

/* ═══════════════════════════════════════════════════════════════════════
 *  内置 5×7 ASCII 字体（32~127 共 96 个字符）
 * ═══════════════════════════════════════════════════════════════════════ */
static const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, /* 空格 */
    {0x00,0x00,0x5F,0x00,0x00}, /* ! */
    {0x00,0x07,0x00,0x07,0x00}, /* " */
    {0x14,0x7F,0x14,0x7F,0x14}, /* # */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* $ */
    {0x23,0x13,0x08,0x64,0x62}, /* % */
    {0x36,0x49,0x55,0x22,0x50}, /* & */
    {0x00,0x05,0x03,0x00,0x00}, /* ' */
    {0x00,0x1C,0x22,0x41,0x00}, /* ( */
    {0x00,0x41,0x22,0x1C,0x00}, /* ) */
    {0x14,0x08,0x3E,0x08,0x14}, /* * */
    {0x08,0x08,0x3E,0x08,0x08}, /* + */
    {0x00,0x50,0x30,0x00,0x00}, /* , */
    {0x08,0x08,0x08,0x08,0x08}, /* - */
    {0x00,0x60,0x60,0x00,0x00}, /* . */
    {0x20,0x10,0x08,0x04,0x02}, /* / */
    {0x3E,0x51,0x49,0x45,0x3E}, /* 0 */
    {0x00,0x42,0x7F,0x40,0x00}, /* 1 */
    {0x42,0x61,0x51,0x49,0x46}, /* 2 */
    {0x21,0x41,0x45,0x4B,0x31}, /* 3 */
    {0x18,0x14,0x12,0x7F,0x10}, /* 4 */
    {0x27,0x45,0x45,0x45,0x39}, /* 5 */
    {0x3C,0x4A,0x49,0x49,0x30}, /* 6 */
    {0x01,0x71,0x09,0x05,0x03}, /* 7 */
    {0x36,0x49,0x49,0x49,0x36}, /* 8 */
    {0x06,0x49,0x49,0x29,0x1E}, /* 9 */
    {0x00,0x36,0x36,0x00,0x00}, /* : */
    {0x00,0x56,0x36,0x00,0x00}, /* ; */
    {0x08,0x14,0x22,0x41,0x00}, /* < */
    {0x14,0x14,0x14,0x14,0x14}, /* = */
    {0x00,0x41,0x22,0x14,0x08}, /* > */
    {0x02,0x01,0x51,0x09,0x06}, /* ? */
    {0x32,0x49,0x79,0x41,0x3E}, /* @ */
    {0x7E,0x11,0x11,0x11,0x7E}, /* A */
    {0x7F,0x49,0x49,0x49,0x36}, /* B */
    {0x3E,0x41,0x41,0x41,0x22}, /* C */
    {0x7F,0x41,0x41,0x22,0x1C}, /* D */
    {0x7F,0x49,0x49,0x49,0x41}, /* E */
    {0x7F,0x09,0x09,0x09,0x01}, /* F */
    {0x3E,0x41,0x49,0x49,0x7A}, /* G */
    {0x7F,0x08,0x08,0x08,0x7F}, /* H */
    {0x00,0x41,0x7F,0x41,0x00}, /* I */
    {0x20,0x40,0x41,0x3F,0x01}, /* J */
    {0x7F,0x08,0x14,0x22,0x41}, /* K */
    {0x7F,0x40,0x40,0x40,0x40}, /* L */
    {0x7F,0x02,0x0C,0x02,0x7F}, /* M */
    {0x7F,0x04,0x08,0x10,0x7F}, /* N */
    {0x3E,0x41,0x41,0x41,0x3E}, /* O */
    {0x7F,0x09,0x09,0x09,0x06}, /* P */
    {0x3E,0x41,0x51,0x21,0x5E}, /* Q */
    {0x7F,0x09,0x19,0x29,0x46}, /* R */
    {0x46,0x49,0x49,0x49,0x31}, /* S */
    {0x01,0x01,0x7F,0x01,0x01}, /* T */
    {0x3F,0x40,0x40,0x40,0x3F}, /* U */
    {0x1F,0x20,0x40,0x20,0x1F}, /* V */
    {0x3F,0x40,0x38,0x40,0x3F}, /* W */
    {0x63,0x14,0x08,0x14,0x63}, /* X */
    {0x07,0x08,0x70,0x08,0x07}, /* Y */
    {0x61,0x51,0x49,0x45,0x43}, /* Z */
    {0x00,0x7F,0x41,0x41,0x00}, /* [ */
    {0x02,0x04,0x08,0x10,0x20}, /* \ */
    {0x00,0x41,0x41,0x7F,0x00}, /* ] */
    {0x04,0x02,0x01,0x02,0x04}, /* ^ */
    {0x40,0x40,0x40,0x40,0x40}, /* _ */
    {0x00,0x01,0x02,0x04,0x00}, /* ` */
    {0x20,0x54,0x54,0x54,0x78}, /* a */
    {0x7F,0x48,0x44,0x44,0x38}, /* b */
    {0x38,0x44,0x44,0x44,0x20}, /* c */
    {0x38,0x44,0x44,0x48,0x7F}, /* d */
    {0x38,0x54,0x54,0x54,0x18}, /* e */
    {0x08,0x7E,0x09,0x01,0x02}, /* f */
    {0x0C,0x52,0x52,0x52,0x3E}, /* g */
    {0x7F,0x08,0x04,0x04,0x78}, /* h */
    {0x00,0x44,0x7D,0x40,0x00}, /* i */
    {0x20,0x40,0x44,0x3D,0x00}, /* j */
    {0x7F,0x10,0x28,0x44,0x00}, /* k */
    {0x00,0x41,0x7F,0x40,0x00}, /* l */
    {0x7C,0x04,0x18,0x04,0x78}, /* m */
    {0x7C,0x08,0x04,0x04,0x78}, /* n */
    {0x38,0x44,0x44,0x44,0x38}, /* o */
    {0x7C,0x14,0x14,0x14,0x08}, /* p */
    {0x08,0x14,0x14,0x18,0x7C}, /* q */
    {0x7C,0x08,0x04,0x04,0x08}, /* r */
    {0x48,0x54,0x54,0x54,0x20}, /* s */
    {0x04,0x3F,0x44,0x40,0x20}, /* t */
    {0x3C,0x40,0x40,0x20,0x7C}, /* u */
    {0x1C,0x20,0x40,0x20,0x1C}, /* v */
    {0x3C,0x40,0x30,0x40,0x3C}, /* w */
    {0x44,0x28,0x10,0x28,0x44}, /* x */
    {0x0C,0x50,0x50,0x50,0x3C}, /* y */
    {0x44,0x64,0x54,0x4C,0x44}, /* z */
    {0x00,0x08,0x36,0x41,0x00}, /* { */
    {0x00,0x00,0x7F,0x00,0x00}, /* | */
    {0x00,0x41,0x36,0x08,0x00}, /* } */
    {0x10,0x08,0x08,0x10,0x08}, /* ~ */
    {0x00,0x00,0x00,0x00,0x00}, /* DEL */
};

/* ═══════════════════════════════════════════════════════════════════════
 *  内部辅助：发送命令 / 数据
 * ═══════════════════════════════════════════════════════════════════════ */
static void lcd_write_cmd(uint8_t cmd)
{
    LCD_DC_LOW();
    LCD_CS_LOW();
    SPI_DMA_SendByte(cmd);
    LCD_CS_HIGH();
}

static void lcd_write_data(uint8_t data)
{
    LCD_DC_HIGH();
    LCD_CS_LOW();
    SPI_DMA_SendByte(data);
    LCD_CS_HIGH();
}

static void lcd_write_data16(uint16_t data)
{
    LCD_DC_HIGH();
    LCD_CS_LOW();
    SPI_DMA_SendByte((uint8_t)(data >> 8));
    SPI_DMA_SendByte((uint8_t)(data & 0xFF));
    LCD_CS_HIGH();
}

/* ═══════════════════════════════════════════════════════════════════════
 *  简单延时（粗略，不依赖 SysTick）
 * ═══════════════════════════════════════════════════════════════════════ */
static void delay_ms(uint32_t ms)
{
    /* 72 MHz 时约每循环 4 周期，1 ms ≈ 18000 次 */
    volatile uint32_t i;
    while (ms--)
        for (i = 0; i < 18000; i++) __NOP();
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_Init  — 硬件复位 + ST7789 寄存器初始化序列
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_Init(void)
{
    SPI_DMA_Init();

    /* ── 硬件复位 ── */
    LCD_RST_HIGH();
    delay_ms(10);
    LCD_RST_LOW();
    delay_ms(10);
    LCD_RST_HIGH();
    delay_ms(120);  /* 等待 ST7789 上电完成 */

    /* ── 初始化序列 ── */
    lcd_write_cmd(ST7789_SWRESET);  /* 软件复位 */
    delay_ms(150);

    lcd_write_cmd(ST7789_SLPOUT);   /* 退出睡眠 */
    delay_ms(10);

    /* 像素格式：16-bit RGB565 */
    lcd_write_cmd(ST7789_COLMOD);
    lcd_write_data(0x55);           /* 0x55 = 16bpp */
    delay_ms(10);

    /* 内存访问方向（默认竖屏 0°） */
    lcd_write_cmd(ST7789_MADCTL);
    lcd_write_data(LCD_ROTATION_0);

    /* Porch 控制 */
    lcd_write_cmd(ST7789_PORCTRL);
    lcd_write_data(0x0C);
    lcd_write_data(0x0C);
    lcd_write_data(0x00);
    lcd_write_data(0x33);
    lcd_write_data(0x33);

    /* Gate 控制 */
    lcd_write_cmd(ST7789_GCTRL);
    lcd_write_data(0x35);

    /* VCOM */
    lcd_write_cmd(ST7789_VCOMS);
    lcd_write_data(0x19);

    /* LCM 控制 */
    lcd_write_cmd(ST7789_LCMCTRL);
    lcd_write_data(0x2C);

    /* VDV/VRH 使能 */
    lcd_write_cmd(ST7789_VDVVRHEN);
    lcd_write_data(0x01);

    /* VRH */
    lcd_write_cmd(ST7789_VRHS);
    lcd_write_data(0x12);

    /* VDV */
    lcd_write_cmd(ST7789_VDVS);
    lcd_write_data(0x20);

    /* 帧率：60 Hz */
    lcd_write_cmd(ST7789_FRCTRL2);
    lcd_write_data(0x0F);

    /* 电源控制 */
    lcd_write_cmd(ST7789_PWCTRL1);
    lcd_write_data(0xA4);
    lcd_write_data(0xA1);

    /* 正极性伽马曲线 */
    lcd_write_cmd(ST7789_PVGAMCTRL);
    lcd_write_data(0xD0); lcd_write_data(0x04); lcd_write_data(0x0D);
    lcd_write_data(0x11); lcd_write_data(0x13); lcd_write_data(0x2B);
    lcd_write_data(0x3F); lcd_write_data(0x54); lcd_write_data(0x4C);
    lcd_write_data(0x18); lcd_write_data(0x0D); lcd_write_data(0x0B);
    lcd_write_data(0x1F); lcd_write_data(0x23);

    /* 负极性伽马曲线 */
    lcd_write_cmd(ST7789_NVGAMCTRL);
    lcd_write_data(0xD0); lcd_write_data(0x04); lcd_write_data(0x0C);
    lcd_write_data(0x11); lcd_write_data(0x13); lcd_write_data(0x2C);
    lcd_write_data(0x3F); lcd_write_data(0x44); lcd_write_data(0x51);
    lcd_write_data(0x2F); lcd_write_data(0x1F); lcd_write_data(0x1F);
    lcd_write_data(0x20); lcd_write_data(0x23);

    lcd_write_cmd(ST7789_INVON);    /* 反色开（大多数 2.8" 模组需要） */
    delay_ms(10);

    lcd_write_cmd(ST7789_NORON);    /* 正常显示模式 */
    delay_ms(10);

    lcd_write_cmd(ST7789_DISPON);   /* 开启显示 */
    delay_ms(10);

    LCD_BLK_ON();                   /* 打开背光 */
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_SetRotation
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_SetRotation(uint8_t madctl)
{
    lcd_write_cmd(ST7789_MADCTL);
    lcd_write_data(madctl);
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_SetBacklight
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_SetBacklight(uint8_t on)
{
    if (on) LCD_BLK_ON();
    else     LCD_BLK_OFF();
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_SetWindow  — 设置后续 RAMWR 的像素窗口
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    lcd_write_cmd(ST7789_CASET);
    lcd_write_data16(x0);
    lcd_write_data16(x1);

    lcd_write_cmd(ST7789_RASET);
    lcd_write_data16(y0);
    lcd_write_data16(y1);

    lcd_write_cmd(ST7789_RAMWR);
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_FillScreen  — 全屏填充纯色（DMA）
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_FillScreen(uint16_t color)
{
    LCD_SetWindow(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    LCD_DC_HIGH();
    LCD_CS_LOW();
    SPI_DMA_FillColor(color, (uint32_t)LCD_WIDTH * LCD_HEIGHT);
    SPI_DMA_WaitDone();
    LCD_CS_HIGH();
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_FillRect  — 矩形填充（DMA）
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;
    if (x + w > LCD_WIDTH)  w = LCD_WIDTH  - x;
    if (y + h > LCD_HEIGHT) h = LCD_HEIGHT - y;

    LCD_SetWindow(x, y, x + w - 1, y + h - 1);
    LCD_DC_HIGH();
    LCD_CS_LOW();
    SPI_DMA_FillColor(color, (uint32_t)w * h);
    SPI_DMA_WaitDone();
    LCD_CS_HIGH();
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawPixel
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= LCD_WIDTH || y >= LCD_HEIGHT) return;
    LCD_SetWindow(x, y, x, y);
    lcd_write_data16(color);
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawLine  — Bresenham 直线
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int16_t err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;)
    {
        LCD_DrawPixel((uint16_t)x0, (uint16_t)y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 > -dx) { err -= dy; x0 += sx; }
        if (e2 <  dy) { err += dx; y0 += sy; }
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawRect  — 空心矩形
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    LCD_DrawLine(x,         y,         x + w - 1, y,         color);
    LCD_DrawLine(x,         y + h - 1, x + w - 1, y + h - 1, color);
    LCD_DrawLine(x,         y,         x,         y + h - 1, color);
    LCD_DrawLine(x + w - 1, y,         x + w - 1, y + h - 1, color);
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawCircle  — 中点圆算法（空心）
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawCircle(int16_t cx, int16_t cy, int16_t r, uint16_t color)
{
    int16_t x = 0, y = r, d = 3 - 2 * r;
    while (y >= x)
    {
        LCD_DrawPixel((uint16_t)(cx + x), (uint16_t)(cy + y), color);
        LCD_DrawPixel((uint16_t)(cx - x), (uint16_t)(cy + y), color);
        LCD_DrawPixel((uint16_t)(cx + x), (uint16_t)(cy - y), color);
        LCD_DrawPixel((uint16_t)(cx - x), (uint16_t)(cy - y), color);
        LCD_DrawPixel((uint16_t)(cx + y), (uint16_t)(cy + x), color);
        LCD_DrawPixel((uint16_t)(cx - y), (uint16_t)(cy + x), color);
        LCD_DrawPixel((uint16_t)(cx + y), (uint16_t)(cy - x), color);
        LCD_DrawPixel((uint16_t)(cx - y), (uint16_t)(cy - x), color);
        x++;
        if (d < 0) d += 4 * x + 6;
        else       { d += 4 * (x - --y) + 10; }
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_FillCircle  — 实心圆
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_FillCircle(int16_t cx, int16_t cy, int16_t r, uint16_t color)
{
    int16_t x = 0, y = r, d = 3 - 2 * r;
    while (y >= x)
    {
        LCD_FillRect((uint16_t)(cx - x), (uint16_t)(cy - y), (uint16_t)(2 * x + 1), 1, color);
        LCD_FillRect((uint16_t)(cx - y), (uint16_t)(cy - x), (uint16_t)(2 * y + 1), 1, color);
        LCD_FillRect((uint16_t)(cx - x), (uint16_t)(cy + y), (uint16_t)(2 * x + 1), 1, color);
        LCD_FillRect((uint16_t)(cx - y), (uint16_t)(cy + x), (uint16_t)(2 * y + 1), 1, color);
        x++;
        if (d < 0) d += 4 * x + 6;
        else       { d += 4 * (x - --y) + 10; }
    }
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawImage  — DMA 批量发送像素数组
 *  img 须为大端 RGB565，行主序，尺寸 w×h 像素
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                   const uint16_t *img)
{
    LCD_SetWindow(x, y, x + w - 1, y + h - 1);
    LCD_DC_HIGH();
    LCD_CS_LOW();
    SPI_DMA_SendBuffer16(img, (uint32_t)w * h);
    SPI_DMA_WaitDone();
    LCD_CS_HIGH();
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawChar  — 绘制单个 ASCII 字符
 *  size: 放大倍数（1 = 5×7，2 = 10×14，...）
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawChar(uint16_t x, uint16_t y, char c,
                  uint16_t fg, uint16_t bg, uint8_t size)
{
    if (c < 32 || c > 127) c = '?';
    const uint8_t *glyph = font5x7[c - 32];

    for (uint8_t col = 0; col < 5; col++)
    {
        uint8_t bits = glyph[col];
        for (uint8_t row = 0; row < 7; row++)
        {
            uint16_t color = (bits & (1 << row)) ? fg : bg;
            if (size == 1)
                LCD_DrawPixel(x + col, y + row, color);
            else
                LCD_FillRect(x + col * size, y + row * size,
                             size, size, color);
        }
    }
    /* 右侧 1 列间距 */
    if (size == 1)
        for (uint8_t row = 0; row < 7; row++)
            LCD_DrawPixel(x + 5, y + row, bg);
    else
        LCD_FillRect(x + 5 * size, y, size, 7 * size, bg);
}

/* ═══════════════════════════════════════════════════════════════════════
 *  LCD_DrawString  — 绘制 ASCII 字符串（超出屏幕宽度自动换行）
 * ═══════════════════════════════════════════════════════════════════════ */
void LCD_DrawString(uint16_t x, uint16_t y, const char *str,
                    uint16_t fg, uint16_t bg, uint8_t size)
{
    uint16_t cur_x = x;
    uint16_t char_w = (uint16_t)(6 * size); /* 5 像素 + 1 间距 */
    uint16_t char_h = (uint16_t)(8 * size); /* 7 像素 + 1 间距 */

    while (*str)
    {
        if (*str == '\n' || cur_x + char_w > LCD_WIDTH)
        {
            cur_x = x;
            y += char_h;
            if (*str == '\n') { str++; continue; }
        }
        LCD_DrawChar(cur_x, y, *str, fg, bg, size);
        cur_x += char_w;
        str++;
    }
}


