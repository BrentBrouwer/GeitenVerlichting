#include <Arduino.h>
#include "WiFi.h"

#define LedPin 5

#pragma region Properties
// Wifi Server
WiFiServer m_WifiServer;
uint16_t m_PortNr = 80; // Default port number for HTTP web sockets, 443 for HTTPS
const char *m_SSID = "vBakel";
const char *m_Password = "1001100111";

// Wifi Client
const long m_TimeOut = 2000;
unsigned long m_CurrentTime = millis();
unsigned long m_ConnectionTime = 0;

#pragma endregion

#pragma region Methods
void InitSerialMonitor()
{
    Serial.begin(115200);

    while (!Serial.available())
    {
        delay(10);
    }

    Serial.println("Serial monitor opened");
}

void InitOutputs()
{
    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);

    Serial.printf("Output: %i initialized\n", LedPin);
}

void InitWifi()
{
    // Connect to the local Network
    Serial.printf("Connect to %s\n", m_SSID);
    WiFi.begin(m_SSID, m_Password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }

    Serial.printf("Connected to %s\n", m_SSID);

    Serial.print("Server address: ");
    Serial.println(WiFi.localIP());

    // Create the Server
    m_WifiServer.begin(m_PortNr);
}

void CheckClients()
{
    // Check for clients
    WiFiClient client = m_WifiServer.available();

    // Check for a new client and if it has data available
    if (client)
    {
        m_ConnectionTime = millis();

        String incomingMsg = "";
        bool msgValid = false;

        // Read the buffer while there is a connection and in the timeout time
        while (client.connected && (m_CurrentTime - m_ConnectionTime) <= m_TimeOut)
        {
            // Update the time
            m_CurrentTime = millis();

            // Read a single byte from the buffer
            char c = client.read();

            // Check the incoming byte
            switch (c)
            {
                // New Line
                case '\n':
                    msgValid = true;
                    break;

                // Carriage return
                case '\r'
                    // Do nothing
                    break;

                default:
                    // Add to the message
                    incomingMsg.concat(c);
                    break;
            }
        }

        // Print the full message
        Serial.println(incomingMsg);

        if (msgValid)
        {
            SendHttpOkResponse(client);
        }
    }
}

void SendHttpOkResponse(WiFiClient client)
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
}
#pragma endregion

#pragma region Program
void setup()
{
    InitSerialMonitor();
    InitOutputs();
    InitWifi();
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Serial.println("Looping");

    delay(1000);
}
#pragma endregion