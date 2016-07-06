/*
Code for Project LiNo
Module :- Node MCU development board

Description:-
The module creates a Access point and is configured as a Webserver.
Client modules connect to it and hit a specific URL, for desired action.

Author:-
Akshat Wahi | akshatwahi(at)gmail(dot)com

Date Created:-
27 Dec 2015
Date Modified:-
24 Feb 2016
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "LiNo";
const char *password = "LiNo@12345";

const int esp_led = 16, lino_led = 4;
int light_value = 0, counter = 0, state = 0;


unsigned long current_millis = 0;

ESP8266WebServer server ( 80 );

void setup( void )
{
  pinMode(esp_led, OUTPUT);
  pinMode(lino_led, OUTPUT);
  digitalWrite(esp_led, 0);       // Node MCU on-board LED works on opposite Logic. 0 - HIGH : 1 - LOW
  digitalWrite(lino_led, 0);
  Serial.begin(115200);
  delay(500);
  digitalWrite(esp_led, 1);

  Serial.println("Configuring access point...");

  WiFi.softAP(ssid, password); // You can remove the password parameter if you want the AP to be open.
  delay(500);
  IPAddress myIP = WiFi.softAPIP(); // Swithing ON access point with defined SSID & Password

  Serial.print("AP IP address: ");
  Serial.println(myIP);             // IP of access point its pre-configured to 192.168.4.1

  // setting up request Handlers
  server.on( "/", handle_root );
  server.on( "/switch", handle_switch );
  server.on( "/bell", handle_bell );
  server.onNotFound( handle_not_found );
  server.begin();
  delay(500);
  Serial.println( "HTTP server started" );
}

void loop( void )
{
  server.handleClient();

  switch (state)
  {

    case 1:        // State - 1 Switch event triggered
      if (((unsigned long)(millis() - current_millis)) >= 2000)
      {
        digitalWrite(lino_led, light_value);  
        if (light_value)
          light_value = 0;
        else
          light_value = 1;
        counter += 1;

        if (counter > 10) // 20 seconds of notification
          state = 0;
        current_millis = millis();
      }
      delay(200);
      break;

    case 2:         //State - 2 Bell event triggered

      if (((unsigned long)(millis() - current_millis)) >= 300)
      {
        //Serial.println(state);
        //Serial.println(light_value);
        digitalWrite(lino_led, light_value); 
        if (light_value)
          light_value = 0;
        else
          light_value = 1;
        counter += 1;

        if (counter > 50) // 15 seconds of notification
          state = 0;
        current_millis = millis();
      }
      delay(200);
      break;

    
    default:
           digitalWrite(lino_led,0);
      break;

  }

}

void handle_root()
{
  
  state = 0;
  digitalWrite(esp_led, 0);
  server.send(200, "text/html", "<h1>You are connected to LiNo Webserver</h1>");
  delay(100);
  digitalWrite(esp_led, 1);
}


void handle_switch()
{
  
  digitalWrite ( esp_led, 0 );
  server.send(200, "text/html", "<h1>Switch event triggered</h1>");
  delay(100);
  digitalWrite(esp_led, 1);
  if (state == 0 || state == 1)
  {
    state = 1;
    current_millis = millis();
    counter = 0;
    light_value = 1;
    digitalWrite(lino_led, light_value);
    delay(10);
  }


}


void handle_bell()
{
  
  digitalWrite( esp_led, 0 );
  server.send(200, "text/html", "<h1>Bell event triggered</h1>");
  delay(100);
  digitalWrite(esp_led, 1);
  state = 2;
  current_millis = millis();
  counter = 0;
  light_value = 1;
  digitalWrite(lino_led, light_value);
  delay(10);
}


void handle_not_found()
{
 
  digitalWrite(esp_led, 0);
  server.send(404, "text/html", "<h1>Wrong Request, wrong button pressed ;P</h1>");
  delay(100);
  digitalWrite(esp_led, 1);
}