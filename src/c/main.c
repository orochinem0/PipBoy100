// Pip Boy 100 Copyright 2013 Bert de Ruiter (www.bertderuiter.nl/)
// Pip Boy 300 Copyright 2017 nem0 (www.nem0.net)

// INCLUDES
#include "pebble.h"
#include "config.h"

// VARIABLES
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_date_layer;
static TextLayer *s_xp_layer;
static TextLayer *s_nextLvl_layer;
static TextLayer *s_lvl_layer;

static BitmapLayer *s_background_layer;
static BitmapLayer *s_vaultBoy_layer;
static BitmapLayer *s_crippledAL_layer;

static GBitmap *s_background_bitmap;
static GBitmap *s_vaultBoy_Bitmap;
static GBitmap *s_crippledAL_bitmap;

// UPDATE TIME
static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

// HANDLE TICKS
static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
  /*if (units_changed & DAY_UNIT) {
		static char date_text[20];
		strftime(date_text, sizeof(date_text), date_format, tick_time);
		text_layer_set_text(date_layer, date_text);
	}*/
	update_time();
}

// WINDOW : LOAD
static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect frame = layer_get_bounds(window_layer);
	
	// Draw Background
	s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);
	s_background_layer = bitmap_layer_create(frame);
	bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
	layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
	
	// Draw Vault Boy
	s_vaultBoy_Bitmap = gbitmap_create_with_resource(RESOURCE_ID_VAULT_BOY);
	s_vaultBoy_layer = bitmap_layer_create(GRect(3, 26, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_vaultBoy_layer, s_vaultBoy_Bitmap);
  bitmap_layer_set_alignment(s_vaultBoy_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_vaultBoy_layer));
	
	// Draw Vault Boy - Crippled Arm (Right)
	s_vaultBoy_Bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLEDAL);
	s_vaultBoy_layer = bitmap_layer_create(GRect(3, 26, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledAL_layer, s_crippledAL_bitmap);
  bitmap_layer_set_alignment(s_crippledAL_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledAL_layer));
	
	// Create Time child layer
	s_time_layer = text_layer_create(GRect(-8, 133, frame.size.w , 34));
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
	
	// Create Battery child layer
	s_battery_layer = text_layer_create(GRect(-8, 4, frame.size.w, 34));
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
  text_layer_set_text(s_battery_layer, "HP 100/100");
	
	// Create Date child layer
	s_date_layer = text_layer_create(GRect(8, 4, frame.size.w, 34));
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  text_layer_set_text(s_date_layer, "1-1-2013");
	
	// Create XP child layer
	s_xp_layer = text_layer_create(GRect(8, 133, frame.size.w, 34));
  text_layer_set_text_color(s_xp_layer, GColorWhite);
  text_layer_set_background_color(s_xp_layer, GColorClear);
  text_layer_set_font(s_xp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_xp_layer, GTextAlignmentLeft);
  text_layer_set_text(s_xp_layer, "XP");
	
	// Create Next Level child layer
	s_nextLvl_layer = text_layer_create(GRect(8, 146, frame.size.w, 34));
  text_layer_set_text_color(s_nextLvl_layer, GColorWhite);
  text_layer_set_background_color(s_nextLvl_layer, GColorClear);
  text_layer_set_font(s_nextLvl_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_nextLvl_layer, GTextAlignmentLeft);
  text_layer_set_text(s_nextLvl_layer, "Next");
	
	// Create Level child layer
	s_lvl_layer = text_layer_create(GRect(0, 121, frame.size.w, 34));
  text_layer_set_text_color(s_lvl_layer, GColorWhite);
  text_layer_set_background_color(s_lvl_layer, GColorClear);
  text_layer_set_font(s_lvl_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_lvl_layer, GTextAlignmentCenter);
	text_layer_set_text(s_lvl_layer, "Level 1");

  // Add each layer as a child layer to the Window's root layer
	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_xp_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_nextLvl_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_lvl_layer));
}

// WINDOW : UNLOAD
static void main_window_unload(Window *window) {
	gbitmap_destroy(s_background_bitmap);
	gbitmap_destroy(s_vaultBoy_Bitmap);
	
	bitmap_layer_destroy(s_background_layer);
	bitmap_layer_destroy(s_vaultBoy_layer);

  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_battery_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_xp_layer);
	text_layer_destroy(s_nextLvl_layer);
	text_layer_destroy(s_lvl_layer);
}

// INITIALIZE
static void init(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
  	.load = main_window_load,
    .unload = main_window_unload
  });
	
	window_set_background_color(s_main_window, GColorBlack);
	
	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
  /*handle_battery(battery_state_service_peek());
  battery_state_service_subscribe(&handle_battery);
  bluetooth_connection_service_subscribe(&handle_bluetooth);*/
	
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);	
	update_time();
}

// DEINITIALIZE
static void deinit(void) {
  /*persist_write_int(PIPEXP, xp_counter);
  persist_write_int(PIPE_LAST_XP, lastXp);
  persist_write_int(PIPE_LAST_GAIN,lastGain);
  persist_write_int(PIPE_CURRENT_CRIPPLED,currentVaultBoy);
  persist_write_data(PIPE_TOTAL, &totalAccel, sizeof(totalAccel));*/
	
  tick_timer_service_unsubscribe();
  /*battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  accel_data_service_unsubscribe();*/
  
  window_destroy(s_main_window);
}

// RUN APPLICATION
int main(void) {
  init();
  app_event_loop();
  deinit();
}