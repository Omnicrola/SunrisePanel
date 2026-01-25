void syncTimeWithNtp() {
  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("\nConnecting");

  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  waitForSync(); // wait for NTP time sync
  
  Timezone timeProvider;
  timeProvider.setLocation("America/Detroit");
  Serial.print("Current time: ");
  Serial.println(timeProvider.dateTime());
}