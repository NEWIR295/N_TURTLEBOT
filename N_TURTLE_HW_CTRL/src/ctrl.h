#ifndef  myCtrl
#define  myCtrl

#include <Arduino.h>
#include <ros.h>
#include <util/atomic.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float32.h>
#include <sensor_msgs/Imu.h>
#include "MPU9250.h"

//set RTOS parameters
#define interval 100.0 //set rtos intervals

//encoder pins
#define encoderLeftA 2
#define encoderLeftB 3
#define encoderRightA 18
#define encoderRightB 19
#define PPR 440.0

//motor pins
//motor driver left side
#define pinDirLeft1 8 // dir left motor = Pin 8
#define pinPwmLeft1 9 // PWM left motor = Pin 9

//motor driver right side
#define pinDirRight1 10 // dir right motor = Pin 10
#define pinPwmRight1 11 // PWM right motor = Pin 11

extern ros::NodeHandle nh; // Declare the nh object as external

//ctrl func prototype
void robotMove(void); //set motors speed
void ctrlInit(void); //encoder & motor init
void updateEncoderLeft(void);
void updateEncoderRight(void);
void setSpeed(int pwm, int dirPin, int PwmPin);//set motor speed
int mypwm(float x);//convert speed to motor effort

#endif