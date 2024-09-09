#include "USART2.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "sys.h"



u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//发送缓冲,最大USART2_MAX_SEND_LEN字节
u16 USART2_RX_STA=0; 

u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节
u16 USART3_RX_STA=0; 

  
u8 UART4_RX_BUF[UART4_MAX_RECV_LEN]; 				//接收缓冲,最大UART4_MAX_RECV_LEN个字节. 
u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 			//发送缓冲,最大UART4_MAX_SEND_LEN字节
u16 UART4_RX_STA=0; 

  
u8 UART5_RX_BUF[UART5_MAX_RECV_LEN]; 				//接收缓冲,最大UART5_MAX_RECV_LEN个字节.
u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			//发送缓冲,最大UART5_MAX_SEND_LEN字节
u16 UART5_RX_STA=0; 


////----------------------------------------------------------串口2-------------------------------------------------------------//

//串口接收缓存区 	

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度

  	 
void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART2->SR&(1<<5))//接收到数据
	{	 
		res=USART2->DR; 		
				
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
				else USART2_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
					else if(USART2_RX_BUF[0]!=0X41)USART2_RX_STA=0;//接收数据错误,重新开始接收
				}		 
			}
		}
	}  				 											 
}   
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart2_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<2;   	//使能PORTA口时钟  
 	GPIOA->CRL&=0XFFFF00FF;	//IO状态设置 PA2-TX PA3-RX
	GPIOA->CRL|=0X00008B00;	//IO状态设置 
	
	RCC->APB1ENR|=1<<17;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<17;   //复位串口3
	RCC->APB1RSTR&=~(1<<17);//停止复位	
	//波特率设置
 	USART2->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	USART2->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
	USART2->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//组2
	USART2_RX_STA=0;		//清零
}

//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART2->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART2->DR=USART2_TX_BUF[j];  
	} 
}

/**************************实现函数**********************************************

*功    能:		usart2发送一个字节

*********************************************************************************/
void usart2_send(u8 data)

{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	

}

/**************************实现函数**********************************************

*功    能:		usart2发送一个字符串

*********************************************************************************/

void usart2_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//循环发送数据
	{
		while((USART2->SR&0X40)==0);//等待发送结束		  
		USART2->DR=data[i];
	}	 
	while((USART2->SR&0X40)==0);//等待发送结束	
}


//////----------------------------------------------------------串口3-------------------------------------------------------------//



////串口接收缓存区 	

//void USART3_IRQHandler(void)
//{
//	u8 res;	   
//	static bool isGotFrameHeader = false;
//	static uint8_t frameHeaderCount = 0;
//	static uint8_t dataLength = 2;
//	static uint8_t dataCount = 0;   
//	if(USART3->SR&(1<<5))//接收到数据
//	{	 
//		res=USART3->DR; 	
//		if (!isGotFrameHeader) {  //判断帧头
//			if (res == 0x55) {
//				frameHeaderCount++;
//				if (frameHeaderCount == 2) {
//					frameHeaderCount = 0;
//					isGotFrameHeader = true;
//					dataCount = 1;
//				}
//			} else {
//				isGotFrameHeader = false;
//				dataCount = 0;
//				frameHeaderCount = 0;
//			}
//		}
//		if (isGotFrameHeader) { //接收接收数据部分
//			USART3_RX_BUF[dataCount] = res;
//			if (dataCount == 3) {
//				dataLength = USART3_RX_BUF[dataCount];
//				if (dataLength < 3 || dataLength > 7) {
//					dataLength = 3;
//					isGotFrameHeader = false;
//				}
//			}
//			dataCount++;
//			if (dataCount == dataLength + 3) {
//				if (isUartRxCompleted == false) {
//					isUartRxCompleted = true;
//					memcpy(LobotRxBuf, USART3_RX_BUF, dataCount+2);	
//				}
//				isGotFrameHeader = false;
//			}
//		}		
//	}  				 											 
//} 


void USART3_IRQHandler(void)
{
	u8 res;	      
	if(USART3->SR&(1<<5))//接收到数据
	{	 
		res=USART3->DR; 		
				
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
				else USART3_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}  				 											 
}   
//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void usart3_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<0;   	//开启辅助时钟 
	AFIO->MAPR|=0x00000030;
	
	RCC->APB2ENR|=1<<5;   	//使能PORTB口时钟  
 	GPIOD->CRH&=0XFFFFFF00;	//IO状态设置
	GPIOD->CRH|=0X0000008B;	//IO状态设置 
	
	
	RCC->APB1ENR|=1<<18;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<18;   //复位串口3
	RCC->APB1RSTR&=~(1<<18);//停止复位	
	//波特率设置
 	USART3->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	USART3->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
	USART3->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//组2
	USART3_RX_STA=0;		//清零
}

//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((USART3->SR&0X40)==0);			//循环发送,直到发送完毕   
		USART3->DR=USART3_TX_BUF[j];  
	} 
}


/**************************实现函数**********************************************

*功    能:		usart3发送一个字节

*********************************************************************************/
void usart3_send(u8 data)

{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	

}

