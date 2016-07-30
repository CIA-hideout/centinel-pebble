#include <pebble.h>
#include <Centinel.h>

//Declare variables
static Window *s_window;
static TextLayer *s_title_layer;
static TextLayer *s_spent_layer;
static TextLayer *s_saved_layer;
static Layer *s_savings_layer;

static GFont title_font;
static GFont money_font; //Use for spend & saved text layer

static char spent_header[64] = "SPENT: $";
char *spent_amount_daily = "65";
char *spent_amount_weekly = "500";
char *spent_amount_monthly = "1200";
static char saved_header[64] = "SAVED: $";
char *saved_amount_daily = "16";
char *saved_amount_weekly = "200";
char *saved_amount_monthly = "650";

double weekly_savings[] = {16,200,650};
double weekly_spendings[] = {65,500,1200};
int draw_spent_layer_basalt, draw_spent_layer_chalk;

int id = 1; //this will be used to iterate through the arrays when clicking up or down

/*
Start Programming here
*/

// Start app
int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}


//Deinitialise Application and Remove Windows from memory
static void prv_deinit(void) {
  window_destroy(s_window);
}

//Initialise Windows
static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });

  //Create BG Layer (SPENDINGS) - FFAA00 (orange)
  window_set_background_color(s_window, GColorChromeYellow);

  const bool animated = true;
  window_stack_push(s_window, animated);
}

//Destory Resources to clear memory
static void prv_window_unload(Window *window) {

  //Unload Text Layer from memory
  text_layer_destroy(s_title_layer);
  text_layer_destroy(s_spent_layer);
  text_layer_destroy(s_saved_layer);
  layer_destroy(s_savings_layer);

  //Unload GFont form memory
  fonts_unload_custom_font(title_font);
  fonts_unload_custom_font(money_font);
}

/*
Comment block to seperate app loading and unloading from the other methods.
*/

//Load resources into window
static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  //Create GFont
  title_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_MEDIUM_24));
  money_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_REGULAR_14));

  //Create canvase layer
  s_savings_layer = layer_create(bounds);

  // Assign the custom drawing procedure
  layer_set_update_proc(s_savings_layer, canvas_update_proc);

  //Add to Windows
  layer_add_child(window_get_root_layer(window), s_savings_layer);


  //Set title text layer accordingly
  s_title_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0,10), PBL_IF_ROUND_ELSE(58,15),
                      bounds.size.w, 28));
  text_layer_set_font(s_title_layer, title_font);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Set Title Text font");  //LOGGING
  text_layer_set_text(s_title_layer, "WEEKLY");
  text_layer_set_text_alignment(s_title_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft));
  text_layer_set_background_color(s_title_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_title_layer));


  char* test_spent = strcat(spent_header, spent_amount_weekly);
  APP_LOG(APP_LOG_LEVEL_DEBUG, test_spent);           //LOGGING
  char* test_saved = strcat(saved_header, saved_amount_weekly);
  APP_LOG(APP_LOG_LEVEL_DEBUG, test_saved);           //LOGGING


  //Set SPENT Text Layer accordingly
  s_spent_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0,10), PBL_IF_ROUND_ELSE(91,48),
                      bounds.size.w, 16));
  text_layer_set_font(s_spent_layer, money_font);
  text_layer_set_text(s_spent_layer, test_spent);
  text_layer_set_text_alignment(s_spent_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft));
  text_layer_set_background_color(s_spent_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_spent_layer));

  //Set SAVED Text Layer accordingly
  s_saved_layer = text_layer_create(GRect(PBL_IF_ROUND_ELSE(0,10), PBL_IF_ROUND_ELSE(106,63),
                      bounds.size.w, 16));
  text_layer_set_font(s_saved_layer, money_font);
  text_layer_set_text(s_saved_layer, test_saved);
  text_layer_set_text_alignment(s_saved_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter,GTextAlignmentLeft));
  text_layer_set_background_color(s_saved_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(s_saved_layer));

}

//Subscribe to clicks handlers (Make sure that you can click buttons in app)
static void prv_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
}

//What happens when UP button is pressed
static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (id != 2) {
    id ++;

    if (id == 1){
      text_layer_set_text(s_title_layer, "WEEKLY");
      text_layer_set_text(s_spent_layer, "SPENT: $500");
      text_layer_set_text(s_saved_layer, "SAVED: $200");
    }

    if (id == 2){
      text_layer_set_text(s_title_layer, "MONTHLY");
      text_layer_set_text(s_spent_layer, "SPENT: $1200");
      text_layer_set_text(s_saved_layer, "SAVED: $650");
    }
  }
}

//What happens when DOWN button is pressed
static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (id != 0){
    id--;

    if (id == 1){
      text_layer_set_text(s_title_layer, "WEEKLY");
      text_layer_set_text(s_spent_layer, "SPENT: $500");
      text_layer_set_text(s_saved_layer, "SAVED: $200");
    }

    if (id == 0){
      text_layer_set_text(s_title_layer, "DAILY");
      text_layer_set_text(s_spent_layer, "SPENT: $65");
      text_layer_set_text(s_saved_layer, "SAVED: $16");
    }
  }
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!
  draw_spent_layer_basalt = (weekly_spendings[id]/(weekly_spendings[id] + weekly_savings[id])) * 168;
  draw_spent_layer_chalk = (weekly_spendings[id]/(weekly_spendings[id] + weekly_savings[id])) * 180;

  GRect rect_bounds = GRect(0, PBL_IF_ROUND_ELSE(draw_spent_layer_chalk, draw_spent_layer_basalt),
                        PBL_IF_ROUND_ELSE(180,144), PBL_IF_ROUND_ELSE(draw_spent_layer_chalk, draw_spent_layer_basalt));

  graphics_context_set_fill_color(ctx, GColorTiffanyBlue);
  graphics_fill_rect(ctx, rect_bounds,0, GCornerNone);

}
