// Pip Boy 100 Copyright 2013 Bert de Ruiter (www.bertderuiter.nl/)
// Pip Boy 300 Copyright 2017 nem0 (www.nem0.net)

// INCLUDES
#include "pebble.h"
#include "config.h"

// VARIABLES
static int s_battery_level;
static bool s_connected;

static bool crippledAL = false;
static bool crippledAR = false;
static bool crippledLL = false;
static bool crippledLR = false;
static bool crippledH1 = false;
static bool crippledH2 = false;
static bool dead = false;

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
static BitmapLayer *s_crippledAR_layer;
static BitmapLayer *s_crippledLL_layer;
static BitmapLayer *s_crippledLR_layer;
static BitmapLayer *s_crippledH1_layer;
static BitmapLayer *s_crippledH2_layer;
static BitmapLayer *s_dead_layer;

static GBitmap *s_background_bitmap;
static GBitmap *s_vaultBoy_bitmap;
static GBitmap *s_crippledAL_bitmap;
static GBitmap *s_crippledAR_bitmap;
static GBitmap *s_crippledLL_bitmap;
static GBitmap *s_crippledLR_bitmap;
static GBitmap *s_crippledH1_bitmap;
static GBitmap *s_crippledH2_bitmap;
static GBitmap *s_dead_bitmap;

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
	
	// Write current date to buffer
	static char date_text[20];
	strftime(date_text, sizeof(date_text), "%m-%d-%Y", tick_time);
	
	// Display date
	text_layer_set_text(s_date_layer, date_text);
}

// GET BATTERY LEVEL
static void battery_callback(BatteryChargeState state) {
  s_battery_level = state.charge_percent;
}

static void battery_update_proc(GRect frame) {
	static char labelText[] = "BP XXX/100";
	snprintf(labelText, sizeof(labelText)+1, "%s %d %s", "BP",s_battery_level, "/100");
	
	s_battery_layer = text_layer_create(GRect(-8, 4, frame.size.w, 34));
  text_layer_set_text_color(s_battery_layer, GColorWhite);
  text_layer_set_background_color(s_battery_layer, GColorClear);
  text_layer_set_font(s_battery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_battery_layer, GTextAlignmentRight);
	text_layer_set_text(s_battery_layer, labelText);
	
	if (s_battery_level <= 30) {
		crippledAL = true;
	}
	else {
		crippledAL = false;
	}
	
	if (s_battery_level <= 10) {
		dead = true;
	}
	else {
		dead = false;
	}
}

// BLUETOOTH HANDLER
static void bluetooth_callback(bool connected) {
	s_connected = connected;
}

static void bluetooth_update_proc() {
  if(!s_connected) {
		crippledAR = true;
		vibes_double_pulse();
	}
	else {
		crippledAR = false;
  }	
}

// HEALTH HANDLER
static void health_handler(HealthEventType event, void *context) {
  if(event == HealthEventMovementUpdate) {
    APP_LOG(APP_LOG_LEVEL_INFO, "New health movement event");
  }
}

