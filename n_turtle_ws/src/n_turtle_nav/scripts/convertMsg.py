#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import PoseWithCovarianceStamped
from nav_msgs.msg import Odometry

def callback(data):
    odom = Odometry()
    odom.header = data.header
    odom.pose = data.pose
    pub.publish(odom)

rospy.init_node('pose_to_odom_converter')
sub = rospy.Subscriber('/robot_pose_ekf/odom_combined', PoseWithCovarianceStamped, callback)
pub = rospy.Publisher('/odom_combined_converted', Odometry, queue_size=10)
rospy.spin()
