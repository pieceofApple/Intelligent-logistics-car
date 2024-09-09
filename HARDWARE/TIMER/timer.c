#include "timer.h"
#include "sys.h"

//��ʱ��6�жϷ������	
int time=0;
void TIM6_IRQHandler(void)
{ 		   
	if(TIM6->SR&0X0001)//����ж�
	{			
		BJ_Motor_move();	
		
		time++;
		if(time%10==0)
		{			
			UASRT_Process();
			UART5_Process();
			if(Servo_no)
			{
				Servo_move();
			}
		}
		if(time==1000)
		{
			LED=~LED;    
			time=0;		
		}
			
	}				   
	TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��6!
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;	//TIM6ʱ��ʹ��    
 	TIM6->ARR=arr;  	//�趨�������Զ���װֵ//�պ�1ms    
	TIM6->PSC=psc;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM6->DIER|=1<<0;   //��������ж�	  
	TIM6->CR1|=0x01;    //ʹ�ܶ�ʱ��6
  MY_NVIC_Init(1,2,TIM6_IRQn,2);//��ռ1�������ȼ�3����2									 
}
void LED_Init(void)
{
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��     	 	 
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 �������   	 
     
}
