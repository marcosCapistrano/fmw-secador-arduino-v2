/*
   Declaração dos Componentes Nextion
*/

NexPage pageMain = NexPage(1, 0, "page1");                 //--> Tela Principal
NexPage pageMassHI = NexPage(2, 0, "page2");                 //--> Tela massa acima
NexPage pageEntrHI = NexPage(4, 0, "page4");                 //--> Tela entrada acima
NexPage pageEntrLO = NexPage(5, 0, "page5");                 //--> Tela entrada abaixo
NexPage pageMassLO = NexPage(3, 0, "page3");                 //--> Tela massa abaixo

NexButton btnPLNex = NexButton(1, 23, "b0");                                //--> Palha/lenha

NexButton btnMuteEntrHI = NexButton(4, 1, "b0");
NexButton btnMuteEntrLO = NexButton(5, 1, "b0");
NexButton btnMuteMassHI = NexButton(2, 1, "b0");
NexButton btnMuteMassLO = NexButton(3, 1, "b0");

NexButton btnMinusMaxEntrNex = NexButton(1, 5, "bt1");
NexButton btnPlusMaxEntrNex = NexButton(1, 6, "bt2");
NexButton btnMinusMinEntrNex = NexButton(1, 7, "bt3");
NexButton btnPlusMinEntrNex = NexButton(1, 8, "bt4");

NexButton btnMinusMaxMassNex = NexButton(1, 9, "bt5");
NexButton btnPlusMaxMassNex = NexButton(1, 10, "bt6");
NexButton btnMinusMinMassNex = NexButton(1, 11, "bt7");
NexButton btnPlusMinMassNex = NexButton(1, 12, "bt8");

NexText entrTempNex  = NexText(1, 2, "t0");                          //--> Temperatura da Entrada
NexText massTempNex  = NexText(1, 3, "t1");                          //--> Temperatura da Massa
char entrTempStr[3];
char massTempStr[3];

NexNumber maxEntrTempNex = NexNumber(1, 4, "n0");
NexNumber minEntrTempNex = NexNumber(1, 14, "n2");
NexNumber maxMassTempNex = NexNumber(1, 13, "n1");
NexNumber minMassTempNex = NexNumber(1, 15, "n3");
/*
   ------------------------------------------------------------------------------------------------------------------------
*/

/*
   Definição da lista de componentes que iremos querer ouvir cliques,
   no caso ouviremos apenas quando o botao é solto (POP)
*/

NexTouch *nex_listen_list[] =
{
  &btnMinusMaxEntrNex,
  &btnPlusMaxEntrNex,
  &btnMinusMinEntrNex,
  &btnPlusMinEntrNex,
  &btnMinusMaxMassNex,
  &btnPlusMaxMassNex,
  &btnMinusMinMassNex,
  &btnPlusMinMassNex,
  &btnPLNex,
  &btnMuteEntrHI,
  &btnMuteEntrLO,
  &btnMuteMassHI,
  &btnMuteMassLO,
  NULL
};

//blockUpdate existe para bloquear atualizações na tela que acontecam enquanto o cara ta aumentando algum numero de max/min,
//se nao tivesse isso, quando a tela atualizasse por causa de alguma mudança de temperatura, iria zerar o numero que o cara tava colocando
bool blockUpdate = false;

// Forward refs
void btnMinusMaxEntrCB(void *ptr);
void btnPlusMaxEntrCB(void *ptr);
void btnMinusMinEntrCB(void *ptr);
void btnPlusMinEntrCB(void *ptr);

void btnMinusMaxMassCB(void *ptr);
void btnPlusMaxMassCB(void *ptr);
void btnMinusMinMassCB(void *ptr);
void btnPlusMinMassCB(void *ptr);

void btnPLPushCB(void *ptr);
void btnPLPopCB(void *ptr);

void btnMuteEntrHICB(void *ptr);
void btnMuteEntrLOCB(void *ptr);
void btnMuteMassHICB(void *ptr);
void btnMuteMassLOCB(void *ptr);

void checkFlicker(state_prefs_t op);
/*
   ------------------------------------------------------------------------------------------------------------------------
*/

enum lcd_page_t {
  PAGE_INIT,
  PAGE_MAIN,
  PAGE_MASS_HI,
  PAGE_MASS_LO,
  PAGE_ENTR_HI,
  PAGE_ENTR_LO
};

