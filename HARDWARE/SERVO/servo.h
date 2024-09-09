#ifndef __SERVO_H
#define __SERVO_H
#include "sys.h"
#include "math.h"

#define PI 3.14159265
#define Ratio 3.7037f
#define Ratio1 7.4074f

#define PWM1   TIM3->CCR4  //PB0	250->0.5ms
#define PWM2   TIM3->CCR3  //PB1
#define PWM3   TIM4->CCR3  //PD14
#define PWM4   TIM4->CCR2  //PD13
#define PWM5   TIM4->CCR1  //PD12

extern u8 Servo_flag,Servo_no;

extern float TargetX,TargetY,TargetZ,Target_claw;     					//电机目标值
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);

u8 Kinematic_Analysis_limb(float x,float y,float z,float t);
void Servo_move(void);
void Servo_data(float x,float y,float z);
void Set_Pwm_limb(float velocity1,float velocity2,float velocity3,float velocity4,float velocity5);
void Xianfu_Pwm_limb(void);
void Xianfu_Pwm2_limb(void);
float Position_PID1_limb (float Encoder_limb,float Target_limb);
float Position_PID2_limb (float Encoder_limb,float Target_limb);
float Position_PID3_limb (float Encoder_limb,float Target_limb);
float Position_PID4_limb (float Encoder_limb,float Target_limb);
float Position_PID5_limb (float Encoder_limb,float Target_limb);

void Set_PWM(int p1,int p2,int p3,int p4,int p5);

int myabs(int a);										                    //绝对值函数
#endif


