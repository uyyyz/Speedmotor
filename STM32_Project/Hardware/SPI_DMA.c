#include "SPI_DMA.h"

/* -----------------------------------------------------------------------
 * 内部状态
 * ---------------------------------------------------------------------- */
static volatile uint8_t  dma_busy = 0;

/* 纯色填充时用的双字节缓冲（大端 RGB565） */
static uint8_t fill_buf[2];

/* -----------------------------------------------------------------------
 * SPI_DMA_Init
 *   初始化 GPIO、SPI1、DMA1 Channel3
 * ---------------------------------------------------------------------- */
void SPI_DMA_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    SPI_InitTypeDef   SPI_InitStruct;
    DMA_InitTypeDef   DMA_InitStruct;
    NVIC_InitTypeDef  NVIC_InitStruct;

    /* ── 1. 时钟使能 ── */
    RCC_APB2PeriphClockCmd(LCD_GPIO_CLK | LCD_SPI_CLK, ENABLE);
    RCC_AHBPeriphClockCmd(LCD_DMA_CLK, ENABLE);

    /* ── 2. GPIO 初始化 ── */
    /* SPI 复用推挽输出：SCK、MOSI */
    GPIO_InitStruct.GPIO_Pin   = LCD_SCK_PIN | LCD_MOSI_PIN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStruct);

    /* 普通推挽输出：CS、DC、RST、BLK */
    GPIO_InitStruct.GPIO_Pin  = LCD_CS_PIN | LCD_DC_PIN |
                                 LCD_RST_PIN | LCD_BLK_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_GPIO_PORT, &GPIO_InitStruct);

    /* 默认状态 */
    LCD_CS_HIGH();
    LCD_DC_HIGH();
    LCD_RST_HIGH();
    LCD_BLK_OFF();

    /* ── 3. SPI1 初始化 ── */
    SPI_InitStruct.SPI_Direction         = SPI_Direction_1Line_Tx; /* 仅发送 */
    SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High; /* ST7789: CPOL=1, CPHA=1 */
    SPI_InitStruct.SPI_CPHA             = SPI_CPHA_2Edge;
    SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
    /*
     * PCLK2 = 72 MHz (STM32F103 最高频率)
     * SPI 预分频 2 → 36 MHz（ST7789 最大 SPI 时钟约 62.5 MHz，36 MHz 足够）
     */
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial     = 7;
    SPI_Init(LCD_SPI, &SPI_InitStruct);

    SPI_Cmd(LCD_SPI, ENABLE);
    SPI_I2S_DMACmd(LCD_SPI, SPI_I2S_DMAReq_Tx, ENABLE); /* 使能 SPI TX DMA 请求 */

    /* ── 4. DMA1 Channel3 基础配置（不启动，每次传输前重新设置地址和长度） ── */
    DMA_DeInit(LCD_DMA_CHANNEL);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&LCD_SPI->DR;
    DMA_InitStruct.DMA_MemoryBaseAddr     = 0;                   /* 传输前设置 */
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralDST;
    DMA_InitStruct.DMA_BufferSize         = 0;                   /* 传输前设置 */
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority           = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;
    DMA_Init(LCD_DMA_CHANNEL, &DMA_InitStruct);

    /* ── 5. DMA 传输完成中断（可选，也可轮询标志位） ── */
    DMA_ITConfig(LCD_DMA_CHANNEL, DMA_IT_TC, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel                   = DMA1_Channel3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/* -----------------------------------------------------------------------
 * DMA1_Channel3_IRQHandler  — SPI1 TX DMA 传输完成
 * ---------------------------------------------------------------------- */
void DMA1_Channel3_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC3))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC3);

        /* 等待 SPI 把最后一个字节移出移位寄存器 */
        while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) == SET) {}

        dma_busy = 0;
    }
}

/* -----------------------------------------------------------------------
 * SPI_DMA_WaitDone  — 阻塞等待当前 DMA 传输结束
 * ---------------------------------------------------------------------- */
void SPI_DMA_WaitDone(void)
{
    while (dma_busy) {}
}

