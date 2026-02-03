
void updateLightPanel() {
  int currentTime = getCurrentTimeInMillis();
  bool isAfterStartTime = currentTime > currentStartTime;
  bool isBeforeEndTime = currentTime < currentEndTime;

  debugTime(currentTime, currentStartTime, currentEndTime);

  if(isAfterStartTime && isBeforeEndTime) {
    int millisPastStart = currentTime - currentStartTime;
    float progression = millisPastStart / (currentFadeInDuration * 60 * 1000);
    
    debugTimeProgression(millisPastStart, currentFadeInDuration);

    progression = constrain(progression, 0.0, 1.0);
    updatePanelBrightness(progression);
  } else {
    strip.clear();
    strip.show();
  }
}

// each pixel contributes light, so the brightness is scaled by multiplying
// the total number of pixels by 255 (the maximum possible brightness)
const int totalIncrements = TOTAL_PIXELS * 255;
void updatePanelBrightness(float panelProgression) { // panelProgression 0.0 - 1.0

  // the human eye percieves brightness on a curve, not linearly
  // so here we apply a simple exp^2 curve
  float progressionCurved = pow(panelProgression, 2);

  int primaryBrightness = floor(255.0 * progressionCurved) + 1;
  int primaryCount = (int)(totalIncrements * progressionCurved) % TOTAL_PIXELS;

  debugLightProgression(panelProgression, progressionCurved, primaryBrightness, primaryCount);

  int brightness = 0;
  for(int p=0; p<TOTAL_PIXELS; p++) {
    if(p<primaryCount) {
      brightness = primaryBrightness;
    } else {
      brightness = max(primaryBrightness - 1, 0);
    }
    strip.setPixelColor(p, strip.Color(brightness, brightness, brightness));
  }

  strip.show();
}

int testPatternPosition;
void runTestPattern() {

    strip.setPixelColor(testPatternPosition, strip.Color(128,128,128));
    strip.show();
    testPatternPosition++;

    if(testPatternPosition > TOTAL_PIXELS) {
      testPatternPosition = 0;
      strip.clear();
      strip.show();
    }

}

void blinkPanel(int count) {
  strip.clear();
  for(int i=0; i<count; i++) {
    strip.setPixelColor(1, strip.Color(255,0,0));
    strip.show();
    delay(250);
    strip.clear();
    strip.show();
    delay(250);
  }
}

int fullPowerValue;
void fullPower() {

  fullPowerValue++;

  if(fullPowerValue > 255){
    fullPowerValue = 0;
  }
  strip.fill(strip.Color(fullPowerValue, fullPowerValue, fullPowerValue));
  strip.show();
}

int getCurrentTimeInMillis() {
   struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return 0;
  }
  
  struct timeval tv_now;
  gettimeofday(&tv_now, NULL);
  int64_t millis = (int64_t)tv_now.tv_usec/1000;

   return getTimeOfDayInMillis(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, millis);
}


void debugTime(int currentTime, int currentStartTime, int currentEndTime) {
  Serial.print("Time,");
  Serial.print(currentTime);
  Serial.print(",");
  Serial.print(currentStartTime);
  Serial.print(",");
  Serial.println(currentEndTime);
}

void debugTimeProgression(float millisPastStart, float fadeInDuration) {
    Serial.print("progression,");
    Serial.print(millisPastStart);
    Serial.print(",");
    Serial.println(currentFadeInDuration);
}

void debugLightProgression(float panelProgression, float progressionCurved, int primaryBrightness, int primaryCount) {
  Serial.print("progression,");
  Serial.print(panelProgression*100);
  Serial.print(",");
  Serial.print(progressionCurved*100);
  Serial.print(",");
  Serial.print(primaryBrightness);
  Serial.print(",");
  Serial.print(primaryCount);
  Serial.println();
}