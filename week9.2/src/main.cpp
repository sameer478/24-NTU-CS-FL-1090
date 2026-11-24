#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Static IP settings (must match your router's network)
IPAddress local_IP(192, 168, 1, 200);   // IP you want the ESP32 to have
IPAddress gateway(192, 168, 1, 1);      // Router IP
IPAddress subnet(255, 255, 255, 0);     // Subnet mask
IPAddress primaryDNS(8, 8, 8, 8);       // Optional (Google DNS)
IPAddress secondaryDNS(8, 8, 4, 4);     // Optional

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Configuring static IP...");

  WiFi.mode(WIFI_STA);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Static IP configuration failed!");
  }

  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("Subnet: ");
    Serial.println(WiFi.subnetMask());
  } else {
    Serial.println("\nConnection failed. Check SSID/password.");
  }
}

void loop() {
  // nothing here
}