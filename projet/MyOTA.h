/**
 * \file MyOTA.h
 * \page ota Over The Air & Remote Debug
 * \brief Mise à jour du firmware sans fil
 * 
 * Jusqu'ici notre objet connecté est resté relité à notre ordinateur via un câble USB pour
 * le téléversement du code et l'accès aux messages, le tout via le port série. Très bien mais
 * un objet connecté a vocation à être débranché du câble USB qui le relie à un ordinateur et
 * vivre sa vie à distance et en tout autonomie.
 * 
 * Toutefois, une fois dans la nature, avec une alimentation électrique et le réseau WiFi,
 * comme réaliser des mises à jour du firmware sans port série ? Avec la bibliothèque <b>Over 
 * The Air</b> ! Cette dernière permet d'ajouter des ports réseau en plus des ports série
 * et de téléverser via l'IDE Arduino de la même façon.
 * 
 * \note Avant de téléverser ce programme pour la première fois via la liaison série, vérifier 
 * dans le menu "Outils" que la "Flash size" sélectionnée correspond bien à votre matériel. 
 * Classiquement pour un ESP8266 NodeMCU, il faut renseigner 4M (1M SPIFFS); on maximise
 * ainsi l'espace disponible pour OTA, cf. \ref spiffs .
 * 
 * Pour éviter que toute personne ayant accès au réseau WiFi sur lequel est branché votre
 * objet puisse téléverser un nouveau firmware, il est préférable d'ajouter un peu de sécurité 
 * en demandant un mot de passe lors des mises à jour.
 * 
 * S'affranchir de la liaison série est appréciable, toutefois on perd en même temps une
 * fonctionnalité très utile : l'affichage des messages de debug !
 * Une solution pour palier à ce problème est l'utilisation de la bibliothèque RemoteDebug qui 
 * permet de se connecter en telnet à notre ESP8266 et de récupérer les messages par ce biais.
 * 
 * \note \b Tous les messages de debug qui sont présents dans votre code doivent être modifiés
 * pour appeler les fonctions de génération de debug de la bibliothèque RemoteDebug. 
 * Il existe un convertisseur, mais celui ci ne fonctionne que sur les fichier avec l'extension 
 * .ino : https://github.com/JoaoLopesF/RemoteDebugConverter
 * 
 * Il y a possibilités de créer des messages avec différents niveaux de logs, les voici par niveau 
 * de détail :
 * - Verbose
 * - Debug
 * - Info
 * - Warnings
 * - Errors
 * Quand vous sélectionner un niveau de détail, tous les messages de niveau "inférieur" sont
 * également visibles.
 * 
 * Une fois ce firmware téléversé:
 * - Dans le menu "Outils/Port", vérifier que le nouveau port <OTA_HOSTNAME> at <IP ADDRESS> 
 *   apparaît dans la liste des "Ports réseau".
 * - Si tel est le cas, vous pouvez débrancher votre objet du port série de votre ordinateur
 *   et le brancher à une autre source d'alimentation.
 * - Il n'apparait donc plus dans la liste de vos "Ports série" mais est toujours présent dans
 *   les "Ports réseau".
 * - Vous pouvez désormais téléversez vos nouveaux firmwares \b Over \b The \b Air.
 * - Pour accéder aux logs, 2 possibilités :
 *   - Ouvrez un terminal et saisissez "telnet <adresse IP>" pour y voir
 *     apparaître vos messages de logs.
 *   - Remote Debug embarque une bibliothèque de web socket qui permet d'accéder aux logs.
 *     Ouvrez la Web app d'affichage des logs : http://joaolopesf.net/remotedebugapp/ ou en
 *     la téléchargeant sur votre ordinateur : https://github.com/JoaoLopesF/RemoteDebugApp
 * 
 * Bibliothèques à installer pour utiliser ce module :
 * - Arduino OTA by Arduino Juraj Andrassy : https://github.com/jandrassy/ArduinoOTA
 * - RemoteDebug by Joao Lopez : https://github.com/JoaoLopesF/RemoteDebug
 * 
 * Fichier \ref MyOTA.h
 */

