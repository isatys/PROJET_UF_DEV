/**
 * \file MyWiFi.h
 * \page esp8266 WiFi ESP8266
 * \brief Module WiFi de l'ESP8266
 * 
 * description de la page ESP8266
 * 
 * Fichier \ref MyWiFi.h
 */

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

char* ssid     = "YNOVAIX IOT";
char* password = "9xUNb8mk7Vs3A3Y";

// ------------------------------------------------------------------------------------------------
// SCAN WIFI
// ------------------------------------------------------------------------------------------------
void scanWifi(){
  Serial.print("WiFi SCAN : Start ... ");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("Done");

  if (n == 0) {
    Serial.println("NO networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID, Channel, RSSI and Encryption Type for each network found
      String encryptionType;
      switch (WiFi.encryptionType(i)){
        case ENC_TYPE_WEP : encryptionType = "WEP"; break;
        case ENC_TYPE_TKIP : encryptionType = "TKIP"; break;
        case ENC_TYPE_CCMP : encryptionType = "CCMP"; break;
        case ENC_TYPE_NONE : encryptionType = "OPEN"; break;
        case ENC_TYPE_AUTO : encryptionType = "AUTO"; break;
        default : encryptionType = "???"; break;
      }
      Serial.printf("[%d]: %s, Ch:%d, %d dBm, %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), encryptionType.c_str());
    }
  }
  Serial.println("");
}

// ------------------------------------------------------------------------------------------------
// CONNECT TO WIFI
// ------------------------------------------------------------------------------------------------
void setupWiFi(){
  MYDEBUG_PRINTLN();
  MYDEBUG_PRINT("-WIFI : Connexion en cours à ");
  MYDEBUG_PRINT(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    MYDEBUG_PRINT(".");
  }
  MYDEBUG_PRINTLN("");

  MYDEBUG_PRINT("-WIFI : connecté avec l'adresse IP : ");
  MYDEBUG_PRINTLN(WiFi.localIP());
}
