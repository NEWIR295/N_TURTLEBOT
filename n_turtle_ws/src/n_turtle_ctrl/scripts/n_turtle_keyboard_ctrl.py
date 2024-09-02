#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist
import sys, select, termios, tty

# Instructions for controlling the robot
msg = """
Control Your Robot!
---------------------------
Moving around:
        w
   a    s    d
        x

w/x : increase/decrease linear velocity
a/d : increase/decrease angular velocity

CTRL-C to quit
"""

# Define movement bindings for keys
# (linear velocity, angular velocity)
moveBindings = {
    'w': (0.1, 0),
    's': (-0.1, 0),
    'a': (0, -0.1),
    'd': (0, 0.1),
}

def getKey():
    """
    Capture key press from the terminal.
    This function sets the terminal to raw mode to capture the key input without requiring Enter.
    """
    tty.setraw(sys.stdin.fileno())  # Set the terminal to raw mode
    select.select([sys.stdin], [], [], 0)  # Wait for input from stdin
    key = sys.stdin.read(1)  # Read one character
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)  # Restore terminal settings
    return key

# Initialize speed and turn factors
speed = 1.0  # Linear velocity factor
turn = 1.0   # Angular velocity factor

def vels(speed, turn):
    """
    Print the current speed and turn values.
    """
    return "currently:\tspeed %s\tturn %s " % (speed, turn)

if __name__ == "__main__":
    # Save terminal settings to restore later
    settings = termios.tcgetattr(sys.stdin)

    # Initialize ROS node and create publisher for cmd_vel topic
    rospy.init_node('n_turtle_keyboard')
    pub = rospy.Publisher('/N_Turtle_CMD_VEL', Twist, queue_size=1)

    # Initialize movement variables
    x = 0  # Linear velocity
    th = 0  # Angular velocity

    try:
        print(msg)  # Display control instructions
        print(vels(speed, turn))  # Display initial speed and turn values

        while not rospy.is_shutdown():
            key = getKey()  # Get the pressed key

            if key in moveBindings.keys():
                # Update linear and angular velocities based on key input
                x += moveBindings[key][0]
                th += moveBindings[key][1]
            elif key in ['w', 'x']:
                # Increase/decrease speed based on 'w' or 'x'
                speed = max(0.1, speed + (0.1 if key == 'w' else -0.1))
                print(vels(speed, turn))  # Print updated speed
            elif key in ['a', 'd']:
                # Increase/decrease turn rate based on 'a' or 'd'
                turn = max(0.1, turn + (0.1 if key == 'd' else -0.1))
                print(vels(speed, turn))  # Print updated turn rate
            else:
                # Stop the robot on any other key press
                x = 0
                th = 0
                if key == '\x03':  # If CTRL+C is pressed, exit the loop
                    break

            # Create and publish the Twist message
            twist = Twist()
            twist.linear.x = x * speed  # Set linear velocity
            twist.angular.z = th * turn  # Set angular velocity
            pub.publish(twist)  # Publish the Twist message

    except Exception as e:
        print(e)

    finally:
        # On exit, stop the robot by publishing zero velocities
        twist = Twist()
        twist.linear.x = 0
        twist.angular.z = 0
        pub.publish(twist)

        # Restore terminal settings
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
