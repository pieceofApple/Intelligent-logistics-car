#include "USART2.h"	  
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "sys.h"



u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
u16 USART2_RX_STA=0; 

u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
u16 USART3_RX_STA=0; 

  
u8 UART4_RX_BUF[UART4_MAX_RECV_LEN]; 				//���ջ���,���UART4_MAX_RECV_LEN���ֽ�. 
u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 			//���ͻ���,���UART4_MAX_SEND_LEN�ֽ�
u16 UART4_RX_STA=0; 

  
u8 UART5_RX_BUF[UART5_MAX_RECV_LEN]; 				//���ջ���,���UART5_MAX_RECV_LEN���ֽ�.
u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 			//���ͻ���,���UART5_MAX_SEND_LEN�ֽ�
u16 UART5_RX_STA=0; 


////----------------------------------------------------------����2-------------------------------------------------------------//

//���ڽ��ջ����� 	

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���

  	 
void USART2_IRQHandler(void)
{
	u8 res;	      
	if(USART2->SR&(1<<5))//���յ�����
	{	 
		res=USART2->DR; 		
				
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					else if(USART2_RX_BUF[0]!=0X41)USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����
				}		 
			}
		}
	}  				 											 
}   
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart2_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<2;   	//ʹ��PORTA��ʱ��  
 	GPIOA->CRL&=0XFFFF00FF;	//IO״̬���� PA2-TX PA3-RX
	GPIOA->CRL|=0X00008B00;	//IO״̬���� 
	
	RCC->APB1ENR|=1<<17;  	//ʹ�ܴ���ʱ�� 	 
	RCC->APB1RSTR|=1<<17;   //��λ����3
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	
	//����������
 	USART2->BRR=(pclk1*1000000)/(bound);// ����������	 
	USART2->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART2->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//��2
	USART2_RX_STA=0;		//����
}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART2->SR&0X40)==0);			//ѭ������,ֱ���������   
		USART2->DR=USART2_TX_BUF[j];  
	} 
}

/**************************ʵ�ֺ���**********************************************

*��    ��:		usart2����һ���ֽ�

*********************************************************************************/
void usart2_send(u8 data)

{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	

}

/**************************ʵ�ֺ���**********************************************

*��    ��:		usart2����һ���ַ���

*********************************************************************************/

void usart2_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//ѭ����������
	{
		while((USART2->SR&0X40)==0);//�ȴ����ͽ���		  
		USART2->DR=data[i];
	}	 
	while((USART2->SR&0X40)==0);//�ȴ����ͽ���	
}


//////----------------------------------------------------------����3-------------------------------------------------------------//



////���ڽ��ջ����� 	

//void USART3_IRQHandler(void)
//{
//	u8 res;	   
//	static bool isGotFrameHeader = false;
//	static uint8_t frameHeaderCount = 0;
//	static uint8_t dataLength = 2;
//	static uint8_t dataCount = 0;   
//	if(USART3->SR&(1<<5))//���յ�����
//	{	 
//		res=USART3->DR; 	
//		if (!isGotFrameHeader) {  //�ж�֡ͷ
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
//		if (isGotFrameHeader) { //���ս������ݲ���
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
	if(USART3->SR&(1<<5))//���յ�����
	{	 
		res=USART3->DR; 		
				
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}  				 											 
}   
//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void usart3_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<0;   	//��������ʱ�� 
	AFIO->MAPR|=0x00000030;
	
	RCC->APB2ENR|=1<<5;   	//ʹ��PORTB��ʱ��  
 	GPIOD->CRH&=0XFFFFFF00;	//IO״̬����
	GPIOD->CRH|=0X0000008B;	//IO״̬���� 
	
	
	RCC->APB1ENR|=1<<18;  	//ʹ�ܴ���ʱ�� 	 
	RCC->APB1RSTR|=1<<18;   //��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	
	//����������
 	USART3->BRR=(pclk1*1000000)/(bound);// ����������	 
	USART3->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	USART3->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//��2
	USART3_RX_STA=0;		//����
}

//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((USART3->SR&0X40)==0);			//ѭ������,ֱ���������   
		USART3->DR=USART3_TX_BUF[j];  
	} 
}


/**************************ʵ�ֺ���**********************************************

*��    ��:		usart3����һ���ֽ�

*********************************************************************************/
void usart3_send(u8 data)

{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	

}

/**************************ʵ�ֺ���**********************************************

*��    ��:		usart2����һ���ַ���

*********************************************************************************/

void usart3_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//ѭ����������
	{
		while((USART3->SR&0X40)==0);//�ȴ����ͽ���		  
		USART3->DR=data[i];
	}	 
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���	
}


void uartWriteBuf(uint8_t *buf, uint8_t len)
{	
	while(len--)
	{
		while ((USART3->SR & 0x40) == 0);
		usart3_send(*buf++);
	}
}



////----------------------------------------------------------����4-------------------------------------------------------------//



//���ڽ��ջ����� 	

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
  	 
