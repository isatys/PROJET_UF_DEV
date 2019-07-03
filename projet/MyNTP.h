/**
 * \file MyNTP.h
 * \page ntp Network Time Protocol (NTP)
 * \brief Quelle heure est il ?
 * 
 * Les cartes Arduino, ESP8266 et ESP32 ne disposent pas d’horloge temps réel. Nous allons donc récupérer
 * l'heure auprès d'un serveur de temps NTP : Network Time Protocol.
 * Nous pourrons ainsi horodater (timestamp) des mesures, connaître le temps écoulé entre deux événements, 
 * afficher l’heure courante sur une interface WEB, déclencher une action programmée ...
 * 
 * Fichier \ref MyNTP.h
 */

#include <NTPClient.h>           //https://github.com/arduino-libraries/NTPClient
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void getNTP(){
  MYDEBUG_PRINT("-NTP : ");
  timeClient.update();
  MYDEBUG_PRINTLN(timeClient.getFormattedTime());
}

void setupNTP(){
  // On a besoin d'une connexion à Internet !
  if (WiFi.status() != WL_CONNECTED){
    setupWiFi();
  }  
  timeClient.begin();
}
