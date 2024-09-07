#include <ros/ros.h>
#include "n_turtle_ctrl.h"
std_msgs::Float32MultiArray currentWheelAngVelocity;
void cb(const std_msgs::Float32MultiArray &wheel_ang_vel){
  currentWheelAngVelocity = wheel_ang_vel;
}
int main(int argc, char** argv){
  ros::init(argc, argv, "N_turtleTwist");
  ros::NodeHandle n;
  ros::Publisher twist_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 50);
  ros::Subscriber enc_sub = n.subscribe("/encoder", 10, &cb);
  currentWheelAngVelocity.data.resize(2);
  N_TURTLE_CTRL n_turtle;
  //robot param
  float wheel_radius, wheel_separation;
  n.getParam("wheel_radius",wheel_radius);
  n.getParam("wheel_separation",wheel_separation);
  geometry_msgs::Twist currentTwist;
  //our rtos param
  ros::Time current_time, last_time;
  last_time = ros::Time::now();
  ros::Rate r(10.0);//pub rate
  while(ros::ok()){
    current_time = ros::Time::now();// get time stamp of current message
    double dt = (current_time - last_time).toSec();   // time difference between now and last loop iteration
    currentTwist = n_turtle.cmdVel(currentWheelAngVelocity.data[0],currentWheelAngVelocity.data[1],wheel_radius,wheel_separation);   
    /* Update the twist */    
   // Check if wheel angular velocity message is available
    if (!currentWheelAngVelocity.data.empty()) {
      // Update twist
      // Publish twist message
      twist_pub.publish(currentTwist);
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