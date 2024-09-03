#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <iostream>

using namespace std;

// Action specification for move_base
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char **argv)
{

  // Connect to ROS
  ros::init(argc, argv, "N_TurtleGoal");

  // tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait for the action server to come up so that we can begin processing goals.
  while (!ac.waitForServer(ros::Duration(5.0)))
  {
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  float user_choice_x = 0;
  float user_choice_y = 0;

  char choice_to_continue = 'Y';
  bool run = true;

  while (run)
  {

    // Ask the user where he wants the robot to go?
    cout << "\nWhere do you want the robot to go?" << endl;
    cout << "\nEnter Your Robot X coordinates: " << endl;
    cin >> user_choice_x;
    cout << "\nEnter Your Robot y coordinates: " << endl;
    cin >> user_choice_y;

    // Create a new goal to send to move_base
    move_base_msgs::MoveBaseGoal goal;

    // Send a goal to the robot
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = user_choice_x;
    goal.target_pose.pose.position.y = user_choice_y;
    goal.target_pose.pose.orientation.w = 1.0;
    /*
    // Go back to beginning if the selection is invalid.
    if ((user_choice_x > 1) && (user_choice_x<9) && (user_choice_y >1) && (user_choice_y<9))
    {
      cout << "\nThe selection is invalid." << endl;
      continue;
    }
    */
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // Wait until the robot reaches the goal
    ac.waitForResult();

    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
      ROS_INFO("The robot has arrived at the goal location");
    else
      ROS_INFO("The robot failed to reach the goal location for some reason");

    // Ask the user if he wants to continue giving goals
    do
    {
      cout << "\nWould you like to go to another destination? (Y/N)" << endl;
      cin >> choice_to_continue;
      choice_to_continue = tolower(choice_to_continue); // Put your letter to its lower case
    } while (choice_to_continue != 'n' && choice_to_continue != 'y');

    if (choice_to_continue == 'n')
    {
      run = false;
    }
  }

  return 0;
}