#include "control.h"
#include "sys.h"
int servo_data[10];	   

u8 target_data[10]={1,2,3,0,1,2,3}; 	//二维码数据

u8 NO_flag=1;
float line_Ra=91;
float line_La=90;
float line_Ld=140;

int circle_FB=147; 
int circle_LR=139;

void Task_1(void)	//第一轮
{
	int i;
	LEFT(180-15);	delay_ms(500);
	FRONT(640-50+20);	delay_ms(500);
	
	UART4_Process();	//二维码识别
	
	FRONT(820+35-20);	delay_ms(500);
	for(i=0;i<3;i++)
	{
		Get_yuanpan(target_data[i]); //圆盘上物料target_data[i]为抓取颜色	
		Put_wan(target_data[i]);	//放target_data[i]号碗
	}

	FRONT(420);	delay_ms(500);

	Move_rotate(-90);delay_ms(900);	
	
	FRONT(655+100-10);	delay_ms(500);
	RIGHT(15+10);			delay_ms(500);
	
	circle_move();//校准靶区
	
	for(i=0;i<3;i++)
	{
		Get_wan(target_data[i]);	//取target_data[i]号碗
		Put_one(target_data[i]);//放置粗加工区
	}
	
	for(i=0;i<3;i++)
	{
		Get_two(target_data[i]);//抓取粗加工区
		Put_wan(target_data[i]);	//放target_data[i]号碗
	}
	
	FRONT(790-150);	delay_ms(500);
	Move_rotate(-90);delay_ms(900);	
	FRONT(680);	delay_ms(500);

	circle_move();//校准靶区
	
	for(i=0;i<3;i++)
	{
		Get_wan(target_data[i]);	//取target_data[i]号碗
		Put_two_A(target_data[i]);	//放置半成品区 底层码垛
	}	

}
void Task_2(void)	//第二轮
{
	
	int i;
  BACK(850);	delay_ms(500);
	Move_rotate(90);delay_ms(900);	
	
	BACK(805+790-20+25);	delay_ms(500);
	Move_rotate(90);delay_ms(900);	
	BACK(430+20);	delay_ms(500);

	for(i=4;i<7;i++)
	{
		Get_yuanpan(target_data[i]); //圆盘上物料target_data[i]为抓取颜色	
		Put_wan(target_data[i]);	//放target_data[i]号碗
	}

	FRONT(430);	delay_ms(500);

	Move_rotate(-90);delay_ms(900);	
	
	FRONT(655+100);	delay_ms(500);
	
	circle_move();//校准靶区
	
	for(i=4;i<7;i++)
	{
		Get_wan(target_data[i]);	//取target_data[i]号碗
		Put_one(target_data[i]);//放置粗加工区
	}
	
	for(i=4;i<7;i++)
	{
		Get_two(target_data[i]);//抓取粗加工区
		Put_wan(target_data[i]);	//放target_data[i]号碗
	}
	
	
	FRONT(790-130);	delay_ms(500);
	Move_rotate(-90);delay_ms(900);	
	FRONT(840);	delay_ms(500);
	
	Servo_data(97,TargetY,TargetZ);delay_ms(500);
	Servo_data(TargetX,152,191);delay_ms(700);
	Set_PWM(PWM1,810-30,596+150-50,1198-150+50,PWM5);delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);;delay_ms(500);delay_ms(500);
	
	Calibration_green_FB(circle_FB);
	Calibration_green_LR(circle_LR);
	Servo_data(97,35,121);delay_ms(500);

	for(i=4;i<7;i++)
	{
		Get_wan(target_data[i]);	//取target_data[i]号碗
		Put_two_B(target_data[i]);//放置半成品区 顶层码垛
	}	

	FRONT(1000-30);	delay_ms(500);
	
	Move_rotate(-90);delay_ms(900);	
	RIGHT(100-15);	delay_ms(500);
	FRONT(1760-5);	delay_ms(500);
	
}

