#include <FastLED.h>
#include <EasyButton.h>
#include "lab5.h"

#define NUM_LEDS 60
#define LED_DATA_PIN 3
#define LED_TYPE WS2812B
#define BRIGHTNESS 30
#define SATURATION 255
#define POTEN_PIN 5
#define REC_BTN_PIN 5


#define MAX_CHSV_ANGLE 240

CRGB leds[NUM_LEDS];

state CURRENT_STATE;

int shift = 0;

bool music_received;
bool music_playing;

EasyButton recButton(REC_BTN_PIN);

void setup() {
  pinMode(POTEN_PIN, INPUT);
  
  Serial.begin(9600);
  while(!Serial);
  
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN>(leds, NUM_LEDS);

//  recButton.begin();
  recButton.onPressed(recButtonHandle);
  
  CURRENT_STATE = (state) 1;

  music_received = false;
}

void recButtonHandle() {
  //TODO update a button counter somewhere to note that a button has been pressed and that we should recieve music now from serial/wifi
  Serial.println("button pressed");
}

void loop() {
  update_inputs();
  CURRENT_STATE = update_fsm(CURRENT_STATE, millis());
  delay(10);
}

void display_default() {
  Serial.println(shift);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((map(i, 0, NUM_LEDS, 0, MAX_CHSV_ANGLE) + shift) % MAX_CHSV_ANGLE, SATURATION, BRIGHTNESS);
  }
  FastLED.show();
  delay(100);
} 

void update_inputs(){
  recButton.read();
  shift = analogRead(POTEN_PIN);
}

void wait_for_receive(){
  // TODO get the music, and set recieved = true once the music has been recieved
  music_received = true;
  rec_button_pressed = false;
}

void recieve_music(){
  // TODO what does this look like?
}


state update_fsm(state cur_state, long mils) {
  state next_state;
  switch(cur_state) {
  case sDEFAULT_PATTERN:
    if(rec_button_pressed){
      wait_for_receive();
    } else {
      next_state = sDEFAULT_PATTERN;
      display_default();
    }
    break;
  case sRECIEVE_CONNECTION:
  if(music_received){
    
  } else {
    recieve_music();
    // TODO update variables
    next_state = sRECIEVE_CONNECTION;
  }
  break;
  case sMUSIC_PATTERN:
    if(music_playing){
      display_pattern();
      // TODO update variables
      next_state = sMUSIC_PATTERN;
      if(rec_button_pressed){
        wait_for_receive();
        next_state = sRECIEVE_CONNECTION;
      }
    } 
  }
//  Serial.print(cur_state);
//  Serial.print(" -> ");
//  Serial.println(next_state);
  return next_state;
}



