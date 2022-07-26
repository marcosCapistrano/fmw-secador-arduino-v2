/*
   Declaração dos Componentes Nextion
*/

NexPage pageMain = NexPage(0, 0, "page0");                 //--> Tela Principal
NexPage pageMassHI = NexPage(1, 0, "page1");                 //--> Tela massa acima
NexPage pageEntrHI = NexPage(3, 0, "page3");                 //--> Tela entrada acima
NexPage pageEntrLO = NexPage(4, 0, "page4");                 //--> Tela entrada abaixo
NexPage pageMassLO = NexPage(2, 0, "page2");                 //--> Tela massa abaixo

NexDSButton btnPLNex = NexDSButton(0, 1, "bt0");                                //--> Palha/lenha

NexButton btnMuteEntrHI = NexButton(3, 1, "b0");
NexButton btnMuteEntrLO = NexButton(4, 1, "b0");
NexButton btnMuteMassHI = NexButton(1, 1, "b0");
NexButton btnMuteMassLO = NexButton(2, 1, "b0");

NexButton btnMinusMaxEntrNex = NexButton(0, 5, "bt1");
NexButton btnPlusMaxEntrNex = NexButton(0, 6, "bt2");
NexButton btnMinusMinEntrNex = NexButton(0, 7, "bt3");
NexButton btnPlusMinEntrNex = NexButton(0, 8, "bt4");

NexButton btnMinusMaxMassNex = NexButton(0, 9, "bt5");
NexButton btnPlusMaxMassNex = NexButton(0, 10, "bt6");
NexButton btnMinusMinMassNex = NexButton(0, 11, "bt7");
NexButton btnPlusMinMassNex = NexButton(0, 12, "bt8");

NexText entrTempNex  = NexText(0, 2, "t0");                          //--> Temperatura da Entrada        (Write)
NexText massTempNex  = NexText(0, 3, "t1");                          //--> Temperatura da Massa          (Write)
char entrTempStr[3];
char massTempStr[3];

NexNumber maxEntrTempNex = NexNumber(0, 4, "n0");                           //--> Temperatura máxima da Turbina (Read)
NexNumber minEntrTempNex = NexNumber(0, 14, "n2");                          //--> Temperatura mínima da Turbina (Read)
NexNumber maxMassTempNex = NexNumber(0, 13, "n1");                          //--> Temperatura máxima da Massa   (Read)
NexNumber minMassTempNex = NexNumber(0, 15, "n3");                          //--> Temperatura mínima da Massa   (Read)
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
  &btnMuteEntrHI,
  &btnMuteEntrLO,
  &btnMuteMassHI,
  &btnMuteMassLO,
  NULL
};

// Forward refs
void btnMinusMaxEntrCB(void *ptr);
void btnPlusMaxEntrCB(void *ptr);
void btnMinusMinEntrCB(void *ptr);
void btnPlusMinEntrCB(void *ptr);

void btnMinusMaxMassCB(void *ptr);
void btnPlusMaxMassCB(void *ptr);
void btnMinusMinMassCB(void *ptr);
void btnPlusMinMassCB(void *ptr);

void btnMuteEntrHICB(void *ptr);
void btnMuteEntrLOCB(void *ptr);
void btnMuteMassHICB(void *ptr);
void btnMuteMassLOCB(void *ptr);

/*
   ------------------------------------------------------------------------------------------------------------------------
*/

enum lcd_page_t {
  PAGE_MAIN,
  PAGE_MASS_HI,
  PAGE_MASS_LO,
  PAGE_ENTR_HI,
  PAGE_ENTR_LO
};

void page_change_to(lcd_page_t page);

lcd_page_t curr_page;

