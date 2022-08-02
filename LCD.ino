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
  &btnPLNex,
  &btnMuteEntrHI,
  &btnMuteEntrLO,
  &btnMuteMassHI,
  &btnMuteMassLO,
  NULL
};

// Forward refs
void btnPLPushCB(void *ptr);

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

  btnPLNex.attachPush(btnPLPushCB, &btnPLNex);

  btnMuteEntrHI.attachPush(btnMuteEntrHICB, &btnMuteEntrHI);
  btnMuteEntrLO.attachPush(btnMuteEntrLOCB, &btnMuteEntrLO);
  btnMuteMassHI.attachPush(btnMuteMassHICB, &btnMuteMassHI);
  btnMuteMassLO.attachPush(btnMuteMassLOCB, &btnMuteMassLO);

  delay(3000);

  page_change_to(PAGE_MAIN);
}

void lcd_loop(void) {
  nexLoop(nex_listen_list);

  if (state_manager_get(TEMP_ENTR) >= (state_manager_get(MAX_ENTR) + HIST_ENTR) && !state_manager_get(IS_AWARE_ENTR)) {
    //    println("Changing to ENTRHI");
    page_change_to(PAGE_ENTR_HI);
  } else if (state_manager_get(TEMP_ENTR) != 0 && state_manager_get(TEMP_ENTR) <= safe_subtraction(state_manager_get(MIN_ENTR), HIST_ENTR) && !state_manager_get(IS_AWARE_ENTR)) {
    //    // Serial.println"Changing to ENTRLO");
    page_change_to(PAGE_ENTR_LO);
  } else if (state_manager_get(TEMP_MASS) >= (state_manager_get(MAX_MASS) + HIST_MASS) && !state_manager_get(IS_AWARE_MASS)) {
    //    // Serial.println"Changing to MASSHI");
    page_change_to(PAGE_MASS_HI);
  } else if (state_manager_get(TEMP_MASS) != 0 && state_manager_get(TEMP_MASS) <= safe_subtraction(state_manager_get(MIN_MASS), HIST_MASS) && !state_manager_get(IS_AWARE_MASS)) {
    //    // Serial.println"Changing to MASSLO");
    page_change_to(PAGE_MASS_LO);
  } else {
    //    // Serial.println"Changing to MAIN");
    page_change_to(PAGE_MAIN);
  }

  if (curr_page == PAGE_MAIN) {
    checkFlicker(MAX_ENTR);
    checkFlicker(MIN_ENTR);
    checkFlicker(MAX_MASS);
    checkFlicker(MIN_MASS);
    //    checkFlicker(PALHA_LENHA);
  }
}


int maxEntrFlickerCounter = 0;
int minEntrFlickerCounter = 0;
int maxMassFlickerCounter = 0;
int minMassFlickerCounter = 0;
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

    default:
      break;
  }

  switch (op) {
    case MAX_ENTR:
      if (lcdValue != stateValue) {
        maxEntrFlickerCounter++;

        if (maxEntrFlickerCounter > 7) {
          maxEntrFlickerCounter = 0;
          state_manager_set(op, lcdValue);
        }
      }
      break;

    case MIN_ENTR:
      if (lcdValue != stateValue) {
        minEntrFlickerCounter++;

        if (minEntrFlickerCounter > 7) {
          minEntrFlickerCounter = 0;
          state_manager_set(op, lcdValue);
        }
      }
      break;

    case MAX_MASS:
      if (lcdValue != stateValue) {
        maxMassFlickerCounter++;

        if (maxMassFlickerCounter > 7) {
          maxMassFlickerCounter = 0;
          state_manager_set(op, lcdValue);
        }
      }
      break;

    case MIN_MASS:
      if (lcdValue != stateValue) {
        minMassFlickerCounter++;

        if (minMassFlickerCounter > 7) {
          minMassFlickerCounter = 0;
          state_manager_set(op, lcdValue);
        }
      }
      break;

    default:
      break;
  }


}

