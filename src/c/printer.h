// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#pragma once
#include <pebble.h>

enum PRINTERSTATE {
  LOADING,
  OFFLINE,
  CONNECTING,
  OPERATIONAL,
  PRINTING,
  PAUSED,
  ERROR
};

enum PRINTERSTATE printer_get_state(void);
void printer_set_state(enum PRINTERSTATE);
void printer_set_state_by_char(const char *);
char *printer_get_state_text(void);
uint32_t printer_get_finish_timestamp(void);
void printer_set_finish_timestamp(uint32_t);
char *printer_get_remaining(void);
void printer_set_remaining(const char *);
char *printer_get_filename(void);
void printer_set_filename(const char *);
uint8_t printer_get_progress(void);
void printer_set_progress(uint8_t);
uint16_t printer_get_temp0(void);
void printer_set_temp0(uint16_t);
uint16_t printer_get_temp1(void);
void printer_set_temp1(uint16_t);
uint16_t printer_get_tempbed(void);
void printer_set_tempbed(uint16_t);
