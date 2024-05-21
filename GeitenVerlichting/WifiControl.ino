// Libraries
#include <WiFi.h>

#pragma region PreProcessor
// Socket Client
#define m_SSID "24GHz_TestNetwork"
#define m_Password "BloempotKapsel243"
#define m_IPAddress "192.168.0.103"

// Socket Server
#define m_PortNr 2364
#pragma endregion

IPAddress test;

void ConnectToWifi() {
    // Try Connect to the Wifi
    WiFi.begin(m_SSID, m_Password);

    // Wait for Connection
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.println("Connect to Wifi");

        // Control the Signal LED
        delay(500);
        digitalWrite(m_CtrlLED, HIGH);
        delay(500);
        digitalWrite(m_CtrlLED, LOW);
    }

    Serial.println("Connected");
    Serial.print("IP-Address: ");
    Serial.println(WiFi.localIP());
}

void StartServer() {
}

void CheckServer() {
}