void UART4_IRQHandler(void)
{		
	int res;
	if(UART4->SR&(1<<5))	//���յ�����
	{	 
		res=UART4->DR; 
		
		if((UART4_RX_STA&0x4000)==0)//����δ���
		{
			if(UART4_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)UART4_RX_STA=0;//���մ���,���¿�ʼ
				else 
					UART4_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)UART4_RX_STA|=0x4000;
				else
				{
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=res-0x30;
					UART4_RX_STA++;
					if(UART4_RX_STA>(USART_REC_LEN-1))UART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  	
	}
} 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void uart4_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<4;   	//ʹ��PORTA��ʱ��  
 	GPIOC->CRH&=0XFFFF00FF;	//IO״̬���� PC10-TX PC11-RX
	GPIOC->CRH|=0X00008B00;	//IO״̬���� 
	
	RCC->APB1ENR|=1<<19;  	//ʹ�ܴ���ʱ�� 	 
	RCC->APB1RSTR|=1<<19;   //��λ����4
	RCC->APB1RSTR&=~(1<<19);//ֹͣ��λ	
	//����������
 	UART4->BRR=(pclk1*1000000)/(bound);// ����������	 
	UART4->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	UART4->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,UART4_IRQn,2);//��2
	UART4_RX_STA=0;		//����
}

//����4,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u4_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART4_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((UART4->SR&0X40)==0);			//ѭ������,ֱ���������   
		UART4->DR=UART4_TX_BUF[j];  
	} 
}

/**************************ʵ�ֺ���**********************************************

*��    ��:		uart4����һ���ֽ�

*********************************************************************************/
void uart4_send(u8 data)

{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);	

}

/**************************ʵ�ֺ���**********************************************

*��    ��:		uart4����һ���ַ���

*********************************************************************************/

void uart4_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//ѭ����������
	{
		while((UART4->SR&0X40)==0);//�ȴ����ͽ���		  
		UART4->DR=data[i];
	}	 
	while((UART4->SR&0X40)==0);//�ȴ����ͽ���	
}




////----------------------------------------------------------����5-------------------------------------------------------------//



//���ڽ��ջ����� 	

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
  	 
void UART5_IRQHandler(void)
{
//	u8 res;	      
//	if(UART5->SR&(1<<5))//���յ�����
//	{	 
//		res=UART5->DR; 			 
//	}  			
	u8 res;	      
	if(UART5->SR&(1<<5))//���յ�����
	{	 
		res=UART5->DR; 		
				
		if((UART5_RX_STA&0x8000)==0)//����δ���
		{
			if(UART5_RX_STA&0x4000)//���յ���0x0d
			{
				if(res!=0x0a)UART5_RX_STA=0;//���մ���,���¿�ʼ
				else UART5_RX_STA|=0x8000;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)UART5_RX_STA|=0x4000;
				else
				{
					UART5_RX_BUF[UART5_RX_STA&0X3FFF]=res;
					UART5_RX_STA++;
					if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}
	}  				 	
}   
//��ʼ��IO ����5
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������ 
void uart5_init(u32 pclk1,u32 bound)
{  	 
	RCC->APB2ENR|=1<<4;   	//ʹ��PORTC��ʱ��  
 	GPIOC->CRH&=0XFFF0FFFF;	//IO״̬���� PC12-TX PD2-RX
	GPIOC->CRH|=0X000B0000;	//IO״̬���� 
	
	RCC->APB2ENR|=1<<5;   	//ʹ��PORTD��ʱ��  
 	GPIOD->CRL&=0XFFFFF0FF;	//IO״̬���� PC12-TX PD2-RX
	GPIOD->CRL|=0X00000800;	//IO״̬���� 
	
	RCC->APB1ENR|=1<<20;  	//ʹ�ܴ���ʱ�� 	 
	RCC->APB1RSTR|=1<<20;   //��λ����5
	RCC->APB1RSTR&=~(1<<20);//ֹͣ��λ	
	//����������
 	UART5->BRR=(pclk1*1000000)/(bound);// ����������	 
	UART5->CR1|=0X200C;  	//1λֹͣ,��У��λ.
	//ʹ�ܽ����ж� 
	UART5->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,UART5_IRQn,2);//��2
	UART5_RX_STA=0;		//����
}

//����5,printf ����
//ȷ��һ�η������ݲ�����UART5_MAX_SEND_LEN�ֽ�
void u5_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART5_TX_BUF);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while((UART5->SR&0X40)==0);			//ѭ������,ֱ���������   
		UART5->DR=UART5_TX_BUF[j];  
	} 
}

/**************************ʵ�ֺ���**********************************************

*��    ��:		uart5����һ���ֽ�

*********************************************************************************/
void uart5_send(u8 data)

{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	

}

/**************************ʵ�ֺ���**********************************************

*��    ��:		uart5����һ���ַ���

*********************************************************************************/

void uart5_sendString(u8 *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)		//ѭ����������
	{
		while((UART5->SR&0X40)==0);//�ȴ����ͽ���		  
		UART5->DR=data[i];
	}	 
	while((UART5->SR&0X40)==0);//�ȴ����ͽ���	
}
