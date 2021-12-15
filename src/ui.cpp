#include "ros/ros.h"
#include "second_assignment/speed_regulator.h"
#include <std_srvs/Empty.h>

int main(int argc, char **argv) {
	ros::init(argc, argv, "ui");
  ros::NodeHandle nh;
	//create service clients for increse speed and reset pos.
  ros::ServiceClient client1 = nh.serviceClient<second_assignment::speed_regulator>("/speed_regulator");
  ros::ServiceClient client2 = nh.serviceClient<std_srvs::Empty>("/reset_positions");
  second_assignment::speed_regulator srv;
  std_srvs::Empty srv2;

	char userInput;
  while (true) {
  std::cout << "w for increase the speed, s for decrease the speed, and r for reset the positions: ";
  std::cin >> userInput;
  switch (userInput) {
    case 'w':
      srv.request.state_param = 1; // 1 means increase the speed
      client1.call(srv);
      ROS_INFO("Increased");
      ROS_INFO("Speed multipler= %.2f", srv.response.speed_param);
      break;
    case 's':
      srv.request.state_param = 2;// 2 means decrease the speed
      client1.call(srv);
			ROS_INFO("Decreased");
      ROS_INFO("Speed multipler= %.2f", srv.response.speed_param);
      break;
    case 'r':
      client2.call(srv2);
			ROS_INFO("Reseted");
      break;
  }
  ros::spinOnce();

  }
}
