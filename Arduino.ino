//Inclure les bibliothèques
#include <ESP8266WiFi.h>
#include "DHT.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


//Parametre WIFI
const char* ssid = "Freebox-695429";  // Nom du réseau
const char* password = "semodium*-forensia-necandi-visitarem&%";  //Mot de passe

//Parametre Adafruit
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "andiore"
#define AIO_KEY         "a8dbad903d5848ebad0c187860b5247b"


//Soil Sensor
int sensor_pin = A0;  
int sensorValue = 0;  
int percent = 0;

DHT dht2(2, DHT11);

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish humidite = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidite");

void setup() {
       Serial.begin(9600);
       delay(10);
               
       Serial.println("Connexion a : ");
       Serial.println(ssid); 
 
       WiFi.begin(ssid, password); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println(F("addresse IP: "));
      Serial.println(WiFi.localIP());
      connect();
      Serial.println("WiFi connecte!"); 
}

void loop() {
    if(! mqtt.ping(3)) {
    // Reconnnecte à Adafruit
    if(! mqtt.connected())
      connect();
  }
  int temperature_data = Serial.println("Temperature en °C: ");
  Serial.println((dht2.readTemperature()));
  int humidity_data = Serial.println("Humidite en %: "); 
  Serial.println((dht2.readHumidity()));
  
  float moisture_percentage;
    int sensor_analog;
    sensor_analog = analogRead(sensor_pin);
    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
    int humidite_data = Serial.println("Humidité du sol en % = ");
    Serial.print(moisture_percentage);
    Serial.print("%\n\n");
    
  delay(5000); 
  //Publier les data
  if (! temperature.publish(temperature_data))
    Serial.println(F("Echec de publication de la temperature"));
  else
    Serial.println(F("Temperature publie!"));

  if (! humidity.publish(humidity_data))
    Serial.println(F("Echec de publication de l'humidite"));
  else
    Serial.println(F("Humidite publie!"));

  if (! humidite.publish(humidite_data))
    Serial.println(F("Echec de publication de l'humidite du sol"));
  else
    Serial.println(F("Humidite du sol publie!"));
  delay(5000);
  
  // Recommence toutes les 5 secondes
  
}


// Se connecter à adafruit via MQTT
void connect() {

  Serial.print(F("Connexion à Adafruit IO..."));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Mauvais protocole")); break;
      case 2: Serial.println(F("ID rejete")); break;
      case 3: Serial.println(F("Server indisponible")); break;
      case 4: Serial.println(F("Mauvais utilisateur/MDP")); break;
      case 5: Serial.println(F("Non autorisé")); break;
      case 6: Serial.println(F("Echec a subscribe")); break;
      default: Serial.println(F("Connection echoue")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Tentative de reconnexion..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connecte!"));

}
