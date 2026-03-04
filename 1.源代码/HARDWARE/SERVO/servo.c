#include "servo.h"



void Servo_Init(void)
{
    // 1. 开启时钟（必须包含AFIO，用于重映射和JTAG配置）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(SERVO_CLK | RCC_APB2Periph_AFIO, ENABLE);
    
    // 2. 核心步骤：关闭JTAG，释放PB4（NJTRST引脚）
    // 保留SWD调试（不影响下载），仅关闭JTAG功能
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    
    // 3. 配置PB4为复用推挽输出（PWM输出必需）
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = SERVO_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_GPIO_PORT, &GPIO_InitStructure);
    
    // 4. 配置TIM3部分重映射：将CH1映射到PB4
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    
    // 5. 配置TIM3时基单元（20ms周期，舵机标准频率50Hz）
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;  // 20ms = 1MHz × 20000
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;   // 72分频→1MHz计数频率
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    // 6. 配置TIM3_CH1为PWM模式（对应PB4输出）
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500;  // 初始脉宽1.5ms（90°）
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    
    // 7. 启动TIM3
    TIM_Cmd(TIM3, ENABLE);
}

// 设置PWM脉宽（直接控制PB4输出）
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM3, Compare);
}

// 角度转脉宽（0~180°对应500~2500us）
void Servo_SetAngle(float Angle)
{
    // 限制角度范围，避免舵机过载
    if(Angle < 0) Angle = 0;
    if(Angle > 180) Angle = 180;
    PWM_SetCompare1((uint16_t)(Angle / 180 * 2000 + 500));
}

