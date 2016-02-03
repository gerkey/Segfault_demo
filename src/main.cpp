#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include <memory>

void on_parameter_event(const rcl_interfaces::msg::ParameterEvent::SharedPtr event)
{
  std::cout << "Parameter event:" << std::endl << " new parameters:" << std::endl;
  for (auto & new_parameter : event->new_parameters) {
    std::cout << "  " << new_parameter.name << std::endl;
  }
  std::cout << " changed parameters:" << std::endl;
  for (auto & changed_parameter : event->changed_parameters) {
    std::cout << "  " << changed_parameter.name << std::endl;
  }
  std::cout << " deleted parameters:" << std::endl;
  for (auto & deleted_parameter : event->deleted_parameters) {
    std::cout << "  " << deleted_parameter.name << std::endl;
  }
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::node::Node::make_shared("ParameterServer");  
    auto parameter_service = std::make_shared<rclcpp::parameter_service::ParameterService>(node);
    auto parameters_client = std::make_shared<rclcpp::parameter_client::AsyncParametersClient>(node);
    auto sub = parameters_client->on_parameter_event(on_parameter_event);
    std::cout << "Started ParameterServer" << std::endl;
    rclcpp::WallRate loop_rate(30);
    while (rclcpp::ok()) {
	
	rclcpp::spin_some(node);
	loop_rate.sleep();
    }
    std::cout << "Exit ParameterServer" << std::endl;
    return 0;
   
}
