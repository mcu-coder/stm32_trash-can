#ifndef __SERVO_H
#define	__SERVO_H
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
 


/***************根据自己需求更改****************/
// SERVO舵机 GPIO宏定义



// PB4 舵机配置（TIM3_CH1部分重映射）
#define	SERVO_CLK		RCC_APB2Periph_GPIOB
#define SERVO_GPIO_PIN 	GPIO_Pin_4
#define SERVO_GPIO_PORT 	GPIOB

// 函数声明
void Servo_Init(void);
void PWM_SetCompare1(uint16_t Compare);
void Servo_SetAngle(float Angle);

#endif




