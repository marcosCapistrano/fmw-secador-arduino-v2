/*
   Declaração dos Componentes Nextion
*/
NexPage pageMain = NexPage(0, 0, "page0");                 //--> Tela Principal
NexPage pageMassHI = NexPage(1, 0, "page1");                 //--> Tela massa acima
NexPage pageEntrHI = NexPage(2, 0, "page2");                 //--> Tela entrada acima
NexPage pageEntrLO = NexPage(3, 0, "page3");                 //--> Tela entrada abaixo
NexPage pageMassLO = NexPage(4, 0, "page4");                 //--> Tela massa abaixo

NexDSButton btnPLNex = NexDSButton(0, 16, "bt0");                                //--> Palha/lenha

NexDSButton btnMinusMaxEntrNex = NexDSButton(0, 16, "bt1");
NexDSButton btnPlusMaxEntrNex = NexDSButton(0, 16, "bt2");
NexDSButton btnMinusMinEntrNex = NexDSButton(0, 16, "bt3");
NexDSButton btnPlusMinEntrNex = NexDSButton(0, 16, "bt4");

NexDSButton btnMinusMaxMassNex = NexDSButton(0, 16, "bt5");
NexDSButton btnPlusMaxMassNex = NexDSButton(0, 16, "bt6");
NexDSButton btnMinusMinMassNex = NexDSButton(0, 16, "bt7");
NexDSButton btnPlusMinMassNex = NexDSButton(0, 16, "bt8");

NexNumber entrTempNex  = NexNumber(0, 13, "n0");                          //--> Temperatura da Entrada        (Write)
NexNumber massTempNex  = NexNumber(0, 14, "n1");                          //--> Temperatura da Massa          (Write)

NexNumber maxEntrTempNex = NexNumber(0, 9, "n2");                           //--> Temperatura máxima da Turbina (Read)
NexNumber minEntrTempNex = NexNumber(0, 10, "n3");                          //--> Temperatura mínima da Turbina (Read)
NexNumber maxMassTempNex = NexNumber(0, 11, "n4");                          //--> Temperatura máxima da Massa   (Read)
NexNumber minMassTempNex = NexNumber(0, 12, "n5");                          //--> Temperatura mínima da Massa   (Read)
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

  page_change_to(PAGE_MAIN);

  entrTempNex.setValue(state_manager_get(TEMP_ENTR));
  massTempNex.setValue(state_manager_get(TEMP_MASS));

  maxEntrTempNex.setValue(state_manager_get(MAX_ENTR));
  minEntrTempNex.setValue(state_manager_get(MIN_ENTR));
  maxMassTempNex.setValue(state_manager_get(MAX_MASS));
  minMassTempNex.setValue(state_manager_get(MIN_MASS));

  btnPLNex.setValue(state_manager_get(PALHA_LENHA));
}

void lcd_loop(void) {
  nexLoop(nex_listen_list);

  if (state_manager_get(TEMP_ENTR) >= state_manager_get(MAX_ENTR) + HIST_ENTR && !state_manager_get(IS_AWARE_ENTR)) {
    page_change_to(PAGE_ENTR_HI);
  } else if (state_manager_get(TEMP_ENTR) <= state_manager_get(MIN_ENTR) - HIST_ENTR && !state_manager_get(IS_AWARE_ENTR)) {
    page_change_to(PAGE_ENTR_LO);
  } else if (state_manager_get(TEMP_MASS) >= state_manager_get(MAX_MASS) + HIST_MASS && !state_manager_get(IS_AWARE_MASS)) {
    page_change_to(PAGE_MASS_HI);
  } else if (state_manager_get(TEMP_MASS) <= state_manager_get(MIN_MASS) - HIST_MASS && !state_manager_get(IS_AWARE_MASS)) {
    page_change_to(PAGE_MASS_LO);
  } else {
    page_change_to(PAGE_MAIN);
  }

  if(curr_page == PAGE_MAIN) {
    entrTempNex.setValue(state_manager_get(TEMP_ENTR));
    massTempNex.setValue(state_manager_get(TEMP_MASS));

    maxEntrTempNex.setValue(state_manager_get(MAX_ENTR));
    minEntrTempNex.setValue(state_manager_get(MIN_ENTR));
    maxMassTempNex.setValue(state_manager_get(MAX_MASS));
    minMassTempNex.setValue(state_manager_get(MIN_MASS));

    btnPLNex.setValue(state_manager_get(PALHA_LENHA));
  }
}

void page_change_to(lcd_page_t page) {
  if (curr_page != page) {
    switch (page) {
      case PAGE_MAIN:
        pageMain.show();
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
