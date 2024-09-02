# N_TURTLEBOT


Final Project for Advanced Robotics and Artificial Intelligence  
Faculty of Engineering, Cairo University

## Project Overview

The **N_TURTLEBOT** project consists of two main components:

1. **N_TURTLE_HW_CTRL**: A folder for hardware control, which includes a ROS node implemented on an Arduino. This node interfaces with:
   - Two motors with encoders
   - An IMU sensor

2. **n_turtle_ws**: A ROS workspace containing five packages to manage different aspects of the robot's functionality and simulation.

## ROS Workspace Packages

### 1. **n_turtle_ctrl**
- Responsible for communication and control of the robot.
- Contains the following key files:
  - **`n_turtle_ctrl.launch`**: Launches nodes that handle communication with both the main machine and the microcontroller.
  - **`n_turtle_keyboard` node**: Allows keyboard control of the robot.

### 2. **n_turtle_description**
- Contains the URDF file for the robot model.
- To visualize the robot in **Rviz**, use the:
  - **`display.launch`** file.

### 3. **n_turtle_gazebo**
- Contains the simulated environment for the robot.
- To visualize the simulation in **Gazebo**, use the:
  - **`n_turtle_gazebo.launch`** file.

### 4. **n_turtle_slam**
- Applies the **GMapping SLAM algorithm** to generate maps using LiDAR.
- Launch files:
  - **`n_turtle_slam.launch`**: For use with hardware.
  - **`n_turtle_slam_sim.launch`**: For use with simulation.

### 5. **n_turtle_nav**
- Implements navigation and localization using **AMCL** and **Move Base** packages.
- Launch files:
  - **`n_turtle_nav.launch`**: For use with hardware.
  - **`n_turtle_nav_sim.launch`**: For use with simulation.

## Getting Started

1. Clone the repository:
   ```bash
   git clone https://github.com/NEWIR295/N_TURTLEBOT.git

2. Navigate to the ROS workspace:
   ```bash
   cd N_TURTLEBOT/n_turtle_ws

3. Build the workspace:
   ```bash
   catkin_make
   source devel/setup.bash

4. Launch the desired package according to your needs (e.g., visualization, control, SLAM, navigation).

## Usage

- **Hardware Control**:  
  Ensure the Arduino is connected and running the ROS node from `N_TURTLE_HW_CTRL`.

- **Simulated Environment**:  
  Use `n_turtle_gazebo` to launch the Gazebo environment for testing in simulation.

- **SLAM and Navigation**:  
  Use appropriate launch files from `n_turtle_slam` and `n_turtle_nav` for mapping and navigation, respectively.

## Contributors

- Mohamed Newir - Project Lead  
=======

