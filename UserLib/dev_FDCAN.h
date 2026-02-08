#ifndef LASER_GIMBAL_FDCAN_H
#define LASER_GIMBAL_FDCAN_H

#include "dev_FDCAN.h"
#include "stdint.h"
#include "fdcan.h"

typedef struct {
    FDCAN_HandleTypeDef *hcan;
    FDCAN_TxHeaderTypeDef Header;
    uint8_t *Data;
}FDCAN_TxFrame_s;

typedef struct {
    FDCAN_HandleTypeDef *hcan;
    FDCAN_RxHeaderTypeDef Header;
    uint8_t RxData[8];
}FDCAN_RxFrame_s;

void FDCAN_FilterInit();

void FDCAN_Transmit(FDCAN_HandleTypeDef *hfdcan, uint8_t *data, uint32_t id);

#endif //LASER_GIMBAL_FDCAN_H