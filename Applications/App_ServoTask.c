/**
*   @file App_ServoTask.c
*   @brief 
*   @author Wenxin HU
*   @date 2026/2/26
*   @version 1.0
*   @note
*/
#include "App_ServoTask.h"
#include "dev_servo.h"
#include "cmsis_os.h"

ServoInitConfig_s servo1_config = {
    .type = SERVO_NORMAL,
    .tim_handle = &htim1,
    .channel = TIM_CHANNEL_1, // 使用TIM1的通道1
};
ServoInstance_s *servo1 = NULL;


ServoInitConfig_s servo2_config = {
    .type = SERVO_NORMAL,
    .tim_handle = &htim1,
    .channel = TIM_CHANNEL_2, // 使用TIM1的通道2
};
ServoInstance_s *servo2 = NULL;

float angle = 0.0f;
bool isopen = true;

void App_ServoTask(void const * argument) {
    servo1 = Servo_Register(&servo1_config);
    servo2 = Servo_Register(&servo2_config);

    while (1) {
        if (isopen) angle = (angle > 3.14) ? 0 : angle + 0.001f;

        Servo_Control(servo1, angle);//通过修改angle变量来控制舵机角度
        Servo_Control(servo2, angle);//通过修改angle变量来控制舵机角度
        osDelay(1);
    }
}
