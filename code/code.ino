#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "LedDisplay.h"
#include "Requests.h"
#include "constants.h"
#include "secrets.h"

hw_timer_t *timer = NULL;

HTTPClient http;
Adafruit_NeoPixel pixels(NO_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

byte hours = 0, minutes = 0, day = 0, month = 0, year = 0;
volatile byte seconds = 0;

float prices[8];

Request request(&http);
LedDisplay ld(&pixels, &month, &day, &hours, &minutes);

// Handle timer interrupt every 1 second.
void IRAM_ATTR onTimerInterrupt(){
seconds++;

    if (seconds >= 60) {
        seconds = 0;
        minutes++;

        if (minutes >= 60) {
            minutes = 0;
            hours++;

            if (hours >= 24) {
                hours = 0;
                day++;

                if (day > daysInMonth[month - 1]) {
                    if (month == 2 && isLeapYear(year)) {
                        if (day > 29) {
                            day = 1;
                            month++;
                        }
                    } else {
                        day = 1;
                        month++;
                    }

                    if (month > 12) {
                        month = 1;
                        year++;
                    }
                }
            }
        }
    }
}

// If wifi disconnects, connect again.
void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
  WiFi.useStaticBuffers(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
}

void setup() {

  pixels.begin();
  pixels.setBrightness(BRIGHTNESS);

  // Connect to Wifi and play connecting animation
  WiFi.onEvent(onWifiDisconnect, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.useStaticBuffers(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  byte i;
  while(WiFi.status() != WL_CONNECTED){
    pixels.clear();
    pixels.setPixelColor(ld.normalizePixelCoords(i), pixels.Color(0, 255, 0));
    pixels.show();
    i++;
  }
  pixels.clear();
  request.timeInit(year, month, day, hours, minutes, seconds);

  // Begin timer and attach 1s interrupt.
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimerInterrupt, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

}

void loop() {
  ld.display();
}

