#include <geometry_msgs/TransformStamped.h>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <stdlib.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Geometry>
#include <eigen3/Eigen/Core>
#define PI 3.14159265358979323846
using namespace Eigen;
using namespace std;

ros::Subscriber vicon_subscriber;
ros::Publisher rossumo_publisher;
geometry_msgs::TransformStamped msg;
geometry_msgs::Twist v;


double v_x = 0;
double v_y = 0;
double v_z = 0;
double q_x = 0;
double q_y = 0;
double q_z = 0;
double q_w = 0;
double roll=0,pitch=0,yaw=0;
int i,j,k;
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

     Eigen::Matrix4d r0_eig;
     r0_eig << 1,0,0,0,
     	0,1,0,0,
     	0,0,0,0,
     	0,0,0,1;

        Eigen::Affine3d q1_eig;
	//here i had changed q0_eig*r0_eig; to q0_eig*r0_eig.matrix(). 
	//dont know if eigen doesnt define this multiplication implicitly or it interferes with other 
	//cases of the library.

        q1_eig= q0_eig*r0_eig.matrix();
	Eigen::Quaterniond d(1,1,1,1);
	//im thinking the problem below is that eigen doesnt say how to subtract the two eigen datatypes 
	//or maybe because one type is dynamic and the other is constant
	Eigen::Matrix<double,4,1> q0_eig;
	Eigen::Matrix<double,4,1> d1;
	d1=d.toRotationMatrix();
typedef Transform< double, 4, Affine > 	q0_eig,q1_eig,d1;
d1 = q1_eig - q0_eig;
//	float norm;
// 	norm = d1.norm();
//
//	while(n.ok()&& norm > 0)
//	{
//	q0_eig;
// 	//the problem arises here since the operator 
// 	//does not match with the dimensions of q1_eig and q0_eig.
//
//	d = q1_eig-q0_eig;
//	norm;
//		v.linear.x = 0;
//		v.angular.z = 0.35;
//		rossumo_publisher.publish(v);
//		ros::spinOnce();
//		rate.sleep();
//	}
//	//initialize rossumo forward in the x-axis direction
//	Eigen::Matrix4d t0_eig;
//	t0_eig << 1,0,0,0.5,
//		0,1,0,0,
//		0,0,1,0,
//		0,0,0,1;
//	Eigen::Affine3d p1_eig;
//	p1_eig= p0_eig*t0_eig;
//
//
//	Eigen::Vector3d d(1,1,1);
//
//	while(n.ok()&& d !=0)
//
//	{
//		d = p1_eig-p0_eig;
//		v.linear.x = 0.3;
//		v.angular.z = 0;
//		rossumo_publisher.publish(v);
//		ros::spinOnce();
//		rate.sleep();
//	}
//
//	//Inititalize rossumo to the y-axis of the vicon system
//
//	r0_eig <<	1,0,0,0,
//			0,1,0,0,
//			0,0,1,PI/2,
//			0,0,0,1;
//
//	q1_eig= q0_eig*r0_eig;
//
//
//
//	Eigen::Quaterniond d(1,1,1,1);
//
//	while(n.ok()&& d !=0)
//
//	{
//		d = q1_eig-q0_eig;
//		v.linear.x = 0;
//		v.angular.z = 0.35;
//		rossumo_publisher.publish(v);
//		ros::spinOnce();
//		rate.sleep();
//	}
//
//
//
//	//initialize rossumo forward in the y-axis direction
//
//	t0_eig <<1,0,0,0,
//		0,1,0,0.5,
//		0,0,1,0,
//		0,0,0,1;
//
//	p1_eig= p0_eig*t0_eig;
//	Eigen::Vector3d d(1,1,1);
//	while(n.ok()&& d !=0)
//
//	{
//		d= p1_eig-p0_eig;
//		v.linear.x= 0.3;
//		v.angular.z =0;
//		rossumo_publisher.publish(v);
//		ROS_INFO("move forward2");
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
//	Eigen::Quaterniond d(1,1,1,1);
//	while(n.ok()&& d !=0)
//
//	{
//		d = q1_eig-q0_eig;
//		v.linear.x = 0;
//		v.angular.z = 0.35;
//		rossumo_publisher.publish(v);
//		ros::spinOnce();
//		rate.sleep();
//	}
//
//
//
//	//initialize rossumo backward in the x-axis direction
//
//	t0_eig    <<	1,0,0,0,
//			0,1,0,0,
//			0,0,1,0,
//			0,0,0,1;
//
//	p1_eig= p0_eig*t0_eig;
//
//	        Eigen::Vector3d d(1,1,1,1);
//
//	while(n.ok()&& d !=0)
//
//	{
//		d= p1_eig-p0_eig;
//		v.linear.x= 0.3;
//		v.angular.z =0;
//		rossumo_publisher.publish(v);
//		ROS_INFO("move forward3");
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
//	Eigen::Quaterniond d(1,1,1,1);
//
//	while(n.ok()&& d !=0)
//
//	{
//		d = q1_eig-q0_eig;
//		v.linear.x = 0;
//		v.angular.z = 0.35;
//		rossumo_publisher.publish(v);
//		ros::spinOnce();
//		rate.sleep();
//	}
//
//	//initialize rossumo backward in the y-axis direction
//
//	t0_eig    <<  	1,0,0,-0.5,
//			0,1,0,0,
//			0,0,1,0,
//			0,0,0,1;
//
//		p1_eig = p0_eig*t0_eig;
//	        Eigen::Vector3d d(1,1,1);
//
//	while(n.ok()&& d !=0)
//
//	{
//		d= p1_eig-p0_eig;
//		v.linear.x = 0.3;
//		v.angular.z = 0;
//		rossumo_publisher.publish(v);
//		ROS_INFO("move forward2");
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
//	Eigen::Quaterniond d(1,1,1,1);
//	while(n.ok()&& d !=0)
//	{
//		d = q1_eig-q0_eig;
//		v.linear.x = 0;
//		v.angular.z = 0.35;
//		rossumo_publisher.publish(v);
//		ros::spinOnce();
//		rate.sleep();
//	}
//
//
//
//	v.linear.x = 0;
//	v.angular.z = 0;
//	rossumo_publisher.publish(v);
//	ros::spinOnce();
//	return 0;
}

