/*
Code for Project LiNo
Module :- Node MCU development board

Description:-
The module is configured as client and connects to WiFi access point (LiNo).
The code is for notifying button press.

Author:-
Akshat Wahi | akshatwahi(at)gmail(dot)com

Date Created:-
27 Dec 2015
Date Modified:-
24 Feb 2016
*/

#include <ESP8266WiFi.h>

const char* ssid     = "LiNo";
const char* password = "LiNo@12345";

const char* host = "192.168.4.1";
const char* streamId   = "....................";
const char* privateKey = "....................";

const int push_button = 0 , esp_led = 16;

void setup() 
{  
  pinMode(esp_led, OUTPUT);
  pinMode(push_button, INPUT_PULLUP);
  digitalWrite(esp_led, 0);       // Node MCU on-board LED works on opposite Logic. 0 - HIGH : 1 - LOW
  Serial.begin(115200);
  delay(1000);
  digitalWrite(esp_led, 1);

  
  // We start by connecting to a WiFi network

  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
}



void loop() 
{
  if(!digitalRead(push_button))
  {
    digitalWrite(esp_led, 0);
    push_notification();
    digitalWrite(esp_led, 1);
    delay(15000);             // wait to initiate next trigger, considering 15 seconds to be bell length
  }
  delay(10);
}


void push_notification()
{
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    delay(100);
    return;
  }
  
  // We now create a URI for the request
  String url = "/bell";
    
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
    delay(10);
  }
  
  Serial.println();
  Serial.println("closing connection");
}