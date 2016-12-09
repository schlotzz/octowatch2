// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#pragma once
#include <pebble.h>

typedef Layer ProgressLayer;
ProgressLayer* progress_layer_create(GRect frame);
void progress_layer_destroy(ProgressLayer* progress_layer);
void progress_layer_increment_progress(ProgressLayer* progress_layer, int16_t progress);
void progress_layer_set_progress(ProgressLayer* progress_layer, int16_t progress_percent);
void progress_layer_set_corner_radius(ProgressLayer* progress_layer, uint16_t corner_radius);
void progress_layer_set_foreground_color(ProgressLayer* progress_layer, GColor color);
void progress_layer_set_background_color(ProgressLayer* progress_layer, GColor color);
