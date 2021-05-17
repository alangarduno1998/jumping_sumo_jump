#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/TransformStamped.h>
#include <ros/ros.h>
#include <stdlib.h>
#include <iostream>
#include <vector>


using namespace std;

ros::Subscriber vicon_subscriber;
void subscribecall(const geometry_msgs::TransformStamped::ConstPtr& msg)
{
	double v_x = msg->transform.translation.x;
	double v_y = msg->transform.translation.y;
	double v_z = msg->transform.translation.z;
	double q_x = msg->transform.rotation.x;
	double q_y = msg->transform.rotation.y;
	double q_z = msg->transform.rotation.z;
	double q_w = msg->transform.rotation.w;
	ROS_INFO("v = [%lf %lf %lf] q = [%lf %lf %lf %lf]", v_x, v_y, v_z, q_x, q_y, q_z, q_w);
}

int main(int argc, char **argv)

{
	ros::init(argc,argv,"sumo_teleop_alantransform");
	ros::NodeHandle n;
	vicon_subscriber = n.subscribe("/vicon/SUMO_11_5_2018/SUMO_11_5_2018",10,subscribecall);
	ROS_INFO("Init");
	ros::Rate rate(10);


	while(n.ok())
	{
		ros::spinOnce();
		rate.sleep();
	}

	return 0;

}
