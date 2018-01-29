#include <FastLED.h>

#define NUM_LEDS 8
#define DATA_PIN 6
#define BUTTON_PIN 7

CRGB leds[NUM_LEDS];

boolean active = true;
char input;
const int ledsPerSegment = 1;
int counter = 40;
int activeLeds = 0;

// Debounce variables.
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState;
int buttonState;

byte maxBrightness = 64;
int firstDigit = 9;
int secondDigit = 9;

/*  
 *  |-f--|
    e    g
    |-a--|
    d    b
    |-c--|
 *
 */
                    //  a, b, c, d, e, f, g
int numbers[10][7]  = {{0, 1, 1, 1, 1, 1, 1},  // 0
                       {0, 1, 0, 0, 0, 0, 1},  // 1
                       {1, 0, 1, 1, 0, 1, 1},  // 2
                       {1, 1, 1, 0, 0, 1, 1},  // 3
                       {1, 1, 0, 0, 1, 0, 1},  // 4
                       {1, 1, 1, 0, 1, 1, 0},  // 5
                       {1, 1, 1, 1, 1, 1, 0},  // 6
                       {0, 1, 0, 0, 0, 1, 1},  // 7
                       {1, 1, 1, 1, 1, 1, 1},  // 8
                       {1, 1, 1, 0, 1, 1, 1}}; // 9


void setup() {
  // FastLED.addLeds<(Model), (Data pin)>((Store array), (# of leds));
  FastLED.addLeds<WS2813, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(maxBrightness);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    //input = (char) Serial.read();
    input = 'l';
  }

  if (input == 'x') {
    active = !active;
    Serial.print("Switched to ");
    Serial.println(active ? "active." : "inactive.");
  }
  if (active) {
  int buttonPressed = digitalRead(BUTTON_PIN);
  
  // If the button state changed (started pressing, just released or noise)
  if (buttonPressed != lastButtonState) {
    lastDebounceTime = millis();
  }

  // If lastDebounceTime did not get reset long enough ( => consistent button press)
  // Change the button state.
  if (millis() > lastDebounceTime + debounceDelay) {
    if (buttonPressed != buttonState) {
      buttonState = buttonPressed;

      // On button press
      if (buttonState == HIGH) {
        changeNumber();
      }
    }
  }
  lastButtonState = buttonPressed;
  
  if (input == 't') {
      // Draw every LED separately.
      test();
    } else if (input == 'l') {
      // Draw all digits 00, 11, 22 ... 99.
      for (int i = 0; i < 10; i++) {
        drawNumber(true, i);
        //drawNumber(false, i);
      } 
    } else if (input == 'r') {
      // Draw the current counter.
      drawNumber(true, firstDigit);
      //drawNumber(false, secondDigit);
    } else if (input == '0') {
      drawNumber(true, 0);
    } else if (input == '1') {
      drawNumber(true, 1);
    } else if (input == '2') {
      drawNumber(true, 2);
    } else if (input == '3') {
      drawNumber(true, 3);
    } else if (input == '4') {
      drawNumber(true, 4);
    } else if (input == '5') {
      drawNumber(true, 5);
    } else if (input == '6') {
      drawNumber(true, 6);
    } else if (input == '7') {
      drawNumber(true, 7);
    } else if (input == '8') {
      drawNumber(true, 8);
    } else if (input == '9') {
      drawNumber(true, 9);
    }
  }

  input = 'z';
}

void changeNumber() {
  counter--;
  if (counter < 0) {
    counter = 40;
  }
  firstDigit = counter/10;
  secondDigit = counter%10;
  drawNumber(true, firstDigit);
  drawNumber(false, secondDigit);
}

void drawNumber(bool leftDigit, int digit) {
  FastLED.clear();
  for (int i = 0; i < 7; i++) {
    bool segment = (numbers[digit][i]);
    if (segment) {
      for (int j = i * ledsPerSegment; j < i * ledsPerSegment + ledsPerSegment; j++) {
        int led = (leftDigit ? j : (j/* + 42*/));
        // Add one because the first one is broken.
        leds[led + 1] = CRGB::Blue;
      }
    }
  }
  FastLED.show();
}

void test() {
  for (int i = 0; i < NUM_LEDS; i++) {
    FastLED.clear();
    leds[i] = CRGB::Blue;
    FastLED.show();
    delay(30);
  }
  FastLED.clear();
}
