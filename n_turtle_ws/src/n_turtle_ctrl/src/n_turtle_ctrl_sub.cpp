/* std libraries */
#include <ros/ros.h>
#include "n_turtle_ctrl.h"
/* global variables */
std_msgs::Float32MultiArray currentWheelAngVelocity;
/* subscriber call back */
void cb(const std_msgs::Float32MultiArray &wheel_ang_vel){
  currentWheelAngVelocity = wheel_ang_vel;
}
/* this node specialized to calculate odom from encoder */
int main(int argc, char** argv){
  ros::init(argc, argv, "N_TurtleVelENC");
  ros::NodeHandle n;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("/N_TurtleVelENC", 50);
  ros::Subscriber enc_sub = n.subscribe("/encoder", 10, &cb);
  currentWheelAngVelocity.data.resize(2);
  N_TURTLE_CTRL n_turtle;
  //robot param
  float wheel_radius, wheel_separation;
  n.getParam("wheel_radius",wheel_radius);
  n.getParam("wheel_separation",wheel_separation);
  nav_msgs::Odometry currentOdom ;
  //rtos param
  ros::Time current_time, last_time;
  last_time = ros::Time::now();
  ros::Rate r(10.0);
  while(ros::ok()){
    current_time = ros::Time::now();// get time stamp of current message
    double dt = (current_time - last_time).toSec();   // time difference between now and last loop iteration
    /* Update the odometry */    
   currentOdom = n_turtle.distance_covered_enc(dt, currentWheelAngVelocity.data[0],currentWheelAngVelocity.data[1],wheel_radius,wheel_separation,current_time);
   // Check if wheel angular velocity message is available
    if (!currentWheelAngVelocity.data.empty()) {
      // Update odometry
      // Publish odometry message
      odom_pub.publish(currentOdom);
    } 
    else {
            // Warn if no data from wheel angular velocity topic
            ROS_WARN("No data from wheel angular velocity topic.");
            continue;
    }

    last_time = current_time;/* Save time for next iteration */
    r.sleep();              /* Sleep until next cycle */
    ros::spinOnce();               // check for incoming messages
  }
  return 0;
}