#include <pebble.h>
#include "window_confirm.h"

static Window *s_window_main;

static ActionBarLayer *s_actionbar;

static GBitmap *image_tick;
static GBitmap *image_cross;

static TextLayer *s_layer_label;

static Layer *s_layer_icon;

static GDrawCommandImage *s_gdraw_image;

static void (*s_callbackYes)(void) = NULL;


// handle click on up button
static void window_main_actionbar_up(ClickRecognizerRef recognizer, void *context) {
  window_confirm_destroy();
  if (s_callbackYes != NULL)
    s_callbackYes();
}

// handle click on down button
static void window_main_actionbar_down(ClickRecognizerRef recognizer, void *context) {
  window_confirm_destroy();
}


// action bar config
static void window_main_actionbar_config(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler) window_main_actionbar_up);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) window_main_actionbar_down);
}


// render background layer
static void window_main_draw_icon_layer(Layer *layer, GContext *context) {
  GRect bounds = layer_get_bounds(layer);
  const GEdgeInsets frame_insets = {
    .top = ACTION_BAR_WIDTH / 2,
    .left = ACTION_BAR_WIDTH / 2
  };

  if (s_gdraw_image)
    gdraw_command_image_draw(context, s_gdraw_image, grect_inset(bounds, frame_insets).origin);
}


// load window handler
static void window_main_load_handler(Window *window) {
  // create window layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // load images
	image_tick = gbitmap_create_with_resource(RESOURCE_ID_TICK_ICON);
  image_cross = gbitmap_create_with_resource(RESOURCE_ID_CROSS_ICON);
  s_gdraw_image = gdraw_command_image_create_with_resource(RESOURCE_ID_QUESTION_PDC);
  
  // create action bar
	s_actionbar = action_bar_layer_create();
	action_bar_layer_add_to_window(s_actionbar, window);
	action_bar_layer_set_click_config_provider(s_actionbar, window_main_actionbar_config);
	action_bar_layer_set_icon(s_actionbar, BUTTON_ID_UP, image_tick);
  action_bar_layer_set_icon(s_actionbar, BUTTON_ID_DOWN, image_cross);
  
  // create icon
  s_layer_icon = layer_create(bounds);
  layer_set_update_proc(s_layer_icon, window_main_draw_icon_layer);
  layer_add_child(window_layer, s_layer_icon);
  
  // create message
  s_layer_label = text_layer_create(GRect(ACTION_BAR_WIDTH / 2, 90, bounds.size.w - ACTION_BAR_WIDTH * 2, bounds.size.h - 90));
  text_layer_set_text_color(s_layer_label, PBL_IF_COLOR_ELSE(GColorBlack, GColorBlack));
  text_layer_set_background_color(s_layer_label, GColorClear);
  text_layer_set_font(s_layer_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(s_layer_label, GTextAlignmentLeft);
  text_layer_set_text(s_layer_label, "Really?");
  layer_add_child(window_layer, text_layer_get_layer(s_layer_label));

}


// unload window handler
static void window_main_unload_handler(Window *window) {
  // destroy action bar
  action_bar_layer_remove_from_window(s_actionbar);
  action_bar_layer_destroy(s_actionbar);
  
  // destroy icon layers
  layer_destroy(s_layer_icon);
  
  // destroy images
  gbitmap_destroy(image_tick);
  gbitmap_destroy(image_cross);
  gdraw_command_image_destroy(s_gdraw_image);
  
  // destroy text layers
  text_layer_destroy(s_layer_label);
  
  // destroy window
  window_destroy(s_window_main);
}

/*
// set label
void window_main_set_time_remaing_counter(const char *value) {
  text_layer_set_text(s_layer_time_remaining_counter, value);
}
*/

// create new window
void window_confirm_init(void) {
  s_window_main = window_create();
  window_set_background_color(s_window_main, PBL_IF_COLOR_ELSE(GColorJaegerGreen, GColorWhite));
  window_set_window_handlers(s_window_main, (WindowHandlers) {
    .load = window_main_load_handler,
    .unload = window_main_unload_handler
  });
  window_stack_push(s_window_main, true);
}


// create a new window with predefined message
void window_confirm_init_custom(const char *message, GColor bg_color, void(*callbackYes)(void)) {
  window_confirm_init();
  s_callbackYes = callbackYes;
  window_set_background_color(s_window_main, bg_color);
  text_layer_set_text(s_layer_label, message);
}


// pop window from stack
void window_confirm_destroy(void) {
  window_stack_pop(true);
}