/* -----------------------------------------------------------------------
 * 内部：启动 DMA 传输（地址自增模式）
 * ---------------------------------------------------------------------- */
static void dma_start(const uint8_t *buf, uint32_t len, uint8_t inc)
{
    SPI_DMA_WaitDone();                /* 确保上次传输完毕 */
    dma_busy = 1;

    DMA_Cmd(LCD_DMA_CHANNEL, DISABLE);
    DMA_ClearFlag(LCD_DMA_TC_FLAG);

    LCD_DMA_CHANNEL->CMAR  = (uint32_t)buf;
    LCD_DMA_CHANNEL->CNDTR = len;
    LCD_DMA_CHANNEL->CCR  &= ~DMA_CCR1_MINC;           /* 先清地址自增位 */
    if (inc)
        LCD_DMA_CHANNEL->CCR |= DMA_CCR1_MINC;         /* 自增 */

    DMA_Cmd(LCD_DMA_CHANNEL, ENABLE);
}

/* -----------------------------------------------------------------------
 * SPI_DMA_SendByte  — 发送单字节（CPU 轮询，用于命令/少量数据）
 * ---------------------------------------------------------------------- */
void SPI_DMA_SendByte(uint8_t data)
{
    SPI_DMA_WaitDone();
    while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET) {}
    SPI_I2S_SendData(LCD_SPI, data);
    while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) == SET) {}
}

/* -----------------------------------------------------------------------
 * SPI_DMA_SendBuffer  — DMA 发送字节数组（地址自增）
 * ---------------------------------------------------------------------- */
void SPI_DMA_SendBuffer(const uint8_t *buf, uint32_t len)
{
    if (len == 0) return;
    dma_start(buf, len, 1);
}

/* -----------------------------------------------------------------------
 * SPI_DMA_SendBuffer16  — DMA 发送 uint16_t 数组（大端字节序 RGB565）
 *   ST7789 先收高字节，所以每个像素的两字节直接顺序发送即可
 *   （前提：颜色宏已按大端排好，详见 st7789.h 中的 RGB565 定义）
 * ---------------------------------------------------------------------- */
void SPI_DMA_SendBuffer16(const uint16_t *buf, uint32_t len)
{
    SPI_DMA_SendBuffer((const uint8_t *)buf, len * 2);
}

/* -----------------------------------------------------------------------
 * SPI_DMA_FillColor  — 用同一颜色填充 count 个像素
 *   技巧：DMA 地址不自增，重复读同一 2 字节缓冲
 *   注意：fill_buf 为全局，必须在传输完成前保持有效
 * ---------------------------------------------------------------------- */
void SPI_DMA_FillColor(uint16_t color, uint32_t count)
{
    if (count == 0) return;

    /* DMA 一次最多 65535 个字节，大面积填充需分批 */
    fill_buf[0] = (uint8_t)(color >> 8);
    fill_buf[1] = (uint8_t)(color & 0xFF);

    uint32_t bytes = count * 2;
    while (bytes > 0)
    {
        uint32_t chunk = (bytes > 65534) ? 65534 : bytes; /* 保持偶数字节 */
        /* 关闭地址自增，重复读 fill_buf[0] */
        SPI_DMA_WaitDone();
        dma_busy = 1;
        DMA_Cmd(LCD_DMA_CHANNEL, DISABLE);
        DMA_ClearFlag(LCD_DMA_TC_FLAG);
        LCD_DMA_CHANNEL->CMAR  = (uint32_t)fill_buf;
        LCD_DMA_CHANNEL->CNDTR = chunk;
        LCD_DMA_CHANNEL->CCR  &= ~DMA_CCR1_MINC; /* 不自增 → 始终读 fill_buf[0] */
        DMA_Cmd(LCD_DMA_CHANNEL, ENABLE);
        SPI_DMA_WaitDone();
        bytes -= chunk;
    }

    /* 补发最后一个字节（chunk 为奇数时，理论上不会出现，因为像素都是 2 字节） */
}
