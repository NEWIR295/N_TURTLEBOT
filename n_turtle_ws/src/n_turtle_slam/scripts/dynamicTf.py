#!/usr/bin/env python3

import rospy
import tf
from nav_msgs.msg import Odometry

def handle_odom_to_base_link(msg):
    br = tf.TransformBroadcaster()
    br.sendTransform((msg.pose.pose.position.x,
                      msg.pose.pose.position.y,
                      msg.pose.pose.position.z),
                     (msg.pose.pose.orientation.x,
                      msg.pose.pose.orientation.y,
                      msg.pose.pose.orientation.z,
                      msg.pose.pose.orientation.w),
                     rospy.Time.now(),
                     "base_footprint",
                     "odom")

if __name__ == '__main__':
    rospy.init_node('odometry_to_tf')
    rospy.Subscriber('/odom', Odometry, handle_odom_to_base_link)
    rospy.spin()
