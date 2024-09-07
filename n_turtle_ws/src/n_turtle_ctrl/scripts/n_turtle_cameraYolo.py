#!/usr/bin/env python3

import rospy
import requests
import cv2
import numpy as np
from ultralytics import YOLO
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import imutils

# Replace the below URL with your own. Make sure to add "/shot.jpg" at last.
url = "http://192.168.45.254:8080/shot.jpg"

# Colors for bounding box and text
DARK_GREEN = (0, 100, 0)  # Dark Green color for the box
DARK_RED = (0, 0, 139)    # Dark Red color for the text

def fetch_and_publish():
    # Initialize the ROS node
    rospy.init_node('yolov8_image_publisher_node', anonymous=True)

    # Create a Publisher to publish images to a topic
    image_pub = rospy.Publisher('/yolov8_android_cam_image', Image, queue_size=10)
    
    # Create a CvBridge object to convert OpenCV images to ROS Image messages
    bridge = CvBridge()

    # Load the YOLOv8 model
    model = YOLO('yolov8n.pt')  # You can replace this with a custom model if needed

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

            # Perform YOLOv8 object detection
            results = model(img)

            # Annotate the image with bounding boxes and labels
            for result in results:
                for bbox in result.boxes:
                    # Extract bounding box coordinates and label
                    x1, y1, x2, y2 = map(int, bbox.xyxy[0])
                    label = model.names[int(bbox.cls)]

                    # Draw bounding box (Dark Green)
                    cv2.rectangle(img, (x1, y1), (x2, y2), DARK_GREEN, 2)

                    # Display label (Dark Red)
                    cv2.putText(img, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, DARK_RED, 2)

            # Convert the annotated OpenCV image to a ROS Image message
            image_msg = bridge.cv2_to_imgmsg(img, encoding="bgr8")

            # Publish the image message
            image_pub.publish(image_msg)

        except CvBridgeError as e:
            rospy.logerr(f"CvBridge Error: {e}")
        except Exception as e:
            rospy.logerr(f"Failed to fetch, detect, or publish image: {e}")

        # Sleep to maintain the loop rate
        rate.sleep()

if __name__ == '__main__':
    try:
        fetch_and_publish()
    except rospy.ROSInterruptException:
        pass
