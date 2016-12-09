// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#pragma once
#include <pebble.h>

void window_main_actionbar_config(void *);
void window_main_set_time_remaing_counter(const char *);
void window_main_set_filename(const char *);
void window_main_set_nozzle(const uint16_t, const uint16_t);
void window_main_set_bed(const uint16_t);
void window_main_set_state(const char *);
void window_main_set_progress(const uint8_t);
void window_main_timer_fired(struct tm *, TimeUnits);
void window_main_init(void);
void window_main_destroy(void);
