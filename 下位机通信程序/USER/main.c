#include "stm32f10X.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "uart.h"
#include "usart.h"

 

 int main(void)
 {	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	             //��ʱ������ʼ��	  
	Uart1_Init_PC(115200);     //����1��ʼ��������Ϊ115200
	LCD_Init();                //LCD��ʾ����ʼ����������ԡ�
	LCD_Clear(BLACK);
	BACK_COLOR = BLACK;        //����LCD������ɫ
	POINT_COLOR = RED;			   //��������Ϊ��ɫ 
	LCD_ShowString(90,0,210,24,24,"Test"); 

	delay_ms(500);             //��ʱ�ȴ���ʼ���ɹ� 
	 
  while(1)
	{

	}
		
}



