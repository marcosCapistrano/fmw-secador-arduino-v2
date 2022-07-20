#define OUT_QUEIMADOR       12
#define OUT_BUZINA          13
#define LED_MASSA_QUENTE     5
#define LED_MASSA_FRIO       4
#define LED_ENTRADA_QUENTE   0
#define LED_ENTRADA_FRIO     2
#define LED_CONEXAO         18
#define SENSORT             15

OneWire oneWire(SENSORT);
DallasTemperature sensors(&oneWire);

bool ledConexaoIsOn = false;

void peripherals_setup(void) {
  pinMode(OUT_QUEIMADOR, OUTPUT);
  pinMode(OUT_BUZINA, OUTPUT);
  pinMode(LED_MASSA_QUENTE, OUTPUT);
  pinMode(LED_MASSA_FRIO, OUTPUT);
  pinMode(LED_ENTRADA_QUENTE, OUTPUT);
  pinMode(LED_ENTRADA_FRIO, OUTPUT);
  pinMode(LED_CONEXAO, OUTPUT);

  digitalWrite(OUT_QUEIMADOR, HIGH);
  digitalWrite(OUT_BUZINA, HIGH);

  digitalWrite(LED_MASSA_QUENTE, HIGH);
  digitalWrite(LED_MASSA_FRIO, HIGH);
  digitalWrite(LED_ENTRADA_QUENTE, HIGH);
  digitalWrite(LED_ENTRADA_FRIO, HIGH);

  sensors.begin();
}

float sensor_temp;
void peripherals_loop(void) {
  sensors.requestTemperatures();
  sensor_temp = sensors.getTempCByIndex(0);
  if (sensor_temp < 0) sensor_temp = 0;
  state_manager_set(TEMP_ENTR, (uint32_t) sensor_temp);

  if (millis() / 1000 - state_manager_get(LAST_COMM) > 10) {
    if (ledConexaoIsOn) {
      digitalWrite(LED_CONEXAO, LOW);
    }
  } else {
    if (!ledConexaoIsOn) {
      digitalWrite(LED_CONEXAO, HIGH);
    }
  }
}
