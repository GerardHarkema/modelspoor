from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():

    # dit moet nog anders referentio t.o.v. package

    #urdf_launch_dir = os.path.join(get_package_share_directory('railtrack_ui'), 'launch')
    #print(urdf_launch_dir)

    config_file = '/home/gerard/modelspoor_ws/src/config/track_config.json'
    locomotive_images_path = '/home/gerard/modelspoor_ws/src/config/locomotive_images'

    return LaunchDescription([
        Node(
            package='railtrack_ui',
            executable='railtrack_ui.py',
            output='screen',
            parameters=[{'config_file': config_file},
                        {"locomotive_images_path": locomotive_images_path}
                        ],
        ),
    ])
