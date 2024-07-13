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

#pragma region HTML Page
const String html_DocType = "<!doctype html>";
const String html_Html = "<html lang=\"en-US\">";
const String html_HtmlEnd = "</html>";

const String html_Head = "<head><title>Geitjes</title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<link rel=\"icon\" href=\"data:,\">";
const String html_HeadEnd = "</head>";

const String html_Style = "<style>";
const String html_StyleEnd = "</style>";

const String html_BodyEnd = "</body>";

void CreateHtmlPage(WiFiClient client)
{
    bool ledOn = digitalRead(LedPin);

    // Doc Type-------------------------------------------------
    client.println(html_DocType);

    // Head-----------------------------------------------------
    // Header
    client.println(html_Head);
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.printf(".button { background-color: %s; border: 2px solid black; color: black; padding: 16px 40px;\n", ledOn ? "red" : "green");
    client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(html_StyleEnd);
    client.println(html_HeadEnd);
            
    // Body-----------------------------------------------------
    client.printf("<body><h1>%s</h1>\n", ledOn ? "De geitjes zijn je eeuwig dankbaar voor de verlichting" : "Lul! Je hebt de verlichting uit gezet!");

    // Label
    // client.printf("<p1>LED status: %s</p1>\n", ledOn ? "aan" : "uit");

    // Button
    // The href property sets the addres
    client.printf("<p><a href=\"/LED/%s\"><button class=\"button\">Zet %s</button></a></p>\n", ledOn ? "OFF" : "ON", ledOn ? "uit :(" : "aan!");
    client.println(html_BodyEnd);

    // End Response---------------------------------------------
    client.println(html_HtmlEnd);
    client.println();
}
#pragma endregion

#pragma region Methods
void InitSerialMonitor()
{
    Serial.begin(115200);

    m_ConnectionTime = millis();

    while (!Serial.available() && (m_CurrentTime - m_ConnectionTime) < m_TimeOut)
    {
        m_CurrentTime = millis();
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

void SendResponse(WiFiClient client, bool succes)
{
    if (succes)
    {
        client.println("HTTP/1.1 200 OK");
    }
    else
    {
        client.println("HTTP/1.1 400 BAD REQUEST");
    }
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

    if (clientData.message.length() > 0)
    {
        Serial.print(clientData.client.remoteIP());
        Serial.println(" connected");

        if (ControlOutputs(clientData.message))
        {
            SendResponse(clientData.client, true);
        }
        else
        {
            SendResponse(clientData.client, false);
        }

        Serial.println("Send html page");
        CreateHtmlPage(clientData.client);

        // Clear the response message
        clientData.message = "";

        // Close the Connection
        clientData.client.stop();
        Serial.println("Client disconnected");
    }    

    delay(1000);
}
#pragma endregion