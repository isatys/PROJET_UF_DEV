/**
 * \file MyNodeMCU.h
 * \page nodemcu NodeMCU vs Arduino
 * \brief Définition des broches NodeMCU sur IDE Arduino
 * 
 * Le NodeMCU n'est pas originellement pensé pour être utilisé avec l'IDE Arduino. 
 * La correspondance du nom des broches avec leur numéro associé n'est pas la même.
 * 
 * Ainsi nous allons procéder à aux correspondances afin de pouvoir utiliser les 
 * noms des broches inscrites sur la carte et ainsi s'éviter d'éventuels déconvenues 
 * à devoir chercher sur quelle broche est branché notre capteur ou actuateur.
 * 
 * \note La Led de l'ESP8266 est cablée du la broche 2 et les polarités inversées :
 * Le niveau HIGH éteint la LED et inversement, pour ne pas confondre j'imagine.
 * 
 * Fichier \ref MyNodeMCU.h
 */
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define PIN_WIRE_SDA (4)
#define PIN_WIRE_SCL (5)

static const uint8_t SDA = PIN_WIRE_SDA;
static const uint8_t SCL = PIN_WIRE_SCL;

#define LED_BUILTIN 2 /*!< A noter que la Led de l'ESP8266 est cablée du la broche 2 et les polarités inversées*/

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

#endif /* Pins_Arduino_h */
