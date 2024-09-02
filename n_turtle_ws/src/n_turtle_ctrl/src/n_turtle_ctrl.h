#ifndef N_TURTLE_CTRL_H
#define N_TURTLE_CTRL_H
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include "std_msgs/Float32MultiArray.h"
#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

class N_TURTLE_CTRL{
private:

    Matrix<float, 2, 3> matrix1_i;
    Matrix<float, 3, 1> matrix2_i;
    Matrix<float, 3, 2> matrix1_f;
    Matrix<float, 2, 1> matrix2_f;
    double theta_initial;
    double x_initial;
    double y_initial;
public:
    N_TURTLE_CTRL();
    std_msgs::Float32MultiArray stop_n_turtle();
    std_msgs::Float32MultiArray getLinearSpeed(const geometry_msgs::Twist &cmdVel,float wheel_separation,float dt);
    nav_msgs::Odometry distance_covered_enc(float dt, float w_l, float w_r,float wheel_radius,float wheel_separation,ros::Time current_time);
    geometry_msgs::Twist cmdVel(float w_l, float w_r,float wheel_radius,float wheel_separation) ;

    ~N_TURTLE_CTRL();
};

#endif // N_TURTLE_CTRL_H
