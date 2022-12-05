#include "treatbot_globals.h"

WiFiClientSecure espClient;    // oooo secure!

void scan_wifi() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    while(1) {
        int n = WiFi.scanNetworks();
        Serial.println("scan done");
        if (n==0) {
            Serial.println("No networks found");
        } else {
            Serial.print(n);
            Serial.println(" networks found");
            for (int i = 0; i < n; ++i) {
                // Print SSID and RSSI for each network found
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(WiFi.SSID(i));
                Serial.print(" (");
                Serial.print(WiFi.RSSI(i));
                Serial.print(")");
                Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
                delay(10);            
            }
       }

       // wait to try again
        for (int x=0; x<25; x++) {
            yield();
            delay(100);
        }
    }
}

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to '");
  Serial.print(WIFI_SSID); Serial.println("'");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}