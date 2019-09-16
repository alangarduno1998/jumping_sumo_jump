#include <std_msgs/Float.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <stdlib.h>
#include <time.h>
#include <iostream.h>
#include <vector>
#include <math.h>
geometry_msgs::Pose2D current_pose;
ros::Publisher pub_pose2d;

void odomCallback(const nav_msgs::OdometryConstPtr&amp; msg)
{
//linear positions
current_pose.x = msg->pose.pose.position.x;
current_pose.y = msg->pose.pose.position.y;

// quaternion to RPY Conversion
tf::Quaternion q(
msg->pose.pose.orientation.x,
msg->pose.pose.orientation.y,
msg->pose.pose.orientation.z,
msg->pose.pose.orientation.w);
tf::Matrix3x3 m(q)
double roll, pitch, yaw;
m.getRPY(roll, pitch, yaw);

// angular position
pub_pose2d.publish(current_pose);
}

int main(int argc, char, **argv)
const double PI = 3.14159265358979323846;
ROS_INFO("start");

ros::init(argc, argv, "move_pub");
ros::NodeHandle n;
ros::Subscriber sub_odometry = n.subscribe("odom",1, odomCallback);
ros::Publisher movement_pub = n.advertise("cmd_vel",1);
pub_pose2d = n.advertise("cmd_vel",1);
ros::Rate rate(10);

//move forward
ROS_INFO("move forward");
ros::Time start = ros::Time::now();
while(ros::ok() &amp;&amp; current_pose.x &lt; 1.5)
{
geometry_msgs::Twist move;
//velocity controls
move.linear.x= 0.2;
move.angular.z= ;
movement_ppub.publish(move);

ros::spinOnce();
rate.sleep();
}

//move forward again
ROS_INFO("move forward");
ros::Time start2 = ros::Time::now();
while(ros::ok() &amp;&amp; current_pose.y &gt; -1.5)
{
geometry_msgs::Twist move;
//velocity controls
move.linear.x= 0.2;
move.angular.z= ;
movement_ppub.publish(move);

ros::spinOnce();
rate.sleep();
}
//just stop
while(ros::ok()) {
geometry_msgs::Twist move;
move.linear.x = ;
move.angular.z = ;
movement_pub.publish(move);
ros::spinOnce();
rate.sleep();
}

return ;
}





 int main(int argc, char** argv){
 ros::init(argc, argv, "odometry_publisher");
 ros::NodeHandle n;
 ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
 tf::T
