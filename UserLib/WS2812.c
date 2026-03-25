/**
*   @file WS2812.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/3/25
*   @version 1.0
*   @note
*/
#include "WS2812.h"
#include "spi.h"

//WS2812控制函数
void WS2812_Ctrl(SPI_HandleTypeDef *hspi,uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t txbuf[24];
    uint8_t res = 0;
    for (int i = 0; i < 8; i++)
    {
        txbuf[7-i]  = (((g>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
        txbuf[15-i] = (((r>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
        txbuf[23-i] = (((b>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
    }
    HAL_SPI_Transmit(hspi, &res, 0, 0xFFFF);
    while (hspi->State != HAL_SPI_STATE_READY);
    HAL_SPI_Transmit(hspi, txbuf, 24, 0xFFFF);
    for (int i = 0; i < 100; i++)
    {
        HAL_SPI_Transmit(hspi, &res, 1, 0xFFFF);
    }
}

// 0..255 三角波：0->255->0
static uint8_t tri_u8(uint16_t t)
{
    t &= 0x01FFu; // 0..511
    if (t <= 255u) return (uint8_t)t;
    return (uint8_t)(511u - t);
}

// 简单 RGB 呼吸：亮度呼吸 + 颜色缓慢轮转（也可以把 phase 固定为 0 只做红色呼吸）
void WS2812_Breath(SPI_HandleTypeDef *hspi, uint16_t tick, uint8_t g_ws2812_max_brightness)
{
    // breath_raw: 0..255
    uint8_t breath_raw = tri_u8(tick);

    // 幅值限制：把呼吸波形整体缩小到 max_brightness 以内
    // breath: 0..g_ws2812_max_brightness
    uint16_t breath = ((uint16_t)breath_raw * (uint16_t)g_ws2812_max_brightness) / 255u;

    uint8_t phase  = (uint8_t)(tick >> 2);     // 颜色变化慢一点

    uint8_t r = 0, g = 0, b = 0;
    // 3 段线性插值，做一个很轻量的彩虹轮转
    if (phase < 85u) {
        r = (uint8_t)(255u - phase * 3u);
        g = (uint8_t)(phase * 3u);
        b = 0u;
    } else if (phase < 170u) {
        uint8_t p = (uint8_t)(phase - 85u);
        r = 0u;
        g = (uint8_t)(255u - p * 3u);
        b = (uint8_t)(p * 3u);
    } else {
        uint8_t p = (uint8_t)(phase - 170u);
        r = (uint8_t)(p * 3u);
        g = 0u;
        b = (uint8_t)(255u - p * 3u);
    }

    // 亮度缩放（breath/255）
    r = (uint8_t)(((uint16_t)r * breath) / 255u);
    g = (uint8_t)(((uint16_t)g * breath) / 255u);
    b = (uint8_t)(((uint16_t)b * breath) / 255u);

    WS2812_Ctrl(hspi, r, g, b);
}

