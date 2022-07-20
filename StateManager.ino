Preferences preferences;

static const char *pref_name = "state";
static const char *maxEntrKey = "max-entr";
static const char *minEntrKey = "min-entr";
static const char *maxMassKey = "max-mass";
static const char *minMassKey = "min-mass";
static const char *palhaLenhaKey = "palha-lenha";

uint32_t tempEntr = 0;
uint32_t tempMass = 0;

uint32_t maxEntr = 100;
uint32_t minEntr = 0;
uint32_t maxMass = 100;
uint32_t minMass = 0;
uint32_t palhaLenha = 0;
uint32_t lastComm = 0;

uint32_t isAwareEntr = 0;
uint32_t isAwareMass = 0;

void state_manager_setup() {
  Serial.println("Setting up STATE...");

  preferences.begin(pref_name, false); //Abre Preferences com permissão ReadWrite

  maxEntr = preferences.getUInt(maxEntrKey, 100);
  minEntr = preferences.getUInt(minEntrKey, 0);
  maxMass = preferences.getUInt(maxMassKey, 100);
  minMass = preferences.getUInt(minMassKey, 0);

  Serial.println("STATE OK!");
}

uint32_t state_manager_get(state_prefs_t choice) {
  switch (choice) {
    case TEMP_ENTR:
      return tempEntr;
      break;

    case TEMP_MASS:
      return tempMass;
      break;

    case MAX_ENTR:
      return maxEntr;
      break;

    case MIN_ENTR:
      return minEntr;
      break;

    case MAX_MASS:
      return maxMass;
      break;

    case MIN_MASS:
      return minMass;
      break;

    case PALHA_LENHA:
      return palhaLenha;
      break;

    case LAST_COMM:
      return lastComm;
      break;

      case IS_AWARE_ENTR:
      return isAwareEntr;
      break;

    case IS_AWARE_MASS:
      return isAwareMass;
      break;
  }
}

void state_manager_set(state_prefs_t choice, uint32_t value) {
  switch (choice) {
    case TEMP_ENTR:
      tempEntr = value;
      break;

    case TEMP_MASS:
      tempMass = value;
      break;

    case MAX_ENTR: {
        if (maxEntr != value) {
          maxEntr = value;
          preferences.putUInt(maxEntrKey, value);
        }
      }
      break;

    case MIN_ENTR: {
        if (minEntr != value) {
          minEntr = value;
          preferences.putUInt(minEntrKey, value);
        }
      }
      break;

    case MAX_MASS: {
        if (maxMass != value) {
          maxMass = value;
          preferences.putUInt(maxMassKey, value);
        }
      }
      break;

    case MIN_MASS: {
        if (minMass != value) {
          minMass = value;
          preferences.putUInt(minMassKey, value);
        }
      }

    case PALHA_LENHA:
      if (palhaLenha != value) {
        palhaLenha = value;
        preferences.putUInt(palhaLenhaKey, value);
      }
      break;

    case LAST_COMM:
      lastComm = value;
      break;

    case IS_AWARE_ENTR:
      isAwareEntr = value;
      break;

    case IS_AWARE_MASS:
      isAwareMass = value;
      break;
  }
}
