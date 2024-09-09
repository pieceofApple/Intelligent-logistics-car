#include "openmv.h"
#include "sys.h"


int openmv_data[20];
float openmv_mean_1,openmv_mean_2,openmv_mean_3;

//***************直线校准**********************************
void Calibration_line_R(float bias_x)	//直线校准车体 
{
	int mc;
	float x;
	if(bias_x!=0)
	{
		while(1)
		{
			Openmv_U2_read(1);		//模式1 --接收直线信息
			x=openmv_mean_1-bias_x;		////计算转动角度	
			mc=x*18.2;								////计算转动脉冲	
			
			if(x>-10&&x<10)
			{			
				printf("MC:%f\t\r\n\r\n",x);
				if(x>=-1&&x<=1) 
//				if(x==0)
				{
					printf("左直线校准车体角度结束\r\n");
					break;
				}			
				
				if(x>=0) //旋转x度
				{
					Set_BJ_Motor(5,mc);
				}
				else if(x<0)
				{
					Set_BJ_Motor(6,-mc);
				}		
				while(motor_flag);	
				delay_ms(500); 
			}
		}
	}
}


//***************圆校准**********************************
void Calibration_circle_FB(int bias_x)	//圆校准车体 前后
{
	int x;	
	while(1)
		{
			Openmv_U2_read(3);		//模式3 --接收圆信息
			if((openmv_mean_1-bias_x)>-60&&(openmv_mean_1-bias_x)<60)
			{
				x=(openmv_mean_1-bias_x)*16.66;		////计算转动脉冲	
				printf("FB:%d\t\r\n",x);	
				if(x>=-18*2&&x<=18*2) 
				{
					printf("/圆校准车体 前后结束\r\n\r\n");
					break;
				}		
				if(x>0)  Set_BJ_Motor(1,x);
				else Set_BJ_Motor(2,-x);		
				while(motor_flag);	
				delay_ms(500);
			}			
		}
}

void Calibration_circle_LR(int bias_x)	//圆校准车体 左右
{
	int x;	
	while(1)
		{
			Openmv_U2_read(3);		//模式3 --接收圆信息
			x=(openmv_mean_2-bias_x)*16.66;		////计算转动脉冲	
			printf("LR:%d\t\r\n",x);
			if(x>-18*2&&x<18*2) 
			{
				printf("/圆校准车体 左右结束\r\n\r\n");
				break;
			}		
			if(x>0) Set_BJ_Motor(4,x);
			else Set_BJ_Motor(3,-x);
			while(motor_flag);	
			delay_ms(500);
			
		}
}

//---------码垛色块校准-------------------
void Calibration_green_FB(int bias_x)	//圆校准车体 前后
{
	int x;	
	while(1)
		{
			Openmv_U2_read(2);		//模式3 --接收圆信息
			if((openmv_mean_2-bias_x)>-60&&(openmv_mean_2-bias_x)<60)
			{
				x=(openmv_mean_2-bias_x)*16.66;		////计算转动脉冲	
				printf("FB:%d\t\r\n",x);	
				if(x>=-18*2&&x<=18*2)  
				{
					printf("/色块校准车体 前后结束\r\n\r\n");
					break;
				}		
				if(x>0)  Set_BJ_Motor(1,x/5);
				else Set_BJ_Motor(2,-x/5);		
				while(motor_flag);	
				delay_ms(500);
			}			
		}
}

void Calibration_green_LR(int bias_x)	//圆校准车体 左右
{
	int x;	
	while(1)
		{
			Openmv_U2_read(2);		//模式3 --接收圆信息
			x=(openmv_mean_3-bias_x)*16.66;		////计算转动脉冲	
			printf("LR:%d\t\r\n",x);
			if(x>-18*2&&x<18*2) 
			{
				printf("/色块校准车体 左右结束\r\n\r\n");
				break;
			}		
			if(x>0) Set_BJ_Motor(4,x/5);
			else Set_BJ_Motor(3,-x/5);
			while(motor_flag);	
			delay_ms(500);
			
		}
}




void Openmv_U2_read(u8 num)	//接收openmv信息
{
	int len;
	u8 i;
	float x,data_0=0,data_1=0,data_2=0;
	
	switch(num)
	{
		case 1:P2=0;P3=0;i=1;break;
		case 2:P2=0;P3=1;i=1;break;
		case 3:P2=1;P3=0;i=5;break;			
	}	
	x=i;
	while(i--)
	{
		USART2_RX_STA=0;
		while(1)
		{
			if(USART2_RX_STA&0x8000)			      //串口4的接受判断，显示。
			{
				len=USART2_RX_STA&0x0FFF;//得到此次接收到的数据长度
				read(openmv_data,USART2_RX_BUF,len); 
				
				break;
			}
		}
		data_0+=openmv_data[0];
		data_1+=openmv_data[1];
		data_2+=openmv_data[2];
	}
	openmv_mean_1=data_0/x;
	openmv_mean_2=data_1/x;
	openmv_mean_3=data_2/x;
	
	P2=1;P3=1;
	
	switch(num)
	{
		case 1:printf("直线:%f\t%f\t\r\n\r\n",openmv_mean_1,openmv_mean_2);break;
		case 2:printf("颜色:%f\t\r\n\r\n",openmv_mean_1);break;
		case 3:printf("圆:%f\t%f\t\r\n\r\n",openmv_mean_1,openmv_mean_2);break;			
	}	
}

void read(int *data,u8 *num,int len)//数据拆分
{
	u8 i,j;
	data[0]=0;
	for(i=1,j=0;i<len;i++)
	{
		if(num[i]==0x2c)	 j++,data[j]=0;
		else if(num[i]==0x0d&&num[i+1]==0x0a)		break;
		else 	 data[j]=(num[i]-0x30)+data[j]*10;
	}
}

void IO_init(void)
{
	RCC->APB2ENR|=1<<4;   	//使能PORTC口时钟     	 	 
	GPIOC->CRL&=0XFF00FFFF; 
	GPIOC->CRL|=0X00330000;//PC 5 4 推挽输出   	 
     
}

void bubblesort(int *arr,int len)	//排序
{
  int j,k,temp;
	for(j=0;j<len-1;j++)
	{
		for(k=0;k<len-j-1;k++)
		{
			if(arr[k]>arr[k+1])
			{
				temp=arr[k];
				arr[k]=arr[k+1];
				arr[k+1]=temp;
			}
		}
	}
}

float mean(int *arr,u8 len)//掐头去尾平均数
{
	u8 i;
	float data=0;
	for(i=2;i<len-2;i++)
	{
		data+=arr[i];
	}
	data=data/(len-4);
	return data;
}
