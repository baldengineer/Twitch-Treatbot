#include "treatbot_globals.h"

extern PubSubClient client;
extern void do_heartbeat_led();
void mqtt_stay_alive();

unsigned long previous_indicator_millis = 0;
unsigned long indicator_interval = 1000;
bool arm_indicator_countdown = false;
