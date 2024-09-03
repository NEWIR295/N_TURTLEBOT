#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <iostream>

using namespace std;

// Typedef for convenience - defining the MoveBaseClient as a SimpleActionClient for move_base
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char **argv)
{
    // Initialize the ROS node with the name "N_TurtleGoal"
    ros::init(argc, argv, "N_TurtleGoal_Real");

    // Instantiate the action client and tell it to spin a thread by default
    MoveBaseClient ac("move_base", true);

    // Wait for the move_base action server to start
    while (!ac.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting for the move_base action server to start...");
    }

    // Loop for sending goals to the robot
    char choice_to_continue = 'y';  // Changed to lowercase for consistency
    bool flag = true;

    while (flag)
    {
        // Ask the user to select a goal location
        int user_choice;
        cout << "\nSelect a goal location:" << endl;
        cout << "1 = Start" << endl;
        cout << "2 = Middle" << endl;
        cout << "3 = End" << endl;
        cout << "Enter a number: ";
        cin >> user_choice;

        // Create a new goal to send to move_base
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();

        // Determine the goal location based on user input
        bool valid_selection = true;
        switch (user_choice)
        {
        case 1:
            cout << "\nGoal Location: Start\n";
            goal.target_pose.pose.position.x = 10.0;
            goal.target_pose.pose.position.y = 3.7;
            goal.target_pose.pose.orientation.w = 1.0;
            break;
        case 2:
            cout << "\nGoal Location: Middle\n";
            goal.target_pose.pose.position.x = 8.1;
            goal.target_pose.pose.position.y = 4.3;
            goal.target_pose.pose.orientation.w = 1.0;
            break;
        case 3:
            cout << "\nGoal Location: End\n";
            goal.target_pose.pose.position.x = 10.5;
            goal.target_pose.pose.position.y = 2.0;
            goal.target_pose.pose.orientation.w = 1.0;
            break;
        default:
            cout << "\nInvalid selection. Please try again.\n";
            valid_selection = false;
        }

        // If the selection is invalid, skip sending the goal
        if (!valid_selection)
        {
            continue;
        }

        // Send the goal to the action server
        ROS_INFO("Sending goal...");
        ac.sendGoal(goal);

        // Wait for the result
        ac.waitForResult();

        // Check if the robot succeeded in reaching the goal
        if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        {
            ROS_INFO("The robot has arrived at the goal location");
        }
        else
        {
            ROS_INFO("The robot failed to reach the goal location");
        }

        // Ask the user if they want to send another goal
        do
        {
            cout << "\nWould you like to send another goal? (y/n): ";
            cin >> choice_to_continue;
            choice_to_continue = tolower(choice_to_continue); // Convert to lowercase
        } while (choice_to_continue != 'n' && choice_to_continue != 'y');

        // Exit loop if the user chooses 'n'
        if (choice_to_continue == 'n')
        {
            flag = false;
        }
    }

    return 0;
}