/**************************实现函数**********************************************

*功    能:		usart2发送一个字符串

*********************************************************************************/

void usart3_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//循环发送数据
	{
		while((USART3->SR&0X40)==0);//等待发送结束		  
		USART3->DR=data[i];
	}	 
	while((USART3->SR&0X40)==0);//等待发送结束	
}


void uartWriteBuf(uint8_t *buf, uint8_t len)
{	
	while(len--)
	{
		while ((USART3->SR & 0x40) == 0);
		usart3_send(*buf++);
	}
}



////----------------------------------------------------------串口4-------------------------------------------------------------//



//串口接收缓存区 	

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
  	 
void UART4_IRQHandler(void)
{		
	int res;
	if(UART4->SR&(1<<5))	//接收到数据
	{	 
		res=UART4->DR; 
		
		if((UART4_RX_STA&0x4000)==0)//接收未完成
		{
			if(UART4_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)UART4_RX_STA=0;//接收错误,重新开始
				else 
					UART4_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)UART4_RX_STA|=0x4000;
				else
				{
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=res-0x30;
					UART4_RX_STA++;
					if(UART4_RX_STA>(USART_REC_LEN-1))UART4_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  	
	}
} 
//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void uart4_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<4;   	//使能PORTA口时钟  
 	GPIOC->CRH&=0XFFFF00FF;	//IO状态设置 PC10-TX PC11-RX
	GPIOC->CRH|=0X00008B00;	//IO状态设置 
	
	RCC->APB1ENR|=1<<19;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<19;   //复位串口4
	RCC->APB1RSTR&=~(1<<19);//停止复位	
	//波特率设置
 	UART4->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	UART4->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
	UART4->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,UART4_IRQn,2);//组2
	UART4_RX_STA=0;		//清零
}

//串口4,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u4_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART4_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((UART4->SR&0X40)==0);			//循环发送,直到发送完毕   
		UART4->DR=UART4_TX_BUF[j];  
	} 
}

/**************************实现函数**********************************************

*功    能:		uart4发送一个字节

*********************************************************************************/
void uart4_send(u8 data)

{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);	

}

/**************************实现函数**********************************************

*功    能:		uart4发送一个字符串

*********************************************************************************/

void uart4_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//循环发送数据
	{
		while((UART4->SR&0X40)==0);//等待发送结束		  
		UART4->DR=data[i];
	}	 
	while((UART4->SR&0X40)==0);//等待发送结束	
}




////----------------------------------------------------------串口5-------------------------------------------------------------//



//串口接收缓存区 	

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
  	 
void UART5_IRQHandler(void)
{
//	u8 res;	      
//	if(UART5->SR&(1<<5))//接收到数据
//	{	 
//		res=UART5->DR; 			 
//	}  			
	u8 res;	      
	if(UART5->SR&(1<<5))//接收到数据
	{	 
		res=UART5->DR; 		
				
		if((UART5_RX_STA&0x8000)==0)//接收未完成
		{
			if(UART5_RX_STA&0x4000)//接收到了0x0d
			{
				if(res!=0x0a)UART5_RX_STA=0;//接收错误,重新开始
				else UART5_RX_STA|=0x8000;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)UART5_RX_STA|=0x4000;
				else
				{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=res;
					UART5_RX_STA++;
					if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}
	}  				 	
}   
//初始化IO 串口5
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率 
void uart5_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<4;   	//使能PORTC口时钟  
 	GPIOC->CRH&=0XFFF0FFFF;	//IO状态设置 PC12-TX PD2-RX
	GPIOC->CRH|=0X000B0000;	//IO状态设置 
	
	RCC->APB2ENR|=1<<5;   	//使能PORTD口时钟  
 	GPIOD->CRL&=0XFFFFF0FF;	//IO状态设置 PC12-TX PD2-RX
	GPIOD->CRL|=0X00000800;	//IO状态设置 
	
	RCC->APB1ENR|=1<<20;  	//使能串口时钟 	 
	RCC->APB1RSTR|=1<<20;   //复位串口5
	RCC->APB1RSTR&=~(1<<20);//停止复位	
	//波特率设置
 	UART5->BRR=(pclk1*1000000)/(bound);// 波特率设置	 
	UART5->CR1|=0X200C;  	//1位停止,无校验位.
	//使能接收中断 
	UART5->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,UART5_IRQn,2);//组2
	UART5_RX_STA=0;		//清零
}

//串口5,printf 函数
//确保一次发送数据不超过UART5_MAX_SEND_LEN字节
void u5_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART5_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while((UART5->SR&0X40)==0);			//循环发送,直到发送完毕   
		UART5->DR=UART5_TX_BUF[j];  
	} 
}

/**************************实现函数**********************************************

*功    能:		uart5发送一个字节

*********************************************************************************/
void uart5_send(u8 data)

{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	

}

/**************************实现函数**********************************************

*功    能:		uart5发送一个字符串

*********************************************************************************/

void uart5_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//循环发送数据
	{
		while((UART5->SR&0X40)==0);//等待发送结束		  
		UART5->DR=data[i];
	}	 
	while((UART5->SR&0X40)==0);//等待发送结束	
}
