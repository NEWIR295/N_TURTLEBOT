#include <Arduino.h>
#include "ctrl.h"

ros::NodeHandle nh;


void setup() {
  // put your setup code here, to run once:
  nh.getHardware()->setBaud(38400);
  //nh.getHardware()->setBaud(57600);
  nh.initNode();
  //mpu_init();
  ctrlInit();
}

void loop() {
  // put your main code here, to run repeatedly:

  robotMove();
  nh.spinOnce();

}

