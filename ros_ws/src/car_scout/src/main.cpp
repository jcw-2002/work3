#include "car_info.h"
#include "car_control.h"
#include <ros/ros.h>

#include "car_scout/car_msg.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "car_scout");
    ros::NodeHandle nh;

    car_info test_info(nh);
    // car_control test_control;
    ros::Rate rate(10);
    while (ros::ok())
    {
        test_info.publish();
        rate.sleep();
    }

    return 0;
}