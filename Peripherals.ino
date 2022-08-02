#define OUT_QUEIMADOR       12
#define OUT_BUZINA          13
#define LED_MASSA_QUENTE     4
#define LED_MASSA_FRIO       5
#define LED_ENTRADA_QUENTE   2
#define LED_ENTRADA_FRIO     0
#define LED_CONEXAO         18
#define SENSORT             15

OneWire oneWire(SENSORT);
DallasTemperature sensors(&oneWire);

bool ledConexaoIsOn = false;
bool ledEntrQuenteIsOn = false;
bool ledEntrFrioIsOn = false;
bool ledMassQuenteIsOn = false;
bool ledMassFrioIsOn = false;
bool queimadorIsOn = false;
bool queimadorShouldBeOn = false;

bool alarmeIsOn = false;
bool alarmeEntr = false;
bool alarmeMass = false;

uint32_t lastReadTempSensor;

void peripherals_setup(void) {
  pinMode(OUT_QUEIMADOR, OUTPUT);
  pinMode(OUT_BUZINA, OUTPUT);
  pinMode(LED_MASSA_QUENTE, OUTPUT);
  pinMode(LED_MASSA_FRIO, OUTPUT);
  pinMode(LED_ENTRADA_QUENTE, OUTPUT);
  pinMode(LED_ENTRADA_FRIO, OUTPUT);
  pinMode(LED_CONEXAO, OUTPUT);

  digitalWrite(OUT_QUEIMADOR, LOW);
  digitalWrite(OUT_BUZINA, LOW);

  digitalWrite(LED_MASSA_QUENTE, HIGH);
  digitalWrite(LED_MASSA_FRIO, HIGH);
  digitalWrite(LED_ENTRADA_QUENTE, HIGH);
  digitalWrite(LED_ENTRADA_FRIO, HIGH);
  digitalWrite(LED_CONEXAO, LOW);

  sensors.begin();
  lastReadTempSensor = millis() / 1000;
}

uint32_t entrTempPerif;
uint32_t massTempPerif;

uint32_t maxEntrTempPerif;
uint32_t minEntrTempPerif;
uint32_t maxMassTempPerif;
uint32_t minMassTempPerif;

uint32_t palhaLenhaPerif;

uint32_t isAwareEntrPerif;
uint32_t isAwareMassPerif;

float sensor_temp;
void peripherals_loop(void) {
  if (safe_subtraction(millis() / 1000, lastReadTempSensor) > 5) {
    sensors.requestTemperatures();
    sensor_temp = sensors.getTempCByIndex(0);
    lastReadTempSensor = millis() / 1000;

    if (sensor_temp < 0) sensor_temp = 0;
    if(sensor_temp > 150) sensor_temp = 150;
    state_manager_set(TEMP_ENTR, (uint32_t) sensor_temp);

    lcd_trigger_update(TEMP_ENTR);
  }

  if (safe_subtraction(millis() / 1000, state_manager_get(LAST_COMM)) > 15) {
    if (ledConexaoIsOn) {
      digitalWrite(LED_CONEXAO, LOW);
      ledConexaoIsOn = false;
    }
  } else {
    if (!ledConexaoIsOn && state_manager_get(HAS_COMM_ONCE) == 1) {
      digitalWrite(LED_CONEXAO, HIGH);
      ledConexaoIsOn = true;
    }
  }

  entrTempPerif = state_manager_get(TEMP_ENTR);
  massTempPerif = state_manager_get(TEMP_MASS);

  maxEntrTempPerif = state_manager_get(MAX_ENTR);
  minEntrTempPerif = state_manager_get(MIN_ENTR);
  maxMassTempPerif = state_manager_get(MAX_MASS);
  minMassTempPerif = state_manager_get(MIN_MASS);

  palhaLenhaPerif = state_manager_get(PALHA_LENHA);
  isAwareEntrPerif = state_manager_get(IS_AWARE_ENTR);
  isAwareMassPerif = state_manager_get(IS_AWARE_MASS);

  handleAlarme();
  handleLEDsAndIsAware();


  handleQueimador();
}

void handleAlarme() {
  if (entrTempPerif >= maxEntrTempPerif + HIST_ENTR  || (entrTempPerif != 0 && entrTempPerif <= safe_subtraction(minEntrTempPerif, HIST_ENTR))) {
    if (!isAwareEntrPerif) {
      alarmeEntr = true;

      if (!alarmeIsOn) {
//        Serial.println("Turning ON Alarme");
        digitalWrite(OUT_BUZINA, HIGH);
        alarmeIsOn = true;
      }
    } else {
      alarmeEntr = false;
      if (alarmeIsOn && !alarmeMass) {
//        Serial.println("Turning OFF Alarme");
        digitalWrite(OUT_BUZINA, LOW);
        alarmeIsOn = false;
      }
    }
  } else {
    alarmeEntr = false;
    if (alarmeIsOn && !alarmeMass) {
//      Serial.println("Turning OFF Alarme");
      digitalWrite(OUT_BUZINA, LOW);
      alarmeIsOn = false;
    }
  }

  if (massTempPerif >= maxMassTempPerif + HIST_MASS || (massTempPerif != 0 && massTempPerif <= safe_subtraction(minMassTempPerif, HIST_MASS))) {
    if (!isAwareMassPerif) {
      alarmeMass = true;

      if (!alarmeIsOn) {
//        Serial.println("Turning ON Alarme");
        digitalWrite(OUT_BUZINA, HIGH);
        alarmeIsOn = true;
      }
    } else {
      alarmeMass = false;
      if (alarmeIsOn && !alarmeEntr) {
//        Serial.println("Turning OFF Alarme");
        digitalWrite(OUT_BUZINA, LOW);
        alarmeIsOn = false;
      }
    }
  } else {
    alarmeMass = false;
    if (alarmeIsOn && !alarmeEntr) {
//      Serial.println("Turning OFF Alarme");
      digitalWrite(OUT_BUZINA, LOW);
      alarmeIsOn = false;
    }
  }
}

