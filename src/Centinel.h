#ifndef CENTINEL_H
#define CENTINEL_H

//Start Application
int main(void);
static void prv_deinit(void);
static void prv_init(void);

//Load and Unload Windows and Resources
static void prv_window_load(Window *window);
static void prv_window_unload(Window *window);

//Click Handlers
static void prv_click_config_provider(void *context);
static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context);
static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context);
static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context);

#endif
