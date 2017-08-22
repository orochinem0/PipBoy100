// Copyright 2013 Bert de Ruiter (www.bertderuiter.nl/)
// Copyright 2017 nem0 (www.nem0.net)

// INCLUDES
#include "pebble.h"
#include "config.h"

// NEW VARIABLES
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_battery_layer;
static TextLayer *s_date_layer;
static TextLayer *s_xp_layer;
static TextLayer *s_nextLvl_layer;
static TextLayer *s_lvl_layer;

static BitmapLayer *s_background_layer;
static BitmapLayer *s_vaultBoy_layer;

static GBitmap *s_background_bitmap;
static GBitmap *s_vaultBoy_Bitmap;

// OLD VARIABLES
//static TextLayer *time_layer; 
//static TextLayer *date_layer;
//static TextLayer *battery_layer;
//static TextLayer *xp_layer;
//static TextLayer *nextLvl_layer;
//static TextLayer *lvl_layer;

//static BitmapLayer *image_layer;
//static BitmapLayer *vaultBoy_layer;
//static uint32_t xp_counter = 0;
//static uint32_t xp_needed;
//static uint8_t xp_multiplier;
//static uint32_t lvl_counter;
//static uint32_t lastXp = 0;
//static uint32_t lastGain = 0;

//static AccelTotal totalAccel = {0,0,0,0,0,0,0};
	
//static GBitmap *image;
//static GBitmap *vaultBoy;
//static uint8_t currentVaultBoy = RESOURCE_ID_VAULT_BOY;

//static uint8_t loadedImage = 0;
//static bool dead = false;


//X = MULT * L * L - MULT * L
/*static int getXpForNextLvl() {
	int nextLvl = lvl_counter + 1;
	return xp_multiplier * nextLvl * nextLvl  - xp_multiplier * nextLvl;
}*/

/*float my_sqrt(float num) {
  float a, p, e = 0.001, b;
  a = num;
  p = a * a;
  int nb = 0;
  while ((p - num >= e) && (nb++ < SQRT_MAX_STEPS)) {
    b = (a + (num / a)) / 2;
    a = b;
    p = a * a;
  }
  return a;
}*/

/*static bool canGainXP(AccelData *accel) {
	int16_t lastX = totalAccel.lastX;
	int16_t lastY = totalAccel.lastY;
	int16_t lastZ = totalAccel.lastZ;
	totalAccel.lastX = accel->x;
	totalAccel.lastY = accel->y;
	totalAccel.lastZ = accel->z;
	totalAccel.total++;
	APP_LOG(APP_LOG_LEVEL_DEBUG,"Last Accel : (%i,%i,%i)",lastX,lastY,lastZ);
	if(DIVERG(accel->x,lastX) >= MIN_MOVMNT || 
	   DIVERG(accel->y,lastY) >= MIN_MOVMNT || 
	   DIVERG(accel->z,lastZ) >= MIN_MOVMNT) {
		totalAccel.x += accel->x;
		totalAccel.y += accel->y;
		totalAccel.z += accel->z;
		return true;
	}
	APP_LOG(APP_LOG_LEVEL_DEBUG,"No XP");
	return false;

}*/

/*static int getCurrentLvlFromXP() {
	return (int)((xp_multiplier + my_sqrt(xp_multiplier * xp_multiplier - 4 * xp_multiplier * (-xp_counter) ))/ (2 * xp_multiplier));
}*/

/*static void updateXpLayer() {
	static char xp[15];
	snprintf(xp, sizeof(xp), "XP    %lu", xp_counter);	
	text_layer_set_text(xp_layer, xp);
}*/

/*static void updateLvlNextLayers() {	
	static char nextLvl[15];
	static char lvl[10];
	
	snprintf(lvl, sizeof(lvl), "Level %lu", lvl_counter);	
	text_layer_set_text(lvl_layer, lvl);
	snprintf(nextLvl, sizeof(nextLvl), "Next %lu", xp_needed);	
	text_layer_set_text(nextLvl_layer, nextLvl);
}*/

