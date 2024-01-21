import math
import threading
from pathlib import Path

import rclpy
import os

from rclpy.executors import ExternalShutdownException
from rclpy.node import Node

from nicegui import Client, app, ui, ui_run
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy


from std_msgs.msg import Bool;
from railway_interfaces.msg import LocomotiveControl  
from railway_interfaces.msg import LocomotiveState  
from railway_interfaces.msg import TurnoutControl  
from railway_interfaces.msg import TurnoutState  

turnouts = [1, 2, 3, 4, 10, 11, 12, 13, 14, 15, 16]

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

    def __init__(self, turnout_number, control_publisher):
        self.turnout_msg = TurnoutControl()
        self.turnout_msg.number = turnout_number
        self.control_publisher = control_publisher

        self.turnout_number = turnout_number
        with ui.card():
            text = 'Turnout ' + str(self.turnout_number)
            ui.label(text)

            with ui.grid(columns=3):
                self.green_button = ui.button('Green', on_click=lambda: self.set_turnout(True)).classes('drop-shadow bg-green')
                self.rood_button = ui.button('Red', on_click=lambda: self.set_turnout(False)).classes('drop-shadow bg-red')
                self.led = ui.icon('fiber_manual_record', size='3em').classes('drop-shadow text-green')

    def set_turnout(self, control) -> None:
        self.turnout_msg.state = control
        self.control_publisher.publish(self.turnout_msg)
        notify_text = "Set Turnout " + str(self.turnout_msg.number)
        if(control):
            notify_text = notify_text + ": Green"
        else:
            notify_text = notify_text + ": Red"
        ui.notify(notify_text)
    def set_status_indicator(self, status) -> None:
        #print("set_status_indicator")
        if(self.turnout_msg.number == status.number):
            if status.state:
                #print("set green")
                self.led.classes('text-green', remove='text-red')
                text = 'Set turnout ' + str(self.turnout_number)+ ": green" 
            else:
                #print("set red")
                self.led.classes('text-red', remove='text-green')
                text = 'Set turnout ' + str(self.turnout_number) + ": red" 

class locomotive_control(Node):

    def __init__(self, locomotive_descr, control_publisher):
        #super().__init__('nicegui')

        self.locomotive_descr = locomotive_descr
        self.control_publisher = control_publisher;
        self.locomotive_msg = LocomotiveControl()
        match locomotive_descr['protocol']:
            case "MM1":
                self.locomotive_msg.address = locomotive_descr['address']
                self.number_of_functions = 4
            case "MM2":
                self.locomotive_msg.address = locomotive_descr['address']
                self.number_of_functions = 4
            case "DCC":
                self.locomotive_msg.address = locomotive_descr['address'] + 0xC000
                self.number_of_functions = 16
            case "MFX":
                self.locomotive_msg.address = locomotive_descr['address'] + 0x4000
                self.number_of_functions = 32
            case _:
                pass

        #self.locomotive_msg.name = locomotive_descr['name']
        #self.locomotive_msg.address = locomotive_descr['address']
        #self.locomotive_msg.protocol = locomotive_descr['protocol']
        with ui.card():
            text = 'Locomotive: ' + str(locomotive_descr['type'])
            ui.label(text)
            image = "/home/gerard/modelspoor_ws/src/nicegui/gui/gui/locomotive_images/"+ locomotive_descr["image"]
            #image = os.getcwd() + "/"+ locomotive_descr["image"]
            #ui.label(os.getcwd())

            ui.image(image).classes('w-64')
            with ui.grid(columns=4):
                self.direction_button = ui.button('FORWARD', on_click=lambda:self.set_direction()).classes('drop-shadow bg-red')
                self.speed_slider = ui.slider(min=0, max=1000, value=50, on_change=lambda:self.set_speed())
                self.speed_slider.on(type = 'update:model-valuex', leading_events = False, trailing_events = False, throttle = 5.0) 
            
            with ui.dialog() as dialog, ui.card():
                ui.label('Functions')
                self.function_buttons = []
                with ui.grid(columns=4):
                    for i in range(self.number_of_functions):
                        text = "F" + str(i)
                        self.function_buttons.append(ui.button(text, on_click=lambda:self.set_function()))
                ui.button('Close', on_click=dialog.close)
            ui.button('Functions', on_click=dialog.open)
    
        self.qos_profile = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1)

    def set_speed(self):
        self.locomotive_msg.command = LocomotiveControl().__class__.SET_SPEED
        self.locomotive_msg.speed = self.speed_slider.value
        self.control_publisher.publish(self.locomotive_msg)
        notify_text = "Set Speed, Protocol: " + self.locomotive_descr['protocol'] \
                    + ", Loc ID: " + str(self.locomotive_descr['address'])         \
                    + ", Speed: " + str(self.speed_slider.value)
        ui.notify(notify_text)                 
        pass

    def set_direction(self):
        print(self.locomotive_msg)
        self.locomotive_msg.command = LocomotiveControl().__class__.SET_DIRECTION
        self.locomotive_msg.speed = 0
        self.speed_slider.value = 0
        if(self.direction_button.text == 'FORWARD'):
            self.direction_button.text ='REVERSE'
            self.locomotive_msg.direction = LocomotiveControl().__class__.DIRECTION_FORWARD
        else:
            self.direction_button.text ='FORWARD'
            self.locomotive_msg.direction = LocomotiveControl().__class__.DIRECTION_REVERSE
        self.control_publisher.publish(self.locomotive_msg)
        notify_text = "Set Direction, Protocol: " + self.locomotive_descr['protocol'] \
            + ", Loc ID: " + str(self.locomotive_descr['address'])         \
            + ", Direction: " + self.direction_button.text
        ui.notify(notify_text)
        pass

    def set_function(self):
        ui.notify("Set Function")
        pass

    def set_status(self, status) -> None:
        #print("set_status_indicator")
        if(self.locomotive_msg.address == status.address):
            pass

