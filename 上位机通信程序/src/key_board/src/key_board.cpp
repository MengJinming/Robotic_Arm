#include "ros/ros.h"
#include <termio.h>
#include <stdio.h>
#include "std_msgs/Int32.h"


using namespace std;

int scanKeyboard()
{
 
	int in;
 
	struct termios new_settings;
	struct termios stored_settings;
    //设置终端参数
	tcgetattr(0,&stored_settings);
	new_settings = stored_settings;
	new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VTIME] = 0;
	tcgetattr(0,&stored_settings);
	new_settings.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_settings);
	in = getchar();
	tcsetattr(0,TCSANOW,&stored_settings);
 
	return in;
 
}
 
//主函数
int main(int argc,char **argv)
{
    ros::init(argc,argv,"key_board_node");
    ros::NodeHandle nh;
    //定义发布者
    ros::Publisher key_board_pub = nh.advertise<std_msgs::Int32>("key_board",10);
    //设置消息类型
    std_msgs::Int32 key_board_msgs;

	while(ros::ok())
    {
		// printf("%d\n",scanKeyboard());
        
        key_board_msgs.data = scanKeyboard();
		key_board_pub.publish(key_board_msgs);
        cout << key_board_msgs.data << endl;
	}

}