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
