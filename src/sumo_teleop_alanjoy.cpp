#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <nav_msgs/Odometry.h>

using namespace std;

ros::Subscriber joy_subscriber;
ros::Publisher velocity_publisher;
geometry_msgs::Twist v;


void messageCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
v.linear.x = msg->axes[1];
v.angular.z = msg->axes[0];
ROS_INFO("Hey Joy");
}



int main(int argc, char **argv)

{
ros::init(argc,argv, "sumo_teleop_alanjoy");
ros::NodeHandle n;
joy_subscriber = n.subscribe("/joy" ,10,messageCallback);
velocity_publisher = n.advertise<geometry_msgs::Twist>("/rossumo1/cmd_vel_norm", 10);
ROS_INFO("Init");
ros::Rate loop_rate(10);

while(n.ok())
{
velocity_publisher.publish(v);
ros::spinOnce();
loop_rate.sleep();
ROS_INFO("ALive");
}
v.linear.x = 0;
v.angular.z =0;
velocity_publisher.publish(v);
return 0;

}
