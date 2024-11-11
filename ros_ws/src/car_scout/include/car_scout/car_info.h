#include <iostream>

#include <ros/ros.h>

#include "ugv_sdk/scout/scout_base.hpp" //使用小车上的sdk版本

#include "car_scout/car_msg.h"

class car_info
{
private:
    std::string device_name;
    int32_t baud_rate;
    westonrobot::ScoutBase scout;
    car_scout::car_msg msg;
    ros::NodeHandle nh;
    ros::Publisher info_pub;
    westonrobot::ScoutState state;
    void generate_msg();

public:
    car_info(ros::NodeHandle);
    ~car_info();
    void publish();
    void info_callback(const car_scout::car_msg &msg);
    // void move(char);
};
