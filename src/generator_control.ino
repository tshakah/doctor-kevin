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

  if (currentLED < 0) {
    currentLED = MAXLEDS;
  } else if (currentLED > MAXLEDS) {
    currentLED = 0;
  }
}

void turn_off_leds() {
  int i;

  for (i = 0; i < NUMHLEDS; i++) {
    digitalWrite(hourLEDS[i], LOW);
  }

  for (i = 0; i < NUMMLEDS; i++) {
    digitalWrite(minuteLEDS[i], LOW);
  }

}

void change_leds() {
  int i;

  turn_off_leds();

  for (i = 0; i < (currentLED / 6); i++) {
    digitalWrite(hourLEDS[i], HIGH);
  }

  for (i = 0; i < (currentLED % 6); i++) {
    digitalWrite(minuteLEDS[i], HIGH);
  }
}

void flash_leds(int timer) {
  change_leds();
  sleep(timer / 2);

  turn_off_leds();
  sleep(timer / 2);
}

bool toggle_leds(int interval) {
  static long lastTime = millis();
  static bool litUp = false;
  int i;

  if ((lastTime + interval / 2) > millis()) {
    return false;
  }

  if (litUp) {
    turn_off_leds();
    lastTime = millis();
    litUp = false;
    return false;
  } else {
    change_leds();
    lastTime = millis();
    litUp = true;
    return true;
  }
}

void manage_countdown() {
  static int timer = 10;

  if (toggle_leds(1000)) {
    timer--;

    if (timer == 0) {
      currentLED--;
      timer = 10;
    }

    if (currentLED == 0) {
      countDown = false;
    }
  }
}

void shutdown() {
  int i;
  currentLED = MAXLEDS;

  flash_leds(1000);
  flash_leds(500);
  flash_leds(500);
  flash_leds(250);
  flash_leds(250);
  flash_leds(250);

  change_leds();

  digitalWrite(transistor, HIGH);

  sleep(2000);
}

void reset() {
  currentLED = 0;
  digitalWrite(transistor, LOW);

  turn_off_leds();

  for (int i = 0; i < NUMBUTTONS; i++) {
    justPressed[i] = 0;
    integrator[i] = 0;
    pressed[i] = 0;
  }
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
