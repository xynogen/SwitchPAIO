#include <Arduino.h>
#include <ESP8266WiFi.h>

// Global Var
#ifndef STASSID
#define STASSID "PAIO"
#define STAPSK "complexity"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

//Switch State
int SW0;
int SW1;
int SW2;

// Set Static IP Configuration
IPAddress local_IP(192, 168, 0, 101);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

// Set server Port
WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);

  // Clear the serial output
  Serial.println(F(""));
  Serial.println(F(""));
  Serial.print(F("Connecting to : "));
  Serial.println(ssid);

  // Setting Static IP Address to NodeMCU
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println(F("STA Failed to configure"));
  }

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(""));

  Serial.println(F(""));
  Serial.println(F("WiFi Connected"));
  Serial.print(F("IP Address : "));
  Serial.println(WiFi.localIP());
  Serial.println(F(""));

  server.begin();
  Serial.println(F("Server Started"));
}

void loop()
{
  WiFiClient client = server.available();

  if (!client)
    return;

  Serial.println(F("New Client"));

  // Default value 1000
  client.setTimeout(5000);

  // Read the request
  String req = client.readStringUntil('\r');
  Serial.println(F("Request: "));
  Serial.println(req);

  if (req.indexOf(F("/SW/0/0")) != -1) 
  {
    SW0 = 0;
    digitalWrite(D0, SW0);
  }
    
  else if (req.indexOf(F("/SW/0/1")) != -1) 
  {
    SW0 = 1;
    digitalWrite(D0, SW0);
  }
    
  else if (req.indexOf(F("/SW/1/0")) != -1) 
  {
    SW1 = 0;
    digitalWrite(D1, SW0);
  }

  else if (req.indexOf(F("/SW/1/1")) != -1) 
  {
    SW1 = 1;
    digitalWrite(D1, SW0);
  }

  else if (req.indexOf(F("/SW/2/0")) != -1) 
  {
    SW2 = 0;
    digitalWrite(D2, SW2);
  }
   
  else if (req.indexOf(F("/SW/2/1")) != -1)
  {
    SW2 = 1;
    digitalWrite(D2, SW2);
  }
    
  else
  {
    Serial.println(F("Invalid request"));
    SW0 = digitalRead(D0);
    SW1 = digitalRead(D1);
    SW2 = digitalRead(D2);
  }

  while (client.available())
    client.read();


  /*
  Format for JSON data
  {"SW0": X,
  "SW1": X,
  "SW2": X}
  */

  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"));

  client.print(F("{\"SW0\":"));
  client.print(SW0);
  client.print(F(",\"SW1\":"));
  client.print(SW1);
  client.print(F(",\"SW2\":"));
  client.print(SW2);
  client.print(F("}\r\n\r\n"));
  Serial.println(F("Disconnecting from client"));
}