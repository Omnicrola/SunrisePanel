#include "esp_netif_sntp.h"
#include <Button.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <Adafruit_NeoPixel.h> 

// WiFi config
// WIFI : IoT_Guest - iamguest99
const char* WIFI_SSID = "IoT_Guest";
const char* WIFI_PASS = "iamguest99";

///////////// Sunrise config /////////////////
int getTimeOfDayInMillis(int hours, int minutes, int seconds, int millis); // function declaration

// WS2812 configuration
#define LED_PIN 32
#define TOTAL_PIXELS 161

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

Button modeButton(22);

const char* NTP_SERVER = "0.north-america.pool.ntp.org";

float fadeInDuration = 60; // in minutes
int startTime = getTimeOfDayInMillis(6, 30, 0, 0);
int endTime = getTimeOfDayInMillis(9, 0, 0, 0);
int currentStartTime = startTime;
int currentEndTime = endTime;
float currentFadeInDuration = fadeInDuration;
uint32_t skyColor = strip.Color(255, 255, 255);

void setup() {
  modeButton.begin();

  strip.setBrightness(255);
  strip.begin();
  strip.clear();
  strip.show();

  Serial.begin(115200);
  delay(1000);

  syncTimeWithNtp();
  runTestPattern();
}

bool isButtonPressed = false;
int modeState = 0;
void loop() {

  if(modeButton.released()) {
    setMode(modeState + 1);
    
    if(modeState == 3) {
      // only set this once, instead of in the switch statement
      setTestTimes();
    }

    blinkPanel(modeState);

  }

  int currentTime = getCurrentTimeInMillis();

  switch(modeState) {
    case 1 :
      runTestPattern();
      break;
    case 2 :
      fullPower();
      break;
    case 3 :
      if(currentTime > currentEndTime) {
        Serial.print("End time of test mode reached.");
        Serial.print(currentTime);
        Serial.print(" > ");
        Serial.println(currentEndTime);
        setMode(0);
      } else {
        updateLightPanel();
      }
      break;
    case 0 : 
    default : 
      currentStartTime = startTime;
      currentEndTime = endTime;
      currentFadeInDuration = fadeInDuration;
      updateLightPanel();

  }

}

void setMode(int newMode) {
  if(newMode > 3) {
    newMode = 0;
  }

  Serial.print("Mode change : ");
  Serial.print(modeState);
  Serial.print(" -> ");
  Serial.println(newMode);
  modeState = newMode;
}

void setTestTimes() {
  int currentTime = getCurrentTimeInMillis();
  currentStartTime = currentTime + (30 * 1000);
  currentEndTime = currentStartTime + (5 * 60 * 1000);
  currentFadeInDuration = 1;

  Serial.print("Setting test times : ");
  Serial.print(currentStartTime);
  Serial.print(" | ");
  Serial.print(currentEndTime);
  Serial.print(" | ");
  Serial.println(currentFadeInDuration);
}