void circle_move(void)	//校准靶区
{
	int x1,x2,y1,y2;
	float y,a;
	
	Servo_data(97,TargetY,TargetZ);delay_ms(500);
	Servo_data(TargetX,152,191);delay_ms(700);
	Set_PWM(PWM1,810-30,596,1198,PWM5);delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);;delay_ms(500);delay_ms(500);
	
	Calibration_circle_FB(circle_FB);	//圆校准车体 前后
	Calibration_circle_LR(circle_LR);	//圆校准车体 左右
	
	Openmv_U2_read(3);		//模式3 --接收圆信息
	x1=openmv_data[0];
	y1=openmv_data[1];
	
	FRONT(300);delay_ms(500);delay_ms(500);delay_ms(500);delay_ms(500);
	
	Calibration_circle_FB(circle_FB);	//圆校准车体 前后
	Openmv_U2_read(3);		//模式3 --接收圆信息
	x2=openmv_data[0];
  
	
	y=y2-y1;
	a=asin(y/334)*180/PI;
	
	printf("x1:%d\ty1:%d\tx2:%d\ty2:%d\ta:%f\r\n,",x1,y1,x2,y2,a);
		

		if((a>30)||(a<-30)){
			Move_rotate(1);
			Move_rotate(-1);
		}
		else if(a==0){
			Move_rotate(1);
			Move_rotate(-1);				
		}
		else
			Move_rotate(a);
	
	BACK(150);delay_ms(500);delay_ms(500);delay_ms(500);
	Calibration_circle_FB(circle_FB);	//圆校准车体 前后
	Calibration_circle_LR(circle_LR);	//圆校准车体 左右
	
	Servo_data(TargetX,TargetY,TargetZ);delay_ms(500);
	Servo_data(TargetX,35,121);delay_ms(500);
}



void Get_yuanpan(u8 num)	//抓取圆盘上物料 num为抓取颜色
{
	Servo_data(97,TargetY,TargetZ);delay_ms(500);	
		
	Servo_data(97,122-5,140);	Target_claw=-57;delay_ms(500);	
	Set_PWM(PWM1,PWM2-20,PWM3,965,500);delay_ms(500);
	while(1)
	{		
		Openmv_U2_read(2);		//模式2 --接收颜色信息
		if(openmv_data[0]==num) break;
	}
	if(num<3)
	{
	Set_PWM(PWM1,PWM2,PWM3,639+20,PWM5);delay_ms(200);
	Set_PWM(PWM1,787+10,PWM3,PWM4,PWM5);delay_ms(300);
	Set_PWM(PWM1,PWM2,851-0,PWM4,PWM5);delay_ms(200);	
	Set_PWM(PWM1,787-10,PWM3,PWM4,288);delay_ms(300);

	Set_PWM(PWM1,669,PWM3,PWM4,PWM5);delay_ms(300);
	Servo_data(TargetX,TargetY,TargetZ);delay_ms(100);
	Servo_data(TargetX,35,121);delay_ms(200);

	}
	else
	{
		
	Set_PWM(PWM1,PWM2,PWM3,639+20,PWM5);delay_ms(200);
	Set_PWM(PWM1,787+10,PWM3,PWM4,PWM5);delay_ms(300);
	Set_PWM(PWM1,PWM2,851-0,PWM4,PWM5);delay_ms(200);	
	Set_PWM(PWM1,787-10,PWM3,PWM4,288);delay_ms(300);

	Set_PWM(PWM1,669,PWM3,PWM4,PWM5);delay_ms(300);
	Servo_data(TargetX,TargetY,TargetZ);delay_ms(100);
	Servo_data(TargetX,35,121);delay_ms(200);
	}
}

void Put_one(u8 num)//放置粗加工区
{
	switch(num)
	{
		case 1:Put_one_1();break;
		case 2:Put_one_2();break;
		case 3:Put_one_3();break;
	}
}
void Get_two(u8 num)//抓取粗加工区
{
	switch(num)
	{
		case 1:Get_two_1();break;
		case 2:Get_two_2();break;
		case 3:Get_two_3();break;
	}
}
void Put_two_A(u8 num)//放置半成品区 底层码垛
{
	switch(num)
	{
		case 1:Put_one_1();break;
		case 2:Put_one_2();break;
		case 3:Put_one_3();break;
	}
}
void Put_two_B(u8 num)//放置半成品区 顶层码垛
{
	switch(num)
	{
		case 1:Put_two_1();break;
		case 2:Put_two_2();break;
		case 3:Put_two_3();break;
	}
}


void Put_one_1(void) //放加工区1号位
{
	Servo_data(122+1,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);	
	Servo_data(TargetX,246,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,-60+60);delay_ms(900);
	
	//while(NO_flag);NO_flag=1;													//动作精度调试断点
	
	Servo_data(TargetX,TargetY,-80+60);delay_ms(300);	
	Target_claw=0;delay_ms(500);	
//	while(NO_flag);NO_flag=1;	
	Servo_data(TargetX,206,0);delay_ms(300);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}
