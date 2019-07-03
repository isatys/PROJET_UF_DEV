/**\file MyArduino0.16.ino 
 * Fichier principal Arduino.
 * 
 * Il se constitue de 2 fonctions qui permettent dans un premier temsp de configurer la carte : setup() et
 * d'ensuite définir les actions qui seront faites en boucle : loop().
 * \mainpage Bienvenue
 * \tableofcontents
 * \section introduction Introduction
 * Cette documentation a pour objectif de vous fournir tous les éléments pour utiliser et maîtriser l'ensemble
 * de ces composants en vue de vous donner l'autonomie pour apprendre à les utiliser et développer vos objets
 * connectés
 * 
 * \subsection prerequis Prérequis
 * Pour utiliser ce code il est nécessaire de disposer de :
 * - Environnement de développement Arduino
 * - Une carte NodeMCU
 * 
 * \subsection doxygen Configuration de Doxygen
 * Pour lire convenablement cette documentation, vous aurez besoin d'utiliser Doxygen (http://www.doxygen.nl/). 
 * 
 * Pour le configurer, il faut passer en mode \b Expert et renseigner les éléments suivants : 
 * - Project / OUTPUT_LANGUAGE : French
 * - Project / EXTENSION_MAPPING : ajouter "ino=c" à la liste
 * - Input / FILE_PATTERN : ajouter "*.ino" à la liste des fichiers
 * 
 * \section modules Modules
 * Ce projet se compose de différents "modules" indépendants qui vous permettront d'appréhender différents
 * fonctionnalités des objets connectés :
 * - Port Série
 * - \ref debug
 * - \ref esp8266
 * - \ref nodemcu
 * - \ref pwm
 * - \ref soilsensor
 * - \ref dht
 * - \ref tickers
 * - \ref timers
 * - \ref deepsleep
 * - \ref spiffs
 * - \ref ntp
 * - \ref wifimanager
 * - \ref webserver
 * - \ref ota
 * - \ref mqtt
 * - \ref adafruitio
*/

#define FIRMWAREVERSION "1.0"
#define THINGTYPE       "NodeMCU"

// ------------------------------------------------------------------------------------------------
// MODULES
#include "MyDebug.h"        // Debug
#include "MyWiFi.h"         // WiFi du ESP8266
#include "MyNodeMCU.h"      // Correspondance entre les PINs Arduino et NodeMCU
#include "MyPwm.h"          // Pulse Width Modulation (PWM)
#include "MySoilSensor.h"   // Capteur d'humidité du sol
#include "MyDht.h"          // Digital Humidity & Temperature (DHT)
#include "MyTicker.h"       // Tickers
#include "MyTimer.h"        // Timers
#include "MyDeepSleep.h"    // Sleep modes
#include "MySPIFFS.h"       // SPIFFS
#include "MyNTP.h"          // Network Time Protocol
#include "MyWiFiManager.h"  // WiFi Manager
#include "MyWebServer.h"    // Web Server
#include "MyOTA.h"          // Over The Air (OTA)
#include "MyMQTT.h"         // MQTT
#include "MyAdafruitIO.h"     // Adafruit IO
// ------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------
// SETUP
// ------------------------------------------------------------------------------------------------
/**
* Cette est appelée au démarrage du programme, avant tout autre fonction.
* Elle n'est exécutée qu'une seule fois après chaque mise sous tension ou réinitialisation (reset).
* Elle est à utiliser pour initialiser les variables, le sens des broches, les librairies utilisées. 
*/
void setup() {
  Serial.begin(115200);
  MYDEBUG_PRINTLN("------------------- SETUP");
  setupWiFi();        // Initialisation du WiFi
  setupSoilSensor();  // Initialisation du capteur d'humidité du sol
  setupDhtSensor();   // Initialisation du capteur DHT
//  setupTicker();      // Initialisation d'un Ticker
//  setupTimer();       // Initialisation du Timer1 du NodeMCU
//  setupDeepSleep();   // Initialisation du mode Deep Sleep
//  setupSPIFFS();      // Initialisation du SPIFFS avec un fichier de configuration
//  setupNTP();         // Initialisation de l'heure
setupWiFiManager(); // Initialisation du WiFi Manager
//  setupWebServer();   // Initialisation du serveur web
//  setupOTA();         // Initialisation de la mise à jour de firmware OTA
//  setupMQTT();          // Initialisation du client MQTT
  setupAdafruitIO();
}

// ------------------------------------------------------------------------------------------------
// LOOP
// ------------------------------------------------------------------------------------------------
/**
 * Après avoir créé une fonction setup(), qui initialise et fixe les valeurs de démarrage du programme, 
 * la fonction loop () fait exactement ce que son nom suggère et s'exécute en boucle 
 * sans fin, permettant à votre programme de s'exécuter et de répondre.
*/
void loop() {
  MYDEBUG_PRINTLN("------------------- LOOP");
  getSoilData();      // Lecture des données du capteur d'humidité du sol
  getDhtData();       // Lecture des données du capteur DHT
//  getNTP();           // Récupération de l'heure auprès du serveur NTP
//  loopWebServer();    // Gestion des clients du serveur Web
//  loopOTA();          // Gestion des mises à jour de firmware par WiFi
//  loopMQTT();         // Gestion de la connexion au broker MQTT
  loopAdafruitIO();
  delay(5000);        // Délai si nécessaire
}
