#include <iostream>
#include "rclcpp/rclcpp.hpp"
#include <memory>

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    
    auto node = rclcpp::node::Node::make_shared("Parameter_Server_test");

	//Extra scope so the first client will be deleted afterwards
    {
        std::cout << "Creating first client" << std::endl;
        auto parameters_client = std::make_shared<rclcpp::parameter_client::AsyncParametersClient>(node, "ParameterServer");
        auto results = parameters_client->set_parameters({
                                                             rclcpp::parameter::ParameterVariant("foo", 2),
                                                             rclcpp::parameter::ParameterVariant("bar", "hello"),
                                                             rclcpp::parameter::ParameterVariant("baz", 1.45),
                                                             rclcpp::parameter::ParameterVariant("foobar", true),
                                                         });
        // Wait for the results.
        if (rclcpp::spin_until_future_complete(node, results) != rclcpp::executor::FutureReturnCode::SUCCESS)
        {
            printf("set_parameters service call failed. Exiting example.\n");
            return -1;
        }
    }
    std::cout << "Creating second client" << std::endl;
    auto parameters_client_2 = std::make_shared<rclcpp::parameter_client::AsyncParametersClient>(node, "ParameterServer");
    // Get a few of the parameters just set.
    auto parameters = parameters_client_2->get_parameters({"foo", "baz"});
	//Here comes the segfault
    if (rclcpp::spin_until_future_complete(node, parameters) != rclcpp::executor::FutureReturnCode::SUCCESS)
    {
        printf("get_parameters service call failed. Exiting example.\n");
        return -1;
    }
    for (auto & parameter : parameters.get()) {
        std::cout << "Parameter name: " << parameter.get_name() << std::endl;
        std::cout << "Parameter value (" << parameter.get_type_name() << "): " <<
                     parameter.value_to_string() << std::endl;
    }

    std::cout << "Started ParameterServer" << std::endl;
    rclcpp::WallRate loop_rate(30);
    while (rclcpp::ok()) {

        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
    std::cout << "Exit ParameterServer" << std::endl;
    return 0;

}
