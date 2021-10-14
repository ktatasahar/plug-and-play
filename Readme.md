
# Plug and Play sensing platform via web application using IoT

It is the first step to create a plug and play IOT platform.

My mission is to create a web application that attach the data received from the sensors connected to an ESP32.

This IoT project is to measure temperature and relative humidity and distance using DHT11 and HC-SR04 sensors and NodeMCU ESP32.

**Schematics**
![shematic](https://user-images.githubusercontent.com/48835490/137403153-e16a96ac-28df-4355-b279-84f1c4511a95.png)


Data temperature , humidity and distance from sensor is show in serial monitor

Data on ESP32 is converted into JSON file format and then sent to the MQTT broker (in this project I use Flespi MQTT Broker) via WiFi