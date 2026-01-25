
String serverUrl = "https://omnicrola.com/esp32log/log.php";

JSONVar jsonLogEntry;

void beginNewLog() {
  jsonLogEntry = new JSONVar();
}

void sendLog() {

}

void postLog() {
  
}

void httpPost(String url, String body) {
  HTTPClient http;

  http.begin(url.c_str());

  http.addHeader("Content-Type", "application/json");
  int httpResponse = http.POST(body);

  http.end();

}