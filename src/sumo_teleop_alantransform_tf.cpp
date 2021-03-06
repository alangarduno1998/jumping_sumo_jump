//Coded by Alan Garduno
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Empty.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/Float.h>
#include <geometry_msgs/Point.h>

#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_datatypes.h>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Core>

#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <math.h>

#define PI 3.14159265358979323846
using namespace Eigen;
using namespace std;

double heading;						//Drone's Heading/Orientation
int btn_emergency;					//emergency button to stop operations


ros::Subscriber vicon_subscriber;			//declaring node is subscribed to the vicon motion capture system
ros::Publisher rossumo_publisher;			//declaring node is publishing messages to the rossumo
geometry_msgs::TransformStamped msg;			//Receiver message of the vicon
geometry_msgs::Twist cmd_v;				//Command message of the rossumo

double ce_hdg, ce_pos_X, ce_pos_Y, st_alt,st_hdg;
std_msgs::Float64 st_pos_X,st_pos_Y,st_alt,st_hdg;
std_msgs::FLoat64 stp_pos_X,stp_pos_Y,stp_alt,stp_hdg;

struct v_object

double v_x = 0;
double v_y = 0;
double v_z = 0;
double q_x = 0;
double q_y = 0;
double q_z = 0;
double q_w = 0;
double roll=0,pitch=0,yaw=0;
int i,j,k;
Eigen::Affine3d eig;
Eigen::Quaterniond norm;
Eigen::Affine3d q1_eig;
Eigen::Affine3d q0_eig;
Eigen::Matrix4d r0_eig;
Eigen::Affine3d p1_eig;
Eigen::Affine3d p0_eig;
Eigen::Matrix4d t0_eig;



//acquiring and converting quaternion components of roll pitch and yaw
void transformTFToEigen(Eigen::Affine3d &eig, geometry_msgs::TransformStamped::ConstPtr& msg)
{
	double q_x,q_y,q_z,q_w;
	q_x = msg->transform.rotation.x;
	q_y = msg->transform.rotation.y;
	q_z = msg->transform.rotation.z;
	q_w = msg->transform.rotation.w;

	double x,y,z;
	x = msg->transform.translation.x;
	y = msg->transform.translation.y;
	z = msg->transform.translation.z;

	Eigen::Quaterniond q(q_x,q_y,q_z,q_w);
	Eigen::Vector3d pos(x,y,z);

	Eigen::Matrix3d Re(q);
	eig.translation() = pos;
	eig.linear() = Re;
std::cout<<eig.translation()<<std::endl;
std::cout<<eig.linear()<<std::endl;
	return;
}

void do_math(double q_x, double q_y, double q_z, double q_w)
{
	tf::Quaternion q(q_x,q_y,q_z,q_w);
	tf::Matrix3x3 m(q);
	m.getRPY(roll,pitch,yaw);
	roll = roll*(180/PI);
	pitch = pitch*(180/PI);
	yaw = yaw*(180/PI);
}

//listening to the transform stamped msgs from the vicon system and printing them

void subscribecall(const geometry_msgs::TransformStamped::ConstPtr& msg)
{
	v_x = msg->transform.translation.x;
	v_y = msg->transform.translation.y;
	v_z = msg->transform.translation.z;
	q_x = msg->transform.rotation.x;
	q_y = msg->transform.rotation.y;
	q_z = msg->transform.rotation.z;
	q_w = msg->transform.rotation.w;
	do_math(q_x,q_y,q_z,q_w);
	ROS_INFO("v=[%lf %lf %lf] q=[%lf %lf %lf %lf] RPY= [%lf %lf %lf]", v_x, v_y, v_z, q_x, q_y, q_z, q_w, roll, pitch, yaw);
}

//calculate angular orientation to match rossumo orientation to the x-axis of the vicon system
//calculate translated position of jumping sumo moving forward to p1

