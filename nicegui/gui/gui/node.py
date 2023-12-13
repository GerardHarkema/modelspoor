import math
import threading
from pathlib import Path

import rclpy

from std_msgs.msg import Bool
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node

from nicegui import Client, app, ui, ui_run
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy

class NiceGuiNode(Node):

    #self.rood = 'rood'
    #self.groen = 'groen'

    def __init__(self) -> None:
        super().__init__('nicegui')
        
        topic_list = self.get_topic_names_and_types()
        for topic in topic_list:
            print(topic[0])

        self.get_logger().debug(topic_list)
        
        qos_profile = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1
        )
        self.control_publisher = self.create_publisher(Bool, '/wissel10/control', 1)
        self.subscription = self.create_subscription(Bool, '/wissel10/status', self.handel_status, qos_profile=qos_profile)

        with Client.auto_index_client:
            self.rood_button = ui.button('Rood', on_click=lambda: self.set_wissel(True))
            self.green_button = ui.button('Groen', on_click=lambda: self.set_wissel(False))
            self.text = ui.label('some label')
    
    def set_wissel(self, control) -> None:
        ui.notify('Set wissel')
        msg = Bool()
        msg.data = control
        self.control_publisher.publish(msg)

    def handel_status(self, msg: Bool) -> None:
        if msg.data:
            self.text.set_text("Rood" )
        else:
            self.text.set_text("Groen" )

        #print(msg)

def main() -> None:
    # NOTE: This function is defined as the ROS entry point in setup.py, but it's empty to enable NiceGUI auto-reloading
    pass


def ros_main() -> None:
    rclpy.init()
    node = NiceGuiNode()
    try:
        rclpy.spin(node)
    except ExternalShutdownException:
        pass


app.on_startup(lambda: threading.Thread(target=ros_main).start())
ui_run.APP_IMPORT_STRING = f'{__name__}:app'  # ROS2 uses a non-standard module name, so we need to specify it here
ui.run(uvicorn_reload_dirs=str(Path(__file__).parent.resolve()), favicon='🤖')