void page_change_to(lcd_page_t page);

lcd_page_t curr_page = PAGE_INIT;

void lcd_setup(void) {
  nexInit();

  //  btnMinusMaxEntrNex.attachPush(btnMinusMaxEntrPushCB, &btnMinusMaxEntrNex);
  //  btnMinusMaxEntrNex.attachPop(btnMinusMaxEntrPopCB, &btnMinusMaxEntrNex);
  //
  //  btnPlusMaxEntrNex.attachPush(btnPlusMaxEntrPushCB, &btnPlusMaxEntrNex);
  //  btnPlusMaxEntrNex.attachPop(btnPlusMaxEntrPopCB, &btnPlusMaxEntrNex);
  //
  //  btnMinusMinEntrNex.attachPush(btnMinusMinEntrPushCB, &btnMinusMinEntrNex);
  //  btnMinusMinEntrNex.attachPop(btnMinusMinEntrPopCB, &btnMinusMinEntrNex);
  //
  //  btnPlusMinEntrNex.attachPush(btnPlusMinEntrPushCB, &btnPlusMinEntrNex);
  //  btnPlusMinEntrNex.attachPop(btnPlusMinEntrPopCB, &btnPlusMinEntrNex);
  //
  //  btnMinusMaxMassNex.attachPush(btnMinusMaxMassPushCB, &btnMinusMaxMassNex);
  //  btnMinusMaxMassNex.attachPop(btnMinusMaxMassPopCB, &btnMinusMaxMassNex);
  //
  //  btnPlusMaxMassNex.attachPush(btnPlusMaxMassPushCB, &btnPlusMaxMassNex);
  //  btnPlusMaxMassNex.attachPop(btnPlusMaxMassPopCB, &btnPlusMaxMassNex);
  //
  //  btnMinusMinMassNex.attachPush(btnMinusMinMassPushCB, &btnMinusMinMassNex);
  //  btnMinusMinMassNex.attachPop(btnMinusMinMassPopCB, &btnMinusMinMassNex);
  //
  //  btnPlusMinMassNex.attachPush(btnPlusMinMassPushCB, &btnPlusMinMassNex);
  //  btnPlusMinMassNex.attachPop(btnPlusMinMassPopCB, &btnPlusMinMassNex);
  //
  btnPLNex.attachPush(btnPLPushCB, &btnPLNex);
      btnPLNex.attachPop(btnPLPopCB, &btnPLNex);

  btnMuteEntrHI.attachPush(btnMuteEntrHICB, &btnMuteEntrHI);
  btnMuteEntrLO.attachPush(btnMuteEntrLOCB, &btnMuteEntrLO);
  btnMuteMassHI.attachPush(btnMuteMassHICB, &btnMuteMassHI);
  btnMuteMassLO.attachPush(btnMuteMassLOCB, &btnMuteMassLO);

  delay(3000);
  page_change_to(PAGE_MAIN);
}

int flickerCounter = 0;
void lcd_loop(void) {
  if (state_manager_get(TEMP_ENTR) >= (state_manager_get(MAX_ENTR) + HIST_ENTR) && !state_manager_get(IS_AWARE_ENTR)) {
    //    println("Changing to ENTRHI");
    page_change_to(PAGE_ENTR_HI);
  } else if (state_manager_get(TEMP_ENTR) != 0 && state_manager_get(TEMP_ENTR) <= safe_subtraction(state_manager_get(MIN_ENTR), HIST_ENTR) && !state_manager_get(IS_AWARE_ENTR)) {
    //    Serial.println("Changing to ENTRLO");
    page_change_to(PAGE_ENTR_LO);
  } else if (state_manager_get(TEMP_MASS) >= (state_manager_get(MAX_MASS) + HIST_MASS) && !state_manager_get(IS_AWARE_MASS)) {
    //    Serial.println("Changing to MASSHI");
    page_change_to(PAGE_MASS_HI);
  } else if (state_manager_get(TEMP_MASS) != 0 && state_manager_get(TEMP_MASS) <= safe_subtraction(state_manager_get(MIN_MASS), HIST_MASS) && !state_manager_get(IS_AWARE_MASS)) {
    //    Serial.println("Changing to MASSLO");
    page_change_to(PAGE_MASS_LO);
  } else {
    //    Serial.println("Changing to MAIN");
    page_change_to(PAGE_MAIN);
  }


  if (curr_page == PAGE_MAIN) {
    int localFlickerCounter;

      if (flickerCounter == 0) checkFlicker(MAX_ENTR);
      if (flickerCounter == 2) checkFlicker(MIN_ENTR);
      if (flickerCounter == 4) checkFlicker(MAX_MASS);
      if (flickerCounter == 6) checkFlicker(MIN_MASS);
      if (flickerCounter == 8) checkFlicker(PALHA_LENHA);

    flickerCounter++;
    if(flickerCounter >= 10) flickerCounter = 0;
    delay(50);
  }

  nexLoop(nex_listen_list);
}

