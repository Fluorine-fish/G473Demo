//
// Created by 22560 on 25-4-11.
//

#include "M2006.h"
#include "dev_FDCAN.h"

void M2006_Init(M2006_HandleTypeDef* M2006,FDCAN_HandleTypeDef* hfdcan,uint8_t id) {
    M2006->id = id;
    M2006->hfdcan = hfdcan;
}

void M2006_Update(M2006_HandleTypeDef* M2006, uint8_t* data) {
    M2006->angle_ecd = (data[0] << 8) | data[1];
    M2006->raw_speed_rpm = (data[2] << 8) | data[3];
    M2006->torque = (data[4] << 8) | data[5];
    M2006->speed_rpm = M2006->raw_speed_rpm / 36.0f;
}

void M2006_Crtl_Currency(M2006_HandleTypeDef* M2006, int16_t cur) {
    uint8_t TxBuffer[8];

    TxBuffer[0] = cur >> 8;
    TxBuffer[1] = cur;
    TxBuffer[2] = 0x00;
    TxBuffer[3] = 0x00;
    TxBuffer[4] = 0x00;
    TxBuffer[5] = 0x00;
    TxBuffer[6] = 0x00;
    TxBuffer[7] = 0x00;

    FDCAN_Transmit(M2006->hfdcan, TxBuffer, 0x200);
}
