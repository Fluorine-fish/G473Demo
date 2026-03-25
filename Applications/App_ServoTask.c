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
#include "fdcan.h"
#include "M2006.h"
#include "PID.h"

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

M2006_HandleTypeDef M2006_1;

extern PID_Param PID_Speed_M2006_1;
extern PID_Param PID_Angle_M2006_1;
extern int32_t angle;

float servo_angle = 0.0f;
float target_pitch = 10.0f;
bool isopen = true;

void M2006_Angel(double target_angle, int16_t Max_speed) {
    target_angle = (target_angle > 0) ? ((target_angle < 2.2340214) ? target_angle : 2.2340214) : 0;
    int16_t angle2M = 2900.0 / 2.3 * target_angle + 230;
    angle2M = (angle2M > 250) ? ((angle2M < 2900) ? angle2M : 2900) : 250;

    PID_Angle_M2006_1.target = angle2M;

    Angle_Calc(M2006_1.angle_ecd);
    PID_Angle(&PID_Angle_M2006_1, angle / 36, PID_Angle_M2006_1.target, Max_speed);
    PID_Solution(&PID_Speed_M2006_1, M2006_1.raw_speed_rpm, PID_Angle_M2006_1.out);

    M2006_Crtl_Currency(&M2006_1, PID_Speed_M2006_1.out);
}

void App_ServoTask(void const * argument) {
    servo1 = Servo_Register(&servo1_config);
    servo2 = Servo_Register(&servo2_config);
    M2006_Init(&M2006_1, &hfdcan2, 1);

    while (1) {
        // if (isopen) angle = (angle > 3.14) ? 0 : angle + 0.001f;
        //
        // Servo_Control(servo1, servo_angle);//通过修改angle变量来控制舵机角度
        // Servo_Control(servo2, servo_angle);//通过修改angle变量来控制舵机角度

        M2006_Angel(target_pitch, 1000);

        osDelay(1);
    }
}
