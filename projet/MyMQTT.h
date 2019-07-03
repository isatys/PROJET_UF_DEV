/**
 * \file MyMQTT.h
 * \page mqtt MQTT
 * \brief Message Queuing Telemetry Transport
 * 
 * Le protocole Message Queue Telemetry Transport (MQTT) est conçu pour l'échange de messages entre des petits 
 * périphériques sur une faible bande passante, ou pour les connexions coûteuses et envoyer des messages 
 * de façon fiable. Il utilise TCP/IP. http://mqtt.org/
 * MQTT est un protocole ouvert, simple, léger et facile à mettre en œuvre. Ce protocole est idéal pour
 * répondre aux besoins suivants :
 * - Utilisation d’une très faible bande passante,
 * - Utilisation sur les réseaux sans fils,
 * - Faible consommation en énergie,
 * - Rapidité avec un temps de réponse supérieur aux autres standards du web actuel,
 * - Fiabilité,
 * - Utilisation de faibles ressources processeurs et de mémoire.
 * 
 * <H2>Publish/Subscribe</H2>
 * 
 * MQTT est un service de publication/abonnement en contraste avec le protocole HTTP et son architecture 
 * « request/response ». MQTT se base sur TCP/IP et est extrêmement léger. Il fonctionne 
 * sur le principe client/serveur.
 * 
 * <H2>MQTT Broker</H2>
 * 
 * Le point central de la communication est le broker MQTT en charge de relayer les messages des émetteurs
 * vers les clients. Chaque client s’abonne via un message vers le broker : le « topic » (sorte d’information
 * de routage pour le broker) qui permettra au broker de réémettre les messages reçus des producteurs
 * de données vers les clients. Les clients et les producteurs n’ont ainsi pas à se connaître, ne communiquant
 * qu’au travers des topics. Cette architecture permet des solutions multi-échelles.
 * 
 * Le principe d’échange est très proche de celui de Twitter. Les messages sont envoyés par les publishers 
 * sur un canal appelé topic. Ces messages peuvent être lus par les subscribers (abonnés). Les topics 
 * (ou canaux d’informations) peuvent avoir une hiérarchie qui permet de sélectionner finement les 
 * informations que l’on désire.
 * 
 * Chaque client MQTT a une connexion ouverte en permanence avec le broker. Si la connexion s’arrête, le
 * broker bufférise les messages et les émet dès que la reconnexion avec le client est effectuée.
 * 
 * Voici quelques brokers :
 * - Eclipse Mosquitto 
 * - CloudMQTT 
 * - Thingsboard
 * - ThingMQ  
 * - ThingStudio 
 * - CloudMQTT 
 * - IBM Bluemix 
 * - Heroku 
 * - Hivemq 
 * - Microsoft Azure IoT 
 * - MaQiaTTo  
 * 
 * <H2>Topics</H2>
 * 
 * Un « topic MQTT » est une chaîne de caractères qui peut posséder une hiérarchie de niveaux séparés
 * par le caractère « / ». Par exemple, une information de température du salon pourrait être envoyée sur
 * le topic « maison/salon/temperature » et la température de la cuisine sur « maison/cuisine/temperature ».
 * Le signe « + » est un caractère « wildcard » qui permet des valeurs arbitraires pour une hiérarchie
 * particulière et le signe « # » pour plus d’un niveau. 
 * 
 * Par exemple le "topic" ‘/home/salon/temperature’ communiquera les températures du salon si un 
 * quelqu'un (object connecté ou application cliente) s’y abonne. La sonde de température présente dans 
 * le salon publiera régulièrement la température relevée sur ce topic.
 * Si un publisher s’abonne au topic ‘/home/salon/#’ il recevra toutes les données du salon (on peut 
 * imaginer : luminosité, humidité, température ...).
 * S’il s’abonne au topic ‘/home/#’, il collectera toutes les données des sondes de la maison.
 * 
 * <H2>Sécurité</H2>
 * 
 * MQTT permet de sécuriser les échanges à plusieurs niveaux :
 * - Transport en SSL/TLS,
 * - Authentification par certificats SSL/TLS,
 * - Authentification par login/mot de passe.
 * 
 * <H2>Qualité de Service (QoS)</H2>
 * 
 * MQTT intègre en natif la notion de QoS. En effet le publisher à la possibilité de définir la 
 * qualité de son message. Trois niveaux sont possibles :
 * - Un message de QoS niveau 0 « At most once » sera délivré tout au plus une fois. Ce qui 
 *   signifie que le message est envoyé sans garantie de réception, (le broker n’informe pas 
 *   l’expéditeur qu’il l’a reçu et le message)
 * - Un message de QoS niveau 1 « At least once » sera livré au moins une fois. Le client 
 *   transmettra plusieurs fois s’il le faut jusqu’à ce que le Broker lui confirme qu’il a 
 *   était transmis sur le réseau.
 * - Un message de QoS niveau 2 « exactly once » sera obligatoirement sauvegardé par l’émetteur 
 *   et le transmettra toujours tant que le récepteur ne confirme pas son envoi sur le réseau. 
 *   La principale différence étant que l’émetteur utilise une phase de reconnaissance plus 
 *   sophistiquée avec le broker pour éviter une duplication des messages (plus lent mais plus sûr).
 * 
 * <H2>Bibliothèques MQTT</H2>
 * 
 * Il existe de très nombreuses bibliothèques pour programmer des clients MQTT, et ce 
 * pour la plupart des langages (C, C++, Java, JavaScript, PHP, Python ...) et sur la 
 * plupart des plates-formes (GNU/Linux, Windows, iOS, Android, Arduino ...).
 * - Eclipse Paho : https://www.eclipse.org/paho/
 * - HiveMQ
 * - Eclipse Mosquitto
 * 
 * \note Même si on ne sait pas exactement comment, il semblerait que Facebook utilise 
 * des aspects de MQTT dans Facebook Messenger !
 * 
 * <H2>TP</H2>
 * 
 * Dans ce module, nous allons travailler avec la plateforme Thingsboard : https://thingsboard.io/
 * - Rendez vous sur la plateforme de démonstration : https://demo.thingsboard.io/login
 * - Créez vous un compte,
 * - Créez un dispositif avec le \b Nom et le \b Type \b de \b dispositif que vous souhaitez,
 * - Après avoir cliqué sur le nom de votre dispositif, récupérez le jeton d'accès de votre dispositif 
 *   depuis l'onglet \b Détails :
 *   - Copier le jeton d'accès ou 
 *   - Gérer les informations d'identification, en sélectionnant Access Token comme Type d'authentification
 * 
 * Ecrivez un programme qui assurera les fonctionnalités suivantes :
 * -# Connexion au serveur MQTT
 * -# Envoi de données de télémétrie simulées (v1/devices/me/telemetry) :
 *    - Dans la loop dans un premier temps
 *    - Via un Ticker avec un fréquence paramétrable ensuite
 * -# Affichage des données de télémétrie dans un tableau de bord
 * -# Envoi d'attributs de votre objet au broker à la connexion (v1/devices/me/attributes) :
 *    - Type de matériel : NodeMCU dans notre cas
 *    - Le numéro de série récupéré fourni par ESP.getChipId()
 *    - Version du firmware
 * -# Définissez un actuateur, par défaut la BUILTIN_LED sur la broche 2
 *    - Publier l'état de votre actuateur en attribut à la connexion
 * -# Gérer la réception des messages du broker
 *    - Abonnez vous à toutes les commandes RPC (v1/devices/me/rpc/request/+)
 *    - Implémentez une callback pour la réception des messages reçus
 *    - Implémentez 2 fonctions pour récupérer (getActuatorState) et pour changer (setActuatorState)
 *      l'état de votre actuateur
 *    - Ajouter sur votre tableau de bord un Control Widget de type Led Indicator pour afficher
 *      l'état de votre actuateur
 *    - Ajouter sur voter tableau de bord un Control Widget de type Switch Control pour pouvoir
 *      changer l'état de votre actuateur
 * 
 * Les bibliothèques à utiliser sont les suivantes (préférez l'installation des bibliothèques via l'IDE):
 * - PubSubClient by Nick O'Leary : https://pubsubclient.knolleary.net/ https://github.com/knolleary/pubsubclient/releases/tag/v2.7
 * - Arduino JSON by Benoit Blanchon : https://github.com/bblanchon/ArduinoJson
 * 
 * Pour démarrer :
 * - Exemple mqtt_esp8266 de la bibliothèque PubSubClient ou https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino
 * - Hello World Thingsboard : https://thingsboard.io/docs/getting-started-guides/helloworld/
 * - Thingsboard MQTT API (Telemetry, Attributes & RPC) : https://thingsboard.io/docs/reference/mqtt-api/
 * 
 * Fichier \ref MyMQTT.h
 */

