//roscore
//rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
//rosrun second_assignment controller_node
//rosrun second_assignment ui_node
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "second_assignment/speed_regulator.h"

//list of each rosmsg and srv
// ---------------------------------------------------------
// rosmsg show sensor_msgs/LaserScan
// std_msgs/Header header
//   uint32 seq
//   time stamp
//   string frame_id
// float32 angle_min
// float32 angle_max
// float32 angle_increment
// float32 time_increment
// float32 scan_time
// float32 range_min
// float32 range_max
// float32[] ranges
// float32[] intensities
//--------------------------------------------------------------
// rosmsg show geometry_msgs/Twist
// geometry_msgs/Vector3 linear
//   float64 x
//   float64 y
//   float64 z
// geometry_msgs/Vector3 angular
//   float64 x
//   float64 y
//   float64 z

//rossrv second_assignment/speed_regulator
// int32 state_param
// ---
// float32 speed_param


ros::Publisher pub; //init Publisher
float speed_reg = 1.00; //init speed_reg

// This loop will exceute when recive the msg from laser scanner
void robotCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	ROS_INFO("-------------------"); // size == 24

	// ROS_INFO("Size: %ld Laser Scanner subscriber@[%f, %f, %f]",
	// msg->ranges.size(), msg->range_max, msg->angle_max, msg->scan_time); // just for debugging
	for(float range = 1.6; range > 0.5; range = range-0.1){
		// float range = 1.5;
		int j = ((msg->ranges.size()-1)/2); // find the centre == front of robot
		float frontDist = 0;
		float minDist = 100; //init the min distance in scope
		// check the fron of robot
		for (int i = j-30; i < j +61; i++){
			frontDist = frontDist + msg->ranges[i];
			if (msg->ranges[i] < minDist){
				minDist = msg->ranges[i];//assign the value of min if it less than previous one
			}
		}
		frontDist = frontDist/ 60;
		if (frontDist > range && minDist > range-0.2){
			ROS_INFO("Forward %f  ..M=%f", frontDist, minDist);
			geometry_msgs::Twist msg;
			msg.linear.x = (0.8 * (minDist))+1 * speed_reg;
			msg.linear.y = 0;
			msg.linear.z = 0;
			msg.angular.x = 0;
			msg.angular.y = 0;
			msg.angular.z = 0;
			pub.publish(msg);
			range = 0;
			break;
		}
		int ji = 20; // start from instant 20 of laser scanner
		int size = ((msg->ranges.size()-1)/2)-20; //-22 because we ignore the middle
		for (int i = 0 ; i < size; i++) {
			ji = ji+3;
			float avgCW = ( msg->ranges[size - ji - 1]
										+ msg->ranges[size - ji] + msg->ranges[size - ji +1] )/3;
			float avgCCW = ( msg->ranges[size + ji - 1]
										+ msg->ranges[size + ji] + msg->ranges[size + ji +1] )/3;
			//ROS_INFO("for at %d range %f [%f --- %f]",i, range, avg_R, avg_L);
			if(avgCW > range){
				geometry_msgs::Twist msg;
				ROS_INFO("Turn CW-%d . %f ... %f", i ,avgCCW, avgCW);
				msg.linear.x = 0;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = (-8.00*i)-9.00 * speed_reg;
				pub.publish(msg);
				range = 0;
				break;
			// 	ROS_INFO("Publisher at %d",i);
			}
			else if (avgCCW > range){
				geometry_msgs::Twist msg;
				ROS_INFO("Turn CCW-%d %f ... %f", i,avgCCW, avgCW);
				msg.linear.x = 0;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = (8.00*i)+9.00 * speed_reg;
				pub.publish(msg);
				range = 0;
				break;
			}
		}

	}
}

// this function will exceute when UI call the service
bool speed_adj(second_assignment::speed_regulator::Request &req, second_assignment::speed_regulator::Response &res){
	if (req.state_param == 1) {
		speed_reg = speed_reg + 0.2;
	}
	else if (req.state_param == 2) {
		if (speed_reg > 0.4)
		speed_reg = speed_reg - 0.2;
	}
	res.speed_param = speed_reg;
	return true;
}

int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system
	ros::init(argc, argv, "as2Controller");
	ros::NodeHandle nh;
	ros::Subscriber sub = nh.subscribe("/base_scan", 1,robotCallback);
	pub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);
	ros::ServiceServer service = nh.advertiseService("/speed_regulator", speed_adj);
	ROS_INFO("GO");
	ros::spin();
	return 0;
}
