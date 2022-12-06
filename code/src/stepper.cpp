#include "treatbot_globals.h"

void do_heartbeat_led();

bool spin_direction = CW;
bool wiggle_once = false;

void do_motor_tasks() {
  if (wiggle_once) {
    wiggle_once = false;
    print_message("Wiggle");
  }
}

void shake_em_mms(int amt, int speed, int shakes) {
  for (int x = 0; x < (shakes - 1); x++) {
    digitalWrite(DIR_PIN, spin_direction);
    for (uint16_t i = amt; i > 0; i--) {
      // processButtons();
      // process_neo();
      do_heartbeat_led();
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(speed);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(speed);
    }
    yield();
    spin_direction = !spin_direction;
  }
}

void spin_for_treats(bool direction) {
  int amount = SPIN_AMOUNT;
  int speed = SPIN_SPEED;

  digitalWrite(DIR_PIN, direction);

  for (uint16_t i = amount; i > 0; i--) {
    do_heartbeat_led();
    // processButtons();
    // process_neo();
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(speed);

  }
}

void dispense_cycle() {
  //    Almond M&Ms: 350,400,10
  // Hershey Kisses: 600,250,15
  shake_em_mms(600, 250, 15);
  spin_for_treats(false);
}