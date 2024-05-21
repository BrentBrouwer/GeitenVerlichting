#pragma region PreProcessor

// Hardware
#define m_CtrlLED 2
#define m_Light 27

#pragma endregion

void setup()
{
    // Initialize IO's
    pinMode(m_CtrlLED, OUTPUT);
    pinMode(m_Light, OUTPUT);

    // Set Outputs Low at Startup
    digitalWrite(m_CtrlLED, LOW);
    digitalWrite(m_Light, LOW);

    // Initialize Serial Port
    Serial.begin(115200);

    // Debug
    Serial.println("Start program");

    // Connect to the Wifi network
    ConnectToWifi();
}

void loop()
{
    // Control the Build-In LED
    delay(1000);
    digitalWrite(m_CtrlLED, HIGH);
    delay(1000);
    digitalWrite(m_CtrlLED, LOW);
}