/*static uint16_t getModulo(AccelData *data) {
	uint16_t smallest;
	uint8_t nbAccel = totalAccel.total;
	
	uint16_t divergX = DIVERG(totalAccel.x/nbAccel,data->x);
	uint16_t divergY = DIVERG(totalAccel.y/nbAccel,data->y);
	uint16_t divergZ = DIVERG(totalAccel.z/nbAccel,data->z);
	
	smallest = divergX;
	
	if(divergY < smallest) {
		smallest = divergY;
	}
	if(divergZ < smallest) {
		smallest = divergZ;
	}
	
	APP_LOG(APP_LOG_LEVEL_DEBUG,"DIV : (%i,%i,%i)",divergX,divergY,divergZ);
	return smallest;
}*/

/*static void loadVaultBoyState(uint8_t ressource) {
	APP_LOG(APP_LOG_LEVEL_DEBUG,"Load image %i",ressource);
	if (vaultBoy) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Pointer instancied");
			if(ressource == loadedImage) {
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Same image %i", loadedImage);
				return;
			}
		APP_LOG(APP_LOG_LEVEL_DEBUG,"Unload image %i",loadedImage);
		gbitmap_destroy(vaultBoy);	
    	free(vaultBoy);
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Load image to layer");   
	vaultBoy = gbitmap_create_with_resource(ressource);	
    bitmap_layer_set_bitmap(vaultBoy_layer, vaultBoy); 
    loadedImage = ressource;  
}*/

/*static void killVaultBoy() {
	dead = true;
	loadVaultBoyState(RESOURCE_ID_DEAD);
	currentVaultBoy = RESOURCE_ID_VAULT_BOY;
	xp_counter *= XP_LOSS;
	lvl_counter = getCurrentLvlFromXP();
	xp_needed = getXpForNextLvl();
	updateXpLayer();
	updateLvlNextLayers();
	vibes_long_pulse();
	
}*/

/*static void vaultBoy_status() {
	
	if(battery_state_service_peek().is_charging  && currentVaultBoy > RESOURCE_ID_VAULT_BOY) {
		//loadVaultBoyState(--currentVaultBoy);
		//persist_write_int(PIPE_CURRENT_CRIPPLED,currentVaultBoy);
		return;
	}
	
	uint64_t currentGain = xp_counter - lastXp;
	if(currentGain <= lastGain) {
		currentVaultBoy++;
		if(currentVaultBoy == (MAX_CRIPPLED + 1)) {
			killVaultBoy();
		} else {
			loadVaultBoyState(currentVaultBoy);
		}
	} else if(currentVaultBoy > RESOURCE_ID_VAULT_BOY) {
		loadVaultBoyState(--currentVaultBoy);
	}
	lastGain = currentGain;
	lastXp = xp_counter;
	persist_write_int(PIPE_LAST_GAIN,lastGain);
	persist_write_int(PIPE_LAST_XP,lastXp);
	persist_write_int(PIPE_CURRENT_CRIPPLED,currentVaultBoy);
}*/

/*static void handle_battery(BatteryChargeState charge_state) {
  static char battery_text[15];

  if (charge_state.is_charging) {
    snprintf(battery_text, sizeof(battery_text), "resting");
  } else {
    snprintf(battery_text, sizeof(battery_text), "HP %d/100", charge_state.charge_percent);
  }
  text_layer_set_text(battery_layer, battery_text);
}*/

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


static void tick_handler(struct tm* tick_time, TimeUnits units_changed) {
  /*if (units_changed & DAY_UNIT) {
		static char date_text[20];
		strftime(date_text, sizeof(date_text), date_format, tick_time);
		text_layer_set_text(date_layer, date_text);
	}*/
	update_time();
}

