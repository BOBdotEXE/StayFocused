#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *text_layer;
static TextLayer *status_layer;
static TextLayer *countdown_layer;
bool enabeled =false;
bool countDnShow=false;
int timeLeft=0;
AppTimer *tazeTimer;
AppTimer *refreshTimer;

int tme=0;
int aInt = 368;
char str[15];
char timerText[40]; //string for timmer text
char countDownText[8]; //string for countdown
int countdownStyle=1; // 0= OFF, 1 = 4second refresh, 2= 2second refresh, 3 = 1second Refresh  


static void countDown() //update the countdown timer
{
  if (timeLeft <999999)
  {
    char bufCount[] = "000000";
    snprintf(bufCount, sizeof(bufCount), "%d", timeLeft); //convert the number to a string
    snprintf(countDownText, sizeof(countDownText),"%s %s %s","-",bufCount,"-"); // combine the strings  (%string %string %string) ("-" NUMBER "-")
  }
  else 
    snprintf(countDownText, sizeof(countDownText),"-999999-");
}


static void swapCountdown(bool counterOn)
  {
  if (counterOn==true)
    {
    text_layer_set_font(countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
     text_layer_set_text(countdown_layer, "------");
     
  }
  if 
    (counterOn==false)
      text_layer_set_text(countdown_layer, "Long press 'Select'\n To Enable Countdown!");
      text_layer_set_font( countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
}

static void secSpit()
{
int timeSec =0;
int timeMin =0;
  
  if (tme < 60)
  {
    timeSec =tme;
    timeMin =0;
   
  }
  else
  {

     int secLeft=tme;
      for(int i=0;secLeft>=60;i++)
     {
        secLeft=secLeft-60;
        timeMin=timeMin+1;
      }
       timeSec =secLeft;
  
    
  }
  char bufMin[] = "00000";
  char bufSec[] = "00";

  snprintf(bufMin, sizeof(bufMin), "%d", timeMin);
  snprintf(bufSec, sizeof(bufSec), "%d", timeSec);
  
  snprintf(timerText, sizeof(timerText),"%s %s %s %s %s","Remind me every: \n ", bufMin," Mins, ", bufSec," Sec."); // combine the strings

}

void resetCountdown()
{
  timeLeft=tme;
  countDown();
}

void refresh (void * data){ 
 
  timeLeft=timeLeft-2;//subtract the amount of time that has passed.
  countDown(); //update the string
  
  if (countDnShow==true)  //if the user has the countdown timer ON...
    text_layer_set_text(countdown_layer,countDownText); //display it on screen.
  
  //Done! now let's do this again, REAL soon!
  if (countdownStyle==3)                                      //If user is insane
    refreshTimer=app_timer_register(1000,refresh, NULL); //make sure to set the timer to 1 sec intervals instead of 2
  if (countdownStyle==1 ||countdownStyle==2)  //if user is 'pretty chill'
    refreshTimer=app_timer_register(2000,refresh, NULL); //update every 2 seconds
    
  //if (countdownStyle==0)
    //Then the timer is no longer running, do not start it again..
  
}

// Vibe pattern: ON for 400ms, OFF for 100ms, ON for 100ms, OFF for 100ms, ON for 200ms, OFF for 300ms
static const uint32_t const segments[] = { 400, 100, 100,100,300 };
VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments),
};



void taze(void * data){  
vibes_enqueue_custom_pattern(pat); //vibrate


  resetCountdown(); //The timer whent off, so we need to restart the countdown
  
if (countDnShow==true)  //if the user has the countdown timer ON...
  text_layer_set_text(countdown_layer,countDownText); //display it on screen.
  
  tazeTimer=app_timer_register((tme*1000)-1000, taze, NULL); //set new timer

}
  




