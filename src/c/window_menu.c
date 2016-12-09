// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "window_menu.h"
#include "window_confirm.h"
#include "window_message.h"
#include "messaging.h"

#define MENU_SECTIONS 3
#define MENU_FIRST_ITEMS 1
#define MENU_SECOND_ITEMS 6
#define MENU_THIRD_ITEMS 2

static Window *s_window_main;

static SimpleMenuLayer *s_layer_simplemenu;
static SimpleMenuSection s_menu_sections[MENU_SECTIONS];
static SimpleMenuItem s_menu_items_first[MENU_FIRST_ITEMS];
static SimpleMenuItem s_menu_items_second[MENU_SECOND_ITEMS];
static SimpleMenuItem s_menu_items_third[MENU_THIRD_ITEMS];


// user canceled printjob
static void window_menu_cancel_printjob(void) {
  messaging_outbox_send("cancel", "");
}


// confirm cancel printjob
static void window_menu_confirm_cancel_printjob(int s, void *ctx) {
  window_confirm_init_custom("Abort printjob?", PBL_IF_COLOR_ELSE(GColorRed, GColorWhite), window_menu_cancel_printjob);
}


// preheat bed and nozzle
static void window_menu_preheat(int s, void *ctx) {
  window_message_init_custom("Preheat", GColorOrange);
  messaging_outbox_send("preheat", "");
}


// preheat bed
static void window_menu_preheat_bed(int s, void *ctx) {
  window_message_init_custom("Preheat Bed", GColorOrange);
  messaging_outbox_send("preheat", "bed");
}


// preheat nozzle
static void window_menu_preheat_nozzle(int s, void *ctx) {
  window_message_init_custom("Preheat Nozzle", GColorOrange);
  messaging_outbox_send("preheat", "nozzle");
}


// cooldown bed and nozzle
static void window_menu_cooldown(int s, void *ctx) {
  window_message_init_custom("Cooldown", GColorCyan);
  messaging_outbox_send("gcode", "M117 Cooldown...\nM140 S0\nM104 S0");
}


// turn fans on
static void window_menu_fans_on(int s, void *ctx) {
  window_message_init_custom("Fans on", GColorGreen);
  messaging_outbox_send("gcode", "M117 Fans on...\nM106 S255");
}


// turn fans off
static void window_menu_fans_off(int s, void *ctx) {
  window_message_init_custom("Fans off", GColorGreen);
  messaging_outbox_send("gcode", "M117 Fans off...\nM106 S0");
}


// home all axes
static void window_menu_home_axes(int s, void *ctx) {
  window_message_init_custom("Homing", GColorMagenta);
  messaging_outbox_send("gcode", "M117 Homing...\nG28");
}


// turn all motors off
static void window_menu_motors_off(int s, void *ctx) {
  window_message_init_custom("Motors off", GColorVividViolet);
  messaging_outbox_send("gcode", "M117 Motors off...\nM18");
}


// load window handler
static void window_menu_load_handler(Window *window) {
  // create window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // create first menu section
  s_menu_items_first[0] = (SimpleMenuItem) {
    .title = "Abort printjob",
    .subtitle = "Stop running printjob",
    .callback = window_menu_confirm_cancel_printjob
  };
  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Printjob",
    .num_items = MENU_FIRST_ITEMS,
    .items = s_menu_items_first
  };
  
  // create second menu section
  s_menu_items_second[0] = (SimpleMenuItem) {
    .title = "Preheat",
    .subtitle = "Preheat nozzle and bed",
    .callback = window_menu_preheat
  };
  s_menu_items_second[1] = (SimpleMenuItem) {
    .title = "Preheat bed",
    .subtitle = "Preheat bed only",
    .callback = window_menu_preheat_bed
  };
  s_menu_items_second[2] = (SimpleMenuItem) {
    .title = "Preheat nozzle",
    .subtitle = "Preheat nozzle only",
    .callback = window_menu_preheat_nozzle
  };
  s_menu_items_second[3] = (SimpleMenuItem) {
    .title = "Cooldown",
    .subtitle = "Cooldown nozzle and bed",
    .callback = window_menu_cooldown
  };
  s_menu_items_second[4] = (SimpleMenuItem) {
    .title = "Fans on",
    .subtitle = "Turn the fans on",
    .callback = window_menu_fans_on
  };
  s_menu_items_second[5] = (SimpleMenuItem) {
    .title = "Fans off",
    .subtitle = "Turn the fans off",
    .callback = window_menu_fans_off
  };
  s_menu_sections[1] = (SimpleMenuSection) {
    .title = "Temperature",
    .num_items = MENU_SECOND_ITEMS,
    .items = s_menu_items_second
  };
  
  // create third menu section
  s_menu_items_third[0] = (SimpleMenuItem) {
    .title = "Home all",
    .subtitle = "Home all axes (XYZ)",
    .callback = window_menu_home_axes
  };
  s_menu_items_third[1] = (SimpleMenuItem) {
    .title = "Motors off",
    .subtitle = "Turn the motors off",
    .callback = window_menu_motors_off
  };
  s_menu_sections[2] = (SimpleMenuSection) {
    .title = "Motion",
    .num_items = MENU_THIRD_ITEMS,
    .items = s_menu_items_third
  };
  
  // create simplemenulayer
  s_layer_simplemenu = simple_menu_layer_create(bounds, window, s_menu_sections, MENU_SECTIONS, NULL);
  menu_layer_set_highlight_colors(simple_menu_layer_get_menu_layer(s_layer_simplemenu), PBL_IF_COLOR_ELSE(GColorVividCerulean, GColorBlack), PBL_IF_COLOR_ELSE(GColorBlack, GColorWhite));
  layer_add_child(window_layer, simple_menu_layer_get_layer(s_layer_simplemenu));
}


// unload window handler
static void window_menu_unload_handler(Window *window) {
  // destroy simple menu layer
  simple_menu_layer_destroy(s_layer_simplemenu);
  
  // destroy window
  window_destroy(s_window_main);
}


// create new window
void window_menu_init(void) {
  s_window_main = window_create();
  window_set_background_color(s_window_main, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  window_set_window_handlers(s_window_main, (WindowHandlers) {
    .load = window_menu_load_handler,
    .unload = window_menu_unload_handler
  });
  window_stack_push(s_window_main, true);
}


// destroy window
void window_menu_destroy(void) {
  window_stack_pop(true);
}
