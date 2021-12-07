/*
 * A struct to keep all 9 state variables in one place
 */
typedef struct {
  bool rec_button_pressed;
  bool music_received;
  bool music_playing;
  int default_light_shift;
//  uint8_t song_buf[MAX_SONG_LEN];
//  uint8_t* song_data;
//  int cur_song_spot;
//  int song_length;
//  int shift;
} state_vars;

CRGB end_leds[NUM_LEDS];

bool test_transition(state start_state,
                     state end_state,
                     state_vars start_state_vars,
                     state_vars end_state_vars,
                     bool verbos);
/*        
 * Helper function for printing states
 */
char* s2str(state s) {
  switch(s) {
    case sDEFAULT_PATTERN:
    return "(1) DEFAULT_PATTERN";
    case sRECIEVE_CONNECTION:
    return "(2) RECIEVE_CONNECTION";
    case sMUSIC_PATTERN:
    return "(3) MUSIC_PATTERN";
    default:
    return "???";
  }
}

/*
 * Given a start state, inputs, and starting values for state variables, tests that
 * update_fsm returns the correct end state and updates the state variables correctly.
 * Returns true if this is the case (test passed) and false otherwise (test failed)
 * 
 * Need to use "verbos" instead of "verbose" because verbose is apparently a keyword
 */
bool test_transition(state start_state,
                     state end_state,
//                     state_inputs test_state_inputs, 
                     state_vars start_state_vars,
                     state_vars end_state_vars,
                     bool verbos) {
  rec_button_pressed = start_state_vars.rec_button_pressed;
  music_playing = start_state_vars.music_playing;
  music_received = start_state_vars.music_received;
  default_light_shift = start_state_vars.default_light_shift;
  state result_state = update_fsm(start_state);
  bool passed_test = (end_state == result_state and
                      rec_button_pressed == end_state_vars.rec_button_pressed and
                      music_playing == end_state_vars.music_playing and
                      music_received == end_state_vars.music_received and
                      default_light_shift == end_state_vars.default_light_shift and
                      leds_correct(leds, end_leds));
  if (! verbos) {
    return passed_test;
  } else if (passed_test) {
    char s_to_print[200];
    sprintf(s_to_print, "Test from %s to %s PASSED", s2str(start_state), s2str(end_state));
    Serial.println(s_to_print);
    return true;
  } else {
    char s_to_print[200];
    Serial.println(s2str(start_state));
    sprintf(s_to_print, "Test from %s to %s FAILED", s2str(start_state), s2str(end_state));
    Serial.println(s_to_print);
    sprintf(s_to_print, "End state expected: %s | actual: %s", s2str(end_state), s2str(result_state));
    Serial.println(s_to_print);
    sprintf(s_to_print, "expected: rec_button_pressed: %d | music_playing: %d | music_received: %d | default_light_shift: %d", end_state_vars.rec_button_pressed, end_state_vars.music_playing, end_state_vars.music_received, end_state_vars.default_light_shift);
    Serial.println(s_to_print);
    sprintf(s_to_print, "actual: rec_button_pressed: %d | music_playing: %d | music_received: %d | default_light_shift: %d", rec_button_pressed, music_playing, music_received, default_light_shift);
    Serial.println(s_to_print);
    return false;
  }
}

bool leds_correct(CRGB actual_leds[], CRGB expected_leds[]) {
  int i = 0;
  bool leds_unequal = false;
  while (i < NUM_LEDS and !leds_unequal) {
//    Serial.println(leds_unequal);
//    Serial.println(actual_leds[i]);
//    Serial.println(expected_leds[i]);
//    Serial.println(actual_leds[i] != CRGB(1));
    leds_unequal = (actual_leds[i].r != expected_leds[i].r
    and actual_leds[i].g != expected_leds[i].g
    and actual_leds[i].b != expected_leds[i].b);
    Serial.println(leds_unequal);
    i++;
  }
//  if (!leds_equal) {
//    Serial.println(i - 1);
//    Serial.println(actual_leds[i-1]);
//    Serial.println(expected_leds[i-1]);
//  }
  return !leds_unequal;
}

/*
 * REPLACE THE FOLLOWING 6 LINES WITH YOUR TEST CASES
 */

//const state test_states_in[7] = {(state) 1, (state) 1, (state) 3, (state) 3, (state) 3, (state) 2, (state) 2};
//const state test_states_out[7] = {(state) 1, (state) 2, (state) 3, (state) 2, (state) 1, (state) 2, (state) 3};
//const state_vars test_in_vars[7] = {{false,true,false}, {true,false,false}, {false,false,true}, {true,true,true}, {false,true,false}, {false,false,true}, {true,true,false}};
//const state_vars test_out_vars[7] = {{false,true,false}, {false,true,false}, {false,false,true}, {false,true,true}, {false,true,false}, {false,true,true}, {false,true,true}};
//const int num_tests = 7;

const state test_states_in[7] = {(state) 1, (state) 1, (state) 3, (state) 3, (state) 3, (state) 2, (state) 2};
const state test_states_out[7] = {(state) 1, (state) 2, (state) 3, (state) 2, (state) 1, (state) 2, (state) 3};
const state_vars test_in_vars[7] = {{false,true,false,230}, {true,false,false,0}, {false,false,true,0}, {true,true,true,0}, {false,true,false,0}, {false,false,true,0}, {true,true,false,0}};
const state_vars test_out_vars[7] = {{false,true,false,0}, {false,true,false,0}, {false,false,true,0}, {false,true,true,0}, {false,true,false,0}, {false,true,true,0}, {false,true,true,0}};
const int num_tests = 7;

/*
 * Runs through all the test cases defined above
 */
bool all_tests() {
  populate_test_leds();

  for (int i = 0; i < num_tests; i++) {
    Serial.print("Running test ");
    Serial.println(i);
    if (!test_transition(test_states_in[i], test_states_out[i], test_in_vars[i], test_out_vars[i], true)) {
      return false;
    }
    Serial.println();
  }
  Serial.println("All tests passed!");
  return true;
}

void populate_test_leds() {
  for (int i = 0; i < NUM_LEDS; i++) {
    end_leds[i] = CHSV((map(i, 0, NUM_LEDS-1, 0, 240) + default_light_shift) % 240, SATURATION, BRIGHTNESS);
  }
}
