#define TEST_DATA_LENGTH 600

/*
 * A struct to keep all 9 state variables and 2 test variables in one place
 */
typedef struct {
  bool rec_button_pressed;
  bool music_received;
  bool music_playing;
  int default_light_shift;
  bool check_song_data;
  int cur_song_spot;
  int song_length;
  int shift;
  CRGB leds[NUM_LEDS];
  bool serial_play_sent;
  bool serial_stop_sent;
} state_vars;

uint8_t end_song_data[TEST_DATA_LENGTH];

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
 */
bool test_transition(state start_state,
                     state end_state,
                     state_vars start_state_vars,
                     state_vars end_state_vars,
                     bool verbos) {
  rec_button_pressed = start_state_vars.rec_button_pressed;
  music_playing = start_state_vars.music_playing;
  music_received = start_state_vars.music_received;
  default_light_shift = start_state_vars.default_light_shift;
  cur_song_spot = start_state_vars.cur_song_spot;
  song_length = start_state_vars.song_length;
  shift = start_state_vars.shift;
  set_start_leds(start_state_vars.leds);
  serial_play_sent = start_state_vars.serial_play_sent;
  serial_stop_sent = start_state_vars.serial_stop_sent;

  state result_state = update_fsm(start_state);

  bool song_data_matches = true;
  if (start_state_vars.check_song_data) {
    song_data_matches = song_data_correct(song_data, end_song_data);
  }
  bool leds_match = leds_correct(leds, end_state_vars.leds);
  bool passed_test = (end_state == result_state and
                      rec_button_pressed == end_state_vars.rec_button_pressed and
                      music_playing == end_state_vars.music_playing and
                      music_received == end_state_vars.music_received and
                      default_light_shift == end_state_vars.default_light_shift and
                      cur_song_spot == end_state_vars.cur_song_spot and
                      song_length == end_state_vars.song_length and
                      shift == end_state_vars.shift and
                      serial_play_sent == end_state_vars.serial_play_sent and
                      serial_stop_sent == end_state_vars.serial_stop_sent and
                      song_data_matches and
                      leds_match);
  if (!verbos) {
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
    sprintf(s_to_print, "expected: rec_button_pressed: %d | music_playing: %d | music_received: %d | default_light_shift: %d | cur_song_spot: %d | song_length: %d | shift: %d | serial_play_sent: %d | serial_stop_sent: %d", end_state_vars.rec_button_pressed, end_state_vars.music_playing, end_state_vars.music_received, end_state_vars.default_light_shift, end_state_vars.cur_song_spot, end_state_vars.song_length, end_state_vars.shift, end_state_vars.serial_play_sent, end_state_vars.serial_stop_sent);
    Serial.println(s_to_print);
    sprintf(s_to_print, "actual: rec_button_pressed: %d | music_playing: %d | music_received: %d | default_light_shift: %d | cur_song_spot: %d | song_length: %d | shift: %d serial_play_sent: %d | serial_stop_sent: %d | leds_match: %d | song_data_matches: %d", rec_button_pressed, music_playing, music_received, default_light_shift, cur_song_spot, song_length, shift, serial_play_sent, serial_stop_sent, leds_match, song_data_matches);
    Serial.println(s_to_print);
    return false;
  }
}

bool song_data_correct(uint8_t actual_data[], uint8_t expected_data[]) {
  int i = 0;
  bool data_equal = true;
  while (i < TEST_DATA_LENGTH and data_equal) {
    data_equal = actual_data[i] == expected_data[i];
    i++;
  }
  return data_equal;
}

bool leds_correct(CRGB actual_leds[], CRGB expected_leds[]) {
  int i = 0;
  bool leds_equal = true;
  while (i < NUM_LEDS and leds_equal) {
    leds_equal = (
      actual_leds[i].r == expected_leds[i].r and
      actual_leds[i].g == expected_leds[i].g and
      actual_leds[i].b == expected_leds[i].b
    );
    i++;
  }
  return leds_equal;
}