int maxEntrFlickerCounter = 0;
int minEntrFlickerCounter = 0;
int maxMassFlickerCounter = 0;
int minMassFlickerCounter = 0;
int plFlickerCounter = 0;
void checkFlicker(state_prefs_t op) {
  uint32_t stateValue;
  uint32_t lcdValue;

    stateValue = state_manager_get(op);
    switch (op) {
      case MAX_ENTR:
        maxEntrTempNex.getValue(&lcdValue);
        break;

      case MIN_ENTR:
        minEntrTempNex.getValue(&lcdValue);
        break;

      case MAX_MASS:
        maxMassTempNex.getValue(&lcdValue);
        break;

      case MIN_MASS:
        minMassTempNex.getValue(&lcdValue);
        break;

        case PALHA_LENHA:
        uint32_t picID;
        btnPLNex.Get_background_image_pic(&picID);

        if(picID == 8) {
          lcdValue = 0;
        } else {
          lcdValue = 1;
        }
        break;

      default:
        break;
    }



    switch (op) {
      case MAX_ENTR:
        if (lcdValue != stateValue) {
          maxEntrFlickerCounter++;

          if (maxEntrFlickerCounter >= 5) {
            maxEntrFlickerCounter = 0;
            state_manager_set(op, lcdValue);
          }
        } else {
          maxEntrFlickerCounter = 0;
        }
        break;

      case MIN_ENTR:
        if (lcdValue != stateValue) {
          minEntrFlickerCounter++;

          if (minEntrFlickerCounter >= 5) {
            minEntrFlickerCounter = 0;
            state_manager_set(op, lcdValue);
          }
        } else {
          minEntrFlickerCounter = 0;
        }
        break;

      case MAX_MASS:
        if (lcdValue != stateValue) {
          maxMassFlickerCounter++;
          if (maxMassFlickerCounter >= 5) {
            maxMassFlickerCounter = 0;
            state_manager_set(op, lcdValue);
          }
        } else {
          maxMassFlickerCounter = 0;
        }
        break;

      case MIN_MASS:
        if (lcdValue != stateValue) {
          minMassFlickerCounter++;

          if (minMassFlickerCounter >= 5) {
            minMassFlickerCounter = 0;
            state_manager_set(op, lcdValue);
          }
        } else {
          minMassFlickerCounter = 0;
        }
        break;

       case PALHA_LENHA:
//       Serial.printf("lcdValue: %d | stateValue: %d", lcdValue, stateValue);
       if (lcdValue != stateValue) {
          plFlickerCounter++;

          if (plFlickerCounter >= 5) {
            plFlickerCounter = 0;
            state_manager_set(op, lcdValue);
          }
        } else {
          plFlickerCounter = 0;
        }

      default:
        break;
    }
}

void page_change_to(lcd_page_t page) {
  if (curr_page != page) {
    switch (page) {
      case PAGE_MAIN:
        pageMain.show();

        itoa(state_manager_get(TEMP_ENTR), entrTempStr, 10);
        entrTempNex.setText(entrTempStr);

        itoa(state_manager_get(TEMP_MASS), massTempStr, 10);
        massTempNex.setText(massTempStr);


        maxEntrTempNex.setValue(state_manager_get(MAX_ENTR));
        minEntrTempNex.setValue(state_manager_get(MIN_ENTR));
        maxMassTempNex.setValue(state_manager_get(MAX_MASS));
        minMassTempNex.setValue(state_manager_get(MIN_MASS));

        updatePLBtn(state_manager_get(PALHA_LENHA));
        break;

      case PAGE_MASS_HI:
        pageMassHI.show();
        break;

      case PAGE_MASS_LO:
        pageMassLO.show();
        break;

      case PAGE_ENTR_HI:
        pageEntrHI.show();
        break;

      case PAGE_ENTR_LO:
        pageEntrLO.show();
        break;
    }
    curr_page = page;
  }
}

