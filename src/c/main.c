// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "window_main.h"
#include "messaging.h"
#include "app_glance.h"


// do init
static void init(void) {
  window_main_init();
  messaging_init();
}


// do deinit
static void deinit(void) {
  messaging_destroy();
  window_main_destroy();
  app_glance_destroy();
}


// main
int main(void) {
  init();
  app_event_loop();
  deinit();
}