/*static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {

	if(!(units_changed & MINUTE_UNIT)) {
		return;
	}
	
	setTimeLayers(tick_time,units_changed);
	
	if(dead) {
		dead = rand() %2;
		if(!dead) {
			loadVaultBoyState(currentVaultBoy);
		} else {
			return;
		}
	}
		
	AccelData accel;
	accel_service_peek(&accel);
	
	APP_LOG(APP_LOG_LEVEL_DEBUG,"Accel : (%i,%i,%i)",accel.x,accel.y,accel.z);
	
	if(!canGainXP(&accel)) {
		return;
	}
		
	if(totalAccel.total > 1){
		uint16_t modulo = getModulo(&accel) + 10;
		uint16_t increase = (rand() % modulo) + 1;
		APP_LOG(APP_LOG_LEVEL_DEBUG,"Add xp %i%%%i", increase,modulo);
		xp_counter += increase;
		persist_write_int(PIPEXP, xp_counter);
		updateXpLayer();
	}
	
	bool levelUp = false;
	while(xp_counter >= xp_needed) {
		lvl_counter++;
		xp_needed = getXpForNextLvl();
		levelUp = true;
	}
	
	if(levelUp) {
		updateLvlNextLayers();
	}
	
	if(totalAccel.total == RESET_TOTAL_MIN) {
		totalAccel = (AccelTotal){0,0,0,0,0,0,0};
		vaultBoy_status();		
	}
}*/

/*void update_date_text(){
	time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
	static char date_text[20];
	char *date_format;
	 if (clock_is_24h_style()) {
    	date_format = "%d-%m-%Y";
    } else {
    	date_format = "%m-%d-%Y";
    }
	strftime(date_text, sizeof(date_text), date_format, current_time);
  	text_layer_set_text(date_layer, date_text);
}*/

/*static void handle_bluetooth(bool connected) {
   if (connected) {
    vibes_enqueue_custom_pattern(BLUETOOTH_CONNECT_VIBE);
  } else{
    vibes_enqueue_custom_pattern(BLUETOOTH_DISCONNECT_VIBE);
  }
}*/

/*static void handle_accel(AccelData *accel_data, uint32_t num_samples) {
  //doing nothing here
}*/

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
  //loadVaultBoyState(currentVaultBoy);		
  bitmap_layer_set_bitmap(s_vaultBoy_layer, s_vaultBoy_Bitmap);
  bitmap_layer_set_alignment(s_vaultBoy_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_vaultBoy_layer));
	
	// Create Time child layer
	s_time_layer = text_layer_create(GRect(-8, 133, frame.size.w , 34));
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentRight);
	//text_layer_set_text(s_time_layer, "12:00");
	
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
	bitmap_layer_destroy(s_background_layer);

  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_battery_layer);
	text_layer_destroy(s_date_layer);
	text_layer_destroy(s_xp_layer);
	text_layer_destroy(s_nextLvl_layer);
	text_layer_destroy(s_lvl_layer);
}

// INITIALIZE
static void init(void) {
	// Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
  	.load = main_window_load,
    .unload = main_window_unload
  });
	
	window_set_background_color(s_main_window, GColorBlack);
	
	// Register with TickTimerService
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
  /**/
	
  //time_t now = time(NULL);
  //struct tm *current_time = localtime(&now);
  /*handle_battery(battery_state_service_peek());
  accel_data_service_subscribe(0, &handle_accel);
  setTimeLayers(current_time,SECOND_UNIT);
  tick_timer_service_subscribe(MINUTE_UNIT, &handle_second_tick);
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
  persist_write_data(PIPE_TOTAL, &totalAccel, sizeof(totalAccel));
	
  tick_timer_service_unsubscribe();
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  accel_data_service_unsubscribe();*/
  
  /*bitmap_layer_destroy(image_layer);
  gbitmap_destroy(image);
  bitmap_layer_destroy(vaultBoy_layer);
  gbitmap_destroy(vaultBoy);	
	if (vaultBoy) {
    	free(vaultBoy);
    } 
	if (image_layer) {
    	free(image_layer);
    }*/
	
	// Destroy Window*/
  window_destroy(s_main_window);
}

// RUN APPLICATION
int main(void) {
  init();
  app_event_loop();
  deinit();
}