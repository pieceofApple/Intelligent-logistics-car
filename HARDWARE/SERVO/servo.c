#include "servo.h"

float Target1=905,Target2=519,Target3=940,Target4=785,Target5=500;     //���Ŀ��ֵ
float Velocity1,Velocity2,Velocity3,Velocity4,Velocity5;     //���PWM����
float Position1=905,Position2=519,Position3=940,Position4=785,Position5=500;    
float Speed=25;                   //�˶��ٶ�����
float	Position_KP_limb=5,Position_KI_limb=0,Position_KD_limb=20;  //λ�ÿ���PID����

float TargetX=0,TargetY=35-1,TargetZ=121,Target_claw=-50;     					//���Ŀ��ֵ

u8 Servo_flag=0,Servo_no=1;


void Servo_move(void)
{
	
	Kinematic_Analysis_limb(TargetX,TargetY,TargetZ,Target_claw);//����ֵΪ0ʱ������ɣ��õ�Target1,Target2...������ֵ
	
	Xianfu_Pwm_limb();   //
	Velocity1=Position_PID1_limb(Position1,Target1);//���PID����
	Velocity2=Position_PID2_limb(Position2,Target2);//���PID����
	Velocity3=Position_PID3_limb(Position3,Target3);//���PID����
	Velocity4=Position_PID4_limb(Position4,Target4);//���PID����
	Velocity5=Position_PID5_limb(Position5,Target5);//���PID���� 
	Xianfu_Pwm2_limb();
	Set_Pwm_limb(Velocity1,Velocity2,Velocity3,Velocity4,Velocity5);

}
void Servo_data(float x,float y,float z)
{
	Servo_no=1;
	
	TargetX=x;
	TargetY=y;
	TargetZ=z;
}





