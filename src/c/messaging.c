// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "messaging.h"
#include "window_main.h"
#include "printer.h"


// inbox success handler
static void messaging_inbox_received(DictionaryIterator *iter, void *context) {
  // read tuples
	Tuple *tuple_filename = dict_find(iter, 1);
	Tuple *tuple_remain = dict_find(iter, 2);
  Tuple *tuple_state = dict_find(iter, 3);
  Tuple *tuple_progress = dict_find(iter, 4);
  Tuple *tuple_secs = dict_find(iter, 5);
  Tuple *tuple_temp0 = dict_find(iter, 6);
  Tuple *tuple_temp1 = dict_find(iter, 7);
  Tuple *tuple_tempbed = dict_find(iter, 8);
  
  // received filename
  if (tuple_filename) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting filename to: %s", tuple_filename->value->cstring);
    printer_set_filename(tuple_filename->value->cstring);
    window_main_set_filename(printer_get_filename());
  }
  
  // received remaining time, human readable
  if (tuple_remain) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting time remaining to: %s", tuple_remain->value->cstring);
    printer_set_remaining(tuple_remain->value->cstring);
    window_main_set_time_remaing_counter(printer_get_remaining());
  }
  
  // received state
  if (tuple_state) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting state to: %s", tuple_state->value->cstring);
    printer_set_state_by_char(tuple_state->value->cstring);
    window_main_set_state(printer_get_state_text());
  }
  
  // received progress
  if (tuple_progress) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting progress to: %u", (uint)tuple_progress->value->uint8);
    printer_set_progress(tuple_progress->value->uint8);
    window_main_set_progress(printer_get_progress());
  }
  
  // received remaining time, seconds
  if (tuple_secs) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting secs to: %u", (uint)tuple_secs->value->uint32);
    printer_set_finish_timestamp(tuple_secs->value->uint32);
  }
  
  // received nozzle0 temp
  if (tuple_temp0) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting nozzle0 temp to: %u", (uint)tuple_temp0->value->uint16);
    printer_set_temp0(tuple_temp0->value->uint16);
    window_main_set_nozzle(printer_get_temp0(), printer_get_temp1());
  }
  
  // received nozzle1 temp
  if (tuple_temp1) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting nozzle1 temp to: %u", (uint)tuple_temp1->value->uint16);
    printer_set_temp1(tuple_temp1->value->uint16);
    window_main_set_nozzle(printer_get_temp0(), printer_get_temp1());
  }
  
  // received bed temp
  if (tuple_tempbed) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "setting bed temp to: %u", tuple_tempbed->value->uint16);
    printer_set_tempbed(tuple_tempbed->value->uint16);
    window_main_set_bed(printer_get_tempbed());
  }
}


// inbox failure handler
static void messaging_inbox_dropped(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Dropped %d", (int)reason);
  vibes_double_pulse();
}


// send an outgoing message
void messaging_outbox_send(const char *command, const char *value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (!iter) {
    // Error creating outbound message
    APP_LOG(APP_LOG_LEVEL_ERROR, "messaging_outbox_send: cannot send app message");
    return;
  }

  dict_write_cstring(iter, 0, command);
  dict_write_cstring(iter, 1, value);
  dict_write_end(iter);

  app_message_outbox_send();
}


// setup app message inbox
void messaging_init(void) {
  app_message_register_inbox_received(messaging_inbox_received);
  app_message_register_inbox_dropped(messaging_inbox_dropped);
  app_message_open(256, 64);
}


// destroy message handlers
void messaging_destroy(void) {
  app_message_deregister_callbacks();  
}
