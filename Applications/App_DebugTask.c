/**
*   @file App_DebugTask.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/2/8
*   @version 1.0
*   @note
*/
#include "App_DebugTask.h"
#include "cmsis_os.h"
#include "dev_FDCAN.h"
#include "spi.h"
#include "WS2812.h"

uint8_t fdcan2_rxbuf[8] = {};
uint8_t fdcan2_txbuf[8] = {0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03};
uint8_t fdcan3_rxbuf[8] = {};
uint8_t fdcan3_txbuf[8] = {0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03};


void App_DebugTask(void const * argument) {

    FDCAN_FilterInit();

    uint16_t led_tick = 0;

    while (1) {
        WS2812_Breath(&hspi1, led_tick++, 64);

        osDelay(1);
    }
}