void lcd_trigger_update(state_prefs_t op) {
  //  if (!blockUpdate) {
  if (curr_page == PAGE_MAIN) {
    if (op == TEMP_ENTR) {
      itoa(state_manager_get(TEMP_ENTR), entrTempStr, 10);
      entrTempNex.setText(entrTempStr);
    } else if (op == TEMP_MASS) {
      itoa(state_manager_get(TEMP_MASS), massTempStr, 10);
      massTempNex.setText(massTempStr);
    }
  }
  //  }
}

void btnMinusMaxEntrPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnMinusMaxEntrPopCB(void *ptr) {
  //  Serial.println("PUSH");
  uint32_t value;
  maxEntrTempNex.getValue(&value);

  state_manager_set(MAX_ENTR, value);
  blockUpdate = false;
}

void btnPlusMaxEntrPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnPlusMaxEntrPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  maxEntrTempNex.getValue(&value);

  state_manager_set(MAX_ENTR, value);
  blockUpdate = false;
}

void btnMinusMinEntrPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnMinusMinEntrPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  minEntrTempNex.getValue(&value);

  state_manager_set(MIN_ENTR, value);
  blockUpdate = false;
}

void btnPlusMinEntrPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnPlusMinEntrPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  minEntrTempNex.getValue(&value);

  state_manager_set(MIN_ENTR, value);
  blockUpdate = false;
}

void btnMinusMaxMassPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnMinusMaxMassPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  maxMassTempNex.getValue(&value);

  state_manager_set(MAX_MASS, value);
  blockUpdate = false;
}

void btnPlusMaxMassPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnPlusMaxMassPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  maxMassTempNex.getValue(&value);

  state_manager_set(MAX_MASS, value);
  blockUpdate = false;
}

void btnMinusMinMassPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnMinusMinMassPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  minMassTempNex.getValue(&value);

  state_manager_set(MIN_MASS, value);
  blockUpdate = false;
}

void btnPlusMinMassPushCB(void *ptr) {
  //  Serial.println("PUSH");
  blockUpdate = true;
}

void btnPlusMinMassPopCB(void *ptr) {
  //  Serial.println("POP");
  uint32_t value;
  minMassTempNex.getValue(&value);

  state_manager_set(MIN_MASS, value);
  blockUpdate = false;
}

void btnPLPushCB(void *ptr) {
  Serial.println("PUSH");
  uint32_t currState = state_manager_get(PALHA_LENHA);
  bool result;

  if (currState == 0) {
    result = updatePLBtn(1);
    if (result)
      state_manager_set(PALHA_LENHA, 1);

//    Serial.print("result");
//    Serial.println(result);
  } else {
    result = updatePLBtn(0);
    if (result)
      state_manager_set(PALHA_LENHA, 0);

//    Serial.print("result");
//    Serial.println(result);
  }
}


void btnPLPopCB(void *ptr) {
  Serial.println("POP!");
}


bool updatePLBtn(uint32_t value) {
  if (value == 0) {
    if(btnPLNex.Set_background_image_pic(8)) {
      return btnPLNex.Set_press_background_image_pic2(8);
    }
  } else {
    if(btnPLNex.Set_background_image_pic(5)) {
      return btnPLNex.Set_press_background_image_pic2(5);  
    }
  }

  return false;
}

void btnMuteEntrHICB(void *ptr) {
  state_manager_set(IS_AWARE_ENTR, 1);
  page_change_to(PAGE_MAIN);
}

void btnMuteEntrLOCB(void *ptr) {
  state_manager_set(IS_AWARE_ENTR, 1);
  page_change_to(PAGE_MAIN);
}

void btnMuteMassHICB(void *ptr) {
  state_manager_set(IS_AWARE_MASS, 1);
  page_change_to(PAGE_MAIN);
}

void btnMuteMassLOCB(void *ptr) {
  state_manager_set(IS_AWARE_MASS, 1);
  page_change_to(PAGE_MAIN);
}