//// FSM variables
//byte x, y;
//orientation o;
//int lxb, uxb;
//int level, time_step, saved_clock, countdown;
//
//
//
//state CURRENT_STATE;
//
//void setup() {
//  Serial.begin(9600);
//  while (!Serial);
//  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.setBrightness(  BRIGHTNESS );
//  
//  /*
//   * LAB STEP 5
//   * Initialize all variables
//   */
//  CURRENT_STATE = (state) 1;
//
//  
//  lxb = 0;
//  uxb = 16;
//  xyo random_xyo = random_location_orientation(lxb, uxb);
//  x = random_xyo.x;
//  y = random_xyo.y;
//  o = random_xyo.o;
//  level = 0;
//  time_step = 1000;
//  saved_clock = millis();
//  countdown = 3;
//  // display something on the LCD
//
//  //test_all_tests();
//
//}
//
//void loop() {
////  for (int i = 0; i < NUM_LEDS; i++) {
////      leds[i] = CHSV(i - (1 * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
////   }
////   FastLED.show();
//for (int j = 0; j < 255; j++) {
//    for (int i = 0; i < NUM_LEDS; i++) {
//      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS); /* The higher the value 4 the less fade there is and vice versa */ 
//    }
//    FastLED.show();
//    delay(25); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
//  }
////  update_inputs();
////  CURRENT_STATE = update_fsm(CURRENT_STATE, millis(), num_buttons_pressed, last_button_pressed);
////  delay(10);
//}
//
///*
// * LAB STEP 4
// */
//bool at_edge(byte x, byte y, byte o, int lxb, int uxb) {
//  switch (o) {
//    case UP:
//      return y <= 0;
//      break;
//    case DOWN:
//      return y >= 3;
//      break;
//    case RIGHT:
//      return x >= uxb;
//      break;
//    case LEFT:
//      return x <= lxb;
//      break;
//  }
//    
//  return false;
//}
//
///*
// * LAB STEP 4
// */
//xy mv(byte x, byte y, byte o) {
//  xy retval = {x, y}; 
//  switch (o) {
//    case UP:
//      retval.y = retval.y - 1;
//      break;
//    case DOWN:
//      retval.y = retval.y + 1;
//      break;
//    case RIGHT:
//      retval.x = retval.x + 1;
//      break;
//    case LEFT:
//      retval.x = retval.x - 1;
//      break;
//  } 
//  return retval;
//}
//
///*
// * LAB STEP 7
// */
//state update_fsm(state cur_state, long mils, int num_buttons, int last_button) {
//  state next_state;
//  switch(cur_state) {
//  case sDISP_COUNTDOWN:
//    if ((mils - saved_clock) >= 500 and countdown >= 0) { // transition 1-1
//      display_level(level, countdown);
//      countdown = countdown - 1;
//      saved_clock = mils;
//      next_state = sDISP_COUNTDOWN;
//    } else if (mils - saved_clock >= 500 and countdown < 0 and not at_edge(x, y, o, lxb, uxb)) { // wait_after_rot     
//      display_cursor(x, y, o, false, lxb, uxb);
//      saved_clock = mils;
//      reset_buttons();
//      next_state = sWAIT_AFTER_ROT;
//    } else if (mils - saved_clock >= 500 and countdown < 0 and at_edge(x, y, o, lxb, uxb)) {
//      display_cursor(x, y, o, false, lxb, uxb); 
//      reset_buttons();
//      saved_clock = mils;
//      next_state = sWAIT_FOR_BUT;
//    } else {
//      next_state = sDISP_COUNTDOWN;
//    }
//    // add else if/else
//    break;
//  case sWAIT_AFTER_ROT:
//    if ((mils - saved_clock) >= time_step and num_buttons > 0) { // transition 2-3 (a)
//      display_cursor(x, y, o, false, lxb, uxb);
//      lu shrunken = shrink_bounds(x, o, lxb, uxb);
//      lxb = shrunken.l;
//      uxb = shrunken.u;
//      Serial.println("shrunk in 2 - 3");
//      xy new_xy = mv(x, y, o);
//      x = new_xy.x;
//      y = new_xy.y;
//      next_state = sMOV;
//    } else if ((mils - saved_clock) >= time_step and num_buttons == 0) { // transition 2-3 (b)
//      display_cursor(x, y, o, false, lxb, uxb);
//      xy new_xy = mv(x, y, o);
//      x = new_xy.x;
//      y = new_xy.y;
//      next_state = sMOV;
//    } else {
//      next_state = sWAIT_AFTER_ROT;
//    }
//    break;
//  case sMOV: // transitions to game_over and wait_after_mov
//    if (lxb < uxb) {
//      display_cursor(x, y, o, true, lxb, uxb);
//      // wait after move
//      // move
//      saved_clock = mils;
//      
//      reset_buttons();
//      next_state = sWAIT_AFTER_MOV;
//    } else if (lxb == uxb) {
//      // game over
//      display_game_over(level);
//      next_state = sGAME_OVER;
//    } else {
//      //display_cursor(x, y, o, false, lxb, uxb);
//      next_state = sMOV; 
//    }
//    break;
//  case sWAIT_AFTER_MOV:
//    // rotate a
//    if ((mils - saved_clock) >= time_step and num_buttons > 0) {
//      display_cursor(x, y, o, true, lxb, uxb);
//      lu shrunken = shrink_bounds(x, o, lxb, uxb);
//      lxb = shrunken.l;
//      uxb = shrunken.u;
//      Serial.println("shrunk 4 to 5");
//      o = random_turn(o, lxb, uxb);
//      
//      next_state = sROT;
//    } else if ((mils - saved_clock) >= time_step and num_buttons == 0) {
//      display_cursor(x, y, o, true, lxb, uxb);
//      o = random_turn(o, lxb, uxb);
//      
//      next_state = sROT;
//    } else {
//      //display_cursor(x, y, o, false, lxb, uxb);
//      next_state = sWAIT_AFTER_MOV;
//    }
//    // rotate b
//    
//    break;
//  case sROT:
//    // wait_after_rotate
//    if (lxb < uxb and not at_edge(x, y, o, lxb, uxb)) {
//      display_cursor(x, y, o, false, lxb, uxb);
//      saved_clock = mils;
//      next_state = sWAIT_AFTER_ROT;
//      reset_buttons();
//    } else if (lxb < uxb and at_edge(x, y, o, lxb, uxb)) {
//      display_cursor(x, y, o, false, lxb, uxb);
//      saved_clock = mils;
//      reset_buttons();
//      next_state = sWAIT_FOR_BUT;
//    } else if (lxb == uxb) {
//      display_game_over(level);
//      next_state = sGAME_OVER;
//    } else {
//      next_state = sROT;
//    }
//    break;
//  case sWAIT_FOR_BUT:
//    if (mils - saved_clock >= time_step and num_buttons == 1 and last_button == o) {
//      display_level(level+1, 3);
//      countdown = 2;
//      saved_clock = mils;
//      level = level + 1;
//      time_step = time_step * 0.95;
//      xyo random_xyo = random_location_orientation(lxb, uxb);
//      x = random_xyo.x;
//      y = random_xyo.y;
//      o = random_xyo.o;
//      
//      next_state = sDISP_COUNTDOWN;
//    } else if (mils - saved_clock >= time_step and not (num_buttons == 1 and last_button == o)) {
//      display_game_over(level);
//      next_state = sGAME_OVER;
//      
//    } else {
//      next_state = sWAIT_FOR_BUT;
//    }
//    break;
//  case sGAME_OVER: // no transitions from state 7
//    next_state = sGAME_OVER;
//    break;
//  }
//  Serial.print(cur_state);
//  Serial.print(" ");
//  Serial.println(next_state);
//  return next_state;
//}
