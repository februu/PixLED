#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "digits.h"
#include "secrets.h"

#define LED_PIN         4     // Digital output for LED Matrix
#define BRIGHTNESS      30    // Brightness of LEDs
#define ROW_LENGTH      16    // Amount of LED diodes in one row
#define NO_OF_PIXELS    256   // Number of all diodes

#define TIME_URL        "https://timeapi.io/api/Time/current/zone?timeZone=Europe/Warsaw"   // Api address

hw_timer_t *timer = NULL;
HTTPClient http;
Adafruit_NeoPixel pixels(NO_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

byte hours = 0, minutes = 0;
volatile byte seconds = 0;

void IRAM_ATTR onTimerInterrupt(){
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes += 1;

      if (minutes == 60) {
      minutes = 0;
      hours = (hours+1)%24;
      }
  }
}

// Handle the LED matrix behavior where odd rows have reversed numbering.
byte normalizePixelCoords(byte pixel) {
  byte row = (pixel / ROW_LENGTH) + 1;

  if (row % 2 == 1)
    return (ROW_LENGTH * row) - (pixel % ROW_LENGTH) - 1;

  return pixel;
}

void displayTime() {

  byte numbersToDisplay[4];

  numbersToDisplay[0] = hours/10;
  numbersToDisplay[1] = hours%10;
  numbersToDisplay[2] = minutes/10;
  numbersToDisplay[3] = minutes%10;

  for (byte i = 0; i < 4; i++) {
    
    for (byte j = 0; j < 18; j++){

      byte pixel = CLOCK_POSITION[i] + DIGITS[numbersToDisplay[i]][j];

      if (pixel == CLOCK_POSITION[i] && j != 0)
        break;

      pixels.setPixelColor(normalizePixelCoords(pixel), pixels.Color(150, 0, 150));
      
    }
  }
}

void updateTime() {
  http.begin(TIME_URL);
    int httpCode = http.GET();

  // Wait for response
  while (httpCode == 0) 
    delay(50);
  
    StaticJsonDocument<1024> doc;

    if (deserializeJson(doc, http.getString()))
      return;
    
    hours = doc["hour"].as<byte>();
    minutes = doc["minute"].as<byte>();
    seconds = doc["seconds"].as<byte>();
   
    http.end();
      
}

void setup() {

  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);

  // Connect to Wifi and play connecting animation
  WiFi.begin(SSID, PASSWORD);
  int i;
  while(WiFi.status() != WL_CONNECTED){
    pixels.clear();
    pixels.setPixelColor(normalizePixelCoords(i), pixels.Color(0, 255, 0));
    pixels.show();
    i++;
  }

  // Send http request
    updateTime();

  // Begin timer and attach 1s interrupt.
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimerInterrupt, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

}

void loop() {
  pixels.clear();
  displayTime();
  pixels.show();
  delay(200);
}