#include <PubSubClient.h>                         // PubSubClient MQTT
#include <ESP8266WiFi.h>                          // ESP8266 WiFi
#include <ArduinoJson.h>                          // JSON

#define MQTT_CLIENTID  "MyNodeMCU"                // Identifiant MQTT
#define MQTT_TOKEN     "EOHShyDxOYLZWBXULdHg"     // TOKEN du dispositif 
#define MQTT_SERVER    "demo.thingsboard.io"      // Adresse IP ou URL du broker MQTT
#define MQTT_PORT      1883                       // Port MQTT
#define MQTT_FREQ      10                         // Fréquence en secondes d'envoi des données pour le ticker

WiFiClient    MyWiFiClient;                       // Instanciation d'un client web
PubSubClient  MyMqttClient(MyWiFiClient);         // Instanciation du client MQTT
Ticker        MyMqttTicker;                       // Ticker pour l'envoi régulier de données au broker
int           iMqttActuatorPin = 2;               // Broche à utiliser pour l'actuateur
String        strActuatorKey = "MyActuatorState"; // Nom de l'attribut pour l'actuateur

/**
 * Fonction de récupération de l'état de l'actuateur.
 * Elle renvoie la payload préparée
 */
String getActuatorStatus() {
  String strActuatorStatus;  
  StaticJsonDocument<200> payload;
  payload[strActuatorKey] = digitalRead(iMqttActuatorPin);
  serializeJson(payload, strActuatorStatus);
  MYDEBUG_PRINT("-MQTT : Get actuator Status : ");
  MYDEBUG_PRINTLN(strActuatorStatus);
  return strActuatorStatus;
}

