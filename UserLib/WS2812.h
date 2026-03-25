/**
*   @file WS2812.h
*   @brief 
*   @author Wenxin HU
*   @date 2026/3/25
*   @version 1.0
*   @note
*/
#ifndef G473TEST_WS2812_H
#define G473TEST_WS2812_H

#include "spi.h"

#define WS2812_LowLevel    0xC0     // 0
#define WS2812_HighLevel   0xF0     // 1

void WS2812_Ctrl(SPI_HandleTypeDef *hspi,uint8_t r, uint8_t g, uint8_t b);
void WS2812_Breath(SPI_HandleTypeDef *hspi, uint16_t tick, uint8_t g_ws2812_max_brightness);

#endif //G473TEST_WS2812_H