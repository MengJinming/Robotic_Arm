#include "stm32f10X.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "uart.h"
#include "usart.h"

 

 int main(void)
 {	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init();	             //延时函数初始化	  
	Uart1_Init_PC(115200);     //串口1初始化波特率为115200
	LCD_Init();                //LCD显示屏初始化，方便测试。
	LCD_Clear(BLACK);
	BACK_COLOR = BLACK;        //设置LCD背景颜色
	POINT_COLOR = RED;			   //设置字体为红色 
	LCD_ShowString(90,0,210,24,24,"Test"); 

	delay_ms(500);             //延时等待初始化成功 
	 
  while(1)
	{

	}
		
}



