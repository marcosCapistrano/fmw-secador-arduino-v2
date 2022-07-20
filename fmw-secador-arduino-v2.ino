#include <Arduino.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Preferences.h>
#include <Nextion.h>

#define HIST_ENTR 7
#define HIST_MASS 7

typedef enum {
  TEMP_ENTR,
  TEMP_MASS,
  MAX_ENTR,
  MIN_ENTR,
  MAX_MASS,
  MIN_MASS,
  PALHA_LENHA,
  LAST_COMM,
  IS_AWARE_ENTR,
  IS_AWARE_MASS
} state_prefs_t;

void setup()
{
  state_manager_setup();
  
  lcd_setup();
  server_setup();
  peripherals_setup();
  
  delay(500);
}

void loop()
{
  Serial.println("Loop!!");
  
  server_loop();      //Pega valores vindos do ESP8266
  peripherals_loop(); //Pega valores vindos do Sensor de Temp
  lcd_loop();
}
