#include "stm32f10x.h"
#include "sys.h"
#include "uart.h"
#include "delay.h"
#include "lcd.h"



unsigned int Receive_buffer[6] = {0};    //������������   �����ŷ������ת�Ǻ��ٶ�
char Usart_Flag = 0;   
u8 temp = 0;
u8 count = 0,last_data = 0,last_last_data = 0; 

//�����ŷ�������Ʊ���     �ٶȺ�λ��
unsigned int servo_motor_V_1 = 0;
unsigned int servo_motor_V_2 = 0;
unsigned int servo_motor_V_3 = 0;
unsigned int servo_motor_S_1 = 0;
unsigned int servo_motor_S_2 = 0;
unsigned int servo_motor_S_3 = 0;


/****************************
*      ����1��ʼ��
****************************/
void Uart1_Init_PC(u32 bound)
{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}



/****************************
*   ����1�����жϺ���
****************************/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)      //�жϴ����ж��Ƿ��յ�����
    {    		
				temp = USART_ReceiveData(USART1);    	//��ȡ���յ�������;	  
	
				//�ж�����ͷ�ǲ���0xa5��0x5a
				if(last_data == 0x5a && last_last_data == 0xa5) 
				{
						Usart_Flag = 1;
						count = 0;
				}

				if(Usart_Flag == 1)               //�������ݽ�����
				{
						Receive_buffer[count] = temp; 
						count++; 
					
						if(count == 6)     //�������ݵĳ���
						{
								servo_motor_V_1 = Receive_buffer[0];
								servo_motor_V_2 = Receive_buffer[1];
								servo_motor_V_3 = Receive_buffer[2];
								servo_motor_S_1 = Receive_buffer[3];
								servo_motor_S_2 = Receive_buffer[4];
								servo_motor_S_3 = Receive_buffer[5];
							
								LCD_ShowNum(50,40,servo_motor_V_1,5,16);    //��ʾ���ڽ��յ�������
								LCD_ShowNum(50,60,servo_motor_V_2,5,16);    //��ʾ���ڽ��յ�������
								LCD_ShowNum(50,80,servo_motor_V_3,5,16);    //��ʾ���ڽ��յ�������
								LCD_ShowNum(50,100,servo_motor_S_1,5,16);    //��ʾ���ڽ��յ�������
								LCD_ShowNum(50,120,servo_motor_S_2,5,16);    //��ʾ���ڽ��յ�������
								LCD_ShowNum(50,140,servo_motor_S_3,5,16);    //��ʾ���ڽ��յ�������
							
								Usart_Flag = 0; 
						}
						 				
				}
				
				last_last_data = last_data;
				last_data = temp;
		}
}








