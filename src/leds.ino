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

