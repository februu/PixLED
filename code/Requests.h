#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "secrets.h"

#define TIME_URL "https://timeapi.io/api/Time/current/zone?timeZone=Europe/Warsaw"    // Time api address
#define CURRENCY_URL CURRENCY_API_URL                                                 // Market api address

class Request {

  private:

    HTTPClient * http; // Pointer to http client.

  public:

    Request(HTTPClient * http): http(http) {}

  void timeInit(byte & year, byte & month, byte & day, byte & hours, byte & minutes, volatile byte & seconds) {
    http -> begin(TIME_URL);
    int httpCode = http -> GET();

    // Wait for response
    while (httpCode == 0)
      delay(50);

    StaticJsonDocument < 1024 > doc;

    if (deserializeJson(doc, http -> getString()))
      return;

    year = doc["year"].as < byte > ();
    month = doc["month"].as < byte > ();
    day = doc["day"].as < byte > ();
    hours = doc["hour"].as < byte > ();
    minutes = doc["minute"].as < byte > ();
    seconds = doc["seconds"].as < byte > ();

    http -> end();
  }

};