int main(int argc, char **argv)
{
	ros::init(argc,argv,"sumo_teleop_alantransform_tf");
	ros::NodeHandle n;
	vicon_subscriber = n.subscribe("/vicon/SUMO_11_5_2018/SUMO_11_5_2018",10,subscribecall);
	rossumo_publisher = n.advertise<geometry_msgs::Twist>("/rossumo1/cmd_vel_norm", 10);
	ROS_INFO("Init");
	ros::Rate rate(10);

	//Inititalize rossumo to to match rossumo orientation to the x-axis of the vicon system
//define rotation matrix as a matrix of 4 dimensions
	Eigen::Matrix4d r0_eig;
	r0_eig << 1,0,0,0,
	       0,1,0,0,
	       0,0,0,0,
	       0,0,0,1;
	Eigen::Matrix4d q0_eig;
	q0_eig= eig.linear();
//d1 is affine data type 
	Eigen::Affine3d d1;

//transform affined3d datatype d1 to a matrix
q1_eig.matrix() = r0_eig*q0_eig.matrix();
	d1.matrix() = q1_eig.matrix() - q0_eig.matrix();
//transform q1_eig and q0_eig into a matrix and then subtract the vectors 
	float norm;
//store the normalization of the d1 matrix into a floating number to later compare in the while loop
	norm = d1.matrix().norm();

	while(n.ok()&& norm > 0)
	{
		//the problem arises here since the operator 
		//does not match with the dimensions of q1_eig and q0_eig.

		d1.matrix() = q1_eig.matrix()-q0_eig.matrix();
		norm = d1.matrix().norm();
		cmd_v.linear.x = 0;
		cmd_v.angular.z = 0.35;
		rossumo_publisher.publish(cmd_v);
		ros::spinOnce();
		rate.sleep();
	}

	while(n.ok()&& norm !=0)

	{
		d1.matrix() = q1_eig.matrix() - q0_eig.matrix();
		norm = d1.matrix().norm();
		cmd_v.linear.x = 0.3;
		cmd_v.angular.z = 0;
		rossumo_publisher.publish(cmd_v);
		ros::spinOnce();
		rate.sleep();
	}


	while(n.ok()&& norm !=0)
	{
		d1.matrix() = q1_eig.matrix() - q0_eig.matrix();
		norm = d1.matrix().norm();
		cmd_v.linear.x = 0;
		cmd_v.angular.z = 0.35;
		rossumo_publisher.publish(cmd_v);
		ros::spinOnce();
		rate.sleep();
	}


	//
	//	//initialize rossumo forward1 in the y-axis direction
	//
	//	t0_eig <<1,0,0,0,
	//		0,1,0,0.5,
	//		0,0,1,0,
	//		0,0,0,1;
	//
	//	q1_eig= q0_eig*t0_eig;
	//	Eigen::Vector3d1 d(1,1,1);
	//	while(n.ok()&& d1 !=0)
	//
	//	{
	//		d1= q1_eig-q0_eig;
	//		cmd_v.linear.x= 0.3;
	//		cmd_v.angular.z =0;
	//		rossumo_publisher.publish(cmd_v);
	//		ROS_INFO("move forward12");
	//		ros::spinOnce();
	//		rate.sleep();
	//	}
	//	//Inititalize rossumo to the negative x-axis of the vicon system
	//
	//	r0_eig  <<	1,0,0,0,
	//			0,1,0,0,
	//			0,0,1,PI/2,
	//			0,0,0,1;
	//
	//	q1_eig= q0_eig*r0_eig;
	//	Eigen::Quaterniond1 d(1,1,1,1);
	//	while(n.ok()&& d1 !=0)
	//
	//	{
	//		d1.matrix() = q1_eig.matrix() - q0_eig.matrix();
	//		cmd_v.linear.x = 0;
	//		cmd_v.angular.z = 0.35;
	//		rossumo_publisher.publish(cmd_v);
	//		ros::spinOnce();
	//		rate.sleep();
	//	}
	//
	//
	//
	//	//initialize rossumo backward1 in the x-axis direction
	//
	//	t0_eig    <<	1,0,0,0,
	//			0,1,0,0,
	//			0,0,1,0,
	//			0,0,0,1;
	//
	//	q1_eig= q0_eig*t0_eig;
	//
	//	        Eigen::Vector3d1 d(1,1,1,1);
	//
	//	while(n.ok()&& d1 !=0)
	//
	//	{
	//		d1= q1_eig-q0_eig;
	//		cmd_v.linear.x= 0.3;
	//		cmd_v.angular.z =0;
	//		rossumo_publisher.publish(cmd_v);
	//		ROS_INFO("move forward13");
	//		ros::spinOnce();
	//		rate.sleep();
	//	}
	//
	//	//Inititalize rossumo to the negative y-axis of the vicon system
	//
	//	r0_eig  <<  	1,0,0,0,
	//			0,1,0,0,
	//			0,0,1,PI/2,
	//			0,0,0,1;
	//
	//	q1_eig= q0_eig*r0_eig;
	//
	//	Eigen::Quaterniond1 d(1,1,1,1);
	//
	//	while(n.ok()&& d1 !=0)
	//
	//	{
	//		d1.matrix() = q1_eig.matrix() - q0_eig.matrix();
	//		cmd_v.linear.x = 0;
	//		cmd_v.angular.z = 0.35;
	//		rossumo_publisher.publish(cmd_v);
	//		ros::spinOnce();
	//		rate.sleep();
	//	}
	//
	//	//initialize rossumo backward1 in the y-axis direction
	//
	//	t0_eig    <<  	1,0,0,-0.5,
	//			0,1,0,0,
	//			0,0,1,0,
	//			0,0,0,1;
	//
	//		q1_eig = q0_eig*t0_eig;
	//	        Eigen::Vector3d1 d(1,1,1);
	//
	//	while(n.ok()&& d1 !=0)
	//
	//	{
	//		d1= q1_eig-q0_eig;
	//		cmd_v.linear.x = 0.3;
	//		cmd_v.angular.z = 0;
	//		rossumo_publisher.publish(cmd_v);
	//		ROS_INFO("move forward12");
	//		ros::spinOnce();
	//		rate.sleep();
	//
	//	}
	//
	//
	//
	//	//Inititalize rossumo to the x-axis of the vicon system
	//
	//	r0_eig    <<  	1,0,0,0,
	//			0,1,0,0,
	//			0,0,1,PI/2,
	//			0,0,0,1;
	//
	//	q1_eig = q0_eig*r0_eig;
	//
	//	Eigen::Quaterniond1 d(1,1,1,1);
	//	while(n.ok()&& d1 !=0)
	//	{
	//		d1.matrix() = q1_eig.matrix() - q0_eig.matrix();
	//		cmd_v.linear.x = 0;
	//		cmd_v.angular.z = 0.35;
	//		rossumo_publisher.publish(cmd_v);
	//		ros::spinOnce();
	//		rate.sleep();
	//	}
	//
	//
	//
	//	cmd_v.linear.x = 0;
	//	cmd_v.angular.z = 0;
	//	rossumo_publisher.publish(cmd_v);
	//	ros::spinOnce();
	//	return 0;
}

