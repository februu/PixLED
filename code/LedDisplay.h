#include <Adafruit_NeoPixel.h>
#include "digits.h"
#include "font.h"

#define LED_PIN                   4     // Digital output for LED Matrix
#define BRIGHTNESS                30    // Brightness of LEDs
#define ROW_LENGTH                16    // Amount of LED diodes in one row
#define NO_OF_PIXELS              256   // Number of all diodes

#define MAX_SCREENS               3     // Number of all screens except of normal clock
#define SCREEN_DISPLAY_DURATION   5300  // Duration of displaying one screen before changing to next one

class LedDisplay {

  private:

    Adafruit_NeoPixel *pixels; // Pointer to LED Matrix.
    byte *month;
    byte *day;
    byte *hours;
    byte *minutes;

    int elapsedTime = 0;
    byte currentScreen = 0;


    // Display Time 
    void displayTimeScreen() {

      byte numbersToDisplay[4];

      numbersToDisplay[0] = *hours/10;
      numbersToDisplay[1] = *hours%10;
      numbersToDisplay[2] = *minutes/10;
      numbersToDisplay[3] = *minutes%10;

      for (byte i = 0; i < 4; i++) {
        
        for (byte j = 0; j < 18; j++){

          byte pixel = CLOCK_POSITION[i] + DIGITS[numbersToDisplay[i]][j];

          if (pixel == CLOCK_POSITION[i] && j != 0)
            break;

          pixels->setPixelColor(normalizePixelCoords(pixel), pixels->Color(150, 0, 150));
          
        }
      }
    }

    // Display Date 
    void displayDateScreen() {

      byte numbersToDisplay[4];

      numbersToDisplay[0] = *day/10;
      numbersToDisplay[1] = *day%10;
      numbersToDisplay[2] = *month/10;
      numbersToDisplay[3] = *month%10;

      for (byte i = 0; i < 4; i++) {
        
        for (byte j = 0; j < 18; j++){

          byte pixel = CLOCK_POSITION[i] + DIGITS[numbersToDisplay[i]][j];

          if (pixel == CLOCK_POSITION[i] && j != 0)
            break;

          pixels->setPixelColor(normalizePixelCoords(pixel), pixels->Color(0, 0, 150));
          
        }
      }
    }

     // Display Date 
    void displayCurrencyScreen() {

      byte numbersToDisplay[4];

      numbersToDisplay[0] = *day/10;
      numbersToDisplay[1] = *day%10;
      numbersToDisplay[2] = *month/10;
      numbersToDisplay[3] = *month%10;

      for (byte i = 0; i < 4; i++) {
        
        for (byte j = 0; j < 18; j++){

          byte pixel = CLOCK_POSITION[i] + DIGITS[numbersToDisplay[i]][j];

          if (pixel == CLOCK_POSITION[i] && j != 0)
            break;

          pixels->setPixelColor(normalizePixelCoords(pixel), pixels->Color(0, 150, 0));
          
        }
      }
    }

     // Display Date 
    void displayTempScreen() {

      byte numbersToDisplay[4];

      numbersToDisplay[0] = *day/10;
      numbersToDisplay[1] = *day%10;
      numbersToDisplay[2] = *month/10;
      numbersToDisplay[3] = *month%10;

      for (byte i = 0; i < 4; i++) {
        
        for (byte j = 0; j < 18; j++){

          byte pixel = CLOCK_POSITION[i] + DIGITS[numbersToDisplay[i]][j];

          if (pixel == CLOCK_POSITION[i] && j != 0)
            break;

          pixels->setPixelColor(normalizePixelCoords(pixel), pixels->Color(150, 0, 0));
          
        }
      }
    }

  public:

    LedDisplay(Adafruit_NeoPixel *pixels, byte *month, byte *day, byte *hours, byte *minutes) : pixels(pixels), month(month), day(day), hours(hours), minutes(minutes) {}

    void display() {
      while (elapsedTime < SCREEN_DISPLAY_DURATION) {
         pixels->clear();
        displayTimeScreen();
        pixels->show();
        elapsedTime += 300;
        delay(300);
      }

      elapsedTime = 0;
     
      while (elapsedTime < SCREEN_DISPLAY_DURATION) {
        pixels->clear();
        switch(currentScreen){
          case 0:
            displayDateScreen();
          break;
          case 1:
            displayCurrencyScreen();
          break;
          case 2:
            displayTempScreen();
          break;
          }
        pixels->show();
        elapsedTime += 300;
        delay(300);
      }
     
      currentScreen = (currentScreen + 1) % MAX_SCREENS;
      elapsedTime = 0;

    }

    // Handle the LED matrix behavior where odd rows have reversed numbering.
    byte normalizePixelCoords(byte pixel) {
      byte row = (pixel / ROW_LENGTH) + 1;

      if (row % 2 == 1)
        return (ROW_LENGTH * row) - (pixel % ROW_LENGTH) - 1;

      return pixel;
    }

};