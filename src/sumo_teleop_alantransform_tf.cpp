#include <geometry_msgs/TransformStamped.h>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <stdlib.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Eigenvalues>
#include <eigen3/Eigen/Geometry>
#define PI 3.14159265358979323846
using namespace std;



ros::Subscriber vicon_subscriber;

ros::Publisher rossumo_publisher;

geometry_msgs::TransformStamped msg;

geometry_msgs::TransformStamped eig;

geometry_msgs::Twist v;


double v_x = 0;

double v_y = 0;

double v_z = 0;

double q_x = 0;

double q_y = 0;

double q_z = 0;

double q_w = 0;

double roll=0,pitch=0,yaw=0;

int r1=4,c1=4,r2=4,c2=1,i,j,k;

Eigen::AffineXd q1_eig;
Eigen::AffineXd q0_eig;
Eigen::MatrixXd r0_eig;
Eigen::AffineXd p1_eig;
Eigen::AffineXd p0_eig;
Eigen::MatrixXd t0_eig;
Eigen::AffineXd d;



//acquiring and converting quaternion components of roll pitch and yaw
Eigen::AffineXd transformTFToEigen(const geometry_msgs::TransformStamped::ConstPtr& msg)
{
Eigen::AffineXd q0_eig;
Eigen::AffineXd p0_eig;
	for (int i=0;i<1;i++)
		{
		q0_eig.matrix()(i,1)=msg->transform.getBasis()[i];
		p0_eig.matrix()(i,1)=msg->transform.getOrigin()[i];
	for(int j=0;j<3;j++)
				{
				q0_eig.matrix()(i,j)=msg->transform.getBasis()[i][j];
		                p0_eig.matrix()(i,j)=msg->transform.getOrigin()[i][j];

				}
		}
return p0_eig;
return q0_eig;
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



Eigen::MatrixXd r0_eig;
r0_eig.row(0) ={1,0,0,0};
r0_eig.row(1) = {0,1,0,0};
r0_eig.row(2) = {0,0,0,0};
r0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd q1_eig;
q1_eig= q0_eig*r0_eig;


Eigen::AffineXd::Ones(4,1) d;

while(n.ok()&& d !=0)

{

d = q1_eig-q0_eig;

v.linear.x = 0;

v.angular.z = 0.35;

rossumo_publisher.publish(v);

ros::spinOnce();

rate.sleep();

}



//initialize rossumo forward in the x-axis direction
Eigen::MatrixXd t0_eig;
t0_eig.row(0) ={1,0,0,0.5};
t0_eig.row(1) = {0,1,0,0};
t0_eig.row(2) = {0,0,1,0};
t0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd p1_eig;
p1_eig= p0_eig*t0_eig;


Eigen::AffineXd::Ones(4,1) d;


while(n.ok()&& d !=0)

{

d = p1_eig-p0_eig;



v.linear.x = 0.3;

v.angular.z = 0;

rossumo_publisher.publish(v);

ros::spinOnce();

rate.sleep();

}

//Inititalize rossumo to the y-axis of the vicon system

Eigen::MatrixXd r0_eig;
r0_eig.row(0) ={1,0,0,0};
r0_eig.row(1) = {0,1,0,0};
r0_eig.row(2) = {0,0,1,PI/2};
r0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd q1_eig;
q1_eig= q0_eig*r0_eig;



Eigen::AffineXd::Ones(4,1) d;


while(n.ok()&& d !=0)

{

d = q1_eig-q0_eig;

v.linear.x = 0;

v.angular.z = 0.35;

rossumo_publisher.publish(v);

ros::spinOnce();

rate.sleep();

}



//initialize rossumo forward in the y-axis direction

Eigen::MatrixXd t0_eig;
t0_eig.row(0) ={1,0,0,0};
t0_eig.row(1) = {0,1,0,0.5};
t0_eig.row(2) = {0,0,1,0};
t0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd p1_eig;
p1_eig= p0_eig*t0_eig;


Eigen::AffineXd::Ones(4,1) d;

while(n.ok()&& d !=0)

{

d= p1_eig-p0_eig;

v.linear.x= 0.3;

v.angular.z =0;

rossumo_publisher.publish(v);

ROS_INFO("move forward2");

ros::spinOnce();

rate.sleep();

}





//Inititalize rossumo to the negative x-axis of the vicon system

Eigen::MatrixXd r0_eig;
r0_eig.row(0) ={1,0,0,0};
r0_eig.row(1) = {0,1,0,0};
r0_eig.row(2) = {0,0,1,PI/2};
r0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd q1_eig;
q1_eig= q0_eig*r0_eig;



Eigen::AffineXd::Ones(4,1) d;


while(n.ok()&& d !=0)

{

d = q1_eig-q0_eig;

v.linear.x = 0;

v.angular.z = 0.35;

rossumo_publisher.publish(v);

ros::spinOnce();

rate.sleep();

}



//initialize rossumo backward in the x-axis direction

Eigen::MatrixXd t0_eig;
t0_eig.row(0) ={1,0,0,-0.5};
t0_eig.row(1) = {0,1,0,0};
t0_eig.row(2) = {0,0,1,0};
t0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd p1_eig;
p1_eig= p0_eig*t0_eig;
Eigen::AffineXd::Ones(4,1) d;

translation(double p0, double p1, double t0);



while(n.ok()&& d !=0)

{

d= p1_eig-p0_eig;

v.linear.x= 0.3;

v.angular.z =0;

rossumo_publisher.publish(v);

ROS_INFO("move forward3");

ros::spinOnce();

rate.sleep();

}



//Inititalize rossumo to the negative y-axis of the vicon system

Eigen::MatrixXd r0_eig;
r0_eig.row(0) ={1,0,0,0};
r0_eig.row(1) = {0,1,0,0};
r0_eig.row(2) = {0,0,1,PI/2};
r0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd q1_eig;
q1_eig= q0_eig*r0_eig;



Eigen::AffineXd::Ones(4,1) d;


while(n.ok()&& d !=0)

{

d = q1_eig-q0_eig;

v.linear.x = 0;

v.angular.z = 0.35;

rossumo_publisher.publish(v);

ros::spinOnce();

rate.sleep();

}





//initialize rossumo backward in the y-axis direction

Eigen::MatrixXd t0_eig;
t0_eig.row(0) ={1,0,0,-0.5};
t0_eig.row(1) = {0,1,0,0};
t0_eig.row(2) = {0,0,1,0};
t0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd p1_eig;
p1_eig= p0_eig*t0_eig;
Eigen::AffineXd::Ones(4,1) d;

translation(double p0, double p1, double t0);



while(n.ok()&& d !=0)

{

d= p1_eig-p0_eig;

v.linear.x = 0.3;

v.angular.z = 0;

rossumo_publisher.publish(v);

ROS_INFO("move forward2");

ros::spinOnce();

rate.sleep();

}



//Inititalize rossumo to the x-axis of the vicon system

Eigen::MatrixXd r0_eig;
r0_eig.row(0) ={1,0,0,0};
r0_eig.row(1) = {0,1,0,0};
r0_eig.row(2) = {0,0,1,PI/2};
r0_eig.row(3) = {0,0,0,1};
Eigen::AffineXd q1_eig;
q1_eig= q0_eig*r0_eig;
Eigen::AffineXd::Ones(4,1) d;


while(n.ok()&& d !=0)

{

d = q1_eig-q0_eig

v.linear.x = 0;

v.angular.z = 0.35;

rossumo_publisher.publish(v);

ros::spinOnce();

rate.sleep();

}



v.linear.x = 0;

v.angular.z = 0;

rossumo_publisher.publish(v);

ros::spinOnce();

return 0;

}