static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (enabeled == false&& tme>0)
  {
  text_layer_set_text(status_layer, "Reminder Enabled!"); //let the uder know the timer is on
  enabeled = true; //let everyone know the timer is on
  tazeTimer=app_timer_register((tme*1000)-1000, taze, NULL); //create a new timer, attach the pointer  'tazeTimer' so we can track it! (-1000 to make it more realistic)
  resetCountdown();
  if (countDnShow ==true)  //if the user has countdown enabled
    text_layer_set_text(countdown_layer,countDownText); //show the text.
  refreshTimer=app_timer_register(2000,refresh, NULL);
  }
  else
  {
   text_layer_set_text(status_layer, "Reminder DISABELED!");  //let the uder know the timer is off
    enabeled = false; //let everyone know the timer is off
    app_timer_cancel(tazeTimer);
    app_timer_cancel(refreshTimer);
    if ( countDnShow==true) // if were showing the countdown...
      text_layer_set_text(countdown_layer, "-----"); // blank it out.
  }
}

static void select_longerClick_handler(ClickRecognizerRef recognizer, void *context) {
 if (countDnShow==false) //if it was OFF before
  {
   countDnShow=true; //turn the countdown ON
   swapCountdown(true); // update the display, true=turn the counter ON
    }
   
   else //if it was ON before
  {
   countDnShow=false; //turn the countdown ON
   swapCountdown(false); // update the display, true=turn the counter ON
    }
   
  
}

static void select_longestClick_handler(ClickRecognizerRef recognizer, void *context) {
//@TODO
  
}


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (enabeled == false)
  {
      tme=tme+5;
      secSpit();
      text_layer_set_text(text_layer,timerText);
  }
}

static void long_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (enabeled == false)
  {
      tme=tme+30;
      secSpit();
      text_layer_set_text(text_layer,timerText);
  }
}

static void longer_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (enabeled == false)
  {
      tme=tme+60;
      secSpit();
      text_layer_set_text(text_layer,timerText);
  }
}



static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (enabeled == false)
  {
    if (tme>0)
      {
        tme=tme-5;
        secSpit();
        text_layer_set_text(text_layer,timerText);
      }
  }
}

static void long_down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (enabeled == false)
  {
    if (tme>0)
      {
        tme=tme-30;
      if (tme<0) //don't let the time go below zero
           tme=0;
        secSpit();
        text_layer_set_text(text_layer,timerText);
      }
  }
}

static void longer_down_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (enabeled == false)
  {
    if (tme>0)
      {
        tme=tme-60;
      if (tme<0) //don't let the time go below zero
           tme=0;
        secSpit();
        text_layer_set_text(text_layer,timerText);
      }
  }
}


static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
 
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
//  ButtonId id = BUTTON_ID_SELECT;  // The select button
 // uint16_t delay_ms = 500;         // Minimum time pressed to fire
// window_long_click_subscribe(BUTTON_ID_UP, 500, long_up_click_handler,NULL); //if UP held for 1/4 sec, increase time by 30 sec  @TODO FIX
// window_long_click_subscribe(BUTTON_ID_DOWN, 500, long_down_click_handler,NULL); //if DOWN held for 1/4 sec, decrease time by 30 sec @TODO FIX
 window_long_click_subscribe(BUTTON_ID_UP, 400, longer_up_click_handler,NULL); //if UP held for 1/4 sec, increase time by 30 sec
 window_long_click_subscribe(BUTTON_ID_DOWN, 400, longer_down_click_handler,NULL); //if DOWN held for 1/4 sec, decrease time by 30 sec
 window_long_click_subscribe(BUTTON_ID_SELECT,600, select_longerClick_handler,NULL);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(GRect(0,0, bounds.size.w, 60));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));

  text_layer = text_layer_create(GRect(2,70, bounds.size.w, 40) );
  text_layer_set_text(text_layer, "Press a Button to start");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  
  //status layer
  status_layer = text_layer_create(GRect(0,100, bounds.size.w, 40));
  text_layer_set_text(status_layer, "Reminder DISABLED!");
  text_layer_set_text_alignment(status_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(status_layer));
  
  //Countdown layer
  countdown_layer= text_layer_create(GRect(0,125, bounds.size.w, 50));
  swapCountdown(false); // at this point the countdown should be off..
  text_layer_set_text_alignment(countdown_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(countdown_layer));
  
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(text_layer);
  text_layer_destroy(status_layer);
  text_layer_destroy(countdown_layer);
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
   //set up click handler
   window_set_click_config_provider(s_main_window, click_config_provider);
  
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}