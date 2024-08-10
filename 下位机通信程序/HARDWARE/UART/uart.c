#include "stm32f10x.h"
#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "lcd.h"



unsigned int Receive_buffer[6] = {0};    //接受六个数据   三组伺服电机的转角和速度
char Usart_Flag = 0;   
u8 temp = 0;
u8 count = 0,last_data = 0,last_last_data = 0; 

//定义伺服电机控制变量     速度和位置
unsigned int servo_motor_V_1 = 0;
unsigned int servo_motor_V_2 = 0;
unsigned int servo_motor_V_3 = 0;
unsigned int servo_motor_S_1 = 0;
unsigned int servo_motor_S_2 = 0;
unsigned int servo_motor_S_3 = 0;


/****************************
*      串口1初始化
****************************/
void Uart1_Init_PC(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
}



/****************************
*   串口1接收中断函数
****************************/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)      //判断串口中断是否收到数据
    {    		
				temp = USART_ReceiveData(USART1);    	//读取接收到的数据;	  
	
				//判断数据头是不是0xa5和0x5a
				if(last_data == 0x5a && last_last_data == 0xa5) 
				{
						Usart_Flag = 1;
						count = 0;
				}

				if(Usart_Flag == 1)               //保存数据进数组
				{
						Receive_buffer[count] = temp; 
						count++; 
					
						if(count == 6)     //接收数据的长度
						{
								servo_motor_V_1 = Receive_buffer[0];
								servo_motor_V_2 = Receive_buffer[1];
								servo_motor_V_3 = Receive_buffer[2];
								servo_motor_S_1 = Receive_buffer[3];
								servo_motor_S_2 = Receive_buffer[4];
								servo_motor_S_3 = Receive_buffer[5];
							
								LCD_ShowNum(50,40,servo_motor_V_1,5,16);    //显示串口接收到的数字
								LCD_ShowNum(50,60,servo_motor_V_2,5,16);    //显示串口接收到的数字
								LCD_ShowNum(50,80,servo_motor_V_3,5,16);    //显示串口接收到的数字
								LCD_ShowNum(50,100,servo_motor_S_1,5,16);    //显示串口接收到的数字
								LCD_ShowNum(50,120,servo_motor_S_2,5,16);    //显示串口接收到的数字
								LCD_ShowNum(50,140,servo_motor_S_3,5,16);    //显示串口接收到的数字
							
								Usart_Flag = 0; 
						}
						 				
				}
				
				last_last_data = last_data;
				last_data = temp;
		}
}








