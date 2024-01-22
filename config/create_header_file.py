import json

with open('track_config.json', 'r', encoding='utf-8') as f:
    track_config = json.load(f)

#print(track_config)


header = "// !!! This is an automated generated header file, do not modify by your self !!!\n"
line = "#ifndef _TRACK_CONFIG_\n"
header = header + line
line = "#define _TRACK_CONFIG_\n"
header = header + line + "\n"

#LOCOMOTIVE active_locomotives[] = {{10, MFX, 0},{5, DCC, 0}};

locomotives = track_config["Locomotives"]
print(locomotives)

line = "LOCOMOTIVE active_locomotives[] = {"
for i in range(len(locomotives) - 1):
    line = line + "{" + str(locomotives[i]["address"]) + ', '
    line = line + str(locomotives[i]["protocol"]) + ', '
    line = line + "0},"

line = line + "{" + str(locomotives[len(locomotives) - 1]["address"]) + ', '
line = line + str(locomotives[len(locomotives) - 1]["protocol"]) + ', '
line = line + " 0}};\n"
header = header + line
line = "#define  NUMBER_OF_ACTIVE_LOCOMOTIVES   " + str(len(locomotives)) + "\n"
header = header + line + "\n"

turnouts = track_config["Turnouts"]["c-type"]
line = "unsigned short int active_turnouts_c[] = {"
for i in range(len(turnouts)-1):
    line = line + str(turnouts[i]) + ", "
line  = line + str(turnouts[len(turnouts)-1]) + "};\n"
header = header + line
line = "#define  NUMBER_OF_ACTIVE_TURNOUTS_C   " + str(len(turnouts)) + "\n"
header = header + line  + "\n"

turnouts = track_config["Turnouts"]["m-type"]
line = "unsigned short int active_turnouts_m[] = {"
for i in range(len(turnouts)-1):
    line = line + str(turnouts[i]) + ", "
line  = line + str(turnouts[len(turnouts)-1]) + "};\n"
header = header + line
line = "#define  NUMBER_OF_ACTIVE_TURNOUTS_M   " + str(len(turnouts)) + "\n"
header = header + line + "\n"

line = "#endif //_TRACK_CONFIG_\n"
header = header + line

#print(header)

with open('/home/gerard/modelspoor_ws/src/esp32/ros2_marklin_canbus_controller/include/track_config.h', 'w', encoding='utf-8') as f:
    f.write(header)

