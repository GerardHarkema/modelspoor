import math
import threading
from pathlib import Path

import rclpy
import os

from std_msgs.msg import Bool
from rclpy.executors import ExternalShutdownException
from rclpy.node import Node

from nicegui import Client, app, ui, ui_run
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy

from railway_interfaces.msg import LocomotiveControl  
from railway_interfaces.msg import TurnoutControl  

turnouts = [10, 11, 12, 13, 14, 15, 32]

stoomtrein  = {
    "name" : "Stoomtrein",
    "type": "Stoomtrein",
    "image": "stoom_loc.jpg",
    "protocol": "MFX",
    "address": 10
}

NS121 = {
    "name" : "NS1212",
    "type": "Electrischetrein",
    "image": "NS1212.jpg",
    "protocol": "DCC",
    "address": 12
}

dieseltrein = {
    "name" : "Dieseltrein",
    "type": "Dieseltrein",
    "image": "diesel_loc.jpg",
    "protocol": "DCC",
    "address": 13
}

locomotives =[stoomtrein, NS121, dieseltrein]

class turnout_control(Node):


    def __init__(self, turnout_number):
        super().__init__('nicegui')

        self.turnout_msg = TurnoutControl()
        self.turnout_msg.number = turnout_number

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

class locomotive_control(Node):

    def __init__(self, locomotive_descr):
        super().__init__('nicegui')

        self.locomotive_msg = LocomotiveControl()
        self.locomotive_msg.name = locomotive_descr['name']
        self.locomotive_msg.address = locomotive_descr['address']
        self.locomotive_msg.protocol = locomotive_descr['protocol']
        with ui.card():
            text = 'Locomotive: ' + str(locomotive_descr['type'])
            ui.label(text)
            image = "/home/gerard/modelspoor_ws/src/nicegui/gui/gui/locomotive_images/"+ locomotive_descr["image"]
            #image = os.getcwd() + "/"+ locomotive_descr["image"]
            #ui.label(os.getcwd())

            ui.image(image).classes('w-64')
            with ui.grid(columns=4):
                self.reverse = ui.button('Reverse')#, on_click=lambda: self.set_turnout(True)).classes('drop-shadow bg-red')
                self.forward = ui.button('Forward')#, on_click=lambda: self.set_turnout(False)).classes('drop-shadow bg-green')
                self.slider = ui.slider(min=0, max=100, value=50)
                ui.label().bind_text_from(self.slider, 'value')
        
        self.qos_profile = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1)
        
        topic = "/locomotive/control"
        self.control_publisher = self.create_publisher(LocomotiveControl, topic, 1)



'''                
        topic = "/locomotive" + str(self.turnout_number) + "/status"
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
        #for topic in topic_list:
        #    print(topic[0])

        self.get_logger().debug(topic_list)

        self.turnoutsui= []
        self.locomotivesui = []
        with Client.auto_index_client:
            with ui.tabs().classes('w-full') as tabs:
                locomotives_tab = ui.tab('Locomotives')
                turnouts_tab = ui.tab('Turnouts')
            with ui.tab_panels(tabs, value=locomotives_tab).classes('w-full'):
                with ui.tab_panel(turnouts_tab):
                    for turnout in turnouts:
                        wc = turnout_control(turnout)
                        self.turnoutsui.append(wc)
                with ui.tab_panel(locomotives_tab):
                    for loc in locomotives:
                        locomotive = locomotive_control(loc)
                        self.locomotivesui.append(locomotive)
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
