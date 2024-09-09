#include "timer.h"
#include "sys.h"

//定时器6中断服务程序	
int time=0;
void TIM6_IRQHandler(void)
{ 		   
	if(TIM6->SR&0X0001)//溢出中断
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
	TIM6->SR&=~(1<<0);//清除中断标志位 	    
}
//通用定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器6!
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;	//TIM6时钟使能    
 	TIM6->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM6->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM6->DIER|=1<<0;   //允许更新中断	  
	TIM6->CR1|=0x01;    //使能定时器6
  MY_NVIC_Init(1,2,TIM6_IRQn,2);//抢占1，子优先级3，组2									 
}
void LED_Init(void)
{
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟     	 	 
	GPIOA->CRH&=0XFFFFFFF0; 
	GPIOA->CRH|=0X00000003;//PA8 推挽输出   	 
     
}
