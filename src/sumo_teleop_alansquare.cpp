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

int axis_linear = -1, axis_angular = -1;

int button_90left = -1, button_90right = -1, button_180turn = -1,
button_360turn = -1;

int axis_90turn = -1, axis_180_360turn = -1;

int button_high_jump = -1, button_posture = -1, button_anim = -1, button_deadman = -1, deadman_ok = -1;

double scale_angular = 1.0, offset_linear = 0.0;

double scale_linear = 1.0, offset_angular = 0.0;

std::string posture = "jumper";

bool high_jump_before = false, posture_before = false, anim_before= false;

bool sharp_turn_before = false;

std_msgs::String string_msg;

ros::Publisher posture_pub, cmd_vel_pub, high_jump_pub, sharp_turn_pub, anim_pub;

geometry_msgs::Twist vel;


///////////////////////////////////////////////////////////////

void callback(const sensor_msgs::Joy::ConstPtr& joy){
int naxes = joy->axes.size(), nbuttons = joy->buttons.size();

bool command_sent = false;
deadman_ok = (button_deadman < 0 || (nbuttons> button_deadman && joy->buttons[button_deadman]));

if (!deadman_ok) {

ROS_INFO_THROTTLE(10,"Dead man button %is not pressed, sending a 0 speed order.", button_deadman);



}
//sharp turns at 90 degrees
bool left90now= (button_90left>=0 && button_90left < nbuttons && joy->buttons[button_90left]);

bool right90now = (button_90right >= 0 && button_90right < nbuttons && joy->buttons[button_90right]);


bool axis90now = (axis_90turn >= 0 && axis_90turn < naxes && fabs(joy->axes[axis_90turn]) > 0.9);

// sharp turns at 180°

bool button_180now = (button_180turn >= 0 && button_180turn < nbuttons && joy->buttons[button_180turn]);
bool button_360now = (button_360turn >= 0 && button_360turn < nbuttons && joy->buttons[button_360turn]);
bool axis_180_360now = (axis_180_360turn >= 0 && axis_180_360turn < naxes && fabs(joy->axes[axis_180_360turn]) > 0.9);
bool sharp_turn_now = left90now || right90now || axis90now || button_180now || button_360now || axis_180_360now;
if (sharp_turn_now && !sharp_turn_before) {
std_msgs::Float32 msg;
if (left90now) msg.data = -M_PI_2;
else if (right90now) msg.data = M_PI_2;
else if (axis90now) msg.data = (joy->axes[axis_90turn] < 0 ? M_PI_2 : -M_PI_2);
else if (button_180now) msg.data = -M_PI;
else if (button_360now) msg.data = 2 * M_PI;
else if (axis_180_360now) msg.data = (joy->axes[axis_180_360turn] > 0 ? 2 * M_PI : -M_PI);
ROS_INFO("Starting sharp turm of %i degrees!", (int)(msg.data * 180 / M_PI));
sharp_turn_pub.publish(msg);
command_sent = true;
}
sharp_turn_before = sharp_turn_now;


// jumps
bool high_jump_now = (button_high_jump >=0 && nbuttons > button_high_jump
&& joy->buttons[button_high_jump]);
if (high_jump_now && !high_jump_before) {
ROS_INFO("Starting high jump!");
high_jump_pub.publish(std_msgs::Empty());
command_sent = true;
}
high_jump_before = high_jump_now;


// postures
bool posture_now = (button_posture >=0 && nbuttons > button_posture
&& joy->buttons[button_posture]);
if (posture_now && !posture_before) {
if (posture == "standing") posture = "jumper";
else if (posture == "jumper") posture = "kicker";
else posture = "standing";
string_msg.data = posture;
posture_pub.publish(string_msg);
command_sent = true;
}
posture_before = nbuttons > button_high_jump && joy->buttons[button_posture];


// anims
bool anim_now = (button_anim >=0 && nbuttons > button_anim
&& joy->buttons[button_anim]);
if (anim_now && !anim_before) {
string_msg.data = "tap";
anim_pub.publish(string_msg);
command_sent = true;
}
anim_before = anim_now;


// if no command was sent till here: move robot with directions of axes
if (command_sent)
//ROS_INFO("Computing speed %g, %g!", joy->axes[axis_linear], joy->axes[axis_angular]);
geometry_msgs::Twist vel;
if (axis_linear < naxes)
vel.linear.x = ((joy->axes[axis_linear]-offset_linear) * scale_linear);
if (axis_angular < naxes)
vel.angular.z = ((joy->axes[axis_angular]-offset_angular) * scale_angular);
cmd_vel_pub.publish(vel);
return;
}
// end callback();


////////////////////////////////////////////////////////////////////////////////

int main (int argc, char* argv[]) {
ros::init(argc, argv, "sumo_teleop_alansquare");
ros::NodeHandle nh_public, nh_private("~");


// params
nh_private.param("axis_180_360turn", axis_180_360turn, axis_180_360turn);
nh_private.param("axis_90turn", axis_90turn, axis_90turn);
nh_private.param("axis_angular", axis_angular, axis_angular);
nh_private.param("axis_linear", axis_linear, axis_linear);
nh_private.param("button_180turn", button_180turn, button_180turn);
nh_private.param("button_360turn", button_360turn, button_360turn);
nh_private.param("button_90left", button_90left, button_90left);
nh_private.param("button_90right", button_90right, button_90right);
nh_private.param("button_anim", button_anim, button_anim);
nh_private.param("button_deadman", button_deadman, button_deadman);
nh_private.param("button_high_jump", button_high_jump, button_high_jump);
nh_private.param("button_posture", button_posture, button_posture);
nh_private.param("offset_angular", offset_angular, offset_angular);
nh_private.param("offset_linear", offset_linear, offset_linear);
nh_private.param("scale_angular", scale_angular, scale_angular);
nh_private.param("scale_linear", scale_linear, scale_linear);
// subscribers
ros::Subscriber joy_sub = nh_public.subscribe<sensor_msgs::Joy>("joy", 10, callback);
// publishers
anim_pub = nh_public.advertise<std_msgs::String>("anim", 10);
cmd_vel_pub = nh_public.advertise<geometry_msgs::Twist>("/rossumo1/cmd_vel_norm", 20);
high_jump_pub = nh_public.advertise<std_msgs::Empty>("high_jump", 10);
posture_pub = nh_public.advertise<std_msgs::String>("set_posture", 10);
sharp_turn_pub = nh_public.advertise<std_msgs::Float32>("sharp_turn", 10);
ros::spin();
return 0;
}
