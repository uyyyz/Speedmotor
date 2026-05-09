#include "TFT.h"


/***********************************/
/********** Basic function **********/
/***********************************/
void TFT_WriteCommand(uint8_t cmd)
{
    GPIO_ResetBits(TFT_PORT, TFT_CS_PIN);
    GPIO_ResetBits(TFT_PORT, TFT_DC_PIN);
    for(uint8_t i=0;i<8;i++)
    {
        if(cmd & 0x80)
        {
            GPIO_SetBits(TFT_PORT, TFT_SDA_PIN);
        }
        else
        {
            GPIO_ResetBits(TFT_PORT, TFT_SDA_PIN);
        }
        cmd <<= 1;
        GPIO_SetBits(TFT_PORT, TFT_SCK_PIN);
        GPIO_ResetBits(TFT_PORT, TFT_SCK_PIN);
    }
    GPIO_SetBits(TFT_PORT, TFT_CS_PIN);
}

void TFT_WriteData(uint8_t data)
{
    GPIO_ResetBits(TFT_PORT, TFT_CS_PIN);
    GPIO_SetBits(TFT_PORT, TFT_DC_PIN);
    for(uint8_t i=0;i<8;i++)
    {
        if(data & 0x80)
        {
            GPIO_SetBits(TFT_PORT, TFT_SDA_PIN);
        }
        else
        {
            GPIO_ResetBits(TFT_PORT, TFT_SDA_PIN);
        }
        data <<= 1;
        GPIO_SetBits(TFT_PORT, TFT_SCK_PIN);
        GPIO_ResetBits(TFT_PORT, TFT_SCK_PIN);
    }
    GPIO_SetBits(TFT_PORT, TFT_CS_PIN);
}

/***********************************/
/********** Init function **********/
/***********************************/
void TFT_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = TFT_CS_PIN | TFT_DC_PIN | TFT_RST_PIN | TFT_SDA_PIN | TFT_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TFT_PORT, &GPIO_InitStructure);

    GPIO_SetBits(TFT_PORT, TFT_CS_PIN);     // CS 高 —— 设备未选中
    GPIO_SetBits(TFT_PORT, TFT_RST_PIN);    // RST 高 —— 不复位
    GPIO_ResetBits(TFT_PORT, TFT_SCK_PIN);  // SCK 低 —— SPI Mode 0 空闲
    GPIO_ResetBits(TFT_PORT, TFT_DC_PIN);
    GPIO_ResetBits(TFT_PORT, TFT_SDA_PIN);
}

void TFT_Reset(void)
{
    GPIO_ResetBits(TFT_PORT,TFT_RST_PIN);
    Delay_ms(200);
    GPIO_SetBits(TFT_PORT,TFT_RST_PIN);
    Delay_ms(200);
}

void TFT_Init(void)
{
    TFT_GPIO_Init();

    TFT_Reset();
    //exist sleep
    TFT_WriteCommand(0x11);
    Delay_ms(120);
    //Pixel
    TFT_WriteCommand(0x3A);
    TFT_WriteData(0x55); //RGB565

    TFT_WriteCommand(0x29); //display on
}

void TFT_SetRegion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    TFT_WriteCommand(0x2A);                       //  address
    TFT_WriteData((x_start >> 8) & 0xFF);         // 0x00
    TFT_WriteData(x_start & 0xFF);                // 0x00
    TFT_WriteData((x_end >> 8) & 0xFF);           // 0x00  (239=0x00EF)
    TFT_WriteData(x_end & 0xFF);                  // 0xEF

    TFT_WriteCommand(0x2B);                       // 行地址
    TFT_WriteData((y_start >> 8) & 0xFF);         // 0x00
    TFT_WriteData(y_start & 0xFF);                // 0x00
    TFT_WriteData((y_end >> 8) & 0xFF);           // 0x01  (319=0x013F ★)
    TFT_WriteData(y_end & 0xFF);                  // 0x3F

    TFT_WriteCommand(0x2C);                       // Write GRAM
}




//User functions

void TFT_FullScreen(uint16_t color)
{
    TFT_SetRegion(0, 0, X_MAX_PIXEL, Y_MAX_PIXEL);
    for (int i = 0; i < X_MAX_PIXEL * Y_MAX_PIXEL; i++)
    {
        TFT_WriteData((color >> 8) & 0xFF);  // RGB565 高字节
        TFT_WriteData(color & 0xFF);         // RGB565 低字节
    }
}

void TFT_DrawChar32(uint16_t x, uint16_t y,const unsigned char *font,uint16_t fg_color, uint16_t bg_color)
{
    TFT_SetRegion(x, y, x + 31, y + 31);  // 设置 32×32 写入区域

    for (uint8_t row = 0; row < 32; row++)
    {
        for (uint8_t col = 0; col < 32; col++)
        {
            /* 每行 4 个字节，定位到当前 bit */
            uint8_t byte_val = font[row * 4 + col / 8];
            uint16_t color;

            if (byte_val & (0x80 >> (col % 8)))  // 该 bit 为 1
                color = fg_color;
            else
                color = bg_color;

            TFT_WriteData((color >> 8) & 0xFF);
            TFT_WriteData(color & 0xFF);
        }
    }
}

void TFT_DrawChar16(uint16_t x, uint16_t y,const unsigned char *font,uint16_t fg_color, uint16_t bg_color)
{
    TFT_SetRegion(x, y, x + 15, y + 15);  // 设置 16×16 写入区域

    for (uint8_t row = 0; row < 16; row++)
    {
        for (uint8_t col = 0; col < 16; col++)
        {
            /* 每行 2 个字节，定位到当前 bit */
            uint8_t byte_val = font[row * 2 + col / 8];
            uint16_t color;

            if (byte_val & (0x80 >> (col % 8)))  // 该 bit 为 1
                color = fg_color;
            else
                color = bg_color;

            TFT_WriteData((color >> 8) & 0xFF);
            TFT_WriteData(color & 0xFF);
        }
    }
}

void TFT_ShowImage(uint16_t x, uint16_t y, uint8_t width, uint8_t height, const unsigned char *image)
{
    uint32_t i;
    uint8_t picL, picH;
    TFT_SetRegion(x, y, x + width - 1, y + height - 1);  // 移除x+2偏移
    for(i=0; i<width*height; i++)
    {
        picL = *(image + i*2);
        picH = *(image + i*2 + 1);				
        TFT_WriteData(picL);
        TFT_WriteData(picH);  						
    }
}



