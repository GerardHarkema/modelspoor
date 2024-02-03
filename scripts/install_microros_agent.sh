#!/bin/bash

sudo apt install -y python3-colcon-common-extensions
sudo apt-get install -y python3-rosdep
sudo rosdep init
rosdep update
sudo -H apt-get install -y clang

# Source the ROS 2 installation
source /opt/ros/$ROS_DISTRO/setup.bash

cd $HOME

# Create a workspace and download the micro-ROS tools
mkdir microros_ws
cd microros_ws
git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup

# Update dependencies using rosdep
sudo apt update && rosdep update
rosdep install --from-paths src --ignore-src -y

# Install pip
sudo apt-get install python3-pip

# Build micro-ROS tools and source them
colcon build
source install/local_setup.bash

echo "source ~/microros_ws/install/local_setup.bash" >> $HOME/.bashrc

ros2 run micro_ros_setup build_firmware.sh
source install/local_setup.bash

# Download micro-ROS-Agent packages
ros2 run micro_ros_setup create_agent_ws.sh

# Build step
ros2 run micro_ros_setup build_agent.sh
source install/local_setup.bash

