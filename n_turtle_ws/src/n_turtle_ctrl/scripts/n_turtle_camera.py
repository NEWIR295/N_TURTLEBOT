#!/usr/bin/env python3

import rospy
import requests
import cv2
import numpy as np
import imutils
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

# Replace the below URL with your own. Make sure to add "/shot.jpg" at last.
url = "http://192.168.45.254:8080/shot.jpg"

def fetch_and_publish():
    # Initialize the ROS node
    rospy.init_node('image_publisher_node', anonymous=True)

    # Create a Publisher to publish images to a topic
    image_pub = rospy.Publisher('/android_cam_image', Image, queue_size=10)
    
    # Create a CvBridge object to convert OpenCV images to ROS Image messages
    bridge = CvBridge()

    # Set a publishing rate (e.g., 10 Hz)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        try:
            # Fetch the image from the URL
            img_resp = requests.get(url)
            img_arr = np.array(bytearray(img_resp.content), dtype=np.uint8)
            img = cv2.imdecode(img_arr, -1)

            # Resize the image
            img = imutils.resize(img, width=1000, height=1800)

            # Convert OpenCV image to ROS Image message
            image_msg = bridge.cv2_to_imgmsg(img, encoding="bgr8")

            # Publish the image message
            image_pub.publish(image_msg)

        except CvBridgeError as e:
            rospy.logerr(f"CvBridge Error: {e}")
        except Exception as e:
            rospy.logerr(f"Failed to fetch or publish image: {e}")

        # Sleep to maintain the loop rate
        rate.sleep()

if __name__ == '__main__':
    try:
        fetch_and_publish()
    except rospy.ROSInterruptException:
        pass