/**
 * (Re)connexion au serveur MQTT
 * - Connexion au serveur
 * - Subscription aux requêtes RPC pour recevoir les commandes
 * - Publication des attributs du client à la connexion
 */
void reconnectMQTT(){
  if (WiFi.status() != WL_CONNECTED){setupWiFi();}                          // Vérification de la connexion WiFi
  while (!MyMqttClient.connected()) {                                       // Vérification de la connexion au serveur MQTT
    MYDEBUG_PRINT("-MQTT : Connexion au serveur MQTT ... ");
    if ( MyMqttClient.connect(MQTT_CLIENTID, MQTT_TOKEN, NULL) ) { // -------------- Connexion OK au serveur MQTT
      MYDEBUG_PRINTLN("[OK]");
      // ------------------------------------------------------------------- SOUSCRIPTION
      MYDEBUG_PRINTLN("-MQTT : Subscription");
      MyMqttClient.subscribe("v1/devices/me/rpc/request/+");                 // A toute les requêtes RPC
      // ------------------------------------------------------------------- PUBLICATION DES ATTRIBUTS
      MYDEBUG_PRINTLN("-MQTT : Publication des attributs");
      DynamicJsonDocument jsonPayload(512);                                  // Préparation de la Payload avec
      jsonPayload["firmwareVersion"] = FIRMWAREVERSION;                      // Version du firmware
      jsonPayload["thingType"] = THINGTYPE;                                  // Type d'objet
      jsonPayload["serialNumber"] = ESP.getChipId();                         // Le numéro de série
      jsonPayload[strActuatorKey] = digitalRead(iMqttActuatorPin);           // Etat de l'actuateur
                                                                             // PAYLOAD 
      String strAttributesPayload;                                           // Préparation de la payload Attributes JSON 
      serializeJson(jsonPayload, strAttributesPayload);                      // Sérialisation du JSON
      MyMqttClient.publish("v1/devices/me/attributes", getActuatorStatus().c_str()); // Publication de la payload
    } else { // ------------------------------------------------------------ Impossible de se connecter au serveur MQTT
      MYDEBUG_PRINT( "[ERREUR] [ rc = " );
      MYDEBUG_PRINT( MyMqttClient.state() );
      MYDEBUG_PRINTLN( " : Nouvelle tentative dans 5 sec]" );
      delay(5000);
    }
  }
}

/**
 * Récupération des données auprès des différents capteurs
 * puis envoi des données de télémétrie au serveur MQTT
 */
void getAndSendMqttData(){
  if (!MyMqttClient.connected() ) { reconnectMQTT(); }
  if (MyMqttClient.connected()){                                           // Si connecté alors on envoi les données
    // ------------------------------------------------------------------- RECUPERATION DES DONNEES
    int myRandomInt = 25+random(-5,5);                                     // Simulation de donnée Int
    float myRandomFloat = 20+(float)random(-100,100)/10;                   // Simulation de donnée Float
    int myRandomBool = rand() % 2;                                         // Simulation de donnée Bool

    // ------------------------------------------------------------------- PUBLICATION DES TELEMETRIES
    DynamicJsonDocument jsonPayload(512);                                  // Préparation de la Payload avec
    jsonPayload["MyTelemetryInt"] = myRandomInt;                           // Un entier
    jsonPayload["MyTelemetryFloat"] = myRandomFloat;                       // Un float
    jsonPayload["MyTelemetryBool"] = myRandomBool;                         // Un booléen
    jsonPayload["MyTelemetryString"] = "MyString";                         // Un String
    String strTelemetryPayload;                                            // Préparation de la payload Attributes JSON 
    serializeJson(jsonPayload, strTelemetryPayload);                       // Sérialisation du JSON
    MyMqttClient.publish("v1/devices/me/telemetry", strTelemetryPayload.c_str()); // Publication de la payload
    MYDEBUG_PRINT("-MQTT : Message envoyé [");
    MYDEBUG_PRINT(strTelemetryPayload);
    MYDEBUG_PRINTLN("]");
  }
}

