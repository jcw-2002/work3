#include "car_info.h"

#include <iostream>

#include <ros/ros.h>

#include "ugv_sdk/scout/scout_base.hpp" //使用小车上的sdk版本

#include "car_scout/car_msg.h"
#include "car_scout/motor_state.h"

using namespace westonrobot;

car_info::car_info(ros::NodeHandle _nh)
{
    device_name = "can0";
    baud_rate = 0;
    scout.Connect(device_name, baud_rate);
    ROS_INFO("conneted!");
    nh = _nh;
    info_pub = nh.advertise<car_scout::car_msg>("/car_info", 100);
    ROS_INFO("car_info inited!");
}

car_info::~car_info()
{
    ROS_INFO("car_info deleted!");
}

void car_info::publish()
{
    generate_msg();
    info_pub.publish(msg);
    return;
}

void car_info::generate_msg()
{
    state = scout.GetScoutState(); //重新获取状态
    msg.stamp = ros::Time::now();
    msg.control_mode = static_cast<int>(state.control_mode);
    msg.battery_voltage = state.battery_voltage;
    msg.front_light_state.mode = static_cast<int>(state.front_light_state.mode);
    msg.rear_light_state.mode = static_cast<int>(state.rear_light_state.mode);
    msg.motor_num = static_cast<int>(state.motor_num);
    msg.motors.clear();
    msg.motors.reserve(msg.motor_num + 1);
    for (size_t i = 0; i < msg.motor_num; i++)
    {
        car_scout::motor_state tempstate;
        tempstate.motor_current = state.actuator_states[i].motor_current;
        tempstate.motor_rpm = state.actuator_states[i].motor_rpm;
        tempstate.motor_pulses = state.actuator_states[i].motor_pulses;
        tempstate.motor_temperature = state.actuator_states[i].motor_temperature;
        tempstate.driver_voltage = state.actuator_states[i].driver_voltage;
        tempstate.driver_temperature = state.actuator_states[i].driver_temperature;
        tempstate.driver_state = state.actuator_states[i].driver_state;
        msg.motors.push_back(tempstate);
    }
    msg.motion.angular_velocity = state.angular_velocity;
    msg.motion.linear_velocity = state.linear_velocity;
    ROS_INFO("msg get");
    return;
}
void car_info::info_callback(const car_scout::car_msg &msg)
{
    ROS_INFO("battery_voltage%f", msg.battery_voltage);
    return;
}