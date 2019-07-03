/**
 * \file MyDHT.h
 * \page dht Capteur de température et d'humidité
 * \brief Capteur numérique Digital Humidity & Temperature (DHT)
 * 
 * Adafruit Unified Sensor Driver https://github.com/adafruit/Adafruit_Sensor
 * Librairies à installer :
 * - DHT sensor library by Adafruit
 * - Adafruit Unified Sensor by Adafruit
 * 
 * Délai entre 2 mesures : il dépend du capteur, si on n'a pas l'information (via la librairie unifiée) on le configure à 5s
 * 
 * Fichier \ref MyDHT.h
 */

#define DHT_U 0                        // 0 si DHT "classique" / 1 pour DHT Unified Sensor

#include "DHT.h"                       // https://github.com/adafruit/DHT-sensor-library
#include <DHT_U.h>                     // https://github.com/adafruit/Adafruit_Sensor

#define DHT_PIN    D2                  // PIN numérique du DHT
#define DHT_TYPE   DHT11               // Type de capteur DHT : DHT11 ou DHT22

#if DHT_U
DHT_Unified myDht(DHT_PIN, DHT_TYPE); // Instantiation du DHT Unified
#else
DHT myDht(DHT_PIN, DHT_TYPE);         // Instantiation du DHT "classique"
#endif
uint32_t delayMS = 5000;              // Délai entre 2 mesures

float fHumidity=0;
float fTemperature=0;


void setupDhtSensor(){
  myDht.begin();

#if DHT_U
  MYDEBUG_PRINTLN("DHT Unified Sensor");
  // Information sur le capteur de température
  sensor_t sensor;
  myDht.temperature().getSensor(&sensor);
  MYDEBUG_PRINTLN("------------------------------------");
  MYDEBUG_PRINTLN("Temperature");
  MYDEBUG_PRINT  ("Sensor:       "); MYDEBUG_PRINTLN(sensor.name);
  MYDEBUG_PRINT  ("Driver Ver:   "); MYDEBUG_PRINTLN(sensor.version);
  MYDEBUG_PRINT  ("Unique ID:    "); MYDEBUG_PRINTLN(sensor.sensor_id);
  MYDEBUG_PRINT  ("Max Value:    "); MYDEBUG_PRINT(sensor.max_value); MYDEBUG_PRINTLN(" *C");
  MYDEBUG_PRINT  ("Min Value:    "); MYDEBUG_PRINT(sensor.min_value); MYDEBUG_PRINTLN(" *C");
  MYDEBUG_PRINT  ("Resolution:   "); MYDEBUG_PRINT(sensor.resolution); MYDEBUG_PRINTLN(" *C");  
  MYDEBUG_PRINTLN("------------------------------------");
  // Information sur le capteur de température
  myDht.humidity().getSensor(&sensor);
  MYDEBUG_PRINTLN("------------------------------------");
  MYDEBUG_PRINTLN("Humidity");
  MYDEBUG_PRINT  ("Sensor:       "); MYDEBUG_PRINTLN(sensor.name);
  MYDEBUG_PRINT  ("Driver Ver:   "); MYDEBUG_PRINTLN(sensor.version);
  MYDEBUG_PRINT  ("Unique ID:    "); MYDEBUG_PRINTLN(sensor.sensor_id);
  MYDEBUG_PRINT  ("Max Value:    "); MYDEBUG_PRINT(sensor.max_value); MYDEBUG_PRINTLN("%");
  MYDEBUG_PRINT  ("Min Value:    "); MYDEBUG_PRINT(sensor.min_value); MYDEBUG_PRINTLN("%");
  MYDEBUG_PRINT  ("Resolution:   "); MYDEBUG_PRINT(sensor.resolution); MYDEBUG_PRINTLN("%");  
  MYDEBUG_PRINTLN("------------------------------------");
  delayMS = sensor.min_delay/1000;
  MYDEBUG_PRINT  ("Delay:   "); MYDEBUG_PRINT(delayMS); MYDEBUG_PRINTLN(" ms");  
  MYDEBUG_PRINTLN("------------------------------------");
#endif
}

void getDhtData(){

  // Délai minimum entre 2 mesures
  MYDEBUG_PRINT("-DHT : Délai entre 2 mesure [");
  MYDEBUG_PRINT(delayMS);
  MYDEBUG_PRINTLN("] ms");
  delay(delayMS);

#if DHT_U
  // Get temperature event and print its value.
  sensors_event_t event;  
  myDht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    MYDEBUG_PRINTLN("-DHT : Erreur de lecture de la température du capteur DHT !");
  }
  else {
    MYDEBUG_PRINT("-DHT : [");
    MYDEBUG_PRINT(event.temperature);
    MYDEBUG_PRINTLN("°C] température !");
  }
  // Get humidity event and print its value.
  myDht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    MYDEBUG_PRINTLN("-DHT : Erreur de lecture de l'humidité du capteur DHT !");
  }
  else {
    MYDEBUG_PRINT("-DHT : [");
    MYDEBUG_PRINT(event.relative_humidity);
    MYDEBUG_PRINTLN("%] Humidité !");
  }
#else
  // Lecture des données sur le capteur
  fHumidity = myDht.readHumidity();
  fTemperature = myDht.readTemperature();

  // Vérification que la lecture est correcte
  if (isnan(fHumidity) || isnan(fTemperature)) {
    MYDEBUG_PRINTLN("-DHT : Erreur de lecture du capteur DHT !");
    return;
  }
  // Affichage
  MYDEBUG_PRINT("-DHT : [");
  MYDEBUG_PRINT(fHumidity);
  MYDEBUG_PRINTLN("%] humidité !");
  MYDEBUG_PRINT("-DHT : [");
  MYDEBUG_PRINT(fTemperature);
  MYDEBUG_PRINTLN("°C] température !");
#endif
}