// TICK HANDLER
static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
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
	
	// Draw Vault Boy
	s_vaultBoy_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VAULT_BOY);
	s_vaultBoy_layer = bitmap_layer_create(GRect(3, 26, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_vaultBoy_layer, s_vaultBoy_bitmap);
  bitmap_layer_set_alignment(s_vaultBoy_layer, GAlignCenter);
	
	// Draw Vault Boy's Damaged Head (Under par sleep)
	s_crippledH1_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLED_H1);
	s_crippledH1_layer = bitmap_layer_create(GRect(3, -4, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledH1_layer, s_crippledH1_bitmap);
  bitmap_layer_set_alignment(s_crippledH1_layer, GAlignCenter);
	
	// Draw Vault Boy's Crippled Head (Really under par sleep)
	s_crippledH2_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLED_H2);
	s_crippledH2_layer = bitmap_layer_create(GRect(3, -4, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledH2_layer, s_crippledH2_bitmap);
  bitmap_layer_set_alignment(s_crippledH2_layer, GAlignCenter);
	
	// Draw Crippled Arm (Left) (Battery <20%)
	s_crippledAL_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLED_AL);
	s_crippledAL_layer = bitmap_layer_create(GRect(30, 18, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledAL_layer, s_crippledAL_bitmap);
  bitmap_layer_set_alignment(s_crippledAL_layer, GAlignLeft);
	
	// Draw Crippled Arm (Right) (Bluetooth Indictor)
	s_crippledAR_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLED_AR);
	s_crippledAR_layer = bitmap_layer_create(GRect(86, 19, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledAR_layer, s_crippledAR_bitmap);
  bitmap_layer_set_alignment(s_crippledAR_layer, GAlignLeft);

	// Draw Crippled Leg (Left) (Below step goal for the half day)
	s_crippledLL_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLED_LL);
	s_crippledLL_layer = bitmap_layer_create(GRect(34, 60, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledLL_layer, s_crippledLL_bitmap);
  bitmap_layer_set_alignment(s_crippledLL_layer, GAlignLeft);
	
	// Draw Crippled Leg (Right) (Below step goal for the whole day)
	s_crippledLR_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CRIPPLED_LR);
	s_crippledLR_layer = bitmap_layer_create(GRect(72, 60, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_crippledLR_layer, s_crippledLR_bitmap);
  bitmap_layer_set_alignment(s_crippledLR_layer, GAlignLeft);
	
	// Draw Dead Vault Boy
	s_dead_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DEAD);
	s_dead_layer = bitmap_layer_create(GRect(3, 26, frame.size.w, 100));
  bitmap_layer_set_bitmap(s_dead_layer, s_dead_bitmap);
  bitmap_layer_set_alignment(s_dead_layer, GAlignCenter);
	
	// Create Time child layer
	s_time_layer = text_layer_create(GRect(-8, 133, frame.size.w , 34));
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
	
	// Create BP child layer (Battery Level)
	battery_update_proc(frame);
	//layer_mark_dirty(s_battery_layer);
	
	bluetooth_update_proc();
	
	// Create Date child layer
	s_date_layer = text_layer_create(GRect(8, 4, frame.size.w, 34));
  text_layer_set_text_color(s_date_layer, GColorWhite);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
  text_layer_set_text(s_date_layer, "1-1-2013");
	
	// Create XP child layer (Current Steps)
	s_xp_layer = text_layer_create(GRect(8, 133, frame.size.w, 34));
  text_layer_set_text_color(s_xp_layer, GColorWhite);
  text_layer_set_background_color(s_xp_layer, GColorClear);
  text_layer_set_font(s_xp_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_xp_layer, GTextAlignmentLeft);
  text_layer_set_text(s_xp_layer, "XP");
	
	// Create Next Level child layer (Step Goal)
	s_nextLvl_layer = text_layer_create(GRect(8, 146, frame.size.w, 34));
  text_layer_set_text_color(s_nextLvl_layer, GColorWhite);
  text_layer_set_background_color(s_nextLvl_layer, GColorClear);
  text_layer_set_font(s_nextLvl_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_nextLvl_layer, GTextAlignmentLeft);
  text_layer_set_text(s_nextLvl_layer, "Next");
	
	// Create Level child layer (live weather)
	s_lvl_layer = text_layer_create(GRect(0, 121, frame.size.w, 34));
  text_layer_set_text_color(s_lvl_layer, GColorWhite);
  text_layer_set_background_color(s_lvl_layer, GColorClear);
  text_layer_set_font(s_lvl_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_lvl_layer, GTextAlignmentCenter);
	text_layer_set_text(s_lvl_layer, "Level 1");

  // Add each layer as a child layer to the Window's root layer
	layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_vaultBoy_layer));

	// Add modifier layers
	if (crippledAL) { // Battery <30%
		layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledAL_layer));
	}
	if (crippledAR) { // Bluetooth disconnected
		layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledAR_layer));
	}
	if (crippledLL) { // Step counter
		layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledLL_layer));
	}
	if (crippledLR) { // Step counter
		layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledLR_layer));
	}
	if (crippledH1) { // Sleep counter
		layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledH1_layer));
	}
	if (crippledH2) { // Sleep counter
		layer_add_child(window_layer, bitmap_layer_get_layer(s_crippledH2_layer));
	}
	if (dead) { // Battery <10%
		layer_add_child(window_layer, bitmap_layer_get_layer(s_dead_layer));
	}
	
	// Add watch function layers
	layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_battery_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_xp_layer)); // Current steps
	layer_add_child(window_layer, text_layer_get_layer(s_nextLvl_layer)); // Weekly step average
	layer_add_child(window_layer, text_layer_get_layer(s_lvl_layer)); // Weather
}

// WINDOW : UNLOAD
static void main_window_unload(Window *window) {
	gbitmap_destroy(s_background_bitmap);
	gbitmap_destroy(s_vaultBoy_bitmap);
	gbitmap_destroy(s_crippledAL_bitmap);
	gbitmap_destroy(s_crippledAR_bitmap);
	gbitmap_destroy(s_crippledLL_bitmap);
	gbitmap_destroy(s_crippledLR_bitmap);
	gbitmap_destroy(s_crippledH1_bitmap);
	gbitmap_destroy(s_crippledH2_bitmap);
	gbitmap_destroy(s_dead_bitmap);
	
	bitmap_layer_destroy(s_background_layer);
	bitmap_layer_destroy(s_vaultBoy_layer);
	bitmap_layer_destroy(s_crippledAL_layer);
	bitmap_layer_destroy(s_crippledAR_layer);
	bitmap_layer_destroy(s_crippledLL_layer);
	bitmap_layer_destroy(s_crippledLR_layer);
	bitmap_layer_destroy(s_crippledH1_layer);
	bitmap_layer_destroy(s_crippledH2_layer);
	bitmap_layer_destroy(s_dead_layer);

  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_battery_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_xp_layer);
	text_layer_destroy(s_nextLvl_layer);
	text_layer_destroy(s_lvl_layer);
}

// INITIALIZE
static void init(void) {
	// Battery subscribe
  battery_state_service_subscribe(battery_callback);
	battery_callback(battery_state_service_peek());
	
	// Bluetooth service subscribe
	connection_service_subscribe((ConnectionHandlers) {.pebble_app_connection_handler = bluetooth_callback});
	bluetooth_callback(connection_service_peek_pebble_app_connection());
	
	// Health subscribe
	health_service_events_subscribe(health_handler, NULL);
	
	// Create main window element and assign to pointer
  s_main_window = window_create();
	
	// Set handlers to manage elements within the window
  window_set_window_handlers(s_main_window, (WindowHandlers) {.load = main_window_load,.unload = main_window_unload});
	
	// Subscribe to window
	window_set_background_color(s_main_window, GColorBlack);
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	// Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
	update_time();
}

// DEINITIALIZE
static void deinit(void) {
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  window_destroy(s_main_window);
}

// RUN APPLICATION
int main(void) {
  init();
  app_event_loop();
  deinit();
}