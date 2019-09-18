#include <geometry_msgs/TransformStamped.h>
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <stdlib.h>
#include <iostream>
#include <geometry_msgs/Twist.h>
#define PI 3.14159265358979323846
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
//acquiring and converting quaternion components of roll pitch and yaw
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

double q1[10][10];
double q0[4][1];
double initial(q1[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double r0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 1}};
        double q0[4][1]={ q_x, q_y, q_z, q_w};
                for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
                {
                q1[i][j] = 0;
                };

                for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                q1[i][j]+=r0[i][k]*q0[k][j];
                };
} 

int main(int argc, char **argv)
{
        ros::init(argc,argv,"sumo_teleop_alantransform_tf");
        ros::NodeHandle n;
        vicon_subscriber = n.subscribe("/vicon/SUMO_11_5_2018/SUMO_11_5_2018",10,subscribecall);
        rossumo_publisher = n.advertise<geometry_msgs::Twist>("/rossumo1/cmd_vel_norm", 10);
        ROS_INFO("Init");
        ros::Rate rate(10);
//calculate angular orientation to match rossumo orientation to the x-axis of the vicon system

/*

//Inititalize rossumo to that angle q1
double d = 1;
while(n.ok()&& d !=0)
{
d = q1[3][1]-q0[3][1];
v.linear.x = 0;
v.angular.z = 0.35;
rossumo_publisher.publish(v);
ros::spinOnce();
rate.sleep();
}

//calculate translated position of jumping sumo moving forward to p1

double p1[10][10];

double  forwardx(p1[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double t0[4][4] = { {1, 0, 0, 0.5}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        double p0[4][1] = {v_x, v_y, v_z, 1};
        	for(i=0; i<r1: ++i)
                for(j=0; j<c2; ++j)
        	{
        	p1[i][j] = 0;
        	}

		for(i=0; i<r1; ++i)
	        for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                p1[i][j]+=t0[i][k]*p0[k][j];
                }
}
//move jumping sumo forward to p1

while(n.ok()&& d !=0)
{
d = p1[1][1]-v_x;

v.linear.x = 0.3;
v.angular.z = 0;
rossumo_publisher.publish(v);
ros::spinOnce();
rate.sleep();
}
//calculate angular orientation of jumping sumo  orientated at 90  degrees to the x-axis of the vicon system

double q2[10][10];
double  rotate(q2[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double r0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, PI/2}, {0, 0, 0, 1}};
        double q1[4][1]={ q_x, q_y, q_z, q_w};
        for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
        {
        q2[i][j] = 0;
        }

for(i=0; i<r1; ++i)
        for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                q2[i][j]+=r0[i][k]*q1[k][j];
                }
}
//Inititalize rossumo to that angle q1
while(n.ok()&& d !=0)
{
d = q2[3][1]-q_z;
v.linear.x = 0;
v.angular.z = 0.35;
rossumo_publisher.publish(v);
ros::spinOnce();
rate.sleep();
}
//calculate translated position of jumping sumo moving forward to p2

double p2[10][10];

double forwardy(p2[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double t0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0.5}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        double p1[4][1] = {v_x, v_y, v_z, 1};
        	for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
	        {
	        p2[i][j] = 0;
	        }

		for(i=0; i<r1; ++i)
        	for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                p2[i][j]+=t0[i][k]*p1[k][j];
                }
}


//move jumping sumo forward to p2

while(n.ok()&& d !=0)
{
d= p2[2][1]-v_y;
v.linear.x= 0.3;
v.angular.z =0;
rossumo_publisher.publish(v);
ROS_INFO("move forward2");
ros::spinOnce();
rate.sleep();
}

//calculate angular orientation of jumping sumo  orientated at 180  degrees to the x-axis of the vicon system

double q3[10][10];

double rotatesecond(q3[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double r0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, PI/2}, {0, 0, 0, 1}};
        double q2[4][1]={ q_x, q_y, q_z, q_w};
        	for(i=0; i<r1: ++i)
                for(j=0; j<c2; ++j)
        	{
        	q3[i][j] = 0;
	        }

		for(i=0; i<r1; ++i)
        	for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                q3[i][j]+=r0[i][k]*q2[k][j];
                }
}

//Inititalize rossumo to that angle q3
while(n.ok()&& d !=0)
{
d = q3[3][1]-q_z;
v.linear.x = 0;
v.angular.z = 0.35;
rossumo_publisher.publish(v);
ros::spinOnce();
rate.sleep();
}
//calculate translated position of jumping sumo moving forward to p3

double p3[10][10];

double backwardx(p3[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double t0[4][4] = { {1, 0, 0, -0.5}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        double p2[4][1] = {v_x, v_y, v_z, 1};
	        for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
	        {
	        p3[i][j] = 0;
	        }

		for(i=0; i<r1; ++i)
        	for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                p3[i][j]+=t0[i][k]*p1[k][j];
                }
}
//move jumping sumo forward to p3

while(n.ok()&& d !=0)
{
d= p3[1][1]-v_x;
v.linear.x= 0.3;
v.angular.z =0;
rossumo_publisher.publish(v);
ROS_INFO("move forward3");
ros::spinOnce();
rate.sleep();
}
//calculate angular orientation of jumping sumo  orientated at 270  degrees to the x-axis of the vicon system

double q4[10][10];

double rotatethird(q4[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double r0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, PI/2}, {0, 0, 0, 1}};
        double q3[4][1]={ q_x, q_y, q_z, q_w};
	        for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
	        {
	        q4[i][j] = 0;
	        }

		for(i=0; i<r1; ++i)
	        for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                q4[i][j]+=r0[i][k]*q3[k][j];
                }
}
//Inititalize rossumo to that angle q4
while(n.ok()&& d !=0)
{
d = q3[3][1]-q_z;
v.linear.x = 0;
v.angular.z = 0.35;
rossumo_publisher.publish(v);
ros::spinOnce();
rate.sleep();
}

//calculate translated position of jumping sumo moving forward to p4

double p4[10][10];

double backwardy(p4[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double t0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, -0.5}, {0, 0, 1, 0}, {0, 0, 0, 1}};
        double p3[4][1] = {v_x, v_y, v_z, 1};
	        for(i=0; i<r1: ++i)
                for(j=0; j<c2; ++j)
	        {
	        p4[i][j] = 0;
	        }

		for(i=0; i<r1; ++i)
	        for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                p4[i][j]+=t0[i][k]*p3[k][j];
                }
}


//move jumping sumo forward to p4

while(n.ok()&& d !=0)
{
d= p4[2][1]-v_y;
v.linear.x = 0.3;
v.angular.z = 0;
rossumo_publisher.publish(v);
ROS_INFO("move forward2");
ros::spinOnce();
rate.sleep();
}
//calculate angular orientation to match rossumo orientation to the x-axis of the vicon system

double q5[10][10];

double final(q5[10][10])
{
        double r1=4,c1=4,r2=4,c2=1,i,j,k;
        double r0[4][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 1}};
        double q4[4][1]={ q_x, q_y, q_z, q_w};
        	for(i=0; i<r1; ++i)
                for(j=0; j<c2; ++j)
	        {
	        q5[i][j] = 0;
	        }

		for(i=0; i<r1; ++i)
	        for(j=0; j<c2; ++j)
                for(k=0; k<c1; ++k)
                {
                q1[i][j]+=r0[i][k]*q0[k][j];
                }
}



//Inititalize rossumo to that angle q1
while(n.ok()&& d !=0)
{
d = q5[3][1]-q_w;
v.linear.x = 0;
v.angular.z = 0.35;
rossumo_publisher.publish(v);
ros::spinOnce();
rate.sleep();
}
*/
v.linear.x = 0;
v.angular.z = 0;
rossumo_publisher.publish(v);
ros::spinOnce();
return 0;
}

