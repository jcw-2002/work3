#include "car_control.h"

#include <iostream>

#include <ros/ros.h>

#include "ugv_sdk/scout/scout_base.hpp" //使用小车上的sdk版本

#include "car_scout/car_msg.h"

// #include<opencv2/opencv.hpp>
using namespace westonrobot;

car_control::car_control(/* args */)
{
    device_name = "can0";
    baud_rate = 0;
    scout.Connect(device_name, baud_rate);
    ROS_INFO("controler inited!");
}

void car_control::move(int keyboard)
{
    if (keyboard == '8')
    {
        scout.SetMotionCommand(0.2, 0.0);
    }
    else if (keyboard == '2')
    {
        scout.SetMotionCommand(-0.2, 0.0);
    }
    else if (keyboard == '4')
    {
        scout.SetMotionCommand(0.0, 0.2);
    }
    else if (keyboard == '6')
    {
        scout.SetMotionCommand(0.0, -0.2);
    }
    else
    {
        ROS_INFO("%d", keyboard);
    }
    return;
}

car_control::~car_control()
{

    ROS_INFO("controler deleted!");
}
