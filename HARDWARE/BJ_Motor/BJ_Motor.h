#ifndef __BJ_MOTOR_H
#define __BJ_MOTOR_H	 
#include "sys.h"

//B��������˿ڶ���
#define En_4 	PAout(4)
#define Stp_4 PCout(6)	
#define Dir_4 PAout(5)	

#define En_1 	PAout(11)		
#define Stp_1 PCout(7)
#define Dir_1 PAout(12)	

#define En_2 	PBout(4)	
#define Stp_2 PCout(8)	
#define Dir_2 PBout(5)	

#define En_3 	PBout(8)
#define Stp_3 PCout(9)	
#define Dir_3 PBout(9)	

extern u8 motor_flag;
extern int BJ_data,motor_delay;
extern float Xs1,Xs2,rotateXs;
void BJ_Motor_Init(void);	//��ʼ��	
void BJ_Motor_move(void);	 				   

void Set_BJ_Motor(u8 mode,int num);

void Move_rotate(float num); //��ת num=��ת�Ƕ�
void FRONT(int n);		 //ǰ��n����  
void BACK(int n);			 //����n����
void LEFT(int n);		   //���n����
void RIGHT(int n);		 //�ҽ�n����

#endif