/**************************************************************************
�������ܣ���ѧģ��
��ڲ�����ĩ��ִ����λ��̬
����  ֵ����
**************************************************************************/
u8 Kinematic_Analysis_limb(float x,float y,float z,float t) 
{
	float low,high,level,w,f,a,R_rot,R_side,rot,theta1,theta2,theta3,theta4,theta5;
		
//	R_rot=sqrt(x*x+y*y);
	R_rot=sqrt(y*y);
	R_side=sqrt(R_rot*R_rot+z*z);
	rot=x;
	high=acos((170*170+170*170-R_side*R_side)/(2*170*170));
	a=acos((170*170+R_side*R_side-170*170)/(2*170*R_side));
	w=asin(z/R_side);
	f=PI/2-a;
	
	low=f-w;

	high=PI-high;
	level=(0.5*PI)-(high+low);
	

	theta1=rot;
	theta2=low*180/PI;
	theta3=high*180/PI;
	theta4=level*180/PI;
	theta5=t;
	
	if(theta2>90)theta2=90;      //���ƶ�������Ƕȡ�135��
	if(theta2<-90)theta2=-90;    //���ƶ�������Ƕȡ�135��

	if(theta3>151)theta3=151;
	if(theta3<-100)theta3=-100;	    //���ƶ�������Ƕȡ�135��
	
	if(theta4>135)theta4=135;
	if(theta4<-135)theta4=-135;	    //���ƶ�������Ƕȡ�135��
	
	if(theta5>90)theta5=90;
	if(theta5<-90)theta5=-90;	    //���ƶ�������Ƕȡ�90��
	
	Target1   = 905-(theta1)*Ratio;
	Target2   = 713+(theta2)*Ratio;
	Target3   = 435+(theta3)*Ratio; 
	Target4   = 750+(theta4)*Ratio; 
	Target5   = 500+(theta5)*Ratio; 
	
	return 0;
}


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���,�����ж�ת��
��ڲ�������̨PWM������PWM���Ҷ��PWM
����  ֵ����
**************************************************************************/
void Set_Pwm_limb(float velocity1,float velocity2,float velocity3,float velocity4,float velocity5)
{	
		Position1+=velocity1;		   //�ٶȵĻ��֣��õ������λ��
		Position2+=velocity2;
		Position3+=velocity3;
		Position4+=velocity4;
		Position5+=velocity5;
	
	 PWM1=Position1;    //��ֵ��STM32�ļĴ���
	 PWM2=Position2;
	 PWM3=Position3;
	 PWM4=Position4;
	 PWM5=Position5;
}
void Xianfu_Pwm_limb(void)
{	
	  int Amplitude_H=1250, Amplitude_L=250;       //���������ֵ
    if(Target1<Amplitude_L)  Target1=Amplitude_L;	
		if(Target1>Amplitude_H)  Target1=Amplitude_H;	
	  if(Target2<Amplitude_L)  Target2=Amplitude_L;	
		if(Target2>Amplitude_H)  Target2=Amplitude_H;	
		if(Target3<Amplitude_L)  Target3=Amplitude_L;	
		if(Target3>Amplitude_H)  Target3=Amplitude_H;	
		if(Target4<Amplitude_L)  Target4=Amplitude_L;	
		if(Target4>Amplitude_H)  Target4=Amplitude_H;		
		if(Target5<Amplitude_L)  Target4=Amplitude_L;	
		if(Target5>Amplitude_H)  Target4=Amplitude_H;		
}
void Xianfu_Pwm2_limb(void)
{	
	  int Amplitude_H=Speed, Amplitude_L=-Speed;  
    if(Velocity1<Amplitude_L)  Velocity1=Amplitude_L;	
		if(Velocity1>Amplitude_H)  Velocity1=Amplitude_H;	
	  if(Velocity2<Amplitude_L)  Velocity2=Amplitude_L;	
		if(Velocity2>Amplitude_H)  Velocity2=Amplitude_H;	
		if(Velocity3<Amplitude_L)  Velocity3=Amplitude_L;	
		if(Velocity3>Amplitude_H)  Velocity3=Amplitude_H;	
		if(Velocity4<Amplitude_L)  Velocity4=Amplitude_L;	
		if(Velocity4>Amplitude_H)  Velocity4=Amplitude_H;	
		if(Velocity5<Amplitude_L)  Velocity4=Amplitude_L;	
		if(Velocity5>Amplitude_H)  Velocity4=Amplitude_H;	
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
float Position_PID1_limb (float Encoder_limb,float Target_limb)
{ 	
	 static float Bias_limb,Pwm_limb,Integral_bias_limb,Last_Bias_limb;
	 Bias_limb=Target_limb-Encoder_limb;                                  //����ƫ��
	 Integral_bias_limb+=Bias_limb;	                                 //���ƫ��Ļ���
	 Pwm_limb=(Position_KP_limb-0)*Bias_limb/100+Position_KI_limb*Integral_bias_limb/100+(Position_KD_limb)*(Bias_limb-Last_Bias_limb)/100;       //λ��ʽPID������
	 Last_Bias_limb=Bias_limb;                                       //������һ��ƫ�� 
	 return Pwm_limb;                                           //�������
}
float Position_PID2_limb (float Encoder_limb,float Target_limb)
{ 	
	 static float Bias_limb,Pwm_limb,Integral_bias_limb,Last_Bias_limb;
	 Bias_limb=Target_limb-Encoder_limb;                                  //����ƫ��
	 Integral_bias_limb+=Bias_limb;	                                 //���ƫ��Ļ���
	 Pwm_limb=Position_KP_limb*Bias_limb/100+Position_KI_limb*Integral_bias_limb/100+Position_KD_limb*(Bias_limb-Last_Bias_limb)/100;       //λ��ʽPID������
	 Last_Bias_limb=Bias_limb;                                       //������һ��ƫ�� 
	 return Pwm_limb;                                           //�������
}
float Position_PID3_limb (float Encoder_limb,float Target_limb)
{ 	
	 static float Bias_limb,Pwm_limb,Integral_bias_limb,Last_Bias_limb;
	 Bias_limb=Target_limb-Encoder_limb;                                  //����ƫ��
	 Integral_bias_limb+=Bias_limb;	                                 //���ƫ��Ļ���
	 Pwm_limb=Position_KP_limb*Bias_limb/100+Position_KI_limb*Integral_bias_limb/100+Position_KD_limb*(Bias_limb-Last_Bias_limb)/100;       //λ��ʽPID������
	 Last_Bias_limb=Bias_limb;                                       //������һ��ƫ�� 
	 return Pwm_limb;                                           //�������
}
float Position_PID4_limb (float Encoder_limb,float Target_limb)
{ 	
	 static float Bias_limb,Pwm_limb,Integral_bias_limb,Last_Bias_limb;
	 Bias_limb=Target_limb-Encoder_limb;                                  //����ƫ��
	 Integral_bias_limb+=Bias_limb;	                                 //���ƫ��Ļ���
	 Pwm_limb=Position_KP_limb*Bias_limb/100+Position_KI_limb*Integral_bias_limb/100+Position_KD_limb*(Bias_limb-Last_Bias_limb)/100;       //λ��ʽPID������
	 Last_Bias_limb=Bias_limb;                                       //������һ��ƫ�� 
	 return Pwm_limb;                                           //�������
}
float Position_PID5_limb (float Encoder_limb,float Target_limb)
{ 	
	 static float Bias_limb,Pwm_limb,Integral_bias_limb,Last_Bias_limb;
	 Bias_limb=Target_limb-Encoder_limb;                                  //����ƫ��
	 Integral_bias_limb+=Bias_limb;	                                 //���ƫ��Ļ���
	 Pwm_limb=Position_KP_limb*Bias_limb/100+Position_KI_limb*Integral_bias_limb/100+Position_KD_limb*(Bias_limb-Last_Bias_limb)/100;       //λ��ʽPID������
	 Last_Bias_limb=Bias_limb;                                       //������һ��ƫ�� 
	 return Pwm_limb;                                           //�������
}

int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
void TIM1_PWM_Init(u16 arr,u16 psc) 
{
	
	RCC->APB2ENR|=1<<11;       //ʹ��TIM1ʱ��   
	RCC->APB2ENR|=1<<6;        //PORTEʱ��ʹ�� 
	GPIOE->CRH&=0XF00F0F0F;    //PORTE9 11 13 14 �������
	GPIOE->CRH|=0X0BB0B0B0;    //PORTE9 11 13 14 �������	  
	
	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XFFFFFF3F; //���MAPR��[9:8]
	AFIO->MAPR|=3<<6;      //��ȫӳ��,TIM1_CH1->PE9 TIM1_CH2->PE11 TIM1_CH3->PE13 TIM1_CH4->PE14 
	
	TIM1->ARR=arr;             //�趨�������Զ���װֵ 
	TIM1->PSC=psc;             //Ԥ��Ƶ������Ƶ
	TIM1->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
	TIM1->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
	TIM1->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM1->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	
	TIM1->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	TIM1->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
	TIM1->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
	TIM1->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
	
	TIM1->CCER|=1<<0;         //CH1���ʹ��	
	TIM1->CCER|=1<<4;         //CH2���ʹ��	   
	TIM1->CCER|=1<<8;         //CH3���ʹ��	 
	TIM1->CCER|=1<<12;        //CH4���ʹ��
	TIM1->BDTR |= 1<<15;       //TIM����Ҫ��仰�������PWM
	TIM1->CR1=0x8000;          //ARPEʹ�� 
	TIM1->CR1|=0x01;          //ʹ�ܶ�ʱ�� 		
	
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;       //ʹ��TIM3ʱ��    
	RCC->APB2ENR|=1<<3;        //PORTBʱ��ʹ��     
	GPIOB->CRL&=0XFFFFFF00;    //PORTB0 1�������
	GPIOB->CRL|=0X000000BB;    //PORTB0 1�������	  
	TIM3->ARR=arr;             //�趨�������Զ���װֵ 
	TIM3->PSC=psc;             //Ԥ��Ƶ������Ƶ
	
	TIM3->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM3->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	
	TIM3->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
	TIM3->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	 
  
	TIM3->CCER|=1<<12;         //CH4���ʹ��	   
	TIM3->CCER|=1<<8;          //CH3���ʹ��	
	TIM3->CR1=0x8000;          //ARPEʹ�� 
	TIM3->CR1|=0x01;          //ʹ�ܶ�ʱ��
}

void TIM4_PWM_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;       //ʹ��TIM4ʱ��    
	RCC->APB2ENR|=1<<5;        //PORTdʱ��ʹ��     
	GPIOD->CRH&=0XF000FFFF;    //PORTD14 12  13�������
	GPIOD->CRH|=0X0BBB0000;    //PORTD14 12  13�������	 

	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
	AFIO->MAPR&=0XFFFFEFFF; //���MAPR��[9:8]
	AFIO->MAPR|=1<<12;      //��ȫӳ��,TIM1_CH1->PD12 TIM1_CH2->PD13 TIM1_CH3->PD14 TIM1_CH4->PD15 
	
	TIM4->ARR=arr;             //�趨�������Զ���װֵ 
	TIM4->PSC=psc;             //Ԥ��Ƶ������Ƶ
		
	TIM4->CCMR1|=6<<4;         //CH1 PWM1ģʽ	
	TIM4->CCMR1|=6<<12;        //CH2 PWM1ģʽ	
	TIM4->CCMR2|=6<<4;         //CH3 PWM1ģʽ	
	TIM4->CCMR2|=6<<12;        //CH4 PWM1ģʽ	
	
	TIM4->CCMR1|=1<<3;         //CH1Ԥװ��ʹ��	  
	TIM4->CCMR1|=1<<11;        //CH2Ԥװ��ʹ��	 
	TIM4->CCMR2|=1<<3;         //CH3Ԥװ��ʹ��	  
	TIM4->CCMR2|=1<<11;        //CH4Ԥװ��ʹ��	  
	
	TIM4->CCER|=1<<0;         //CH1���ʹ��	
	TIM4->CCER|=1<<4;         //CH2���ʹ��	   
	TIM4->CCER|=1<<8;         //CH3���ʹ��	 
	TIM4->CCER|=1<<12;        //CH4���ʹ��
	
	TIM4->CR1=0x8000;          //ARPEʹ�� 
	TIM4->CR1|=0x01;          //ʹ�ܶ�ʱ��
}

void Set_PWM(int p1,int p2,int p3,int p4,int p5)
{
	Servo_no=0;
	
	PWM1=p1;
	PWM2=p2-20;
	PWM3=p3;
	PWM4=p4;
	PWM5=p5;
}








