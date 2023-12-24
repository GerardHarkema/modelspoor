import math
import threading
from pathlib import Path

import rclpy

from std_msgs.msg import Bool
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node

from nicegui import Client, app, ui, ui_run
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy

turnouts = [10, 11, 12, 13, 14, 15, 32]

locs =["stoom_loc.jpg", "NS1212.jpg", "diesel_loc.jpg"]

class turnout_control(Node):


    def __init__(self, turnout_number):
        super().__init__('nicegui')

        self.turnout_number = turnout_number
        with ui.card():
            text = 'Turnout ' + str(self.turnout_number)
            ui.label(text)

            with ui.grid(columns=3):
                self.green_button = ui.button('Green', on_click=lambda: self.set_turnout(False)).classes('drop-shadow bg-green')
                self.rood_button = ui.button('Red', on_click=lambda: self.set_turnout(True)).classes('drop-shadow bg-red')

                self.led = ui.icon('fiber_manual_record', size='3em').classes('drop-shadow text-green')

        self.qos_profile = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1)
        topic = "/turnout" + str(self.turnout_number) + "/control"
        self.control_publisher = self.create_publisher(Bool, topic, 1)
        topic = "/turnout" + str(self.turnout_number) + "/status"
        self.subscription = self.create_subscription(Bool, topic, self.status_callback, qos_profile=self.qos_profile)

    def set_turnout(self, control) -> None:
        
        msg = Bool()
        msg.data = control
        self.control_publisher.publish(msg)
        if control:
            self.led.classes('text-red', remove='text-green')
            text = 'Set turnout ' + str(self.turnout_number) + ": red" 
        else:
            self.led.classes('text-green', remove='text-red')
            text = 'Set turnout ' + str(self.turnout_number)+ ": green" 
        ui.notify(text)

    def status_callback(self, msg: Bool) -> None:
        if msg.data:
            self.text.set_text("Rood" )
            self.led.classes('text-green', remove='text-red')
        else:
            self.text.set_text("Groen" )
            self.led.classes('text-red', remove='text-green')    

class train_control(Node):


    def __init__(self, train_name):
        super().__init__('nicegui')


        self.train_name = train_name
        with ui.card():
            text = 'Train ' + str(self.train_name)
            image = "nicegui/gui/train_images/"+ str(self.train_name)
            ui.label(image)
            ui.image(image).classes('w-16')
            with ui.grid(columns=4):
                self.forward = ui.button('Forward')#, on_click=lambda: self.set_turnout(False)).classes('drop-shadow bg-green')
                self.reverse = ui.button('Reverse')#, on_click=lambda: self.set_turnout(True)).classes('drop-shadow bg-red')
                self.slider = ui.slider(min=0, max=100, value=50)
                ui.label().bind_text_from(self.slider, 'value')



'''                
        self.qos_profile = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1)
        topic = "/train" + str(self.turnout_number) + "/control"
        self.control_publisher = self.create_publisher(Bool, topic, 1)
        topic = "/train" + str(self.turnout_number) + "/status"
        self.subscription = self.create_subscription(Bool, topic, self.status_callback, qos_profile=self.qos_profile)

    def set_turnout(self, control) -> None:
        
        msg = Bool()
        msg.data = control
        self.control_publisher.publish(msg)
        if control:
            self.led.classes('text-red', remove='text-green')
            text = 'Set turnout ' + str(self.turnout_number) + ": red" 
        else:
            self.led.classes('text-green', remove='text-red')
            text = 'Set turnout ' + str(self.turnout_number)+ ": green" 
        ui.notify(text)

    def status_callback(self, msg: Bool) -> None:
        if msg.data:
            self.text.set_text("Rood" )
            self.led.classes('text-green', remove='text-red')
        else:
            self.text.set_text("Groen" )
            self.led.classes('text-red', remove='text-green')    
'''

class NiceGuiNode(Node):

    def __init__(self) -> None:
        super().__init__('nicegui')
        
        topic_list = self.get_topic_names_and_types()
        for topic in topic_list:
            print(topic[0])

        self.get_logger().debug(topic_list)

        self.turnoutsui= []
        self.trainsui = []
        with Client.auto_index_client:
            with ui.tabs().classes('w-full') as tabs:
                trains_tab = ui.tab('Trains')
                turnouts_tab = ui.tab('Turnouts')
            with ui.tab_panels(tabs, value=trains_tab).classes('w-full'):
                with ui.tab_panel(turnouts_tab):
                    for turnout in turnouts:
                        wc = turnout_control(turnout)
                        self.turnoutsui.append(wc)
                with ui.tab_panel(trains_tab):
                    for loc in locs:
                        train = train_control(loc)
                        self.trainsui.append(train)
            self.stop_btn = ui.button('STOP', on_click=lambda: self.stop()).classes('drop-shadow bg-red')
    
    def stop(self):
        pass


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
ui.run(title='Dorst central station')
