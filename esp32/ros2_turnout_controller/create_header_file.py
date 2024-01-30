import json
from datetime import datetime

try:
    import easygui                    
except ImportError:
    Import("env")
    # Install custom packages from the PyPi registry
    env.Execute("$PYTHONEXE -m pip install easygui")
    import easygui

def main():
    agent_config_file = '../../config/micro_ros_agent_config.json'
    with open(agent_config_file, 'r', encoding='utf-8') as f:
        agent_config = json.load(f)

    header = "// !!! This is an automated generated header file, do not modify by your self !!!\n"

    # datetime object containing current date and time
    now = datetime.now()
    line = "// Timestamp: " + now.strftime("%d/%m/%Y %H:%M:%S") + "\n"
    header = header + line
    line = "#ifndef _TURNOUT_CONFIG_\n"
    header = header + line
    line = "#define _TURNOUT_CONFIG_\n"
    header = header + line + "\n"

    line = "// Agent config generated from: " + agent_config_file + "\n"
    header = header + line
    line = "#define SSID   \"" + agent_config['wifi']['ssid'] + "\"\n"
    header = header + line
    line = "#define PASSWORD   \"" + agent_config['wifi']['password'] + "\"\n"
    header = header + line

    ip_address = agent_config['agent']['ip']
    ip_segements = ip_address.split(":")
    line = "uint8_t ip_address[4] = {" + ip_segements[0] + ", "\
                                    + ip_segements[1] + ", "\
                                    + ip_segements[2] + ", "\
                                    + ip_segements[3] + "};\n"
    header = header + line

    line = "#define PORT   " + str(agent_config['agent']['port']) + "\n\n"
    header = header + line

    load_title = "Load turnout config from..."
    turnout_config_file = easygui.fileopenbox(title=load_title, default="../../config/*.json")
    if turnout_config_file is not None:
        with open(turnout_config_file, 'r', encoding='utf-8') as f:
            turnout_config = json.load(f)
    else:
        easygui.msgbox('Turnout controller will be compiled with previous turnout_config.h', 'Warning')
        return

    #turnout_config_file = '../../config/turnout_config_a.json'
    line = "// Turnout config generated from: " + turnout_config_file + "\n"
    header = header + line

    turnouts = turnout_config["Turnouts"]
    line = "TURNOUT_CONFIG turnout_config[] = {"
    for i in range(len(turnouts) - 1):
        line = line + "{" + str(turnouts[i]["number"]) + ', '
        line = line + str(turnouts[i]["red_pin"]) + ', '
        line = line + str(turnouts[i]["green_pin"])+ "},"

    line = line + "{" + str(turnouts[len(turnouts) - 1]["number"]) + ', '
    line = line + str(turnouts[len(turnouts) - 1]["red_pin"]) + ', '
    line = line + str(turnouts[len(turnouts) - 1]["green_pin"]) + "}};\n"
    header = header + line
    line = "#define  NUMBER_OF_TURNOUTS  " + str(len(turnouts)) + "\n"
    header = header + line + "\n"

    line = "#endif //_TURNOUT_CONFIG_\n"
    header = header + line

    with open('include/turnout_config.h', 'w', encoding='utf-8') as f:
        f.write(header)

main()