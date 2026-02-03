void syncTimeWithNtp() {
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  
  esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(NTP_SERVER);
  esp_netif_sntp_init(&config);
  if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
    Serial.println("Failed to update system time within 10s timeout");
  } else {
    Serial.println("System time updated successfully.");
  }

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

int getTimeOfDayInMillis(int hours, int minutes, int seconds, int millis) {
  int convertedHours = hours * 60 * 60 * 1000;
  int convertedMinutes = minutes * 60 * 1000;
  int convertedSeconds = seconds * 1000;
  return convertedHours + convertedMinutes + convertedSeconds + millis;
}