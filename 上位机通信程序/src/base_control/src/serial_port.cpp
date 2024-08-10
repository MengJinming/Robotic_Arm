#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include "ros/time.h"
#include "base_control/serial_port.h"
#include "std_msgs/Int32.h"


using namespace std;
unsigned char Send_buffer[8] = {0};

//定义伺服电机的控制变量
unsigned int servo_motor_V_1 = 0;
unsigned int servo_motor_V_2 = 0;
unsigned int servo_motor_V_3 = 0;
unsigned int servo_motor_S_1 = 0;
unsigned int servo_motor_S_2 = 0;
unsigned int servo_motor_S_3 = 0;


//创建一个serial类
serial::Serial sp;

void Key_Board_Callback(const std_msgs::Int32::ConstPtr& key_board_num)
{
    servo_motor_V_1 = key_board_num->data + 1;
    servo_motor_V_2 = key_board_num->data + 2;
    servo_motor_V_3 = key_board_num->data + 3;
    servo_motor_S_1 = key_board_num->data + 10;
    servo_motor_S_2 = key_board_num->data + 20;
    servo_motor_S_3 = key_board_num->data + 30;

    //向缓存器数组中入数据头
    Send_buffer[0] = 0xa5;      
    Send_buffer[1] = 0x5a;  
    //向缓存器中写入数据
    Send_buffer[2] = servo_motor_V_1;
    Send_buffer[3] = servo_motor_V_2;
    Send_buffer[4] = servo_motor_V_3;
    Send_buffer[5] = servo_motor_S_1;
    Send_buffer[6] = servo_motor_S_2;
    Send_buffer[7] = servo_motor_S_3;

    //终端打印接收到的数据
    for(int i=0;i<6;i++)
        cout << (int)Send_buffer[2+i] << endl;

    //发送缓存器中的八个数据
    sp.write(Send_buffer,8);
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "serial_port");
    //创建句柄
    ros::NodeHandle n;

    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyUSB0");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(to);
    
    //创建一个key_board话题的订阅者
    ros::Subscriber key_board_sub = n.subscribe<std_msgs::Int32>("key_board",10,Key_Board_Callback);

    try
    {
        //打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }
    
    // ros::Rate loop_rate(20);
    while(ros::ok())
    {
        //调用回调函数
        ros::spinOnce();
        // loop_rate.sleep();
    }

    //关闭串口
    sp.close();
 
//    return 0;
}
