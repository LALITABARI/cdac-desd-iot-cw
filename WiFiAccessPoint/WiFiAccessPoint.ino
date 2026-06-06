/*
  ESP32 Access Point (Server)
  Creates a local Wi-Fi network and listens for two numbers from a connected client.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 
#endif

// Wi-Fi Credentials for the AP
const char *ssid = "AP2-Station";
const char *password = "123456789"; 

// Start TCP server on port 80
NetworkServer server(80);

void blink(int value)
{
  for(int i=0; i<value; i++)
  {
    digitalWrite(LED_BUILTIN , HIGH); 
    delay(100);
    digitalWrite(LED_BUILTIN , LOW); 
    delay(100); 
  }
}

void check_nums(int val1 , int val2)
{
  if(val1 > 50)
  {
    blink(4); 
    Serial.println("Number 1 is greater than 50.");
  }
  if(val2 > 50)
  {
    blink(4); 
    Serial.println("Number 2 is greater than 50.");
  }
}
void setup() 
{
  pinMode(LED_BUILTIN , OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring Access Point...");

  // Create the Access Point
  if (!WiFi.softAP(ssid, password))
  {
    Serial.println("Soft AP creation failed.");
    while (1);
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(myIP); // Default is usually 192.168.4.1

  server.begin();
  Serial.println("Server started, waiting for Station...");
}

void loop() 
{
  // Listen for incoming clients (the Station)
  NetworkClient client = server.accept();

  if (client)
  {
    Serial.println("\n--- New Station Connected ---");
    blink(2); 
    String incomingData = "";

    while (client.connected() || client.available())
    {
      if (client.available())
      {
        //Serial.print("Debug 1\n"); 
        char c = client.read();
        
        // Read until a newline character is encountered
        if (c == 10) {

          Serial.print("Received Raw Data: ");
          Serial.println(incomingData);

          // Find the comma separating the two numbers
          int commaIndex = incomingData.indexOf(',');
          if (commaIndex != -1) {
            String num1_str = incomingData.substring(0, commaIndex);
            String num2_str = incomingData.substring(commaIndex + 1);

            // Convert strings to integers or floats
            int num1 = num1_str.toInt();
            int num2 = num2_str.toInt();

            

            Serial.println("Parsed Data:");
            Serial.print("  Number 1: "); Serial.println(num1);
            Serial.print("  Number 2: "); Serial.println(num2);
            check_nums(num1 , num2); 
          } else {
            Serial.println("Error: Data format invalid. Expected 'num1,num2'");
          }
          
          break; // Exit loop after processing the message
        } else if (c != '\r')
        {
          incomingData += c; // Build the string
        }
      }
    }
    
    // Close connection
    client.stop();
    Serial.println("--- Station Disconnected. ---");
  }
}