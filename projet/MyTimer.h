/**
 * \file MyTimer.h
 * \page timers Timers
 * \brief Toc, toc, toc ! Qui est là ?
 * 
 * Vous êtes au téléphone et quelqu'un sonne à votre porte.
 * - "Attends 2 secondes, quelqu'un frappe à la porte, je te reprends".
 * C'est un livreur qui vous apporte un colis. Vous le réceptionnez, signez le reçu.
 * Vous refermez la porte et vous reprenez votre téléphone pour continuer votre discussion.
 * 
 * Lorsque le livreur a sonné à votre porte, il vous a interrompu. Il a vous a fait tout
 * simplement arrêté votre programme principal, votre discussion téléphonique pour réaliser
 * le sous-programme OpenTheDoor().
 * Vous n'avez repris votre programme principal là où vous vous étiez arrêté, une fois que 
 * vous avez terminé la fonction d'interruption.
 * 
 * Sur un NodeMCU, le Hardware Timer0 est utilisé pour les fonctions du module WiFi.
 * On ne peut utiliser que le Timer1. 
 * L'utilisation d'un Timer au lieu d'un Ticker donne plus de précision dans le démarrage de
 * la fonction d'interruption.
 * 
 * Fichier \ref MyTimer.h
 */

#define TIMER_PIN D4
unsigned long myTimerTime;

void ICACHE_RAM_ATTR onTimerISR(){
  MYDEBUG_PRINT("-Timer : Since last call :");
  MYDEBUG_PRINT(micros()-myTimerTime);
  MYDEBUG_PRINT(" ms");
  myTimerTime = micros();

  int state = digitalRead(TIMER_PIN);  // Lecture de l'état
  digitalWrite(TIMER_PIN, !state);     // Allume ou éteint la LED
  timer1_write(6000000);               // Ré-enclement du Timer à 1,2 s

  MYDEBUG_PRINT(" / Duration :");
  MYDEBUG_PRINT(micros()-myTimerTime);
  MYDEBUG_PRINTLN(" ms");
}

void setupTimer(){
  pinMode(TICKER_FUNCTION_PIN, OUTPUT);
  digitalWrite(TICKER_FUNCTION_PIN, LOW);

  noInterrupts();                                 // Désactivation des interruptions
  timer1_attachInterrupt(onTimerISR);             // Association du Timer avec une fonction
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE); // Activation du Timer
  timer1_write(6000000);                          // Configuration du Timer à 1 200 000 us
  myTimerTime = micros();
  interrupts();                                   // Réactivation des interruptions
}
