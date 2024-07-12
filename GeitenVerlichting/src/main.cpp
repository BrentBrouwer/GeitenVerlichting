#include <Arduino.h>
#include "../Wifi/WifiControl.h"

// Initialize the Wifi Client
// This object cannot print to the serial monitor yet, because the serial monitor is opened in the setup
WifiControl wifiControl;

void InitSerialMonitor()
{
    Serial.begin(115200);

    while (!Serial.available())
    {
        delay(10);
    }

    Serial.println("Serial monitor opened");

    // This object can print to the serial monitor because the serial monitor is opened above
    WifiControl wifiControl1;
}

void setup()
{
    InitSerialMonitor();
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Serial.println("Looping");

    delay(1000);
}

