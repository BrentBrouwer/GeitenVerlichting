#include "../../lib/Wifi/WifiControl.h"
#include <Arduino.h>

#pragma region Constructor
WifiControl::WifiControl()
{
    if (Serial.available())
    {
        Serial.println("Wifi instance created");
    }
}
#pragma endregion