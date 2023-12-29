#include <Adafruit_NeoPixel.h>
#include "digits.h"

#define LED_PIN       4
#define ROW_LENGTH  16
#define NO_OF_PIXELS  256

Adafruit_NeoPixel pixels(NO_OF_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

byte hours = 0, minutes = 0;

//Handles the LED matrix behavior where odd rows have reversed numbering.
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


void setup() {
  pixels.begin();
  pixels.setBrightness(20);
}


void loop() {
  pixels.clear();
  displayTime();
  pixels.show();

  
  minutes = (minutes + 1)%100;
  if (minutes == 0)
    hours = (hours + 1)%100;
  delay(100);
}

