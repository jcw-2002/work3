#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_datatypes.h>
#include <nav_msgs/Path.h>
#include <tf/tf.h>
#include <ctime>

using namespace std;

int imuReceivedNumber = 0;

// time_stamp
double time_k, time_k_1, dt;

// gravitational acceleration
const tf::Vector3 g(0, 0, 0);

tf::Vector3 acc_b_k, acc_b_k_1;

tf::Vector3 w_b_k, w_b_k_1;

// linear acceleration at  k+1  after  rotated  and mid-value approximated
tf::Vector3 acc_k_1;

// angular velocity at k+1  after rotated and mid-value approximated
tf::Vector3 w_k_1;

// velocity at k and k+1
tf::Vector3 velo_k, velo_k_1;

// location at k and k+1
tf::Vector3 position_k, position_k_1;

// pose at k and k+1  signalled by quaternion
tf::Quaternion q_k, q_k_1;

ros::Publisher pubdatadisplay;
nav_msgs::Path path;

void pubPathmsg(tf::Vector3 position_, tf::Quaternion quaternion_)
{
    geometry_msgs::PoseStamped display_imu;
    display_imu.header.stamp = ros::Time::now();
    display_imu.header.frame_id = "display";
    display_imu.pose.position.x = position_.getX();
    display_imu.pose.position.y = position_.getY();
    display_imu.pose.position.z = position_.getZ();

    geometry_msgs::Quaternion q_msgs;
    q_msgs.x = quaternion_.getX();
    q_msgs.y = quaternion_.getY();
    q_msgs.z = quaternion_.getZ();
    q_msgs.w = quaternion_.getW();
    display_imu.pose.orientation = q_msgs;

    path.poses.push_back(display_imu);
    path.header.stamp = ros::Time::now();
    path.header.frame_id = "display";
    pubdatadisplay.publish(path);
}

// to receive imu message
void imuHandler(const sensor_msgs::Imu::ConstPtr &imuIn)
{
    if (imuReceivedNumber == 0)
    {
        acc_b_k[0] = imuIn->linear_acceleration.x;
        acc_b_k[1] = imuIn->linear_acceleration.y;
        acc_b_k[2] = 0;

        w_b_k[0] = imuIn->angular_velocity.x;
        w_b_k[1] = imuIn->angular_velocity.y;
        w_b_k[2] = imuIn->angular_velocity.z;

        time_k = imuIn->header.stamp.toSec();

        velo_k[0] = 0;
        velo_k[1] = 0;
        velo_k[2] = 0;

        position_k[0] = 0;
        position_k[1] = 0;
        position_k[2] = 0;

        q_k.setX(0);
        q_k.setY(0);
        q_k.setZ(0);
        q_k.setW(1);

        pubPathmsg(position_k, q_k);
        imuReceivedNumber++;
    }
    else if (imuReceivedNumber == 1)
    {
        acc_b_k_1[0] = imuIn->linear_acceleration.x;
        acc_b_k_1[1] = imuIn->linear_acceleration.y;
        acc_b_k_1[2] = 0;
        // velo_k_1[0] =  velo_k[0] + ((acc_b_k[0]+acc_b_k_1[0])/2)*dt;

        w_b_k_1[0] = imuIn->angular_velocity.x;
        w_b_k_1[1] = imuIn->angular_velocity.y;
        w_b_k_1[2] = imuIn->angular_velocity.z;

        time_k_1 = imuIn->header.stamp.toSec();

        dt = time_k_1 - time_k;

        w_k_1 = (w_b_k + w_b_k_1) / 2;
        tf::Quaternion dw;

        dw.setX(w_k_1[0] * dt);
        dw.setY(w_k_1[1] * dt);
        dw.setZ(w_k_1[2] * dt);
        dw.setW(1);

        // pose
        q_k_1 = q_k * dw;

        // acceleration
        tf::Vector3 acc_b_k_r = acc_b_k.rotate(q_k.getAxis(), q_k.getAngle());
        tf::Vector3 acc_b_k_1_r = acc_b_k_1.rotate(q_k_1.getAxis(), q_k_1.getAngle());
        acc_k_1 = (acc_b_k_r - g + acc_b_k_1_r - g) / 2;

        // position
        position_k_1 = position_k + velo_k * dt + (dt) * (acc_k_1 * dt) / 2;

        // velocity
        velo_k_1 = velo_k + acc_k_1 * dt;

        pubPathmsg(position_k_1, q_k_1);

        w_b_k_1 = w_b_k;
        acc_b_k = acc_b_k_1;
        time_k = time_k_1;
        q_k = q_k_1;
        position_k = position_k_1;
        velo_k = velo_k_1;

        cout << "dt: " << dt << "     x: " << position_k_1[0] << "   y: " << position_k_1[1] << "   z: " << position_k_1[2] << endl;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "scanRegistration");
    ros::NodeHandle nh;

    ros::Subscriber subImu = nh.subscribe<sensor_msgs::Imu>("/imu/data_raw", 50, imuHandler);
    pubdatadisplay = nh.advertise<nav_msgs::Path>("/display", 100);

    ros::spin();

    return 0;
}
