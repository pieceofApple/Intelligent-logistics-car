#include "hmi.h"
#include "sys.h"

void UART5_Process(void)
{		
	int len;
	u8 i;
	if(UART5_RX_STA&0x8000)			      //����2�Ľ����жϣ���ʾ��
	{
		len=UART5_RX_STA&0x0FFF;//�õ��˴ν��յ������ݳ���
		for(i=0;i<len;i++)
		{	}	
		switch(UART5_RX_BUF[0])
		{
			case 0xA0:NO_flag=0; break; 	//����
		}
		UART5_RX_STA=0;  
	}
}