class NiceGuiNode(Node):

    def __init__(self) -> None:
        super().__init__('nicegui')
        
        topic_list = self.get_topic_names_and_types()
        #for topic in topic_list:
        #    print(topic[0])

        self.get_logger().debug(topic_list)

        self.qos_profile = QoSProfile(
            reliability=QoSReliabilityPolicy.BEST_EFFORT,
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=1)

        topic = "/railtrack/turnout/status"
        self.turnout_status_subscription = self.create_subscription(TurnoutState, topic, self.turnout_status_callback, qos_profile=self.qos_profile)

        topic = "/railtrack/turnout/control"
        self.turnout_control_publisher = self.create_publisher(TurnoutControl, topic, 1)

        topic = "/railtrack/locomotive/status"
        self.locomotive_status_subscription = self.create_subscription(LocomotiveState, topic, self.locomotive_status_callback, qos_profile=self.qos_profile)

        topic = "/railtrack/locomotive/control"
        self.locomotive_control_publisher = self.create_publisher(LocomotiveControl, topic, 1)

        topic = "/railtrack/power_status"
        self.power_status_subscription = self.create_subscription(Bool, topic,  self.power_status_callback, qos_profile=self.qos_profile)

        topic = "/railtrack/power_control"
        self.power_control_publisher = self.create_publisher(Bool, topic,  1)

        self.turnoutsui= []
        self.locomotivesui = []
        with Client.auto_index_client:
            with ui.tabs().classes('w-full') as tabs:
                locomotives_tab = ui.tab('Locomotives')
                turnouts_tab = ui.tab('Turnouts')
            with ui.tab_panels(tabs, value=turnouts_tab).classes('w-full'):
                with ui.tab_panel(turnouts_tab):
                    with ui.grid(columns=3):
                        for turnout in turnouts:
                            tc = turnout_control(turnout, self.turnout_control_publisher)
                            self.turnoutsui.append(tc)
                with ui.tab_panel(locomotives_tab):
                    with ui.grid(columns=3):
                        for loc in locomotives:
                            locomotive = locomotive_control(loc, self.locomotive_control_publisher)
                            self.locomotivesui.append(locomotive)
            self.power_button = ui.button('STOP', on_click=lambda:self.power()).classes('drop-shadow bg-red')
        self.power_state = False

    def turnout_status_callback(self, status):
        for turnout in self.turnoutsui:
            turnout.set_status_indicator(status)

    def locomotive_status_callback(self, status):
        for loc in self.locomotivesui:
            loc.set_status(status)

    def power_status_callback(self, power):
        if power.data:
            self.power_state = True
            self.power_button.classes('drop-shadow bg-red')
            self.power_button.text = 'STOP'
        else:
            self.power_state = False
            self.power_button.classes('drop-shadow bg-green') 
            self.power_button.text = 'ENABLE'

        #print("power_callback")
        pass

    def power(self):
        #ui.notify(self.power_button.text)
        msg = Bool()
        if(self.power_button.text == 'STOP'):
            self.power_button.classes('drop-shadow bg-green') 
            self.power_button.text = 'ENABLE'
            msg.data = False
            self.power_state = False
        else:
            self.power_button.classes('drop-shadow bg-red')
            self.power_button.text = 'STOP'
            msg.data = True
            self.power_state = True
        self.power_control_publisher.publish(msg)  
        notify_text = "Power "
        if(self.power_state):
            notify_text = notify_text + ": Enable"
        else:
            notify_text = notify_text + ": Disable"
        ui.notify(notify_text)
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
