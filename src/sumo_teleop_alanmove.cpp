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
v.linear.x = msg->buttons[1];
v.angular.z = msg->buttons[0];
ROS_INFO("Hey Joy");

}



int main(int argc, char **argv)

{
ros::init(argc,argv, "sumo_teleop_alanmove");
ros::NodeHandle n;
joy_subscriber = n.subscribe("/joy" ,10,messageCallback);
velocity_publisher = n.advertise<geometry_msgs::Twist>("/rossumo1/cmd_vel_norm", 10);
ROS_INFO("Init");
ros::Rate rate(10);

while(n.ok()&& v.linear.x != 1)
{
velocity_publisher.publish(v);
ros::spinOnce();
rate.sleep();
ROS_INFO("%lf",v.linear.x);
}

//move forward
ros::Time start = ros::Time::now();
ros::Duration d;

while(n.ok() && d.toSec() <=2)
{
v.linear.x= 0.7;
v.angular.z = 0;
velocity_publisher.publish(v);
ROS_INFO("%lf",v.linear.x);
ROS_INFO("%f",d.toSec());
ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
ROS_INFO("Move forward");
}

start = ros::Time::now();
d = ros::Duration(0);

//rotate right


while(n.ok() && d.toSec() <=2)
{
v.linear.x=0;
v.angular.z =0.34;
velocity_publisher.publish(v);
ROS_INFO("%lf",d.toSec());
ROS_INFO("rotate right1");
ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}

//move forward
start = ros::Time::now();
d = ros::Duration(0);
while(n.ok() && d.toSec() <=2)
{
v.linear.x= 0.7;
v.angular.z =0;
velocity_publisher.publish(v);
ROS_INFO("%lf",d.toSec());
ROS_INFO("move forward2");
ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}

//move right
start = ros::Time::now();
d = ros::Duration(0);
while(n.ok() && d.toSec() <=2)
{
v.linear.x=0;
v.angular.z =0.34;
velocity_publisher.publish(v);
ROS_INFO("%lf",d.toSec());
ROS_INFO("rotate right2");
ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}

//move forward
start = ros::Time::now();
d = ros::Duration(0);
while(n.ok() && d.toSec() <=2)
{
v.linear.x= 0.7;
v.angular.z =0;
velocity_publisher.publish(v);
ROS_INFO("%lf",d.toSec());
ROS_INFO("move forward3");
ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}

//rotate right
start = ros::Time::now();
d = ros::Duration(0);
while(n.ok() && d.toSec() <=2)
{
v.linear.x=0;
v.angular.z =0.34;
velocity_publisher.publish(v);
ROS_INFO("%lf",d.toSec());
ROS_INFO("rotate right3");

ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}

//move forward
start = ros::Time::now();
d = ros::Duration(0);
while(n.ok() && d.toSec() <=2)
{
v.linear.x= 0.7;
v.angular.z =0;
velocity_publisher.publish(v);
ROS_INFO("%lf", d.toSec());
ROS_INFO("move forward4");
ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}

//rotate right
start = ros::Time::now();
d = ros::Duration(0);
while(n.ok() && d.toSec() <=2)
{
v.linear.x=0;
v.angular.z =0.34;
velocity_publisher.publish(v);
ROS_INFO("%lf", d.toSec());
ROS_INFO("rotate right4");

ros::spinOnce();
rate.sleep();
ros::Time current = ros::Time::now();
d = current - start;
}
v.linear.x =0;
v.angular.z=0;
velocity_publisher.publish(v);
ros::spinOnce();
return 0;
}

