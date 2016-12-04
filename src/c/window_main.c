// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "window_main.h"
#include "window_menu.h"
#include "printer.h"
#include "messaging.h"
#include "progress_layer.h"

#define TIMER_INTERVAL 5*1000

static Window *s_window_main;

static ActionBarLayer *s_actionbar;

static GBitmap *image_connect;
static GBitmap *image_refresh;
static GBitmap *image_menu;
static GBitmap *image_pause;
static GBitmap *image_start;
static GBitmap *image_nozzle;
static GBitmap *image_bed;

static Layer *s_bg_layer;

static ProgressLayer *s_progress_layer;

static TextLayer *s_layer_time_remaining_counter;
static TextLayer *s_layer_filename;
static TextLayer *s_layer_nozzle;
static TextLayer *s_layer_bed;
static TextLayer *s_layer_state;

static BitmapLayer *s_bitmaplayer_nozzle;
static BitmapLayer *s_bitmaplayer_bed;


// handle click on up button
static void window_main_actionbar_up(ClickRecognizerRef recognizer, void *context) {
  messaging_outbox_send("pause", "");
}


// handle click on select button
static void window_main_actionbar_select(ClickRecognizerRef recognizer, void *context) {
  window_menu_init();
}


// handle click on down button
static void window_main_actionbar_down(ClickRecognizerRef recognizer, void *context) {
  messaging_outbox_send("update", "");
}


// action bar config
void window_main_actionbar_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) window_main_actionbar_up);
	window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) window_main_actionbar_select);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) window_main_actionbar_down);
}


// render background layer
static void window_main_draw_background_layer(Layer *layer, GContext *context) {
	GRect bounds = layer_get_bounds(layer);
  
  GColor8 s_state_colors[] = {
    PBL_IF_COLOR_ELSE(GColorDarkGray, GColorWhite),    // LOADING
    PBL_IF_COLOR_ELSE(GColorRed, GColorWhite),         // OFFLINE
    PBL_IF_COLOR_ELSE(GColorOrange, GColorWhite),      // CONNECTING
    PBL_IF_COLOR_ELSE(GColorCyan, GColorWhite),        // OPERATIONAL
    PBL_IF_COLOR_ELSE(GColorBrightGreen, GColorWhite), // PRINTING
    PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite),      // PAUSED
    PBL_IF_COLOR_ELSE(GColorFolly, GColorWhite)        // ERROR
  };
	
	graphics_context_set_fill_color(context, GColorBlack);
	
	// time remaining container
	graphics_fill_rect(context, GRect(0, 0, bounds.size.w, 46), 4, GCornersAll);
	
	// progress bar
	graphics_fill_rect(context, GRect(0, 48, bounds.size.w, 10), 4, GCornersAll);
  
	// filename container
  graphics_fill_rect(context, GRect(0, 65, bounds.size.w, 51), 4, GCornersAll);
	//graphics_draw_round_rect(context, GRect(0, 77, bounds.size.w, 36), 4);

	// nozzle/bed container
  graphics_fill_rect(context, GRect(0, 118, bounds.size.w, 22), 4, GCornersAll);
  
	// status container
  graphics_context_set_fill_color(context, s_state_colors[printer_get_state()]);
  graphics_fill_rect(context, GRect(0, bounds.size.h - 22, bounds.size.w, 22), 4, GCornersAll);
}


