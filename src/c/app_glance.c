// OctoWatch2
// A Pebble watch app for monitoring and basic controlling of 3D printers via Octoprint
//
// Licence: CC BY-SA 3.0, http://creativecommons.org/licenses/by-sa/3.0/
// Author: Dominik Scholz <pebble@schlotzz.com>, go4u.de Webdesign <info@go4u.de>

#include <pebble.h>
#include "app_glance.h"
#include "printer.h"

#define APP_GLANCE_DEFAULT_TIMEOUT 30*60 // 30 minutes after last update


static int s_app_glance_timeout = APP_GLANCE_SLICE_NO_EXPIRATION;


// add the app glance
static void app_glance_update(AppGlanceReloadSession *session, size_t limit, void *context) {
  // return, if no app glances allowed
  if (limit < 1) return;

  // cast the context object to a string
  const char *message = (char *)context;

  // create the slice
  const AppGlanceSlice slice = (AppGlanceSlice) {
    .layout = {
      //.icon = APP_GLANCE_SLICE_DEFAULT_ICON,
      .subtitle_template_string = message
    },
    .expiration_time = s_app_glance_timeout
  };
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "app_glance_update time is: %u", s_app_glance_timeout);

  // Add the slice, and check the result
  const AppGlanceResult result = app_glance_add_slice(session, slice);
  if (result != APP_GLANCE_RESULT_SUCCESS) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "app_glance_update Error: %d", result);
  }
}


// do the app glance
void app_glance_destroy(void) {
  static char s_message[128] = "Unknown printer state";
  s_app_glance_timeout = time(0) + APP_GLANCE_DEFAULT_TIMEOUT;
  
  // choose proper message
  switch (printer_get_state()) {
    
    // clear app glance if still loading
    case LOADING:
      app_glance_reload(NULL, NULL);
      return;
    
    // printer is offline
    case OFFLINE:
      snprintf(s_message, sizeof(s_message), "Printer offline");
      break;
    
    // connecting to printer
    case CONNECTING:
      snprintf(s_message, sizeof(s_message), "Connecting");
      break;
    
    // printer finished or is ready/online
    case OPERATIONAL:
      if (printer_get_progress() == 100) snprintf(s_message, sizeof(s_message), "Finished %s", printer_get_filename());
      else if (printer_get_finish_timestamp() > 0) snprintf(s_message, sizeof(s_message), "Ready for %s", printer_get_filename());
      else snprintf(s_message, sizeof(s_message), "Printer online");
      break;
    
    // job is being printed / has been completed
    case PRINTING:
      snprintf(s_message, sizeof(s_message), "{time_until(%u)|format(>0S:'Printing, %%aT left','Print completed')}", (uint)printer_get_finish_timestamp());
      s_app_glance_timeout = printer_get_finish_timestamp() + APP_GLANCE_DEFAULT_TIMEOUT;
      break;
    
    // printer has been paused
    case PAUSED:
      snprintf(s_message, sizeof(s_message), "Print paused");
      break;
    
    // an error occured
    case ERROR:
      snprintf(s_message, sizeof(s_message), "Print error occured");
      break;
  }
  
  app_glance_reload(app_glance_update, s_message);
}
