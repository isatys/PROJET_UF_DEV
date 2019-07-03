/**
 * \file MySoilSensor.h
 * \page soilsensor Capteur d'humidité du sol
 * \brief Capteur analogique et numérique
 * 
 * 
 * Fichier \ref MySoilSensor.h
 */

#define SOIL_DIGITAL_PIN  D0      // PIN numérique de l'information de sécheresse
#define SOIL_ANALOG_PIN   A0      // PIN digitale du niveau d'humidité du sol
#define MOISTURE_HIGH     1024    // Calibration du capteur : valeur haute (sec)
#define MOISTURE_LOW      0       // Calibration du capteur : valeur basse (dans l'eau)
int iSoilMoisture;                // Taux d'humidité du sol, mesure analogique
int iDigitalThreshold;            // 0 ou 1 si le seuil défini par le potentiomètre est atteint

void setupSoilSensor(){
  pinMode(SOIL_ANALOG_PIN, INPUT);
  pinMode(SOIL_DIGITAL_PIN, INPUT);
}

void getSoilData(){
  // MOISTURE - ANALOG
  iSoilMoisture = analogRead(SOIL_ANALOG_PIN);
  iSoilMoisture = map(iSoilMoisture,MOISTURE_HIGH,MOISTURE_LOW,0,100);
  MYDEBUG_PRINT("-SOL : [");
  MYDEBUG_PRINT(iSoilMoisture);
  MYDEBUG_PRINTLN("%] humidité !");  

  // THRESHOLD - DIGITAL
  iDigitalThreshold = digitalRead(SOIL_DIGITAL_PIN);
  if(iDigitalThreshold == HIGH){
    MYDEBUG_PRINTLN("-SOL : Le sol est sec !");
  } else {
    MYDEBUG_PRINTLN("-SOL : Le sol n'est pas trop sec");
  }
}