// load window handler
static void window_main_load_handler(Window *window) {
  // create window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // load images
	image_connect = gbitmap_create_with_resource(RESOURCE_ID_CONNECT_ICON);
  image_refresh = gbitmap_create_with_resource(RESOURCE_ID_REFRESH_ICON);
  image_menu = gbitmap_create_with_resource(RESOURCE_ID_ELLIPSIS_ICON);
  image_start = gbitmap_create_with_resource(RESOURCE_ID_PLAY_ICON);
  image_pause = gbitmap_create_with_resource(RESOURCE_ID_PAUSE_ICON);
  image_nozzle = gbitmap_create_with_resource(RESOURCE_ID_NOZZLE_ICON);
  image_bed = gbitmap_create_with_resource(RESOURCE_ID_BED_ICON);
  
  // create action bar
	s_actionbar = action_bar_layer_create();
	action_bar_layer_add_to_window(s_actionbar, window);
	action_bar_layer_set_click_config_provider(s_actionbar, window_main_actionbar_config);
	action_bar_layer_set_icon(s_actionbar, BUTTON_ID_UP, image_connect);
	action_bar_layer_set_icon(s_actionbar, BUTTON_ID_SELECT, image_menu);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_DOWN, image_refresh);
  
	// background layer
  s_bg_layer = layer_create(GRect(2, 2, (bounds.size.w - ACTION_BAR_WIDTH - 4), bounds.size.h - 4));
	layer_set_update_proc(s_bg_layer, window_main_draw_background_layer);
  layer_add_child(window_layer, s_bg_layer);
  GRect bg_bounds = layer_get_bounds(s_bg_layer);
  
  // create time remaining counter
  s_layer_time_remaining_counter = text_layer_create(GRect(0, 0, bg_bounds.size.w, 46));
  text_layer_set_text_color(s_layer_time_remaining_counter, PBL_IF_COLOR_ELSE(GColorYellow, GColorWhite));
  text_layer_set_background_color(s_layer_time_remaining_counter, GColorClear);
  text_layer_set_font(s_layer_time_remaining_counter, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_layer_time_remaining_counter, GTextAlignmentCenter);
  text_layer_set_text(s_layer_time_remaining_counter, "--:--");
  layer_add_child(s_bg_layer, text_layer_get_layer(s_layer_time_remaining_counter));

  // create progress layer
  s_progress_layer = progress_layer_create(GRect(2, 50, bg_bounds.size.w - 4, 6));
  progress_layer_set_corner_radius(s_progress_layer, 2);
  progress_layer_set_foreground_color(s_progress_layer, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  progress_layer_set_background_color(s_progress_layer, PBL_IF_COLOR_ELSE(GColorDarkGray, GColorBlack));
  layer_add_child(s_bg_layer, s_progress_layer);
  
  // create filename
  s_layer_filename = text_layer_create(GRect(4, 65, bg_bounds.size.w - 8, 45));
  text_layer_set_text_color(s_layer_filename, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  text_layer_set_background_color(s_layer_filename, GColorClear);
  text_layer_set_font(s_layer_filename, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_layer_filename, GTextAlignmentLeft);
  text_layer_set_overflow_mode(s_layer_filename, GTextOverflowModeWordWrap);
  text_layer_set_text(s_layer_filename, "loading...");
  layer_add_child(s_bg_layer, text_layer_get_layer(s_layer_filename));
  
  // create nozzle temp
  s_layer_nozzle = text_layer_create(GRect(13, 119, 60, 14));
  text_layer_set_text_color(s_layer_nozzle, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  text_layer_set_background_color(s_layer_nozzle, GColorClear);
  text_layer_set_font(s_layer_nozzle, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_layer_nozzle, GTextAlignmentLeft);
  text_layer_set_overflow_mode(s_layer_nozzle, GTextOverflowModeFill);
  text_layer_set_text(s_layer_nozzle, "---/---");
  layer_add_child(s_bg_layer, text_layer_get_layer(s_layer_nozzle));
  
  // create bed temp
  s_layer_bed = text_layer_create(GRect(83, 119, 30, 14));
  text_layer_set_text_color(s_layer_bed, PBL_IF_COLOR_ELSE(GColorWhite, GColorWhite));
  text_layer_set_background_color(s_layer_bed, GColorClear);
  text_layer_set_font(s_layer_bed, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_layer_bed, GTextAlignmentLeft);
  text_layer_set_text(s_layer_bed, "---");
  layer_add_child(s_bg_layer, text_layer_get_layer(s_layer_bed));

  // create state
  s_layer_state = text_layer_create(GRect(4, bg_bounds.size.h - 24, bg_bounds.size.w - 8, 24));
  text_layer_set_text_color(s_layer_state, PBL_IF_COLOR_ELSE(GColorDarkGray, GColorWhite));
  text_layer_set_background_color(s_layer_state, GColorClear);
  text_layer_set_font(s_layer_state, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(s_layer_state, GTextAlignmentLeft);
  text_layer_set_overflow_mode(s_layer_filename, GTextOverflowModeTrailingEllipsis);
  text_layer_set_text(s_layer_state, "loading...");
  layer_add_child(s_bg_layer, text_layer_get_layer(s_layer_state));

  // create bitmaplayer nozzle
  s_bitmaplayer_nozzle = bitmap_layer_create(GRect(4, 123, 7, 11));
  bitmap_layer_set_compositing_mode(s_bitmaplayer_nozzle, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmaplayer_nozzle, image_nozzle);
  layer_add_child(s_bg_layer, bitmap_layer_get_layer(s_bitmaplayer_nozzle));
  
  // create bitmaplayer bed
  s_bitmaplayer_bed = bitmap_layer_create(GRect(74, 123, 7, 11));
  bitmap_layer_set_compositing_mode(s_bitmaplayer_bed, GCompOpSet);
  bitmap_layer_set_bitmap(s_bitmaplayer_bed, image_bed);
  layer_add_child(s_bg_layer, bitmap_layer_get_layer(s_bitmaplayer_bed));

	// start timer to auto-update
  tick_timer_service_subscribe(MINUTE_UNIT, window_main_timer_fired);
}


// unload window handler
static void window_main_unload_handler(Window *window) {
  // destroy action bar
  action_bar_layer_remove_from_window(s_actionbar);
  action_bar_layer_destroy(s_actionbar);
  
  // destroy images
  gbitmap_destroy(image_connect);
  gbitmap_destroy(image_refresh);
  gbitmap_destroy(image_menu);
  gbitmap_destroy(image_pause);
  gbitmap_destroy(image_start);
  gbitmap_destroy(image_nozzle);
  gbitmap_destroy(image_bed);
  
  // destroy progress layer
  progress_layer_destroy(s_progress_layer);
  
  // destroy text layers
  text_layer_destroy(s_layer_time_remaining_counter);
  text_layer_destroy(s_layer_filename);
  text_layer_destroy(s_layer_nozzle);
  text_layer_destroy(s_layer_bed);
  text_layer_destroy(s_layer_state);
  
  // destroy bitmap layers
  bitmap_layer_destroy(s_bitmaplayer_nozzle);
  bitmap_layer_destroy(s_bitmaplayer_bed);
  
  // destroy background layer
  layer_destroy(s_bg_layer);
  
  // destroy window
  window_destroy(s_window_main);
}


// set time remaining counter
void window_main_set_time_remaing_counter(const char *value) {
  text_layer_set_text(s_layer_time_remaining_counter, value);
}


// set filename
void window_main_set_filename(const char *value) {
  text_layer_set_text(s_layer_filename, value);
}


// set progress
void window_main_set_progress(const uint8_t progress) {
  progress_layer_set_progress(s_progress_layer, progress);
}


// set nozzle0 temp
void window_main_set_nozzle(const uint16_t temp0, const uint16_t temp1) {
  static char msg[14] = "";
  char t0[4] = "---";
  char t1[4] = "---";
  
  if (temp0 > 1) snprintf(t0, sizeof(t0), "%u", (uint)temp0);
  if (temp1 > 1) snprintf(t1, sizeof(t1), "%u", (uint)temp1);

  if (temp1 > 0) snprintf(msg, sizeof(msg), "%s°/%s°", t0, t1);
  else snprintf(msg, sizeof(msg), "%s°", t0);
  
  text_layer_set_text(s_layer_nozzle, msg);
}


// set bed temp
void window_main_set_bed(const uint16_t temp) {
  static char msg[8] = "---";
  if (temp > 1) snprintf(msg, sizeof(msg), "%u°", temp);
  text_layer_set_text(s_layer_bed, msg);
}


//set state
void window_main_set_state(const char *value) {
  text_layer_set_text(s_layer_state, value);
  text_layer_set_text_color(s_layer_state, GColorBlack);
  
  if (printer_get_state() == OFFLINE) action_bar_layer_set_icon(s_actionbar, BUTTON_ID_UP, image_connect);
  else if (printer_get_state() == PRINTING) action_bar_layer_set_icon(s_actionbar, BUTTON_ID_UP, image_pause);
  else action_bar_layer_set_icon(s_actionbar, BUTTON_ID_UP, image_start);
}


// update data
void window_main_timer_fired(struct tm *tick_time, TimeUnits units_changed) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timer fired, reloading");
  messaging_outbox_send("update", "");
}


// create new window
void window_main_init(void) {
  s_window_main = window_create();
  window_set_background_color(s_window_main, PBL_IF_COLOR_ELSE(GColorDarkGreen, GColorWhite));
  window_set_window_handlers(s_window_main, (WindowHandlers) {
    .load = window_main_load_handler,
    .unload = window_main_unload_handler
  });
  window_stack_push(s_window_main, true);
}


// destroy window
void window_main_destroy(void) {
  window_stack_pop(true);
}
