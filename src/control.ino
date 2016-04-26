void manage_countdown() {
  static int tickDelay = TIMER;

  if (toggle_leds(1000)) {
    tickDelay--;

    if (tickDelay == 0) {
      currentLED--;
      tickDelay = TIMER;
    }

    if (currentLED == 0) {
      countDown = false;
    }
  }
}

void shutdown() {
  shutdown_initiate();

  for (int i = 0; i < 5; i++) {
    shutdown_warning(1000 / (i + 1));
  }

  shutdown_imminent();
  change_leds();

  digitalWrite(transistor, HIGH);
  sleep(50);

  digitalWrite(transistor, LOW);
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