void Put_one_2(void) //放加工区2号位
{
	Servo_data(95+2,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);
	Servo_data(TargetX,196+5,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,-60+60);delay_ms(900);
	
//	while(NO_flag);NO_flag=1;													//动作精度调试断点
	
	Servo_data(TargetX,TargetY,-82+60);delay_ms(300);	
	Target_claw=0;delay_ms(500);	
	Servo_data(TargetX,190,0);delay_ms(300);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}
void Put_one_3(void) //放加工区3号位
{
	Servo_data(69+2,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);	
	Servo_data(TargetX,246-5,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,-60+60);delay_ms(900);
	
//	while(NO_flag);NO_flag=1;													//动作精度调试断点
	
	Servo_data(TargetX,TargetY,-80+60);delay_ms(300);	
	Target_claw=0;delay_ms(500);	
	Servo_data(TargetX,206,0);delay_ms(300);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}


void Get_two_1(void)//抓加工区1号位
{
	Servo_data(122+1,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);	
	Servo_data(TargetX,210,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,-60+80);delay_ms(300);
	Servo_data(TargetX,230,-80+60);delay_ms(200);	
	Servo_data(TargetX,246,TargetZ);delay_ms(500);	
	Target_claw=-59;delay_ms(500);	
	Servo_data(TargetX,206,57);delay_ms(600);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}
void Get_two_2(void)//抓加工区2号位
{
	Servo_data(95+2,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);
	Servo_data(TargetX,176,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,-60+70);delay_ms(300);
	Servo_data(TargetX,180,-84+50);delay_ms(300);	
	Servo_data(TargetX,196+5,TargetZ);delay_ms(500);	
	Target_claw=-59;delay_ms(500);	
	Servo_data(TargetX,190,57);delay_ms(600);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}
void Get_two_3(void)//抓加工区3号位
{
	Servo_data(69+0.5,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);	
	Servo_data(TargetX,210,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,-60+80);delay_ms(300);
	Servo_data(TargetX,230,-80+60);delay_ms(200);	
	Servo_data(TargetX,246,TargetZ);delay_ms(500);	
	Target_claw=-59;delay_ms(500);	
	Servo_data(TargetX,206,57);delay_ms(600);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}


void Put_two_1(void)	//放成品区顶层1号位
{

	Servo_data(105,TargetY,TargetZ);delay_ms(500);
	Servo_data(120-1,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);	
	Servo_data(TargetX,256-3,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,100);delay_ms(900);delay_ms(500);
	Servo_data(TargetX,TargetY,80);delay_ms(300);														//动作精度调试断点
	
	Target_claw=0;delay_ms(500);	
	Servo_data(TargetX,206,57);delay_ms(300);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}
void Put_two_2(void)	//放成品区顶层2号位
{
	Servo_data(97-1.3,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);
	Servo_data(TargetX,206,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,100);delay_ms(900);delay_ms(500);
	Servo_data(TargetX,TargetY,80);delay_ms(300);	
	
//	while(NO_flag);NO_flag=1;													//动作精度调试断点
	
	Target_claw=0;delay_ms(500);	
	Servo_data(TargetX,180,57);delay_ms(300);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}
void Put_two_3(void)	//放成品区顶层3号位
{
	Servo_data(69+2-0.5,TargetY,TargetZ);delay_ms(500);	
	Servo_data(TargetX,100,TargetZ);delay_ms(100);	
	Servo_data(TargetX,256-5,TargetZ);delay_ms(200);	
	Servo_data(TargetX,TargetY,100);delay_ms(900);delay_ms(500);
	Servo_data(TargetX,TargetY,80);delay_ms(300);		
	
//	while(NO_flag);NO_flag=1;													//动作精度调试断点
	
	Target_claw=0;delay_ms(500);	
	Servo_data(TargetX,206,57);delay_ms(300);	
	Servo_data(TargetX,100,121);delay_ms(300);
	Servo_data(97,35,121);delay_ms(500);	
}

