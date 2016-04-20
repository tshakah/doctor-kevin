/*
  Generator control script on a TI MSP430g2452
  Author: Daniel Hastings (tshakah)

  For debouncing ideas, thanks to:
  https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
  http://www.ganssle.com/debouncing.pdf
  http://www.kennethkuhn.com/electronics/debounce.c (this algorithm worked best)
*/

byte buttons[] = {8, 9}; // Pins the buttons are wired to
byte leds[] = {2, 5, 6, 7}; // Pins the LEDs are wired to
int currentLED = -1; // This LED and the ones before it are lit up (-1 is none)

#define NUMBUTTONS sizeof(buttons)
#define NUMLEDS sizeof(leds)
#define MAXIMUM 7

byte pressed[NUMBUTTONS], justPressed[NUMBUTTONS];

void setup() {
  int i;

  for (i = 0; i < NUMLEDS; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  for (i = 0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }
}

void check_switches() {
  byte input[NUMBUTTONS];
  static byte integrator[NUMBUTTONS];

  for (int i = 0; i < NUMBUTTONS; i++) {
    input[i] = digitalRead(buttons[i]);

    if (input[i] == 1) {
      if (integrator[i] > 0) {
        integrator[i]--;
      }
    } else if (integrator[i] < MAXIMUM) {
      integrator[i]++;
    }

    if (integrator[i] == 0) {
      pressed[i] = 0;
      justPressed[i] = 0;
    } else if (integrator[i] >= MAXIMUM) {
      if (pressed[i] == 0) {
        justPressed[i] = 1;
        pressed[i] = 1;
      } else {
        justPressed[i] = 0;
      }
      integrator[i] = MAXIMUM;
    }
  }
}

void count_presses() {
  if (justPressed[0]) {
    currentLED++;
  }

  if (justPressed[1]) {
    currentLED--;
  }

  if (currentLED < -1) {
    currentLED = NUMLEDS - 1;
  } else if (currentLED >= NUMLEDS) {
    currentLED = -1;
  }
}

void change_leds() {
  int i;

  for (i = 0; i < NUMLEDS; i++) {
    digitalWrite(leds[i], LOW);
  }

  for (i = 0; i <= currentLED; i++) {
    digitalWrite(leds[i], HIGH);
  }
}

void loop() {
  int oldCount = currentLED;

  check_switches();

  count_presses();

  if (oldCount != currentLED) {
    change_leds();
  }
}
