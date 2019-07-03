/**
 * \file MyPwm.h
 * \page pwm PWM 
 * \brief La modulation de largeur d'impulsions ou Pulse Width Modulation (PWM)
 * 
 * La modulation de largeur d'impulsions
 * 
 * Fichier \ref MyPwm.h
 */

#define LedPin D4            // D4 correspond également à la LED de l'ESP8266 (HIGH & LOW inversés)
int iPWM = 0;                // Variable de stockage de la fréquence PWM

void setupPwm(){
  pinMode(LedPin, OUTPUT);   // Configuration de la LedPin en sortie
  digitalWrite(LedPin, LOW); // Initialisation de l'état de la LED
}

void playWithPwm(){
  MYDEBUG_PRINTLN("- PWM : clignote");
  for (int i=0; i<10; i++){
    MYDEBUG_PRINT("- PWM : [");
    MYDEBUG_PRINT(i);
    MYDEBUG_PRINTLN("]");
    digitalWrite(LedPin, HIGH);
    delay(500);
    digitalWrite(LedPin, LOW);    
    delay(500);
  }

  // On augmente la fréquence PWN de 0 au Maximum
  // Pour la valeur maximale on utilisera PWMRANGE
  MYDEBUG_PRINTLN("- PWM : crescendo");
  for(int i=0; i<PWMRANGE; i++){
    MYDEBUG_PRINT("- PWM : [");
    MYDEBUG_PRINT(i);
    MYDEBUG_PRINTLN("]");
    analogWrite(LedPin, i);
    delay(10);
  }
  // On diminue la fréquence PWN du maximum à 0
  MYDEBUG_PRINTLN("- PWM : decrescendo");
  for(int i=PWMRANGE; i>0; i--){
    MYDEBUG_PRINT("- PWM : [");
    MYDEBUG_PRINT(i);
    MYDEBUG_PRINTLN("]");
    analogWrite(LedPin, i);
    delay(10);
  }
}
