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

