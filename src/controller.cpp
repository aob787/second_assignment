//roscore
//rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
//rosrun second_assignment controller_node
//
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"

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
	for(float range = 2; range > 0.4; range = range-0.1){
		int jCCW = ((msg->ranges.size()-1)/2) + 5;
		int jCW = ((msg->ranges.size()-1)/2) - 5;
		for (int i = 0 ; i < 14; i++) {
			jCW = jCW + (10 * i);
			jCCW = jCCW + (10 * i);
			float avgCW = (msg->ranges[jCW-5] + msg->ranges[jCW-4] + msg->ranges[jCW-3] + msg->ranges[jCW-2] + msg->ranges[jCW-1]
										+ msg->ranges[jCW] + msg->ranges[jCW+1]+ msg->ranges[jCW+2] + msg->ranges[jCW+3] + msg->ranges[jCW+4])/10;
			float avgCCW = (msg->ranges[jCCW-5] + msg->ranges[jCCW-4] + msg->ranges[jCCW-3] + msg->ranges[jCCW-2] + msg->ranges[jCCW-1]
									+ msg->ranges[jCCW] + msg->ranges[jCCW+1]+ msg->ranges[jCCW+2] + msg->ranges[jCCW+3] + msg->ranges[jCCW+4])/10;
			//ROS_INFO("for at %d range %f [%f --- %f]",i, range, avg_R, avg_L);
			geometry_msgs::Twist msg;
			if (i ==0 && avgCCW > range && avgCW > range){
				ROS_INFO("Forward"); // size == 24
				geometry_msgs::Twist msg;
				msg.linear.x = 3.00;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = 0;
				pub.publish(msg);
				range = 0;
				break;
			}
			else if(avgCW > range){
				ROS_INFO("Turn CW %d", i); // size == 24
				geometry_msgs::Twist msg;
				msg.linear.x = 0.02;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = (-1.2*i)-2.3;
				pub.publish(msg);
				range = 0;
				break;
			// 	ROS_INFO("Publisher at %d",i); // size == 24
			}
			else if (avgCCW > range){
				ROS_INFO("Turn CCW %d", i); // size == 24
				msg.linear.x = 0.02;
				msg.linear.y = 0;
				msg.linear.z = 0;
				msg.angular.x = 0;
				msg.angular.y = 0;
				msg.angular.z = (1.2*i)+2.3;
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
