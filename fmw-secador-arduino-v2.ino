#include <Arduino.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Preferences.h>
#include <Nextion.h>

#define HIST_ENTR 1
#define HIST_MASS 1

typedef enum {
  TEMP_ENTR,
  TEMP_MASS,
  MAX_ENTR,
  MIN_ENTR,
  MAX_MASS,
  MIN_MASS,
  PALHA_LENHA,
  LAST_COMM,
  HAS_COMM_ONCE,
  IS_AWARE_ENTR,
  IS_AWARE_MASS
} state_prefs_t;

void setup()
{
  Serial.begin(115200);
  state_manager_setup();
  peripherals_setup();
  lcd_setup();

  /*
   * O WebServer PRECISA rodar em um core separado, pois se suas respostas demorarem, a conexao é imediatamente quebrada
   */
  xTaskCreatePinnedToCore(
      server_task, /* Function to implement the task */
      "Task1", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      5,  /* Priority of the task */
      NULL,  /* Task handle. */
      0); /* Core where the task should run */
  
  delay(100);
}

void loop()
{  
//  server_loop();      //Pega valores vindos do ESP8266
  peripherals_loop(); //Pega valores vindos do Sensor de Temp
  lcd_loop();
}

/*
   Esta função existe para garantir que o teste: state_manager_get(TEMP_MASS) <= a - b
   Quando a < b, a subtração dos dois irá fazer o resultado causar um WRAP e bugar o resultado
*/
uint32_t safe_subtraction(uint32_t a, uint32_t b) {
  uint32_t diff;
  if (a < b) {
    diff = 0;
  } else {
    diff = a - b;
  }

  return diff;
}
