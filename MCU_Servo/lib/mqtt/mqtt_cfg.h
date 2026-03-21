#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

// FIXME: Change to your MQTT broker details
const char *MQTT_SERVER = "mqtt.eclipseprojects.io";
const int MQTT_PORT = 1883;
const char *MQTT_USER = "your_username";
const char *MQTT_PASSWORD = "your_password";

// MQTT topics
const char *MQTT_TOPIC_SUBSCRIBE = "topic";
const char *MQTT_TOPIC_PUBLISH = "topic";

#endif