void handleLEDsAndIsAware() {
  if (entrTempPerif > maxEntrTempPerif) {
    if (!ledEntrQuenteIsOn) {
//      Serial.println("Turning ON Led ENTR QUENTE");
      digitalWrite(LED_ENTRADA_QUENTE, LOW);
      ledEntrQuenteIsOn = true;
    }

    if (ledEntrFrioIsOn) {
//      Serial.println("Turning OFF Led ENTR QUENTE");
      digitalWrite(LED_ENTRADA_FRIO, HIGH);
      ledEntrFrioIsOn = false;
    }    
  } else if (minEntrTempPerif <= entrTempPerif && entrTempPerif <= maxEntrTempPerif) {
    if (ledEntrQuenteIsOn) {
//      Serial.println("Turning OFF Led ENTR QUENTE");
      digitalWrite(LED_ENTRADA_QUENTE, HIGH);
      ledEntrQuenteIsOn = false;
    }

    if (ledEntrFrioIsOn) {
//      Serial.println("Turning OFF Led ENTR QUENTE");
      digitalWrite(LED_ENTRADA_FRIO, HIGH);
      ledEntrFrioIsOn = false;
    }

    if (isAwareEntrPerif) {
      state_manager_set(IS_AWARE_ENTR, 0);
      isAwareEntrPerif = 0;
    }
  } else {
    if (!ledEntrFrioIsOn) {
//      Serial.println("Turning ON Led ENTR FRIO");
      digitalWrite(LED_ENTRADA_FRIO, LOW);
      ledEntrFrioIsOn = true;
    }

    if (ledEntrQuenteIsOn) {
//      Serial.println("Turning OFF Led ENTR QUENTE");
      digitalWrite(LED_ENTRADA_QUENTE, HIGH);
      ledEntrQuenteIsOn = false;
    }
  }

  if (massTempPerif > maxMassTempPerif) {
    if (!ledMassQuenteIsOn) {
//      Serial.println("Turning ON Led MASSA QUENTE");
      digitalWrite(LED_MASSA_QUENTE, LOW);
      ledMassQuenteIsOn = true;
    }

    if (ledMassFrioIsOn) {
//      Serial.println("Turning OFF Led MASSA FRIO");
      digitalWrite(LED_MASSA_FRIO, HIGH);
      ledMassFrioIsOn = false;
    }
  } else if (minMassTempPerif <= massTempPerif && massTempPerif <= maxMassTempPerif) {
    if (ledMassQuenteIsOn) {
//      Serial.println("Turning OFF Led MASSA QUENTE");
      digitalWrite(LED_MASSA_QUENTE, HIGH);
      ledMassQuenteIsOn = false;
    }

    if (ledMassFrioIsOn) {
//      Serial.println("Turning OFF Led MASSA FRIO");
      digitalWrite(LED_MASSA_FRIO, HIGH);
      ledMassFrioIsOn = false;
    }

    if (isAwareMassPerif) {
      state_manager_set(IS_AWARE_MASS, 0);
      isAwareMassPerif = 0;
    }
  } else {
    if (!ledMassFrioIsOn) {
//      Serial.println("Turning On Led MASSA FRIO");
      digitalWrite(LED_MASSA_FRIO, LOW);
      ledMassFrioIsOn = true;
    }

    if (ledMassQuenteIsOn) {
//      Serial.println("Turning OFF Led MASSA QUENTE");
      digitalWrite(LED_MASSA_QUENTE, HIGH);
      ledMassQuenteIsOn = false;
    }
  }
}

void handleQueimador() {
//  Serial.print("PalhaLenhaPerif: ");
//  Serial.print(palhaLenhaPerif);
//
//  uint32_t value;
//  btnPLNex.getValue(&value);
//  Serial.print(" PalhaLenha NEXTION: ");
//  Serial.print(value);
//  
//  Serial.print(" Should Turn on: ");
//  Serial.print(entrTempPerif < maxEntrTempPerif && massTempPerif < maxMassTempPerif);
//
//  Serial.print(" Is on?: ");
//  Serial.print(queimadorIsOn);
//
//  Serial.println("");

//  Serial.println(palhaLenhaPerif);
  if (palhaLenhaPerif == 0) {
    if (entrTempPerif < maxEntrTempPerif && massTempPerif < maxMassTempPerif) {
      if (!queimadorIsOn) {
        digitalWrite(OUT_QUEIMADOR, HIGH);
        queimadorIsOn = true;
      }
    } else {
      if (queimadorIsOn) {
        digitalWrite(OUT_QUEIMADOR, LOW);
        queimadorIsOn = false;
      }
    }
  } else if(palhaLenhaPerif == 1) {
    if (queimadorIsOn) {
      digitalWrite(OUT_QUEIMADOR, LOW);
      queimadorIsOn = false;
    }
  }
}
