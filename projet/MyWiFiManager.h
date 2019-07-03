/**
 * \file MyWiFiManager.h
 * \page wifimanager WiFi Manager
 * \brief Portail captif de configuration (Wifi et autres paramètres personnalisables)
 * 
 * Le fait d'intégrer l'idenfiant (SSID) et le mode de passe du réseau WiFi dans le code du projet Arduino
 * peut être problématique. Dans la mesure où ce code est intégré dans un objet connecté proposé à des clients,
 * ces identifiants ne sont pas connus au moment du développement; qui plus est ils peuvent changer si on
 * déplace l'objet, si on veut utiliser un autre réseau, ou si le mot de passe change ...
 * 
 * WiFi Manager apporte une solution en proposant un portail captif permettant de configurer des paramètres :
 * - Si les paramètres WiFi ne sont pas présents alors WiFi Manager démarre l'ESP8266 en mode Access Point et
 *   ouvre un portail captif auquel vous pouvez accéder depuis votre ordinateur ou votre téléphone. Le SSID 
 *   est celui défini par AP_SSID.
 * - L'interface proposée vous permet de visualiser les SSID accessibles, d'en sélectionner un et de renseigner
 *   le mot de passe associé.
 * - Une fois validé, l'ESP8266 redémarre et tente d'accéder au SSID. S'il y arrive il est alors en mode client
 *   (STATION) et n'ouvre pas le portail de configuration. S'il n'y arrive pas il se remet en mode configuration.
 * 
 * WiFi Manager offre la possibilité d'étendre la saisie de paramètres complémentaires dans le portail captif.
 * Il est de notre ressort d'en gérer le stockage (SPIFFS, carte SD ou autre).
 * 
 * Il subsiste une problématique : une fois la configuration faite, comment procéder à des modifications WiFi 
 * ou paramètres personnalisés ?
 * - Une première solution consiste à retéléverser le sketch en précisant "Sketch + WiFi Settings" ou "All Flash
 *   Contents" dans le menu "Outils/Erase Flash" du NodeMcu.
 * - Une alternative logicielle sans téléversement serait d'utiliser une fonction du type DoubleResetDetector :  
 *   si le bouton Reset est pressé 2 fois de suite (dans un intervalle de temps donné) alors les paramètres WiFi
 *   sont effacés ce qui a pour conséquence de redémarrer en mode portail.
 * 
 * Bibliothèques à installer pour utiliser ce module :
 * - WiFiManager by tzapu : https://github.com/tzapu/WiFiManager
 * - Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson
 * 
 * Fichier \ref MyWiFiManager.h
 */

#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

/** \brief SSID du portail captif*/
#define AP_SSID "MY SSID"                               /*!< Nom du SSID pour le portail captif */
String strWiFiManagerConfigFile = "/WiFiManager.json";  /*!< Nom du fichier de configuration WiFi Manager à stocker sur le SPIFFS*/
bool bshouldSaveConfig = false;                         /*!< Flag pour savoir si la configuration doit être enregistrée ou pas*/
char cMqttFlag[4];                                      /*!< Flag "ON" ou "OFF" pour activer la fonctionnalité MQTT ou pas*/
char cMqttServer[40];                                   /*!< Nom du serveur MQTT à utiliser*/
char cMqttToken[40];                                    /*!< Token du device MQTT à utiliser*/

/**
 * Fonction de lecture du fichier de configuration strWiFiManagerConfigFile
 * \note Le SPIFFS doit être démarré
 */
boolean readConfigFile(){
  boolean bReadJsonConfigFile = true;

  // Est-ce que le fichier existe ?
  if (SPIFFS.exists(strWiFiManagerConfigFile)) {  // ------------------------------------ Le fichier existe
    MYDEBUG_PRINTLN("-WiFiManager : Lecture du fichier de configuration");

    File configFile = SPIFFS.open(strWiFiManagerConfigFile, "r"); // ------ Ouverture du fichier en lecture

    if (strWiFiManagerConfigFile) { //------------------------------------ Le fichier est ouvert en lecture
      MYDEBUG_PRINTLN("-WiFiManager : Fichier ouvert");

      // Création d'un buffer JSON pour parser le contenu du fichier
      StaticJsonDocument<512> jsonDocument;
      // Désérialisation du document JSON
      DeserializationError error = deserializeJson(jsonDocument, configFile);
      if (error){
        MYDEBUG_PRINTLN("-WiFiManager: Impossible de parser le fichier"); //---- Problème de parsing JSON
        bReadJsonConfigFile = false;
      } else {               //--------------------------------------------------------- Fichier JSON parsé
        MYDEBUG_PRINTLN("-WiFiManager: Fichier parsé");
        // Lecture des paramètres
        strcpy(cMqttFlag, jsonDocument["MQTT Flag"]);
        strcpy(cMqttServer, jsonDocument["MQTT Server"]);
        strcpy(cMqttToken, jsonDocument["MQTT Token"]);
        MYDEBUG_PRINT("-WiFiManager [MQTT Flag] : ");
        MYDEBUG_PRINTLN(cMqttFlag);
        MYDEBUG_PRINT("-WiFiManager [MQTT Server] : ");
        MYDEBUG_PRINTLN(cMqttServer);
        MYDEBUG_PRINT("-WiFiManager [MQTT Token] : ");
        MYDEBUG_PRINTLN(cMqttToken);
      }
      // Fermeture du fichier
      configFile.close();
      MYDEBUG_PRINTLN("-WiFiManager: Fichier fermé");
    }
  } else { //-------------------------------------------------------------------- Le fichier n'existe pas
    bReadJsonConfigFile = false;
  }
  return bReadJsonConfigFile;
}

