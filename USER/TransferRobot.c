#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "FreeRTOS.h"  
#include "task.h"  
#include "queue.h"  
#include "semphr.h"  

void vTaskComInit(void *pvParameters);  
void vTaskMotorControl(void *pvParameters);  
void vTaskServoControl(void *pvParameters);  
void vTaskMainLoop(void *pvParameters);  

int main(void)  
{  
    // 系统和设备初始化  
    Stm32_Clock_Init(9);  
    uart_init(72, 9600);  
    delay_init(72);  
    usart2_init(36, 9600);  
    uart4_init(36, 9600);  
    uart5_init(36, 9600);  
    LED_Init();  
    TIM6_Int_Init(719, 19);  
    BJ_Motor_Init();  
    TIM3_PWM_Init(9999, 143);  
    TIM4_PWM_Init(9999, 143);  
    IO_init();  

    // 创建任务  
    xTaskCreate(vTaskComInit, "ComInit", 128, NULL, 1, NULL);  
    xTaskCreate(vTaskMotorControl, "MotorControl", 1024, NULL, 1, NULL);  
    xTaskCreate(vTaskServoControl, "ServoControl", 512, NULL, 1, NULL);  
    xTaskCreate(vTaskMainLoop, "MainLoop", 128, NULL, 2, NULL);  

    // 启动调度器  
    vTaskStartScheduler();  

    // 如果没有足够的堆空间，将不会到达这里  
    for (;;);  
}  

void vTaskComInit(void *pvParameters)  
{  
    u8 buf[9] = {0x7e, 0x00, 0x08, 0x01, 0x00, 0x02, 0x01, 0xab, 0xcd};  
    
    P2 = 1; P3 = 1;  
    vTaskDelay(500 / portTICK_PERIOD_MS);  
    
    while (NO_flag); // 等待NO_flag被清除  
    NO_flag = 1;  

    uart4_sendString(buf, 9);  
    
    vTaskDelete(NULL); // 完成后删除此任务  
}  

void vTaskMotorControl(void *pvParameters)  
{  
    // 控制电机的代码  
    Task_1(); // 第一轮  
    Task_2(); // 第二轮  
    
    vTaskDelete(NULL); // 完成后删除此任务  
}  

void vTaskServoControl(void *pvParameters)  
{  
    Target_claw = 0;  
    Servo_data(97, TargetY, TargetZ);  
    vTaskDelay(500 / portTICK_PERIOD_MS);  
    
    vTaskDelete(NULL); // 完成后删除此任务  
}  

void vTaskMainLoop(void *pvParameters)  
{  
    while (1)  
    {  

        vTaskDelay(1000 / portTICK_PERIOD_MS); // 示例延迟  
    }  
}
