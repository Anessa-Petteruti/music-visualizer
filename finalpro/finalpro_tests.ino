/////*
//// * A struct to keep all three state inputs in one place
//// */
////typedef struct {
////  int num_buttons;
////  orientation last_button;
////  long mils;
////} state_inputs;
//
///*
// * A struct to keep all 9 state variables in one place
// */
//typedef struct {
//  bool rec_button_pressed;
//  bool music_received;
//  bool music_playing;
//} state_vars;
//
//bool test_transition(state start_state,
//                     state end_state,
//                     state_vars start_state_vars,
//                     state_vars end_state_vars,
//                     bool verbos);
///*        
// * Helper function for printing states
// */
//char* s2str(state s) {
//  switch(s) {
//    case sDEFAULT_PATTERN:
//    return "(1) DEFAULT_PATTERN";
//    case sRECIEVE_CONNECTION:
//    return "(2) RECIEVE_CONNECTION";
//    case sMUSIC_PATTERN:
//    return "(3) MUSIC_PATTERN";
//    default:
//    return "???";
//  }
//}
//
///*
// * Given a start state, inputs, and starting values for state variables, tests that
// * update_fsm returns the correct end state and updates the state variables correctly.
// * Returns true if this is the case (test passed) and false otherwise (test failed)
// * 
// * Need to use "verbos" instead of "verbose" because verbose is apparently a keyword
// */
//bool test_transition(state start_state,
//                     state end_state,
////                     state_inputs test_state_inputs, 
//                     state_vars start_state_vars,
//                     state_vars end_state_vars,
//                     bool verbos) {
//  rec_button_pressed = start_state_vars.rec_button_pressed;
//  music_playing = start_state_vars.music_playing;
//  music_received = start_state_vars.music_received;
//  state result_state = update_fsm(start_state);
//  bool passed_test = (end_state == result_state and
//                      rec_button_pressed == end_state_vars.rec_button_pressed and
//                      music_playing == end_state_vars.music_playing and
//                      music_received == end_state_vars.music_received);
//  if (! verbos) {
//    return passed_test;
//  } else if (passed_test) {
//    char s_to_print[200];
//    sprintf(s_to_print, "Test from %s to %s PASSED", s2str(start_state), s2str(end_state));
//    Serial.println(s_to_print);
//    return true;
//  } else {
//    char s_to_print[200];
//    Serial.println(s2str(start_state));
//    sprintf(s_to_print, "Test from %s to %s FAILED", s2str(start_state), s2str(end_state));
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "End state expected: %s | actual: %s", s2str(end_state), s2str(result_state));
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "expected: rec_button_pressed: %d | music_playing: %d | music_received: %d", end_state_vars.rec_button_pressed, end_state_vars.music_playing, end_state_vars.music_received);
//    Serial.println(s_to_print);
//    sprintf(s_to_print, "actual: rec_button_pressed: %d | music_playing: %d | music_received: %d", rec_button_pressed, music_playing, music_received);
//    Serial.println(s_to_print);
//    return false;
//  }
//}
//
///*
// * REPLACE THE FOLLOWING 6 LINES WITH YOUR TEST CASES
// */
// 
//const state test_states_in[6] = {(state) 1, (state) 1, (state) 3, (state) 3, (state) 2, (state) 2};
//const state test_states_out[6] = {(state) 1, (state) 3, (state) 3, (state) 2, (state) 2, (state) 3};
//const state_vars test_in_vars[6] = {{FALSE,TRUE,FALSE}, {TRUE,FALSE,FALSE}, {FALSE,FALSE,FALSE}, {FALSE,FALSE,TRUE}, {FALSE,TRUE,FALSE}, {TRUE,TRUE,FALSE}};
//const state_vars test_out_vars[6] = {{FALSE,FALSE,TRUE}, {FALSE,FALSE,FALSE}, {TRUE,TRUE,TRUE}, {TRUE,TRUE,TRUE}, {TRUE,TRUE,FALSE}, {FALSE,FALSE,FALSE}};
//const int num_tests = 6;
//
///*
// * Runs through all the test cases defined above
// */
//bool all_tests() {
//  for (int i = 0; i < num_tests; i++) {
//    Serial.print("Running test ");
//    Serial.println(i);
//    if (!test_transition(test_states_in[i], test_states_out[i], test_in_vars[i], test_out_vars[i], true)) {
//      return false;
//    }
//    Serial.println();
//  }
//  Serial.println("All tests passed!");
//  return true;
//}