/**
 * Fonction de sauvegarde du fichier de configuration strWiFiManagerConfigFile
 * \note Le SPIFFS doit être démarré
 */
void writeConfigFile(){
  MYDEBUG_PRINTLN("-WiFiManager : Enregistrement du fichier de configuration");

  // Création du buffer JSON
  DynamicJsonDocument jsonDocument(512);
  // Ecriture des paramètres dans le buffer
  jsonDocument["MQTT Flag"] = cMqttFlag;
  jsonDocument["MQTT Server"] = cMqttServer;
  jsonDocument["MQTT Token"] = cMqttToken;

  // Ouverture du fichier en écriture
  File configFile = SPIFFS.open(strWiFiManagerConfigFile, "w");
  if (!configFile) {
    MYDEBUG_PRINTLN("-WiFiManager : Impossible d'ouvrir le fichier de configuration en écriture");
    return;
  }

  // Serialize JSON to file
  if (serializeJson(jsonDocument, configFile) == 0) {
    Serial.println("-WiFiManager : Impossible d'écrire le JSON dans le fichier de configuration");
  }

  // Fermeture du fichier
  configFile.close(); 
}

/**
 * Fonction callback appelée quand la configuration doit être sauvegardée
 */
void saveConfigCallback () {
  MYDEBUG_PRINTLN("-WiFiManager : La configuration doit être sauvegardée");
  bshouldSaveConfig = true;
}

/**
 * Fonction callback appelée quand on rentre en mode configuration
 */
void configModeCallback (WiFiManager *myWiFiManager) {
  MYDEBUG_PRINTLN("-WiFiManager : Démarrage du mode configuration");
}

/**
 * Fonction principale d'utilisation du WiFi Manager
 */
void setupWiFiManager(){

  // Montage du SPIFFS pour gestion du fichier de configuration
  MYDEBUG_PRINTLN("-WiFiManager : Montage du système de fichier");
  if (SPIFFS.begin()) {
    MYDEBUG_PRINTLN("-WiFiManager : Système de fichier monté");
    //SPIFFS.format(); // -------------------- Au besoin, pour formatter le système de fichiers
    readConfigFile();
  } else {
    MYDEBUG_PRINTLN("-WiFiManager : Impossible de monter le système de fichier");
  }
  
  // Définition locale du WiFiManager car une fois son travail fait, il n'est pas nécessaire
  // de le conserver.
  WiFiManager wifiManager;
  //Configuration des callbacks du WiFi Manager
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setAPCallback(configModeCallback);
  // Ajout des paramètres supplémentaires à configurer et donc à inclure dans le portail captif
  WiFiManagerParameter custom_MqttFlag("MQTT Flag", "ON/OFF", cMqttFlag, 4);  
  WiFiManagerParameter custom_MqttServer("MQTT Server", "MQTT server", cMqttServer, 40);
  WiFiManagerParameter custom_MqttToken("MQTT Token", "MQTT device token", cMqttToken, 40);
  wifiManager.addParameter(&custom_MqttFlag);
  wifiManager.addParameter(&custom_MqttServer);
  wifiManager.addParameter(&custom_MqttToken);
  
  //Au besoin, fonction à utiliser pour effacer les valeurs des paramètres
  //wifiManager.resetSettings();

  //Récupération du SSID et du mot de passe pour une tentative de connextion
  //Si la connexion échoue alors le WiFi Manager passe en mode Access Point
  //avec le SSID paramétré et attend que la configuration soit saisie.
  if (!wifiManager.autoConnect(AP_SSID)) {
    MYDEBUG_PRINTLN("-WiFiManager : Impossible de connecter au WiFi - Timeout");
    delay(3000);
    //Reset et nouvelle tentative, possibilité de le mettre en Deep Sleep aussi
    ESP.reset();
    delay(5000);
  }

  //Connexion au WiFi effectuée
  MYDEBUG_PRINT("-WiFiManager : Connecté au réseau WiFi : ");
  MYDEBUG_PRINTLN(WiFi.localIP());

  // Lecture des paramètres saisis dans le portail captif
  strcpy(cMqttFlag, custom_MqttFlag.getValue());
  strcpy(cMqttServer, custom_MqttServer.getValue());
  strcpy(cMqttToken, custom_MqttToken.getValue());

  // Sauvegarde des paramètres dans le fichier de configuration
  if (bshouldSaveConfig) {
    writeConfigFile();
  }

  //Fermeture du SPIFFS
  SPIFFS.end();
}
