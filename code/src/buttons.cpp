#include "treatbot_globals.h"

extern bool wiggle_once;
void handle_buttons();

void setup_buttons() {
    print_message("Buttons");
    pinMode(WIGGLE_BTN, INPUT_PULLDOWN);
    handle_buttons();
    Serial.println(F("done"));
}

// using plural in the names, but treatbot only has one
void handle_buttons() {
    static bool previous_button_state = digitalRead(WIGGLE_BTN);
    bool current_button_state = digitalRead(WIGGLE_BTN);
    if (current_button_state != previous_button_state) {
        if (current_button_state == PRESSED) {
            wiggle_once = true;
        }
        previous_button_state = current_button_state;
    }
}