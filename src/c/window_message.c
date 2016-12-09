// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "window_message.h"

static Window *s_window_main;

static TextLayer *s_layer_label;

static Layer *s_layer_background;
static Layer *s_layer_icon;

static GDrawCommandImage *s_gdraw_image;

static Animation *s_animation_appear = NULL;

static AppTimer *s_timer = NULL;

static GColor s_color;


// render icon layer
static void window_message_draw_icon_layer(Layer *layer, GContext *context) {
  GRect bounds = layer_get_bounds(layer);
  const GEdgeInsets frame_insets = {
    .top = ACTION_BAR_WIDTH / 2,
    .left = ACTION_BAR_WIDTH / 2
  };

  if (s_gdraw_image)
    gdraw_command_image_draw(context, s_gdraw_image, grect_inset(bounds, frame_insets).origin);
}


// render background layer
static void window_message_draw_background_layer(Layer *layer, GContext *context) {
  graphics_context_set_fill_color(context, PBL_IF_COLOR_ELSE(s_color, GColorWhite));
  graphics_fill_rect(context, layer_get_bounds(layer), 0, 0);
}


// window_message_hide_automatically
static void window_message_hide_automatically(void *data) {
  s_timer = NULL;
  window_message_destroy();
}

// animation stopped
static void window_message_animation_stopped(Animation *animation, bool finished, void *context) {
  s_animation_appear = NULL;
}


// animation appear start
static void window_message_appear_handler(Window *window) {
  // if timer running, cancel it
  if (s_timer)
    app_timer_cancel(s_timer);
  
  // if animation running, cancel it
  if (s_animation_appear)
    animation_unschedule(s_animation_appear);
  
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  GRect start = layer_get_frame(s_layer_background);
  GRect finish = bounds;
  s_animation_appear = (Animation*)property_animation_create_layer_frame(s_layer_background, &start, &finish);
    
  animation_set_handlers(s_animation_appear, (AnimationHandlers) {
    .stopped = window_message_animation_stopped
  }, NULL);
  animation_set_delay(s_animation_appear, 500);
  animation_schedule(s_animation_appear);
  
  s_timer = app_timer_register(1250, window_message_hide_automatically, NULL);
}


// animation disappear start
static void window_message_disappear_handler(Window *window) {
  // if timer running, cancel it
  if (s_timer) {
    app_timer_cancel(s_timer);
    s_timer = NULL;
  }
  
  // if animation running, cancel it
  if (s_animation_appear)
    animation_unschedule(s_animation_appear);
}


// load window handler
static void window_message_load_handler(Window *window) {
  // create window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // create background layer
  const GEdgeInsets background_insets = {.top = bounds.size.h /* Start hidden */};
  s_layer_background = layer_create(grect_inset(bounds, background_insets));
  layer_set_update_proc(s_layer_background, window_message_draw_background_layer);
  layer_add_child(window_layer, s_layer_background);
  
  // load images
  s_gdraw_image = gdraw_command_image_create_with_resource(RESOURCE_ID_NOTIFICATION_PDC);
  
  // create icon
  s_layer_icon = layer_create(bounds);
  layer_set_update_proc(s_layer_icon, window_message_draw_icon_layer);
  layer_add_child(s_layer_background, s_layer_icon);
  
  // create message
  s_layer_label = text_layer_create(GRect(ACTION_BAR_WIDTH / 2, 90, bounds.size.w - ACTION_BAR_WIDTH * 2, bounds.size.h - 90));
  text_layer_set_text_color(s_layer_label, PBL_IF_COLOR_ELSE(GColorBlack, GColorBlack));
  text_layer_set_background_color(s_layer_label, GColorClear);
  text_layer_set_font(s_layer_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_layer_label, GTextAlignmentLeft);
  text_layer_set_text(s_layer_label, "Got it.");
  layer_add_child(s_layer_background, text_layer_get_layer(s_layer_label));
}


// unload window handler
static void window_message_unload_handler(Window *window) {
  // if animation running, cancel it
  if (s_animation_appear)
    animation_unschedule(s_animation_appear);
  
  // destroy icon layers
  layer_destroy(s_layer_icon);
  
  // destroy images
  gdraw_command_image_destroy(s_gdraw_image);
  
  // destroy text layers
  text_layer_destroy(s_layer_label);
  
  // destroy background layer
  layer_destroy(s_layer_background);
  
  // destroy window
  window_destroy(s_window_main);
}


// create new window
static void window_message_init(void) {
  s_window_main = window_create();
  window_set_background_color(s_window_main, GColorClear);
  window_set_window_handlers(s_window_main, (WindowHandlers) {
    .load = window_message_load_handler,
    .unload = window_message_unload_handler,
    .appear = window_message_appear_handler,
    .disappear = window_message_disappear_handler
  });
  window_stack_push(s_window_main, true);
}


// create a new window with predefined message
void window_message_init_custom(const char *message, GColor bg_color) {
  s_color = bg_color;
  window_message_init();
  text_layer_set_text(s_layer_label, message);
}


// pop window from stack
void window_message_destroy(void) {
  window_stack_pop(true);
}