void Put_wan(u8 num)
{
	//放碗里动作组关闭坐标控制，采用独立舵机PWM控制
	switch(num)
	{
		case 1:	Servo_data(141,TargetY,TargetZ);delay_ms(500);break;
		case 2:	Servo_data(48,TargetY,TargetZ);delay_ms(500);break;
		case 3:	Servo_data(-48,TargetY,TargetZ);delay_ms(900);break;
	}
	Set_PWM(PWM1,572,1001,670,PWM5);delay_ms(500);	
	Set_PWM(PWM1,605,1040,615,PWM5);delay_ms(300);
	Set_PWM(PWM1,644,1030,598,PWM5);delay_ms(200);
	Set_PWM(PWM1,PWM2,PWM3,PWM4,500);delay_ms(300);
	Set_PWM(PWM1,518,1049,638,PWM5);delay_ms(200);
	Set_PWM(PWM1,519,940,800,PWM5);delay_ms(400);
	
	Target_claw=0;	Servo_data(97,TargetY,TargetZ);delay_ms(500);	//恢复到机械臂坐标控制
}
void Get_wan(u8 num)
{
	//取碗里动作组关闭坐标控制，采用独立舵机PWM控制
	
	Target_claw=-59;		
	switch(num)
	{
		case 1:	Servo_data(121,TargetY-5,TargetZ);delay_ms(500);Servo_data(141,TargetY-5,TargetZ);delay_ms(500);break;
		case 2:	Servo_data(48,TargetY,TargetZ);delay_ms(500);break;
		case 3:	Servo_data(-48,TargetY,TargetZ);delay_ms(900);break;
	}
	if(num<3)
	{
		//while(NO_flag);NO_flag=1;
		Set_PWM(PWM1,PWM2,PWM3,PWM4,500);delay_ms(200);
		Set_PWM(PWM1,518,1049+0,638,PWM5);delay_ms(200);
		Set_PWM(PWM1,569+0,PWM3,PWM4,PWM5);delay_ms(100);
		Set_PWM(PWM1,657+0,1036+10,580,PWM5);delay_ms(700);
		//while(NO_flag);NO_flag=1;
		Set_PWM(PWM1,PWM2,PWM3,PWM4,275);delay_ms(400);
		Set_PWM(PWM1,589,PWM3,600,PWM5);delay_ms(200);
		Set_PWM(PWM1,589,1015,646,PWM5);delay_ms(200);
		Set_PWM(PWM1,PWM2,911,PWM4,PWM5);delay_ms(200);
		Set_PWM(PWM1,519,940,800,PWM5);delay_ms(300);
	}
	else
	{
		Set_PWM(PWM1,PWM2,PWM3,PWM4,500);delay_ms(200);
		Set_PWM(PWM1,518,1049,638,PWM5);delay_ms(200);
		Set_PWM(PWM1,569+10,PWM3,PWM4,PWM5);delay_ms(100);
		Set_PWM(PWM1,657,1036+10,580,PWM5);delay_ms(700);
		//while(NO_flag);NO_flag=1;
		Set_PWM(PWM1,PWM2,PWM3,PWM4,275);delay_ms(400);
		Set_PWM(PWM1,589,PWM3,600,PWM5);delay_ms(200);
		Set_PWM(PWM1,589,1015,646,PWM5);delay_ms(200);
		Set_PWM(PWM1,PWM2,911,PWM4,PWM5);delay_ms(200);
		Set_PWM(PWM1,519,940,800,PWM5);delay_ms(300);
	}
	
	Servo_data(97,TargetY,TargetZ);delay_ms(900);	//恢复到机械臂坐标控制
}




void UART4_Process(void)	//二维码识别
{		
	int len;
	u8 i;
//	u8 buf[9]={0x7e,0x00,0x08,0x01,0x00,0x02,0x01,0xab,0xcd};
//	uart4_sendString(buf,9);	
	while(1)
	{
		if(UART4_RX_STA&0x4000)			      //串口4的接受判断，显示。
		{
			len=UART4_RX_STA&0x0FFF;//得到此次接收到的数据长度
			for(i=7;i<len;i++)
			{
				target_data[i-7]= UART4_RX_BUF[i];
			}
		
			UART4_RX_STA=0; 
			break;
		}
	}
	
	u5_printf("n0.val=%d",target_data[0]*100+target_data[1]*10+target_data[2]);
	for(i=0;i<3;i++)		//循环发送数据
	{
		while((UART5->SR&0X40)==0);//等待发送结束		  
		UART5->DR=0XFF;
	}	 
	
	u5_printf("n1.val=%d",target_data[4]*100+target_data[5]*10+target_data[6]);
	for(i=0;i<3;i++)		//循环发送数据
	{
		while((UART5->SR&0X40)==0);//等待发送结束		  
		UART5->DR=0XFF;
	}	 
	
} 


