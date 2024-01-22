from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    default_rviz = os.path.join(get_package_share_directory('gui'),
                                'rviz', 'spatialDetections.rviz')

    print(default_rviz)
    print("hallo")

    nnName = LaunchConfiguration('nnName', default = "SimpleFruitsv1iyolov5pytorch_openvino_2021.4_6shave.blob")

    declare_nnName_cmd = DeclareLaunchArgument(
        'nnName',
        default_value=nnName,
        description='Path to the object detection blob needed for detection')
    
    return LaunchDescription([
        Node(
            package='gui',
            executable='nicegui_node',
            name='example_gui',
            output='screen',
            parameters=[{'nnName': nnName},
                        {"hoi", "hoi"}],
        ),
    ])
