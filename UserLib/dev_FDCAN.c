#include "dev_FDCAN.h"

#include "fdcan.h"
#include "M2006.h"

extern M2006_HandleTypeDef M2006_1;

FDCAN_RxFrame_s FDCAN3_RxFrame = {
    .hcan = &hfdcan3,
};
FDCAN_RxFrame_s FDCAN2_RxFrame = {
    .hcan = &hfdcan2,
};

void FDCAN_FilterInit() {
    FDCAN_FilterTypeDef filter_config1;
    FDCAN_FilterTypeDef filter_config2;

    filter_config1.IdType = FDCAN_STANDARD_ID;
    filter_config1.FilterIndex = 0;
    filter_config1.FilterType = FDCAN_FILTER_MASK;
    filter_config1.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    filter_config1.FilterID1 = 0x00000000;
    filter_config1.FilterID2 = 0x00000000;

    filter_config2.IdType = FDCAN_STANDARD_ID;
    filter_config2.FilterIndex = 1;
    filter_config2.FilterType = FDCAN_FILTER_MASK;
    filter_config2.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
    filter_config2.FilterID1 = 0x00000000;
    filter_config2.FilterID2 = 0x00000000;

    HAL_FDCAN_ConfigFilter(&hfdcan2, &filter_config1);
    HAL_FDCAN_ConfigFilter(&hfdcan3, &filter_config2);

    HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_ACCEPT_IN_RX_FIFO0, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);
    HAL_FDCAN_ConfigGlobalFilter(&hfdcan3, FDCAN_ACCEPT_IN_RX_FIFO1, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE);

    HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
    HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);

    HAL_FDCAN_Start(&hfdcan2);
    HAL_FDCAN_Start(&hfdcan3);
}

void FDCAN_Transmit(FDCAN_HandleTypeDef *hfdcan, uint8_t *data, uint32_t id) {
    FDCAN_TxFrame_s frame = {
        .hcan = hfdcan,
        .Header.IdType = FDCAN_STANDARD_ID,
        .Header.TxFrameType = FDCAN_DATA_FRAME,
        .Header.DataLength = 8,
        .Header.ErrorStateIndicator = FDCAN_ESI_ACTIVE,
        .Header.BitRateSwitch = FDCAN_CLASSIC_CAN,
        .Header.TxEventFifoControl = FDCAN_NO_TX_EVENTS,
        .Header.MessageMarker = 0,
        .Header.Identifier = id,
        .Data = data,
    };

    HAL_FDCAN_AddMessageToTxFifoQ(frame.hcan, &frame.Header, frame.Data);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
    if (hfdcan->Instance == hfdcan3.Instance) {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN3_RxFrame.Header, FDCAN3_RxFrame.RxData);
    }else if (hfdcan->Instance == hfdcan2.Instance) {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &FDCAN2_RxFrame.Header, FDCAN2_RxFrame.RxData);
        M2006_Update(&M2006_1,FDCAN2_RxFrame.RxData);
    }
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs) {
    if (hfdcan->Instance == hfdcan3.Instance) {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN3_RxFrame.Header, FDCAN3_RxFrame.RxData);
    }else if (hfdcan->Instance == hfdcan2.Instance) {
        HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &FDCAN2_RxFrame.Header, FDCAN2_RxFrame.RxData);
    }
}
