#include <iostream>

#include <ros/ros.h>

#include "ugv_sdk/scout/scout_base.hpp" //使用小车上的sdk版本

#include "car_scout/car_msg.h"

class car_control
{
private:
    std::string device_name;
    int32_t baud_rate;
    westonrobot::ScoutBase scout;

public:
    car_control(/* args */);
    ~car_control();
    void move(int keyboard);
};
