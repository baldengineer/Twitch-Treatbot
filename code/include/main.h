#include "treatbot_globals.h"

extern PubSubClient client;
void setup_oled();
extern void do_heartbeat_led();
void mqtt_stay_alive();
void handle_buttons();
void do_motor_tasks();

unsigned long previous_indicator_millis = 0;
unsigned long indicator_interval = 1000;
bool arm_indicator_countdown = false;
