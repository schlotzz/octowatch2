#pragma once

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
