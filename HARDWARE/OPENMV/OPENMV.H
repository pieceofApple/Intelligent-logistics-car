#ifndef __OPENMV_H
#define __OPENMV_H	 
#include "sys.h"

#define P2 PCout(4)	
#define P3 PCout(5)	

#define Move_xs 13.89	//校准系数


extern int openmv_data[20];
extern float openmv_mean_1,openmv_mean_2;

void Calibration_line_R(float bias_x);	//直线校准车体 
void Calibration_circle_FB(int bias_x);	//圆校准车体 前后
void Calibration_circle_LR(int bias_x);	//圆校准车体 左右
	

void Openmv_U2_read(u8 num);	//接收openmv信息
void read(int *data,u8 *num,int len);//数据拆分
void IO_init(void);
void bubblesort(int *arr,int len);	//排序
float mean(int *arr,u8 len);//掐头去尾平均数

#endif


