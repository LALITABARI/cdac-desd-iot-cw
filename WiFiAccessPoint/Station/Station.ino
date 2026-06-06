/*
  ESP32 Station (Client)
  Connects to the ESP32 AP and sends two numbers over a TCP connection.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClient.h>

// Wi-Fi Credentials of the AP to connect to
const char *ssid = "AP2-Station";
const char *password = "123456789";

// Server IP Address (the AP's IP) and Port
const char *serverIP = "192.168.4.1"; 
const uint16_t serverPort = 80;

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  
  // Connect to the Access Point
  Serial.print("Connecting to AP: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to AP!");
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("\nAttempting to connect to Server...");
  NetworkClient client;

  // Try to connect to the AP server
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to Server!");

    // Generate two example numbers to send
    int number1 = random(10, 100);  // Random number between 10 and 99
    int number2 = random(10, 100); // Random number between 100 and 499

    // Format data as "num1,num2\n"
    Serial.print("Sending: ");
    //Serial.trim();
    Serial.printf("%d,%d\n", number1, number2);

    // Send the numbers over TCP
    client.printf("%d,%d\n", number1, number2);
    client.flush(); 
    // Close the connection
    client.stop();
    Serial.println("Data sent and connection closed.");
  } else {
    Serial.println("Connection to Server failed.");
  }

  // Wait 5 seconds before sending data again
  delay(5000);
}