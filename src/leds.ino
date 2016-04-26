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

void shutdown_initiate() {
  int i;

  turn_off_leds();
  currentLED = MAXLEDS;
  sleep(500);

  for (i = 0; i < NUMHLEDS; i++) {
    digitalWrite(hourLEDS[i], HIGH);
    sleep(500);
  }

  for (i = 0; i < NUMMLEDS; i++) {
    digitalWrite(minuteLEDS[i], HIGH);
    sleep(500);
  }
}

void shutdown_warning(int interval) {
  static int count = 5;

  for (int i = 0; i < count; i++) {
    digitalWrite(hourLEDS[4 - i], HIGH);
    digitalWrite(minuteLEDS[i], HIGH);
  }

  sleep(interval / 2);

  if (count > 1) {
    count--;
  } else {
    count = 5;
  }

  turn_off_leds();

  sleep(interval / 2);
}

void shutdown_imminent() {
  turn_off_leds();

  sleep(125);

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i <= j; i++) {
      digitalWrite(hourLEDS[4 - i], HIGH);
      digitalWrite(minuteLEDS[i], HIGH);
    }

    sleep(125 * (j + 1));
  }

  turn_off_leds();

  sleep(250);
}

bool toggle_leds(int interval) {
  static long lastTime = millis();
  static bool litUp = true;
  int i;

  if ((lastTime + interval / 2) > millis()) {
    return false;
  }

  if (litUp) {
    currentLED--;
    litUp = false;
  } else {
    currentLED++;
    litUp = true;
  }

  change_leds();
  lastTime = millis();

  return litUp;
}
