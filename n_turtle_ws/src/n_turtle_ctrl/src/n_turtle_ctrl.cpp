#include "n_turtle_ctrl.h"

N_TURTLE_CTRL::N_TURTLE_CTRL() {
    theta_initial = 0;
    x_initial = 0;
    y_initial = 0;
}

std_msgs::Float32MultiArray N_TURTLE_CTRL::getLinearSpeed(const geometry_msgs::Twist &cmdVel,float wheel_separation,float dt) {
    float omega = cmdVel.angular.z;
    float Vx = cmdVel.linear.x;
    float Vy = cmdVel.linear.y;
    float theta = omega*dt;//atan2(Vy, Vx);

    matrix1_i << cos(theta), sin(theta), wheel_separation,
                 cos(theta), sin(theta), -wheel_separation;
    matrix2_i << Vx,
                 Vy,
                 omega;
    Matrix<float, 2, 1> result = matrix1_i * matrix2_i;

    std_msgs::Float32MultiArray x;
    x.data.resize(2);
    x.data[0]=result[0]; // Left speed
    x.data[1]=result[1]; // Right speed
    return x;
}
std_msgs::Float32MultiArray N_TURTLE_CTRL::stop_n_turtle() {
    std_msgs::Float32MultiArray stop;
    stop.data = {0,0};
    return stop;
}
nav_msgs::Odometry N_TURTLE_CTRL::distance_covered_enc(float dt, float w_l, float w_r,float wheel_radius,float wheel_separation,ros::Time current_time) {
    matrix1_f << cos(theta_initial), cos(theta_initial),
                 sin(theta_initial), sin(theta_initial),
                 1 / wheel_separation, -1 / wheel_separation;
    matrix2_f << w_r,
                 w_l;
    Matrix<float, 3, 1> result = (wheel_radius / 2) * matrix1_f * matrix2_f;

    x_initial += (dt * result[0]);
    y_initial += (dt * result[1]);
    theta_initial += (dt * result[2]);

    geometry_msgs::Quaternion odom_quat;
    odom_quat = tf::createQuaternionMsgFromYaw(theta_initial);
    tf::TransformBroadcaster odom_broadcaster ;
    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "Agribot_base_link";
    odom_trans.transform.translation.x = x_initial;
    odom_trans.transform.translation.y = y_initial;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;
    //send the transform
    odom_broadcaster.sendTransform(odom_trans);
    //set position
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";
    odom.pose.pose.position.x = x_initial;
    odom.pose.pose.position.y = y_initial;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = result[0];
    odom.twist.twist.linear.y = result[1];
    odom.twist.twist.angular.z = result[2];

    return odom;
}
geometry_msgs::Twist N_TURTLE_CTRL::cmdVel(float w_l, float w_r,float wheel_radius,float wheel_separation) {
    matrix1_f << cos(theta_initial), cos(theta_initial),
                 sin(theta_initial), sin(theta_initial),
                 1 / wheel_separation, -1 / wheel_separation;
    matrix2_f << w_r,
                 w_l;
    Matrix<float, 3, 1> result = (wheel_radius / 2) * matrix1_f * matrix2_f;


    geometry_msgs::Twist twistVel;
    twistVel.linear.x = result[0]*2.5;
    twistVel.linear.y = result[1]*2.5;
    twistVel.linear.z = 0;
    twistVel.angular.x = 0;
    twistVel.angular.y = 0;
    twistVel.angular.z = -result[2]*2.5;

    return twistVel;
}
N_TURTLE_CTRL::~N_TURTLE_CTRL() {
    // Destructor
}
