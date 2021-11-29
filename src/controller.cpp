//roscore
//rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
//rosrun second_assignment controller_node
//
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
int state= 0;

// /base_pose_ground_truth
// /base_scan
// /clock
// /cmd_vel
// /odom
// /rosout
// /rosout_agg
// /tf

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

ros::Publisher pub;

void turtleCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	ROS_INFO("-------------------"); // size == 24

	// ROS_INFO("Size: %ld Laser Scanner subscriber@[%f, %f, %f]", // size == 24
	// msg->ranges.size(), msg->range_max, msg->angle_max, msg->scan_time);
	for(float range = 1.6; range > 0.7; range = range-0.1){
		// float range = 1.5;
		int j = ((msg->ranges.size()-1)/2);
		float frontDist = 0;
		float minDist = 100;
		for (int i = j-30; i < j +61; i++){
			frontDist = frontDist + msg->ranges[i];
			if (msg->ranges[i] < minDist){
				minDist = msg->ranges[i];
			}
		}
		frontDist = frontDist/ 60;
		if (frontDist > range && minDist > 0.9){
			ROS_INFO("Forward %f  ..M=%f", frontDist, minDist); // size == 24
			geometry_msgs::Twist msg;
			msg.linear.x = (0.8 * (minDist))+1;
			msg.linear.y = 0;
			msg.linear.z = 0;
			msg.angular.x = 0;
			msg.angular.y = 0;
			msg.angular.z = 0;
			pub.publish(msg);
			range = 0;
			break;
		}
		int ji = 20;
		int size = ((msg->ranges.size()-1)/2)-22;
		for (int i = 0 ; i < size; i++) {
			ji = ji+3;
			float avgCW = ( msg->ranges[size - ji - 1]
										+ msg->ranges[size - ji] + msg->ranges[size - ji +1] )/3;
			float avgCCW = ( msg->ranges[size + ji - 1]
										+ msg->ranges[size + ji] + msg->ranges[size + ji +1] )/3;
			//ROS_INFO("for at %d range %f [%f --- %f]",i, range, avg_R, avg_L);
			if(avgCW > range){
				geometry_msgs::Twist msg;
				ROS_INFO("Turn CW-%d . %f ... %f", i ,avgCCW, avgCW); // size == 24
				msg.linear.x = 0;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = (-8.00*i)-9.00;
				pub.publish(msg);
				range = 0;
				break;
			// 	ROS_INFO("Publisher at %d",i); // size == 24
			}
			else if (avgCCW > range){
				geometry_msgs::Twist msg;
				ROS_INFO("Turn CCW-%d %f ... %f", i,avgCCW, avgCW); // size == 24
				msg.linear.x = 0;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = (8.00*i)+9.00;
				pub.publish(msg);
				range = 0;
				break;
			}
			}

	}
	// geometry_msgs::Twist my_vel;
	// my_vel.linear.x = 1.0;
	// // my_vel.angular.z = 1.0;
	// pub.publish(my_vel);

}

int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system
	ros::init(argc, argv, "as2Controller");
	ros::NodeHandle nh;
	//ros::Subscriber sub = nh.subscribe("turtle1/pose", 1,turtleCallback);
	ros::Subscriber sub = nh.subscribe("/base_scan", 1,turtleCallback);
	// Define the subscriber to turtle's position
	pub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);

	ROS_INFO("GO");

	// 		sleep(1);
	// }

	// pub2 =nh.advertise<geometry_msgs::Vel> ;
	// ros::ServiceClient client1 = nh.serviceClient<turtlesim::Spawn>("/spawn");
	// turtlesim::Spawn srv1;
	// srv1.request.x= 1.0;
	// srv1.request.y= 1.0;
	// srv1.request.theta= 0.0;
	// srv1.request.name = "rt1_turtle";
	//
	// client1.waitForExistence();
	// client1.call(srv1);

	ros::spin();
	return 0;
}
