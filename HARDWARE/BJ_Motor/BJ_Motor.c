#include "BJ_Motor.h"

u8 motor_flag=0;
int BJ_data=0,motor_delay=16-0;
//float Xs1=8.488;
float Xs1=16.666667;
float Xs2=17.777778;
float rotateXs=56.8889;
//float rotateXs=28.4444;

void BJ_Motor_Init(void)
{
		
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��     	 	 
	GPIOA->CRL&=0XFF00FFFF; 
	GPIOA->CRL|=0X00330000;//PA4 5 �������  
	GPIOA->CRH&=0XFFF00FFF; 
	GPIOA->CRH|=0X00033000;//PA11 12 ������� 
	
	RCC->APB2ENR|=1<<3;   	//ʹ��PORTB��ʱ��     	 	 
	GPIOB->CRL&=0XFF00FFFF; 
	GPIOB->CRL|=0X00330000;//PB4 5 �������  
	GPIOB->CRH&=0XFFFFFF00; 
	GPIOB->CRH|=0X00000033;//PB8 9 ������� 
	
	RCC->APB2ENR|=1<<4;   	//ʹ��PORTC��ʱ��     	 	 
	GPIOC->CRL&=0X00FFFFFF; 
	GPIOC->CRL|=0X33000000;//PC6 7 �������  
	GPIOC->CRH&=0XFFFFFF00; 
	GPIOC->CRH|=0X00000033;//PC8 9 ������� 
     
	En_1=0;	Stp_1=0;	Dir_1=0;
	En_2=0;	Stp_2=0;	Dir_2=0;
	En_3=0;	Stp_3=0;	Dir_3=0;
	En_4=0;	Stp_4=0;	Dir_4=0;
}
void BJ_Motor_move(void)
{	
	static int delay_num=0,BJ_num=0,MC_num=0;
	if(motor_flag)
	{
		delay_num++;
		if(delay_num==motor_delay)
		{
			Stp_1=~Stp_1;
			Stp_2=~Stp_2;
			Stp_3=~Stp_3;
			Stp_4=~Stp_4;
			
			delay_num=0;
			BJ_num++;
			if((BJ_data-BJ_num)<1600)	//���һȦ����
			{
				MC_num++;
				if(MC_num==200)
				{
					MC_num=0;
					motor_delay++;
					if(motor_delay>8) motor_delay=8;
				}
			}
			else if(BJ_num<1600)	//��һȦ����
			{				
				MC_num++;
				if(MC_num==200)
				{
					MC_num=0;
					motor_delay--;
					if(motor_delay<1) motor_delay=1;
				}				
			}
			if(BJ_num==BJ_data)
				{
					BJ_num=0;
					motor_flag=0;
				}		
		}
	}
}

void Set_BJ_Motor(u8 mode,int num)
{
	switch(mode)
	{
		case 1: Dir_1=0;Dir_2=0;Dir_3=1;Dir_4=1;break;
		case 2: Dir_1=1;Dir_2=1;Dir_3=0;Dir_4=0;break;
		case 3: Dir_1=1;Dir_2=0;Dir_3=1;Dir_4=0;break;
		case 4: Dir_1=0;Dir_2=1;Dir_3=0;Dir_4=1;break;//
		case 5: Dir_1=0;Dir_2=0;Dir_3=0;Dir_4=0;break;
		case 6: Dir_1=1;Dir_2=1;Dir_3=1;Dir_4=1;break;
	}
	BJ_data=num;
	motor_delay=8;
	motor_flag=1; 
}

void FRONT(int n)		 //ǰ��n����  
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
	while(motor_flag);	
	delay_ms(500); 
}
void BACK(int n)		 //����n����
{
	n=n*Xs1;
	Set_BJ_Motor(2,n);
	while(motor_flag);	
	delay_ms(500); 
}
void LEFT(int n)	   //���n����
{
	n=n*Xs2;
	Set_BJ_Motor(3,n);
	while(motor_flag);	
	delay_ms(500); 
}
void RIGHT(int n)		 //�ҽ�n����
{
	n=n*Xs2;
	Set_BJ_Motor(4,n);
	while(motor_flag);	
	delay_ms(500); 
}

void Move_rotate(float num) //��ת num=��ת�Ƕ� 
{
	num=num*rotateXs;
	if(num>=0) 
	{
		Set_BJ_Motor(5,num);
	}
	else if(num<0)
	{
		num=-num;
		Set_BJ_Motor(6,num);
	}		
	while(motor_flag);	
	delay_ms(500); 
}

//////////////�޵ȴ������ڵ����˶�����ʵ�������޹�///////////////////////////////////////


void U_FRONT(int n)		 //ǰ��n����  
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}
void U_BACK(int n)			 //����n����
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}
void U_LEFT(int n)		   //���n����
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}
void U_RIGHT(int n)		 //�ҽ�n����
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}


void U_FRONT_C(int n)		 //ǰ��n���� 
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}
void U_BACK_C(int n)			 //����n���� 
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}
void U_LEFT_C(int n)		   //���n���� 
{
	n=n*Xs1;
	Set_BJ_Motor(1,n);
}
void U_RIGHT_C(int n)		 //�ҽ�n���� 
{
	Dir_1=0,Dir_2=1,Dir_3=0,Dir_4=1,BJ_data=n;
	motor_delay=10;
	motor_flag=1; 
}

void U_Move_rotate(int num) //��ת num=��ת�Ƕ�
{
	if(num>0) Dir_1=0,Dir_2=0,Dir_3=0,Dir_4=0,BJ_data=num*rotateXs;
	else if(num<0) Dir_1=1,Dir_2=1,Dir_3=1,Dir_4=1,BJ_data=-num*rotateXs;
	motor_delay=10;
	motor_flag=1; 
}

void U_Move_rotate_C(int num) //��ת num=��ת����
{
	if(num>0) Dir_1=0,Dir_2=0,Dir_3=0,Dir_4=0,BJ_data=num;
	else if(num<0) Dir_1=1,Dir_2=1,Dir_3=1,Dir_4=1,BJ_data=-num;
	motor_delay=10;
	motor_flag=1; 
}

