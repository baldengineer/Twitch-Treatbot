#include <treatbot_globals.h>

uint32_t treat_cycle_interval = 1000;
uint32_t previous_treat_cycle_millis = 0;

bool treat_cycle_arm = false;

extern void candy_test();
extern void oled_clear();
extern void dispense_cycle();

void start_treats_cycle() {
  // previous_treat_cycle_millis = millis();
  // treat_cycle_arm = true;
  //neo_panel_enable = true;
  print_message("Candy!");
  candy_test();
  dispense_cycle();
  oled_clear();
  print_message("No moar");
}

// void handle_treats() {
//   if ((treat_cycle_arm) && (millis() - previous_treat_cycle_millis >= treat_cycle_interval)) {
//     treat_cycle_arm = false;
//     oled_clear();
//     print_message("No moar");
//   }
// }