/*
 * TEST CASES
 */
const state test_states_in[9] = {(state) 1, (state) 1, (state) 1, (state) 3, (state) 3, (state) 3, (state) 3, (state) 2, (state) 2};
const state test_states_out[9] = {(state) 1, (state) 1, (state) 2, (state) 3, (state) 3, (state) 2, (state) 1, (state) 2, (state) 3};
const state_vars test_in_vars[9] = {{false,true,false,0,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {false,true,false,230,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {true,false,false,0,true,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {false,false,true,0,false,595,600,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0)},false,false}, {false,false,true,0,false,2,300,0,{CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(1,1,1), CRGB(1,1,1), CRGB(1,1,1), CRGB(1,1,1), CRGB(1,1,1), CRGB(0,0,1), CRGB(0,0,1), CRGB(0,0,1), CRGB(1,1,1)},false,false}, {true,true,true,0,true,2,500,0,{CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(1,1,1), CRGB(1,1,1), CRGB(1,1,1), CRGB(1,1,1), CRGB(1,1,1), CRGB(0,0,1), CRGB(0,0,1), CRGB(0,0,1), CRGB(1,1,1)},false,false}, {false,true,false,0,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {false,false,true,0,true,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {true,true,false,0,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}};
const state_vars test_out_vars[9] = {{false,true,false,10,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {false,true,false,0,false,2,0,0,{CRGB(7,0,2), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,8,2), CRGB(0,7,3), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,2)},false,false}, {false,true,false,0,true,2,600,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0)},false,true}, {false,false,false,0,false,600,600,175,{CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(16,22,43), CRGB(0,17,239), CRGB(17,16,48), CRGB(0,0,0)},false,false}, {false,false,true,0,false,7,300,175,{CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(27,16,38), CRGB(103,0,153), CRGB(31,16,34), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0), CRGB(0,0,0)},false,false}, {false,true,true,0,true,2,600,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0)},false,true}, {false,true,false,0,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},false,false}, {false,true,true,0,true,2,600,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0)},false,true}, {false,true,true,0,false,2,0,0,{CRGB(10,0,0), CRGB(10,0,0), CRGB(10,0,0), CRGB(9,1,0), CRGB(9,1,0), CRGB(8,2,0), CRGB(8,2,0), CRGB(7,3,0), CRGB(7,3,0), CRGB(7,4,0), CRGB(7,4,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,5,0), CRGB(7,6,0), CRGB(7,6,0), CRGB(7,7,0), CRGB(6,7,0), CRGB(5,8,0), CRGB(4,8,0), CRGB(3,9,0), CRGB(2,9,0), CRGB(1,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,10,0), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,9,1), CRGB(0,8,2), CRGB(0,7,2), CRGB(0,7,3), CRGB(0,6,4), CRGB(0,5,5), CRGB(0,5,5), CRGB(0,4,6), CRGB(0,3,7), CRGB(0,2,8), CRGB(0,1,9), CRGB(0,0,10), CRGB(0,0,10), CRGB(0,0,10), CRGB(1,0,9), CRGB(1,0,9), CRGB(2,0,8), CRGB(2,0,8), CRGB(3,0,7), CRGB(3,0,7), CRGB(3,0,7), CRGB(4,0,6), CRGB(4,0,6), CRGB(5,0,5), CRGB(5,0,5), CRGB(6,0,4), CRGB(6,0,4), CRGB(7,0,3), CRGB(7,0,3), CRGB(8,0,2), CRGB(8,0,2), CRGB(10,0,0)},true,false}};
const int num_tests = 9;

/*
 * Runs through all the test cases defined above
 */
bool all_tests() {
  populate_end_song_data();

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

void set_start_leds(CRGB start_leds[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = start_leds[i];
  }
}

void populate_end_song_data() {
  end_song_data[0] = 88;
  end_song_data[1] = 2;
  for (int j = 0; j < TEST_DATA_LENGTH - 2; j++) {
    end_song_data[j + 2] = j % 60;
  }
}
