# ESP8266_MQTT_Lamp
Instructions and files to build a lamp which can be controlled by an ESP8266 board

The project is based around MQTT protocol which allows client devices to connect to a broker and send/recieve information without having to hardcode each device. This means many new devices can easily be added to your home system and communicate effortlessly

The Publisher will be a switch that sends either a 1 or a 0 to the topic **home/room/strip** and the Reciever subscribes to the same topic and turns on the light when it recieves a 1, turns of the light when it recieves a 0 aswell as publishing to the topic **home/room/confirm** what the state of the light currently is

The broker is my phone which can be made possible by installing MQTT broker app and entering the IP adress into the code