void lcd_setup(void) {
  nexInit();

  btnMinusMaxEntrNex.attachPop(btnMinusMaxEntrCB);
  btnPlusMaxEntrNex.attachPop(btnPlusMaxEntrCB);
  btnMinusMinEntrNex.attachPop(btnMinusMinEntrCB);
  btnPlusMinEntrNex.attachPop(btnPlusMinEntrCB);

  btnMinusMaxMassNex.attachPop(btnMinusMaxMassCB);
  btnPlusMaxMassNex.attachPop(btnPlusMaxMassCB);
  btnMinusMinMassNex.attachPop(btnMinusMinMassCB);
  btnPlusMinMassNex.attachPop(btnPlusMinMassCB);

  btnMuteEntrHI.attachPop(btnMuteEntrHICB, &btnMuteEntrHI);
  btnMuteEntrLO.attachPop(btnMuteEntrLOCB, &btnMuteEntrLO);
  btnMuteMassHI.attachPop(btnMuteMassHICB, &btnMuteMassHI);
  btnMuteMassLO.attachPop(btnMuteMassLOCB, &btnMuteMassLO);

  page_change_to(PAGE_MAIN);

  itoa(state_manager_get(TEMP_ENTR), entrTempStr, 10);
  entrTempNex.setText(entrTempStr);

  itoa(state_manager_get(TEMP_MASS), massTempStr, 10);
  massTempNex.setText(massTempStr);

  maxEntrTempNex.setValue(state_manager_get(MAX_ENTR));
  minEntrTempNex.setValue(state_manager_get(MIN_ENTR));
  maxMassTempNex.setValue(state_manager_get(MAX_MASS));
  minMassTempNex.setValue(state_manager_get(MIN_MASS));

  btnPLNex.setValue(state_manager_get(PALHA_LENHA));
}

/*
 * Esta função existe para garantir que o teste: state_manager_get(TEMP_MASS) <= a - b
 * Quando a < b, a subtração dos dois irá fazer o resultado causar um WRAP e bugar o resultado
 */
uint32_t safe_subtraction(uint32_t a, uint32_t b) {
  uint32_t diff;
  if (a < b){
    diff = 0;
  } else {
    diff = a - b;
  }
  
  return diff;
}

void lcd_loop(void) {
  nexLoop(nex_listen_list);

  if (state_manager_get(TEMP_ENTR) >= (state_manager_get(MAX_ENTR) + HIST_ENTR) && !state_manager_get(IS_AWARE_ENTR)) {
    Serial.println("Changing to ENTRHI");
    
    page_change_to(PAGE_ENTR_HI);
  } else if (state_manager_get(TEMP_ENTR) <= safe_subtraction(state_manager_get(MIN_ENTR), HIST_ENTR) && !state_manager_get(IS_AWARE_ENTR)) {
    Serial.println("Changing to ENTRLO");
    
    page_change_to(PAGE_ENTR_LO);
  } else if (state_manager_get(TEMP_MASS) >= (state_manager_get(MAX_MASS) + HIST_MASS) && !state_manager_get(IS_AWARE_MASS)) {
    Serial.println("Changing to MASSHI");
    
    page_change_to(PAGE_MASS_HI);
  } else if (state_manager_get(TEMP_MASS) <= safe_subtraction(state_manager_get(MIN_MASS),HIST_MASS) && !state_manager_get(IS_AWARE_MASS)) {
    Serial.println("Changing to MASSLO");
    
    page_change_to(PAGE_MASS_LO);
  } else {
    Serial.println("Changing to MAIN");
    
    page_change_to(PAGE_MAIN);
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

        btnPLNex.setValue(state_manager_get(PALHA_LENHA));
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

void btnMinusMaxEntrCB(void *ptr) {
  uint32_t value;
  maxEntrTempNex.getValue(&value);

  state_manager_set(MAX_ENTR, value);
}

void btnPlusMaxEntrCB(void *ptr) {
  uint32_t value;
  maxEntrTempNex.getValue(&value);

  state_manager_set(MAX_ENTR, value);
}

void btnMinusMinEntrCB(void *ptr) {
  uint32_t value;
  minEntrTempNex.getValue(&value);

  state_manager_set(MIN_ENTR, value);
}

void btnPlusMinEntrCB(void *ptr) {
  uint32_t value;
  minEntrTempNex.getValue(&value);

  state_manager_set(MIN_ENTR, value);
}

void btnMinusMaxMassCB(void *ptr) {
  uint32_t value;
  maxMassTempNex.getValue(&value);

  state_manager_set(MAX_MASS, value);
}

void btnPlusMaxMassCB(void *ptr) {
  uint32_t value;
  maxMassTempNex.getValue(&value);

  state_manager_set(MAX_MASS, value);
}

void btnMinusMinMassCB(void *ptr) {
  uint32_t value;
  minMassTempNex.getValue(&value);

  state_manager_set(MIN_MASS, value);
}

void btnPlusMinMassCB(void *ptr) {
  uint32_t value;
  minMassTempNex.getValue(&value);

  state_manager_set(MIN_MASS, value);
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
