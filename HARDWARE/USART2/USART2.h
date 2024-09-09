#ifndef __USART2_H
#define __USART2_H	 
#include "sys.h"
#include "stdio.h"	 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200

#define USART2_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART2_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART2_RX_EN 			1					//0,������;1,����.
	  	
#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

#define UART4_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define UART4_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define UART4_RX_EN 			1					//0,������;1,����.

#define UART5_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define UART5_RX_EN 			1					//0,������;1,����.
			

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 		//���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
extern u16 USART2_RX_STA;   						//��������״̬

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
extern u16 USART3_RX_STA;   		

extern u8  UART4_RX_BUF[UART4_MAX_RECV_LEN]; 		//���ջ���,���USART4_MAX_RECV_LEN�ֽ�
extern u8  UART4_TX_BUF[UART4_MAX_SEND_LEN]; 		//���ͻ���,���USART4_MAX_SEND_LEN�ֽ�
extern u16 UART4_RX_STA;   						//��������״̬


extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���USART5_MAX_RECV_LEN�ֽ�
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���USART5_MAX_SEND_LEN�ֽ�
extern u16 UART5_RX_STA;   						//��������״̬

//����봮���жϽ��գ��벻Ҫע�����º궨��

void usart2_init(u32 pclk1,u32 bound);
void u2_printf(char* fmt,...);
void usart2_send(u8 data);
void usart2_sendString(u8 *data,u8 len);
void uartWriteBuf(uint8_t *buf, uint8_t len);

void usart3_init(u32 pclk1,u32 bound);
void u3_printf(char* fmt,...);
void usart3_send(u8 data);
void usart3_sendString(u8 *data,u8 len);

void uart4_init(u32 pclk1,u32 bound);
void u4_printf(char* fmt,...);
void uart4_send(u8 data);
void uart4_sendString(u8 *data,u8 len);

void uart5_init(u32 pclk1,u32 bound);
void u5_printf(char* fmt,...);
void uart5_send(u8 data);
void uart5_sendString(u8 *data,u8 len);


#endif
