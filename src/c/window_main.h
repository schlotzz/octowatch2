#pragma once

//static void window_main_actionbar_up(ClickRecognizerRef, void *);
//static void window_main_actionbar_select(ClickRecognizerRef, void *);
//static void window_main_actionbar_down(ClickRecognizerRef, void *);
void window_main_actionbar_config(void *);
//static void window_main_load_handler(Window *);
//static void window_main_unload_handler(Window *);
void window_main_set_time_remaing_counter(const char *);
void window_main_set_filename(const char *);
void window_main_set_nozzle(const uint16_t, const uint16_t);
void window_main_set_bed(const uint16_t);
void window_main_set_state(const char *);
void window_main_set_progress(const uint8_t);
void window_main_timer_fired(struct tm *, TimeUnits);
void window_main_init(void);
void window_main_destroy(void);
