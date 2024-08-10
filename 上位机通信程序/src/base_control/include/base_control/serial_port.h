#ifndef _SERIAL_PORT_H
#define _SERIAL_PORT_H



bool Usart_Flag = 0;
unsigned char buffer[16];
unsigned char tempdata;
std:: string recvstr;
uint8_t temp;
unsigned char count,last_data,last_last_data; // 临时变量

int ENCODER = 0;
short tempaccelX, tempaccelY, tempaccelZ; //加速度缓存区
short tempgyroX, tempgyroY, tempgyroZ;    //角速度缓存区
double accelX, accelY, accelZ;            //加速度 
double gyroX, gyroY, gyroZ;               //角速度 


void Sensor_processing(ros::Publisher &imu_pub,ros::Publisher &encoder_pub);




#endif


