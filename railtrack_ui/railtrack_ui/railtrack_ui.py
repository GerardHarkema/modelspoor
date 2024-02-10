#!/usr/bin/env python3
import math
import threading
from pathlib import Path

import rclpy
import os
import json

from rclpy.executors import ExternalShutdownException
from rclpy.node import Node

from nicegui import Client, app, ui, ui_run, events
from functools import partial

from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSHistoryPolicy

from std_msgs.msg import Bool;
from railway_interfaces.msg import LocomotiveControl  
from railway_interfaces.msg import LocomotiveState  
from railway_interfaces.msg import TurnoutControl  
from railway_interfaces.msg import TurnoutState  

from turnout_control import turnout_control
from locomotive_control import locomotive_control

class rio_class():
    def __init__(self):
        self.xmin = 0;
        self.xmax = 0;
        self.ymin = 0;
        self.ymax = 0;

class point_class():
    def __init__(self):
        self.x = 0;
        self.y = 0;

class turnout_control_on_layout(Node):
    def __init__(self, turnout, image, turnout_control_publisher):
        self.image = image
        self.rios =[]
        self.red_contents = []
        self.green_contents = []
        self.status = False

        self.turnout_msg = TurnoutControl()
        self.turnout_msg.number = turnout["number"]
        self.control_publisher = turnout_control_publisher

        try:
            for layout_position in turnout["layout_positions"]:
                x = layout_position["position"]["x"]
                y = layout_position["position"]["y"]
                radius = layout_position["size"]["radius"]
                
                color = 'Red'
                #red_content = image_content()
                red_content = f'<circle cx="{x}" cy="{y}" r="{radius}" fill="none" stroke="{color}" stroke-width="4" />'
                self.red_contents.append(red_content)
                
                color = 'Green'
                #green_content = image_content()
                green_content = f'<circle cx="{x}" cy="{y}" r="{radius}" fill="none" stroke="{color}" stroke-width="4" />'
                self.green_contents.append(green_content)

                self.image.content += red_content
                
                rio = rio_class()
                rio.xmin = x - radius/2
                rio.xmax = x + radius/2
                rio.ymin = y - radius/2
                rio.ymax = y + radius/2
                self.rios.append(rio)
                #print("Added")
        except:
            #print("ERROR")
            pass
    
    def checkPointInRio(self, rio, point):
        if point.x >= rio.xmin and point.x <= rio.xmax:
            if point.y >= rio.ymin and point.y <= rio.ymax:
                return True
        return False
    
    def handleEvent(self, point):
        found = False
        for rio in self.rios:
            found = self.checkPointInRio(rio, point)
            if found:
                break
        if not found:
            return
        self.turnout_msg.state = not self.status
        self.control_publisher.publish(self.turnout_msg)
        notify_text = "Set Turnout " + str(self.turnout_msg.number)
        if(self.turnout_msg.state):
            notify_text = notify_text + ": Green"
        else:
            notify_text = notify_text + ": Red"
        ui.notify(notify_text)

    def set_status_indicator(self, status):

        if(self.turnout_msg.number == status.number):
            if status.state:
                for content in self.green_contents:
                    self.image.content += content
            else:
                for content in self.red_contents:
                    self.image.content += content 
        pass

class railtracklayout_control(Node):
    def __init__(self, turnouts, railtracklayout_image_filemage, turnout_control_publisher):
        self.turnout_control_publisher = turnout_control_publisher;
        self.turnouts = []
        self.layout = ui.interactive_image(railtracklayout_image_filemage, on_mouse=self.mouse_handler, events=['mousedown', 'mouseup'], cross=True)
        for turnout in turnouts["c-type"]:
            tc = turnout_control_on_layout(turnout, self.layout, self.turnout_control_publisher)
            self.turnouts.append(tc)
        for turnout in turnouts["m-type"]:
            tc = turnout_control_on_layout(turnout, self.layout, self.turnout_control_publisher)
            self.turnouts.append(tc)
        self.notify_mouse_events = False
        self.switch = ui.switch('Show mouse click locations').bind_value(self, 'notify_mouse_events')    

    def mouse_handler(self, e: events.MouseEventArguments):
        if e.type == 'mousedown':
            for turnout in self.turnouts:
                point = point_class()
                point.x = e.image_x
                point.y = e.image_y
                turnout.handleEvent(point)
            if self.notify_mouse_events:
                ui.notify(f'{e.type} at ({e.image_x:.1f}, {e.image_y:.1f})')
                
    def set_status_indicator(self, status):
        for turnout in self.turnouts:
            turnout.set_status_indicator(status)