//*******以下串口调试所用，与程序运行无关***************//
void UASRT_Process(void)
{		
	int len;
	if(USART_RX_STA&0x8000)			      //串口2的接受判断，显示。
	{
		len=USART_RX_STA&0x0FFF;//得到此次接收到的数据长度
		
		switch(USART_RX_BUF[0])
		{
			case 0xA0:TargetX-=1; 
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw);break;
			
			case 0xA1:TargetX+=1;
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw); break;
			
			case 0xB0:TargetY-=1;
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw); break;
			
			case 0xB1:TargetY+=1; 
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw);break;
			
			case 0xC0:TargetZ-=1;
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw); break;
			
			case 0xC1:TargetZ+=1;
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw); break;
			
			case 0xD0:Target_claw-=1;
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw); break;
			
			case 0xD1:Target_claw+=1; 
				printf("x:%f\ty:%f\tz:%f\tclaw:%f\r\n",TargetX,TargetY,TargetZ,Target_claw);break;
				
			
			
			
			case 0x1a:PWM1++;printf("1:%d\r\n",PWM1);break;
			case 0x0a:PWM1--;printf("1:%d\r\n",PWM1);break;
			case 0x1b:PWM2++;printf("2:%d\r\n",PWM2);break;
			case 0x0b:PWM2--;printf("2:%d\r\n",PWM2);break;
			case 0x1c:PWM3++;printf("3:%d\r\n",PWM3);break;
			case 0x0c:PWM3--;printf("3:%d\r\n",PWM3);break;
			case 0x1d:PWM4++;printf("4:%d\r\n",PWM4);break;
			case 0xdd:PWM4--;printf("4:%d\r\n",PWM4);break;
			case 0x1e:PWM5++;printf("5:%d\r\n",PWM5);break;
			case 0x0e:PWM5--;printf("5:%d\r\n",PWM5);break;
			
			case 0x88:printf("1:%d\t2:%d\t3:%d\t4:%d\t5:%d\r\n",PWM1,PWM2,PWM3,PWM4,PWM5);break;
			
			case 0x80:Servo_no=0;printf("Servo_no=0\r\n");break;	//打开舵机独立控制标志位
			case 0x81:Servo_no=1;printf("Servo_no=1\r\n");break;	//打开机械臂坐标控制标志位
			
			case 0xee:NO_flag=0;break;
			case 0xff:printf("1:%ld\t2:%ld\t3:%ld\t4:%ld\r\n",Dir_1,Dir_2,Dir_3,Dir_4);break;
			
			
			
			
			case 0x2b:Set_BJ_Motor(1,U1_data(USART_RX_BUF,len)*Xs1);break;	//前	+
			case 0x2d:Set_BJ_Motor(2,U1_data(USART_RX_BUF,len)*Xs1);break;	//后  -
			case 0x2a:Set_BJ_Motor(3,U1_data(USART_RX_BUF,len)*Xs2);break;	//左  *
			case 0x2f:Set_BJ_Motor(2,U1_data(USART_RX_BUF,len)*Xs2);break;	//右  /
						
			case 0x65:Set_BJ_Motor(5,U1_data(USART_RX_BUF,len)*rotateXs);break;//顺时针  e
			case 0x66:Set_BJ_Motor(6,U1_data(USART_RX_BUF,len)*rotateXs);break;//逆时针  f
			case 0x67:Set_BJ_Motor(5,U1_data(USART_RX_BUF,len)*rotateXs);break;
			case 0x68:Set_BJ_Motor(6,U1_data(USART_RX_BUF,len)*rotateXs);break;
			
			
		case 0xF1:P2=0;P3=0;break;
		case 0xF2:P2=0;P3=1;break;
		case 0xF3:P2=1;P3=0;break;			
		}
		USART_RX_STA=0;  
	}
}

int U1_data(u8 *num,int x)
{
	int data=0,i;
	for(i=1;i<x;i++)
	{
		data=data*10+(num[i]-0x30);
	}
	printf("len:%d\r\n",x);
	printf("data:%d\r\n",data);
	return data;
}





















