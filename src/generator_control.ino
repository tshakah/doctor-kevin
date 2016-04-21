/*
  Generator control script on a TI MSP430g2452
  Author: Daniel Hastings (tshakah)

  For debouncing ideas, thanks to:
  https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/
  http://www.ganssle.com/debouncing.pdf
  http://www.kennethkuhn.com/electronics/debounce.c (this algorithm worked best)
*/

byte buttons[] = {8, 9, 12}; // Pins the buttons are wired to
byte leds[] = {2, 5, 6, 7}; // Pins the LEDs are wired to
int transistor = 13;
int power = 14;
int currentLED = -1; // This LED and the ones before it are lit up (-1 is none)
bool countDown;

#define NUMBUTTONS sizeof(buttons)
#define NUMLEDS sizeof(leds)
#define MAXIMUM 7 // Max number of debouncing passes before value changes

byte justPressed[NUMBUTTONS], integrator[NUMBUTTONS], pressed[NUMBUTTONS];

void setup() {
  int i;

  for (i = 0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  for (i = 0; i < NUMLEDS; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  pinMode(transistor, OUTPUT);
  digitalWrite(transistor, LOW);
  pinMode(power, OUTPUT);
  digitalWrite(power, HIGH);
}

void check_switches() {
  byte input[NUMBUTTONS];

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

  if (justPressed[2]) {
    countDown = true;
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

void flash_leds(int timer) {
  int i;

  for (i = 0; i <= currentLED; i++) {
    digitalWrite(leds[i], HIGH);
  }

  sleep(timer / 2);

  for (i = 0; i < NUMLEDS; i++) {
    digitalWrite(leds[i], LOW);
  }

  sleep(timer / 2);
}

void manage_countdown() {
  while (countDown) {
    int timer = 2;

    do {
      flash_leds(1000);
      timer--;
    } while(timer > 0);

    currentLED--;

    if (currentLED == -1) {
      countDown = false;
      break;
    }
  }
}

void shutdown() {
  int i;
  currentLED = NUMLEDS;

  flash_leds(1000);
  flash_leds(500);
  flash_leds(500);
  flash_leds(250);
  flash_leds(250);
  flash_leds(250);

  for (i = 0; i < NUMLEDS; i++) {
    digitalWrite(leds[i], HIGH);
  }

  digitalWrite(transistor, HIGH);

  sleep(2000);
}

void reset() {
  int i;
  currentLED = -1;
  digitalWrite(transistor, LOW);

  for (i = 0; i < NUMLEDS; i++) {
    digitalWrite(leds[i], LOW);
  }

  for (i = 0; i < NUMBUTTONS; i++) {
    justPressed[i] = 0;
    integrator[i] = 0;
    pressed[i] = 0;
  }
}

void loop() {
  int oldCount = currentLED;

  check_switches();

  count_presses();

  if (oldCount != currentLED) {
    change_leds();
  }

  if (countDown) {
    for (int i = 0; i < NUMLEDS; i++) {
      digitalWrite(leds[i], LOW);
    }

    manage_countdown();

    shutdown();
    reset();
  }
}
