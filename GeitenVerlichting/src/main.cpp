#include <Arduino.h>
#include "WiFi.h"
#include "json_parser.h"

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

// Messages
const char * LedOn = "GET /LED/ON";
const char * LedOff = "GET /LED/OFF";

struct ClientMsg
{
    WiFiClient client;
    String message;
};

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

#pragma region Outputs
void InitOutputs()
{
    pinMode(LedPin, OUTPUT);
    digitalWrite(LedPin, LOW);

    Serial.printf("Output: %i initialized\n", LedPin);
}

bool ControlOutputs(String msg)
{
    if (msg.indexOf(LedOn) >= 0)
    {
        digitalWrite(LedPin, HIGH);
        Serial.println("Led on");
        return true;
    }
    else if (msg.indexOf(LedOff) >= 0)
    {
        digitalWrite(LedPin, LOW);
        Serial.println("Led off");
        return true;
    }

    return false;
}
#pragma endregion

#pragma region WiFi
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

void SendValidResponse(WiFiClient client)
{
    client.println("HTTP/1.1 200 OK");
    // client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
}

void SendInvalidResponse(WiFiClient client)
{
    client.println("HTTP/1.1 400 BAD REQUEST");
    // client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
}

ClientMsg GetMessage()
{
    ClientMsg newClient;
    String incomingMsg = "";

    // Check for clients
    WiFiClient client = m_WifiServer.available();
    newClient.client = client;

    // Check for a new client and if it has data available
    if (client)
    {
        m_ConnectionTime = millis();
        Serial.print("Connection time: ");
        Serial.println(m_ConnectionTime);

        // Read the buffer
        incomingMsg = client.readStringUntil('\n');
        Serial.print("Incoming msg: ");
        Serial.println(incomingMsg);
    }

    newClient.message = incomingMsg;
    return newClient;
}
#pragma endregion
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
    // Read incoming clients
    ClientMsg clientData = GetMessage();

    if (sizeof(clientData.message) > 0 && ControlOutputs(clientData.message))
    {
        SendValidResponse(clientData.client);
    }
    else
    {
        SendInvalidResponse(clientData.client);
    }

    delay(100);
}
#pragma endregion