class RailTrackNode(Node):

    def __init__(self) -> None:
        super().__init__('railtrack_gui')
        
        self.declare_parameter("config_file", "");
        self.config_file = self.get_parameter("config_file").get_parameter_value().string_value
        self.declare_parameter("locomotive_images_path", "");
        self.locomotive_images_path = self.get_parameter("locomotive_images_path").get_parameter_value().string_value
        self.declare_parameter("railtracklayout_images_path", "");
        self.railtracklayout_images_path = self.get_parameter("railtracklayout_images_path").get_parameter_value().string_value

        with open(self.config_file, 'r', encoding='utf-8') as f:
            self.track_config = json.load(f)

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
        self.turnouts = []
        with Client.auto_index_client:
            with ui.tabs().classes('w-full') as tabs:
                locomotives_tab = ui.tab('Locomotives')
                turnouts_tab = ui.tab('Turnouts')
                tracklayouts_tab = ui.tab('Track Layout')
            with ui.tab_panels(tabs, value=turnouts_tab).classes('w-full'):
                with ui.tab_panel(turnouts_tab):
                    with ui.grid(columns=3):

                        for turnout in self.track_config["Turnouts"]["c-type"]:
                            self.turnouts.append(turnout["number"])   
                        for turnout in self.track_config["Turnouts"]["m-type"]:
                            self.turnouts.append(turnout["number"])   
                        self.turnouts.sort()
                        for turnout in self.turnouts:
                            tc = turnout_control(turnout, self.turnout_control_publisher)
                            self.turnoutsui.append(tc)
                with ui.tab_panel(locomotives_tab):
                    with ui.grid(columns=3):
                        for loc in self.track_config['Locomotives']:
                            locomotive = locomotive_control(loc, self.locomotive_control_publisher, self.locomotive_images_path)
                            self.locomotivesui.append(locomotive)
                with ui.tab_panel(tracklayouts_tab):
                    railtracklayout_image_file = self.railtracklayout_images_path + "/"+ self.track_config["railtrack_layout_image"]
                    self.track_control = railtracklayout_control(self.track_config["Turnouts"], railtracklayout_image_file, self.turnout_control_publisher)
                    pass
            self.power_button = ui.button('STOP', on_click=lambda:self.power()).classes('drop-shadow bg-red')
            self.active = ui.icon('fiber_manual_record', size='3em').classes('drop-shadow text-green')
            self.active_status = False;

        self.power_state = False

    def turnout_status_callback(self, status):
        self.track_control.set_status_indicator(status)
        for turnout in self.turnoutsui:
            turnout.set_status_indicator(status)

    def locomotive_status_callback(self, status):
        #print(status)
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
        if(self.active_status):
            self.active.classes('text-green', remove='text-red')
            self.active_status = False
        else:
            self.active.classes('text-red', remove='text-green')
            self.active_status = True


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

    node = RailTrackNode()
    try:
        rclpy.spin(node)
    except ExternalShutdownException:
        pass


app.on_startup(lambda: threading.Thread(target=ros_main).start())
ui_run.APP_IMPORT_STRING = f'{__name__}:app'  # ROS2 uses a non-standard module name, so we need to specify it here
ui.run(uvicorn_reload_dirs=str(Path(__file__).parent.resolve()), favicon='🤖')
ui.run(title='Dorst central station')


