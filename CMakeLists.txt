cmake_minimum_required(VERSION 2.8.3)

project(segfault_demo)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
endif()

find_package(ament_cmake REQUIRED)

find_package(builtin_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rmw REQUIRED)
find_package(rmw_implementation_cmake REQUIRED)


macro(targets)
	if(NOT "${target_suffix} " STREQUAL " ")
	  get_rclcpp_information("${rmw_implementation}" "rclcpp${target_suffix}")
	endif()
	
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

	add_executable(segfault_demo_server${target_suffix} src/main.cpp)
	add_executable(segfault_demo_client${target_suffix} src/test.cpp)
	ament_target_dependencies(segfault_demo_server${target_suffix}
        	"rclcpp${target_suffix}"
		"std_msgs"
	)
	ament_target_dependencies(segfault_demo_client${target_suffix}
        	"rclcpp${target_suffix}"
		"std_msgs"
	)
   
   


endmacro()



call_for_each_rmw_implementation(targets GENERATE_DEFAULT)

ament_package()
