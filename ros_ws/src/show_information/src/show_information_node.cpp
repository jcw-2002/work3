#include <iostream>
#include <unistd.h>

#include <ros/ros.h>

// #include "ugv_sdk/mobile_robot/scout_robot.hpp"
// #include "ugv_sdk/utilities/protocol_detector.hpp"

#include "ugv_sdk/scout/scout_base.hpp" //使用小车上的sdk版本

using namespace westonrobot;

int main(int argc, char **argv)
{
    std::cout << argv[0] << std::endl;
    std::string device_name;
    int32_t baud_rate = 0;

    if (argc == 2)
    {
        device_name = {argv[1]};
        std::cout << "Specified CAN: " << device_name << std::endl;
    }
    else if (argc == 3)
    {
        device_name = {argv[1]};
        baud_rate = std::stol(argv[2]);
        std::cout << "Specified serial: " << device_name << "@" << baud_rate
                  << std::endl;
    }
    else
    {
        std::cout << "Usage: app_scout_demo <interface>" << std::endl
                  << "Example 1: ./app_scout_demo can0" << std::endl
                  << "Example 2: ./app_scout_demo /dev/ttyUSB0 115200" << std::endl;
        return -1;
    }

    ScoutBase scout;
    scout.Connect(device_name, baud_rate);

    for (size_t i = 0; i < 100; i++)
    {
        auto state = scout.GetScoutState(); //获取小车状态

        //控制状态
        std::cout << "control mode:" << static_cast<int>(state.control_mode)
                  << ",base state:" << static_cast<int>(state.base_state) << std::endl;

        std::cout << "battery voltage: " << state.battery_voltage << std::endl; //电池电压

        //车灯
        std::cout << "light_control_enabled: " << state.light_control_enabled
                  << "front_light_state_mode:" << state.front_light_state.mode
                  << "front_light_state_custom_value:" << state.front_light_state.custom_value
                  << std::endl;
        //电机
        std::cout << " motor state:"
                  << "motor_num:" << state.motor_num << std::endl; //之后再考虑显示每个电机的状态

        //运动状态
        std::cout << "motion state: "
                  << "linear_velocity" << state.linear_velocity
                  << "angular_velocity" << state.angular_velocity
                  << std::endl;
        usleep(20000);
    }

    return 0;
}