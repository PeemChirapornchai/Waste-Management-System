#include <Arduino.h>
// #include <WiFiManager.h>
#include "wifi.h"

// void wifi_setup()
// {
//     WiFiManager wm;
//     bool res;
//     // res = wm.autoConnect(); // auto generated AP name from chipid
//     res = wm.autoConnect("MCU_Servo_AP"); // anonymous ap
//     // res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

//     if (!res)
//     {
//         Serial.println("Failed to connect");
//         // ESP.restart();
//     }
//     else
//     {
//         // if you get here you have connected to the WiFi
//         Serial.println("WiFi connected");
//     }
// };