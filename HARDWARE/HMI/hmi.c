#include "hmi.h"
#include "sys.h"

void UART5_Process(void)
{		
	int len;
	u8 i;
	if(UART5_RX_STA&0x8000)			      //串口2的接受判断，显示。
	{
		len=UART5_RX_STA&0x0FFF;//得到此次接收到的数据长度
		for(i=0;i<len;i++)
		{	}	
		switch(UART5_RX_BUF[0])
		{
			case 0xA0:NO_flag=0; break; 	//启动
		}
		UART5_RX_STA=0;  
	}
}