void page_change_to(lcd_page_t page) {
  bool result;
  if (curr_page != page) {
    switch (page) {
      case PAGE_MAIN:
        while ((result = pageMain.show()) != true) {
          Serial.println("Retrying pageMain");
          delay(1000);
        }
        curr_page = page;
        itoa(state_manager_get(TEMP_ENTR), entrTempStr, 10);
        entrTempNex.setText(entrTempStr);

        itoa(state_manager_get(TEMP_MASS), massTempStr, 10);
        massTempNex.setText(massTempStr);


        while ((result = maxEntrTempNex.setValue(state_manager_get(MAX_ENTR))) != true) {
          Serial.println("Retrying maxEntr");
          delay(1000);
        }

        while ((result = minEntrTempNex.setValue(state_manager_get(MIN_ENTR))) != true) {
          Serial.println("Retrying minEntr");
          delay(1000);
        }
        while ((result = maxMassTempNex.setValue(state_manager_get(MAX_MASS))) != true) {
           Serial.println("Retrying maxMass");
          delay(1000);
        }
        while ((result = minMassTempNex.setValue(state_manager_get(MIN_MASS))) != true) {
          Serial.println("Retrying minMass");
          delay(1000);
        }

        while ((result = updatePLBtn(state_manager_get(PALHA_LENHA))) != true) {
           Serial.println("Retrying updatebTNPL");
          delay(1000);
        }
        break;

      case PAGE_MASS_HI:
        while ((result = pageMassHI.show()) != true) {
          Serial.println("Retrying pageMassHI");
          delay(1000);
        }
        curr_page = page;
        break;

      case PAGE_MASS_LO:
        while ((result = pageMassLO.show()) != true) {
          Serial.println("Retrying pageMassLO");
          // Serial.print"Result: ");
          // Serial.printlnresult);
          delay(1000);
        }
        curr_page = page;
        break;

      case PAGE_ENTR_HI:
        while ((result = pageEntrHI.show()) != true) {
          Serial.println("Retrying pageEntrHI");
          // Serial.print"Result: ");
          // Serial.printlnresult);
          delay(1000);
        }
        curr_page = page;
        break;

      case PAGE_ENTR_LO:
        while ((result = pageEntrLO.show()) != true) {
          Serial.println("Retrying pageEntrLO");
          // Serial.print"Result: ");
          // Serial.printlnresult);
          delay(1000);
        }
        curr_page = page;
        break;
    }

  }
}

void lcd_trigger_update(state_prefs_t op) {
  if (curr_page == PAGE_MAIN) {
    if (op == TEMP_ENTR) {
      itoa(state_manager_get(TEMP_ENTR), entrTempStr, 10);
      entrTempNex.setText(entrTempStr);
    } else if (op == TEMP_MASS) {
      itoa(state_manager_get(TEMP_MASS), massTempStr, 10);
      massTempNex.setText(massTempStr);
    }
  }
}

void btnPLPushCB(void *ptr) {
  //  // Serial.println"PUSH");
  uint32_t currState = state_manager_get(PALHA_LENHA);
  bool result;

  if (currState == 0) {
    result = updatePLBtn(1);
    if (result)
      state_manager_set(PALHA_LENHA, 1);
  } else {
    result = updatePLBtn(0);
    if (result)
      state_manager_set(PALHA_LENHA, 0);
  }
}

bool updatePLBtn(uint32_t value) {
  if (value == 0) {
    return btnPLNex.Set_background_image_pic(8) && btnPLNex.Set_press_background_image_pic2(8);
  } else {
    return btnPLNex.Set_background_image_pic(5) && btnPLNex.Set_press_background_image_pic2(5);
  }
}

void btnMuteEntrHICB(void *ptr) {
  page_change_to(PAGE_MAIN);
  state_manager_set(IS_AWARE_ENTR, 1);
}

void btnMuteEntrLOCB(void *ptr) {
  page_change_to(PAGE_MAIN);
  state_manager_set(IS_AWARE_ENTR, 1);
}

void btnMuteMassHICB(void *ptr) {
  page_change_to(PAGE_MAIN);
  state_manager_set(IS_AWARE_MASS, 1);
}

void btnMuteMassLOCB(void *ptr) {
  page_change_to(PAGE_MAIN);
  state_manager_set(IS_AWARE_MASS, 1);
}
