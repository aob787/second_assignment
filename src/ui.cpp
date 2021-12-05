#include "ros/ros.h"
#include "second_assignment/speed_regulator.h"
#include <std_srvs/Empty.h>

int main(int argc, char **argv) {
	ros::init(argc, argv, "ui");
  ros::NodeHandle nh;
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
      ROS_INFO("Incresed"); // size == 24
      srv.request.state_param = 1;
      client1.call(srv);
      ROS_INFO("Speed multipler= %.2f", srv.response.speed_param);
      break;
    case 's':
      ROS_INFO("Decresed"); // size == 24
      srv.request.state_param = 2;
      client1.call(srv);
      ROS_INFO("Speed multipler= %.2f", srv.response.speed_param);
      break;
    case 'r':
      ROS_INFO("Reseted"); // size == 24
      client2.call(srv2);
      break;
  }
  ros::spinOnce();

  }
}
