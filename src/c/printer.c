// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "printer.h"

static enum PRINTERSTATE s_state = LOADING;
static char s_state_text[16] = "loading...";
static uint32_t s_finish_timestamp = 0;
static char s_remaining[8] = "--:--";
static char s_filename[64] = "no file loaded";
static uint8_t s_progress = 0;
static uint16_t s_temp0 = 0;
static uint16_t s_temp1 = 0;
static uint16_t s_tempbed = 0;


// get printer state
enum PRINTERSTATE printer_get_state(void) {
  return s_state;
}


// set printer state
void printer_set_state(enum PRINTERSTATE state) {
  s_state = state;
}


// set printer state by name
void printer_set_state_by_char(const char *name) {
  if (0 == strcmp(name, "Loading")) s_state = LOADING;
  else if (0 == strcmp(name, "Offline")) s_state = OFFLINE;
  else if (0 == strcmp(name, "Connecting")) s_state = CONNECTING;
  else if (0 == strcmp(name, "Operational")) s_state = OPERATIONAL;
  else if (0 == strcmp(name, "Printing")) s_state = PRINTING;
  else if (0 == strcmp(name, "Paused")) s_state = PAUSED;
  else if (0 == strcmp(name, "Error")) s_state = ERROR;
  else APP_LOG(APP_LOG_LEVEL_DEBUG, "unknown printerstate: %s", name);
  snprintf(s_state_text, sizeof(s_state_text), "%s", name);
}


// get printer state text
char *printer_get_state_text(void) {
  return s_state_text;
}


// get remaining seconds
uint32_t printer_get_finish_timestamp(void) {
  return s_finish_timestamp;
}


// set remaining seconds
void printer_set_finish_timestamp(uint32_t timestamp) {
  s_finish_timestamp = timestamp;
}


// get remaining
char *printer_get_remaining(void) {
  return s_remaining;
}


// set remaining
void printer_set_remaining(const char *remaining) {
  snprintf(s_remaining, sizeof(s_remaining), "%s", remaining);
}


// get filename
char *printer_get_filename(void) {
  return s_filename;
}


// set filename
void printer_set_filename(const char *filename) {
  snprintf(s_filename, sizeof(s_filename), "%s", filename);
}


// get progress
uint8_t printer_get_progress(void) {
  return s_progress;
}


// set progress
void printer_set_progress(uint8_t progress) {
  s_progress = progress;
}


// get nozzle0 temp
uint16_t printer_get_temp0(void) {
  return s_temp0;
}


// set nozzle0 temp
void printer_set_temp0(uint16_t temp) {
  s_temp0 = temp;
}


// get nozzle1 temp
uint16_t printer_get_temp1(void) {
  return s_temp1;
}


// set nozzle1 temp
void printer_set_temp1(uint16_t temp) {
  s_temp1 = temp;
}


// get bed temp
uint16_t printer_get_tempbed(void) {
  return s_tempbed;
}


// set bed temp
void printer_set_tempbed(uint16_t temp) {
  s_tempbed = temp;
}