#include <ArduinoOTA.h>
#include <RemoteDebug.h>
#include <ESP8266WebServer.h>

#define OTA_HOSTNAME  "My NodeMCU"
#define OTA_PASSWORD  "MyPassword"

RemoteDebug Debug;
ESP8266WebServer HTTPServer(80);
Ticker debugTicker;

/**
 * Fonction appelée par un ticker pour générer des messages de debug à différents niveaux
 */
void generateDebugLog(){
  rdebugVln("-Remote DEBUG : Message VERBOSE");
  rdebugDln("-Remote DEBUG : Message DEBUG");
  rdebugIln("-Remote DEBUG : Message INFO");
  rdebugWln("-Remote DEBUG : Message WARNING");
  rdebugEln("-Remote DEBUG : Message ERROR");
}

void handleRoot() {
  // Page web Root
  String strRootWebPage;
  strRootWebPage += ("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  strRootWebPage += ("<link rel=\"icon\" href=\"data:,\">");
  strRootWebPage += ("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  strRootWebPage += (".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  strRootWebPage += ("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  strRootWebPage += (".button2 {background-color: #77878A;}</style></head>");
  strRootWebPage += ("<body><h1>OTA & Remote Debug</h1>");
  strRootWebPage += ("<p><a href=\"http://joaolopesf.net/remotedebugapp/\">Remote Debug Web App</a></p>");
  HTTPServer.send(200, "text/html", strRootWebPage);

//  HTTPServer.send(200, "text/plain", "hello from esp - RemoteDebug Sample!");
}

void handleNotFound(){
   // Page not Found
   String message = "File Not Found\n\n";
   message.concat("URI: ");
   message.concat(HTTPServer.uri());
   message.concat("\nMethod: ");
   message.concat((HTTPServer.method() == HTTP_GET)?"GET":"POST");
   message.concat("\nArguments: ");
   message.concat(HTTPServer.args());
   message.concat("\n");
   for (uint8_t i=0; i<HTTPServer.args(); i++){
       message.concat(" " + HTTPServer.argName(i) + ": " + HTTPServer.arg(i) + "\n");
   }
   HTTPServer.send(404, "text/plain", message);
}

/**
 * Configuration et démarrage des services OTA & Remote Debug
 */
void setupOTA(){
  // On a besoin d'une connexion WiFi !
  if (WiFi.status() != WL_CONNECTED){setupWiFi();}  // Connexion WiFi

  // Démarrage d'OTA
  MYDEBUG_PRINTLN("-OTA : Démarrage");
  ArduinoOTA.setHostname(OTA_HOSTNAME); // Nommage pour l'identification
  ArduinoOTA.setPassword(OTA_PASSWORD); // Mot de passe pour les téléversements
  ArduinoOTA.begin();                   // Initialisation de l'OTA

  // Initialisation de la librairie RemoteDebug
  Debug.begin(OTA_HOSTNAME); 
  Debug.setPassword(OTA_PASSWORD);      // Mot de passe pour les connexions telnet et webapp
  Debug.setResetCmdEnabled(true);       // Pour permettre le Reset par telnet
  Debug.showColors(true);               // Un peu de couleurs pour faire joli
  //Debug.setSerialEnabled(true);       // Pour activer un écho des logs sur le port série (si branché)

  HTTPServer.on("/", handleRoot);
  HTTPServer.onNotFound(handleNotFound);
  HTTPServer.begin();

  // Ticker pour générer des logs
  debugTicker.attach(2, generateDebugLog);
}

/**
 * A chaque itération, on verifie si une mise a jour nous est envoyée.
 * Si tel est cas, la bibliothèque ArduinoOTA se charge de tout !
 */
void loopOTA(){
  ArduinoOTA.handle();          // Gestion des demandes de téléversement
  Debug.handle();               // Gestion des messages de remote debug
  HTTPServer.handleClient();    // Gestion du serveur web
}
