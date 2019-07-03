/**
 * \file MyWebServer.h
 * \page webserver Serveur Web
 * \brief Un serveur web dans un NodeMCU
 * 
 * Un serveur web qui reçoit des requêtes HTTP et y répond.
 * Il propose une interface affichant l'état d'un actuateur et un bouton pour en changer l'état.
 * 
 * Fichier \ref MyWebServer.h
 */

WiFiServer server(80);                  // Serveur web sur le port 80
String header;                          // Requête HTTP 
String strActuatorState = "off";        // Etat de l'actuateur
const int iActuatorPin = D4;            // Pin de l'actuateur

/**
 * Initialisation du serveur web
 */
void setupWebServer(){
  // Initialisation de la broche pour l'actuateur
  pinMode(iActuatorPin, OUTPUT);                    // Mode Output
  digitalWrite(iActuatorPin, LOW);                  // Initialisation à LOW

  // On a besoin d'une connexion WiFi !
  if (WiFi.status() != WL_CONNECTED){setupWiFi();}  // Connexion WiFi
  MYDEBUG_PRINTLN("-Web Server : Démarrage");
  server.begin();                                   // Démarrage du serveur
}

/**
 * Traitement des requêtes HTTP reçues par le serveur
 */
void loopWebServer(){
  WiFiClient client = server.available();           // Attente de clients
  if (client) {                                     // Si un nouveau client se connecte
    MYDEBUG_PRINTLN("-Web Server : Nouveau client");
    String currentLine = "";                        // Initialisation d'un String pour les données du client
    while (client.connected()) {                    // Boucle tant que le client est connecté
      if (client.available()) {                     // S'il y a des données du client
        char c = client.read();                     // Lecture d'un octet
        header += c;                                // Stockage dans le header
        if (c == '\n') {                            // Si l'octet est un retour à la ligne
          // Si la ligne courante est vide, on a deux caractères de fin de ligne
          // on identifie donc la fin de la requête HTTP, il faut donc désormais répondre
          if (currentLine.length() == 0) {
            // Construction de la réponse avec le HTTP header qui commence par la code retour
            // (e.g. HTTP/1.1 200 OK) et une description du contenu pour que le client sache ce
            // qui lui arrive, et finalement une ligne vide pour indiquer la fin
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Analyse de la requête pour savoir si une action a été demandée
            if (header.indexOf("GET /LED/on") >= 0) {
              MYDEBUG_PRINTLN("-Web Server : Actuateur ON");
              strActuatorState = "on";
              digitalWrite(iActuatorPin, HIGH);
            } else if (header.indexOf("GET /LED/off") >= 0) {
              MYDEBUG_PRINTLN("-Web Server : Actuateur OFF");
              strActuatorState = "off";
              digitalWrite(iActuatorPin, LOW);
            }
            
            // Affichage de la page web
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>My Web Server</h1>");
            client.println("<p>Etat de l'actuateur : " + strActuatorState + "</p>");
            // Affichage du bouton selon l'état de l'actuateur
            if (strActuatorState=="off") {
              client.println("<p><a href=\"/LED/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/LED/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("</body></html>");
            // On termine avec une ligne vide pour indiquer la fin de la réponse            
            client.println();
            break;
          } else { // C'est une fin de ligne, on "nettoye" la ligne courante
            currentLine = "";
          }
        } else if (c != '\r') {  // C'est autre chose qu'un retour à la ligne
          currentLine += c;      // alors on l'ajoute à la ligne courante
        }
      }
    }
    // "Nettoyage" du header
    header = "";
    // Fermeture de la connection client
    client.stop();
    MYDEBUG_PRINTLN("-Web Server : Client déconnecté");
  }
}
