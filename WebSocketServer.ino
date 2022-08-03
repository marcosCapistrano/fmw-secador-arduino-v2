const char *ssid = "CT_33";
const char *password = "AusyxSolucoes";

WebSocketsServer webSocket = WebSocketsServer(80);

void server_task(void *pvParameters) {
  Serial.println();
  Serial.println("Configurando access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.status());
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("Server started!");

  webSocket.begin();
  webSocket.onEvent(handle_events);

  for(;;) {
    webSocket.loop();
    vTaskDelay(100);
  }
}

void handle_events(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) { 
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
//        IPAddress ip = webSocket.remoteIP(num);
//        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      if (length > 0) {
        float temperatura = atoi((const char *) payload);
        if(temperatura < 0) temperatura = 0;
        
        state_manager_set(TEMP_MASS, (uint32_t) temperatura);
        state_manager_set(LAST_COMM, millis()/1000);
        state_manager_set(HAS_COMM_ONCE, 1);

        lcd_trigger_update(TEMP_MASS);
      }
      break;
    default:
      break;
  }

}