/**
 * Fonction de traitement des messages reçus :
 * - Décodage de la payload réceptionnée au format JSON
 * - Récupération du nom de la méthode appelée
 * - Traitement spécifique à chaque méthode
 * - Envoi d'une réponse pour chaque message reçu
 * \note Si la valeur d'un attribut change, alors il est nécessaire de republier l'attribut auprès du broker
 * avec la nouvelle valeur. Message en plus de la réponse à la requête, cf. ci-dessus.
 */
void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<200> jsonDocument;                                 // Préparation du document JSON
  DeserializationError error = deserializeJson(jsonDocument, payload);  // Désérialisation de la Payload en JSON
  if (error) {
    MYDEBUG_PRINT("-MQTT : Parsing JSON IMPOSSIBLE");
    MYDEBUG_PRINTLN(error.c_str());
    return;
  }
#ifdef MYDEBUG
  String strMessage;
  serializeJson(jsonDocument, strMessage);
#endif
  MYDEBUG_PRINT("-MQTT : Message reçu / ");
  MYDEBUG_PRINT("Topic : ");
  MYDEBUG_PRINT(topic);
  MYDEBUG_PRINT(" / Message : ");
  MYDEBUG_PRINTLN(strMessage);

  String strMethod = jsonDocument["method"]; // Récupération du nom de la méthode
  MYDEBUG_PRINT("-MQTT : Méthode appelée : ");
  MYDEBUG_PRINTLN(strMethod);
  if(strMethod == "getActuatorState"){ // -------------------------------------- getActuatorState
      String responseTopic = String(topic);                                      // Préparation de la réponse
      responseTopic.replace("request", "response");                              // Remplacement de la request en response
      MyMqttClient.publish(responseTopic.c_str(), getActuatorStatus().c_str());  // Envoi de la réponse à la requête
  } else if (strMethod == "setActuatorState"){ // -------------------------------setActuatorState
      String strValue = jsonDocument["params"];                                  // Récupération de la valeur demandée
      String responseTopic = String(topic);                                      // Préparation de la réponse
      responseTopic.replace("request", "response");                              // Remplacement de la request en response
      MYDEBUG_PRINT("-MQTT : paramètre : ");
      MYDEBUG_PRINTLN(strValue);                                                 
      if (strValue.equals("true")){ digitalWrite(iMqttActuatorPin, HIGH); }
      else{ digitalWrite( iMqttActuatorPin, LOW); }
      MyMqttClient.publish(responseTopic.c_str(), getActuatorStatus().c_str());  // Envoi de la réponse à la requête
      MyMqttClient.publish("v1/devices/me/attributes", getActuatorStatus().c_str()); // Mise à jour de l'attributs de l'actuateur
  } else {
    MYDEBUG_PRINT("-MQTT : Méthode inconnue");
  }
}

/**
 * Initialisation des éléments MQTT
 * - Serveur et port du broker MQTT
 * - La fonction callback appelée lors de la réception de messages
 * - Un ticker pour l'emission régulière de données vers le broker
 * - L'initialisation d'un actuateur
 */
void setupMQTT(){
  MYDEBUG_PRINT("-MQTT : Initialisation Serveur : ");
  MYDEBUG_PRINT(MQTT_SERVER);
  MYDEBUG_PRINT(" / Port : ");
  MYDEBUG_PRINTLN(MQTT_PORT);

  MyMqttClient.setServer(MQTT_SERVER, MQTT_PORT);      // Configuration de la connexion au serveur MQTT
  MyMqttClient.setCallback(onMqttMessage);             // La fonction de callback qui est executée à chaque réception de message
  MyMqttTicker.attach(MQTT_FREQ, getAndSendMqttData);  // Envoi régulier des données à l'aide d'un Ticker

  pinMode(iMqttActuatorPin, OUTPUT);                       // Configuration de l'actuateur
  digitalWrite(iMqttActuatorPin, LOW);                     // Initialisation de l'actuateur à LOW
}

/**
 * Boucle MQTT où il n'y a pas grand chose à faire à part attendre et traiter les messages reçus.
 */
void loopMQTT(){
  // Vérification de l'état de la connexion au serveur MQTT
  if ( !MyMqttClient.connected() ) { reconnectMQTT(); }
  MyMqttClient.loop();
}
