/**
 * \file MyTicker.h
 * \page tickers Tickers
 * \brief Repassez dans 5 minutes s'il vous plaît
 * 
 * Un Ticker est un objet qui est appelé à une fréquence définie.
 * Chaque Ticker est associé à une fonction.
 * On peut définir autant de Ticker qu'on le souhaite, dans la limite de la mémoire.
 * On peut attacher et détacher une fonction à un Ticker.
 * Il y a 2 façons d'attacher une fonction avec attach(secondes) et attach_ms(milliseconds)
 * 
 * Fichier \ref MyTicker.h
 */
 
#include <Ticker.h>
#define TICKER_FUNCTION_PIN D4

Ticker myTicker;

int count = 0;
unsigned long myTickerTime;

void tickerFunction() {
  MYDEBUG_PRINT("-Ticker : Since last call :");
  MYDEBUG_PRINT(micros()-myTickerTime);
  MYDEBUG_PRINTLN(" ms");
  myTickerTime = micros();
  
  int state = digitalRead(TICKER_FUNCTION_PIN);  // Lecture de l'état
  digitalWrite(TICKER_FUNCTION_PIN, !state);     // set pin to the opposite state

  ++count;
  // when the counter reaches a certain value, start blinking like crazy
  if (count == 100) {
    myTicker.attach(0.1, tickerFunction);
  } else if (count == 200) {
    digitalWrite(TICKER_FUNCTION_PIN, HIGH);
    myTicker.detach();
  }

  MYDEBUG_PRINT("-Ticker : Duration :");
  MYDEBUG_PRINT(micros()-myTickerTime);
  MYDEBUG_PRINTLN(" ms");
}

void setupTicker(){
  pinMode(TICKER_FUNCTION_PIN, OUTPUT);
  digitalWrite(TICKER_FUNCTION_PIN, LOW);
  myTicker.attach(0.5, tickerFunction);         // Association du Ticker avec une fonction
  myTickerTime = micros();
}
