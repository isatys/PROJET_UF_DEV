/**
 * \file MyDeepSleep.h
 * \page deepsleep Deep Sleep
 * \brief Optimisation de la consommation électrique
 * 
 * Nous allons chercher à économiser l'énergie et ainsi prolonger la durée de vie de la batterie d'un objet connecté.
 * 
 * L'ESP8266 dispose d'un mode "Deep Sleep" qui permet de le mettre dans différents modes de veille.
 * - Modem-Sleep : Ce mode est utilisé si votre objet utilise le processeur (PWM ou I2S) mais ne requiert pas de 
 *   communication continue. Le modem Wi-Fi est arrêté tout en maintenant quand même une connexion Wi-Fi sans 
 *   transmission de données, juste pour optimiser la consommation d’énergie. Il existe 3 modes : DTIM1 à 3. Par 
 *   exemple dans le mode DTIM3, le modem est mis en sommeil durant 300 ms puis activé durant 3 ms pour maintenir
 *   la connexion. Ce mode de fonctionnement est conforme à la norme 802.11 (U-APSD). 
 * - Light-Sleep : En plus de la suspension du modem WiFI, le CPU est mis en attente et réactivé si nécessaire.
 * - Deep-Sleep : Le mode le plus simple à mettre en oeuvre et le plus économe en énergie pour des projets qui 
 *   fonctionnent sur batterie.
 * 
 * L'ESP8266 peut être réveillé :
 * - A intervalle régulier pour faire des mesures et les publier,
 * - A l'aide de la broche Reset(RST) relié à un bouton,
 * - En utilisant le signal d'un capteur (détecteur de mouvement - PIR), par exemple. 
 * 
 * Cablage
 * Le mode Deep-sleep est activé en reliant la broche RST à la broche Wake (D0 d'un NodeMCU). A noter donc que cette
 * broche n’est donc plus disponible pour un autre usage.
 * 
 * Point d'attention
 * Une fois que les broches D0 et RST sont reliées, il n’est plus possible de téléverser un programme. Il faut alors
 * déconnecter la broche RST avant de pouvoir téléverser un programme. Le mieux est de prévoir un jumper.
 * La connexion WiFi étant arrêté, voir ne pourrez plus accéder à une interface WEB de configuration. Il faudra prévoir 
 * une condition dans votre programme pour désactiver le mode deep-sleep.
 * 
 * Code 
 * Pour endormir notre ESP8266, nous utilisons :
 * ESP.deepsleep(durée, Le mode du WiFi au redémarrage);
 * - Durée : La durée de mise en veille en microsecondes. Si on souhaite réveiller l'ESP8266 manuellement ou avec 
 *   une action extérieure, par exemple un interrupteur relié sur la broche RST, il suffit d’indiquer un temps nul (0).
 * - Mode du WiFi au réveil :
 *   - RF_DEFAULT, qui est équivalent est RF_CAL. Le modem WiFi est activé normalement à son réveil.
 *   - RF_NO_CAL (NO CALibration) est une version alternative moins consommatrice en énergie. Le modem WiFi 
 *     est activé mais sans qu’aucun étalonnage du signal radio soit effectué.
 *   - RF_DISABLED si vous n’avez pas besoin de la connexion WiFi on peut garder le modem WiFi en veille. 
 *     Attention, si ce mode est activé, il devient impossible de se connecter au réseau WiFi.
 * Quand il se réveille, l'ESP8266 redémarre comme lorsqu'il est allumé. Le code est donc exécuté depuis le début.
 * Ce n'est pas un mode veille qui vous permet de récupérer un contexte, on redémarre depuis le setup().
    
 * Documentation officielle : https://www.espressif.com/sites/default/files/9b-esp8266-low_power_solutions_en_0.pdf
 * 
 * Fichier \ref MyDeepSleep.h
 */

#define NB_TRYWIFI      20 // Nombre maximum de tentatives de connextion WiFi
#define DURATION_SLEEP  5 // Durée de la période de sommeil

/**
 * Dans cet exemple nous allons, à l'initialisation, tenter de se connecter au réseau WiFi.
 * Si au bout de NB_TRYWIFI nous n'y sommes pas arrivés alors nous allons nous "endormir", en espérant que le
 * réseau soit de nouveau disponible à notre réveil !
 */
void setupDeepSleep(){
  int iNbTry = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    MYDEBUG_PRINT(".");
    if (iNbTry++ >= NB_TRYWIFI) {
      MYDEBUG_PRINTLN("Impossible de se connecter au WiFi, je vais faire la sieste !");
      ESP.deepSleep(DURATION_SLEEP * 1000000, RF_CAL);  // Durée en millisecondes
    }
    delay(500);
  }

  MYDEBUG_PRINTLN(" WiFi connected");
  MYDEBUG_PRINT("IP address: ");
  MYDEBUG_PRINTLN(WiFi.localIP());
}
