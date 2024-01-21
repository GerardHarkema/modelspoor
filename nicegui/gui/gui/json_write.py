import json

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
trains = { "locomotives": [
                {
                    "name" : "Stoomtrein",
                    "type": "Stoomtrein",
                    "image": "stoom_loc.jpg",
                    "protocol": "MFX",
                    "address": 10
                },
                {
                    "name" : "NS1212",
                    "type": "Electrischetrein",
                    "image": "NS1212.jpg",
                    "protocol": "DCC",
                    "address": 12
                }],
            "Turnouts" :[1, 2, 3, 4, 10, 11, 12, 13, 14, 15, 16]
         }



# convert into JSON:
y = json.dumps(trains, indent=4)
# the result is a JSON string:
print(y)


locom = trains["locomotives"]
for loc in locom:
    print(loc)
turnouts = trains["Turnouts"]
for turn in turnouts:
    print(turn)

