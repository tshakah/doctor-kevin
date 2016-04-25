/*
  Generator control script on a TI MSP430g2452
  Author: Daniel Hastings (tshakah)

  For debouncing ideas, thanks to:
  https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
  http://www.ganssle.com/debouncing.pdf
  http://www.kennethkuhn.com/electronics/debounce.c (this algorithm worked best)
*/

byte buttons[] = {14, 15, 19}; // Pins the buttons are wired to
byte hourLEDS[] = {9, 8, 7, 6, 5}; // Pins the hour LEDs are wired to
byte minuteLEDS[] = {4, 3, 2, 13, 12}; // Pins the minute LEDs are wired to
int transistor = 18;
int currentLED = 0; // This LED and the ones before it are lit up (0 is none)
bool countDown;

#define NUMBUTTONS sizeof(buttons)
#define NUMHLEDS sizeof(hourLEDS)
#define NUMMLEDS sizeof(minuteLEDS)
#define MAXLEDS (NUMHLEDS + 1) * (NUMMLEDS + 1) - 1
#define MAXIMUM 10 // Max number of debouncing passes before value changes

byte justPressed[NUMBUTTONS], integrator[NUMBUTTONS], pressed[NUMBUTTONS];

void setup() {
  BCSCTL1 = CALBC1_16MHZ;
  DCOCTL = CALDCO_16MHZ;
  int i;

  for (i = 0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  for (i = 0; i < NUMHLEDS; i++) {
    pinMode(hourLEDS[i], OUTPUT);
    digitalWrite(hourLEDS[i], LOW);
  }

  for (i = 0; i < NUMMLEDS; i++) {
    pinMode(minuteLEDS[i], OUTPUT);
    digitalWrite(minuteLEDS[i], LOW);
  }

  pinMode(transistor, OUTPUT);
  digitalWrite(transistor, LOW);
}

void loop() {
  int oldCount = currentLED;

  if (countDown) {
    if (justPressed[2]) {
      justPressed[2] = 0;
    }

    manage_countdown();

    if (!countDown) {
      shutdown();
      reset();
    }
  } else {
    check_switches();

    count_presses();

    if (oldCount != currentLED) {
      change_leds();
    }
  }
}
