#include "ugv_sdk/scout/scout_base.hpp"

using namespace westonrobot;

int main(int argc, char **argv)
{
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

    scout.EnableCommandedMode();
    size_t count = 100;
    for (size_t i = 0; i < count; i++)
    {
        std::cout << "Motor:0.2,0" << std::endl;
        scout.SetMotionCommand(0.2, 0.0); //设定线速度为0.2

        usleep(20000);//挂起两万微秒,20毫秒
    }

    return 0;
}