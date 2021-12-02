/* IO IMPORTS/VARS */
#include <FastLED.h>
#include <EasyButton.h>
#define NUM_LEDS 60
#define LED_DATA_PIN 7
#define LED_TYPE WS2812B
#define BRIGHTNESS 30
#define SATURATION 255
#define POTEN_PIN 4
#define REC_BTN_PIN 5
#define MAX_CHSV_ANGLE 240
#define MAX_SONG_LEN 10000
CRGB leds[NUM_LEDS];
EasyButton recButton(REC_BTN_PIN);

//#define TESTING
//all_tests();

/* WIFI IMPORTS/VARS - From Lab 7 */
#include <SPI.h>
#include <WiFi101.h>
WiFiClient client;

char ssid[] = "Ligma2";        // your network SSID (name)
char pass[] = "politephoenix279";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;
char server[] = "192.168.5.128"; // your computers ipv4 address

//SONG VARIABLES
uint8_t song_buf[MAX_SONG_LEN];
uint8_t* song_data;
int cur_song_spot;
int song_length;
#define FREQS_PER_TIME 5

#include "finalpro.h"





state CURRENT_STATE;

int shift = 0;
int default_light_shift = 0;

bool music_received;
bool music_playing;
bool rec_button_pressed = false;


void setup() {
  //SETUP WDT
  NVIC_SetPriority(TC3_IRQn, 0);
  NVIC_EnableIRQ(TC3_IRQn);

  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0);
  NVIC_EnableIRQ(WDT_IRQn);

  // Configure and enable WDT GCLK:
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) | GCLK_GENDIV_ID(0x5);
  while (GCLK->STATUS.bit.SYNCBUSY);
  //   set GCLK->GENCTRL.reg and GCLK->CLKCTRL.reg
  GCLK->GENCTRL.reg = GCLK_GENCTRL_SRC(0x03) | GCLK_GENCTRL_ID(0x05) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_DIVSEL;
  while (GCLK->STATUS.bit.SYNCBUSY);
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0x5) | GCLK_CLKCTRL_ID(0x03);
  while (GCLK->STATUS.bit.SYNCBUSY);
   
  // Configure and enable WDT:
  WDT->CONFIG.reg = WDT_CONFIG_PER(0x9);
  while (WDT->STATUS.bit.SYNCBUSY);
  WDT->EWCTRL.reg = WDT_EWCTRL_EWOFFSET(0x9);
  while (WDT->STATUS.bit.SYNCBUSY);
  WDT->CTRL.reg = WDT_CTRL_ENABLE;
  while (WDT->STATUS.bit.SYNCBUSY);
  // Enable early warning interrupts on WDT:
  WDT->INTENSET.reg = WDT_INTENSET_EW;
  while (WDT->STATUS.bit.SYNCBUSY); 
  
  pinMode(POTEN_PIN, INPUT);
  
  Serial.begin(9600);
  while(!Serial);
  
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN>(leds, NUM_LEDS);

  recButton.onPressed(recButtonHandle);
  
  CURRENT_STATE = (state) 1;

  music_received = false;
  rec_button_pressed = false;

  setup_wifi();
}

//Function to setup the arduinos connection to the wifi and our computer's server
void setup_wifi() {
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    status = WiFi.begin(ssid, pass);  //if using password
//    status = WiFi.begin(ssid);  //if no password
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
    delay(1000);
  }

  Serial.println("Connected to wifi");
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  if (client.connect(server, 5000)) {
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
    Serial.println("connected to server");
  } else {
    Serial.println("failed to connect to server");
    while(true); //Infinite while loop to trigger watchdog in event the server does not respond.
  }
}

//
void loop() {
  update_inputs();
  CURRENT_STATE = update_fsm(CURRENT_STATE);
  delay(10);
}

//Function that gets called when the button is pressed
void recButtonHandle() {
  //TODO update a button counter somewhere to note that a button has been pressed and that we should recieve music now from serial/wifi
  rec_button_pressed = true;
}

//Function to display a moment of the default light show
void display_default() {
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((map(i, 0, NUM_LEDS-1, 0, MAX_CHSV_ANGLE) + default_light_shift) % MAX_CHSV_ANGLE, SATURATION, BRIGHTNESS);
  }
  FastLED.show();
  default_light_shift = (default_light_shift + 10) % MAX_CHSV_ANGLE;
  delay(75);
}


//Function to update our user inputs
void update_inputs(){
  recButton.read();
}

//Function which waits to recieve music and updates variables accordingly
void wait_for_receive(){
  receive_music();
  
  music_received = true;
  rec_button_pressed = false;
}

void receive_music(){
//  Serial.write("in recieve music");
  Serial.write("stopmusic");
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, SATURATION, BRIGHTNESS);
  }
  FastLED.show();

  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);

  if (client.connect(server, 5000)) {
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);

//    Serial.println("connected to server");

    // Make a HTTP request:
    client.println("GET /data HTTP/1.1");
    client.println("Connection: keep-alive");
    client.println();

  }

  int counter = 0;
  uint8_t* readptr = &(song_buf[0]);
  while (client.connected()) {
      WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
      if (client.available()) {
        WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
        int c = client.read(readptr, 6157);
        readptr = &(readptr[c+1]);
        counter += 1;
      }
  }

  Serial.println(counter);
  Serial.println("HELLO");

  int i;
  int breaker = 0;

  for(i=0; i<300; i++){ //header is certainly less than 300 bytes long
    if(song_buf[i] == 13 and song_buf[i+1] == 10 and song_buf[i+2] == 13 and song_buf[i+3] == 10) {
      //Sequence which signifies the end of the header and the beginning of the actual payload
      // -the index where our payload actually starts
      breaker = i+4;
    }
  }
  Serial.println("breaker");
  Serial.println(breaker);

  //set pointer to the start of the actual payload
  song_data = &(song_buf[breaker]);

  

  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  delay(1000);

  //calculate total num samples of the data, stored in first two bytes
  int total_len;
  total_len = song_data[1];
  total_len = total_len *256;
  total_len = total_len | song_data[0];

  //set song length and current position in song
  song_length = total_len;
  //+2 to account for first two bytes representing data length
  cur_song_spot = 2;
  
  Serial.println("length");
  Serial.println(total_len);

  if (!client.connected()) {
    Serial.println("client disconnected");
    client.stop(); // do we want this?
  }
}

//Function which displays one unit time piece of the song on the LED strip
void display_pattern(){
  FastLED.clear();
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  int inc;

  //read shift from potentiometer to calculate LED colors
  shift = map(analogRead(POTEN_PIN), 0, 1023, 0, MAX_CHSV_ANGLE);
  
  //for each frequency in this chunk
  for(inc =0; inc<FREQS_PER_TIME; inc++){
    uint8_t curLED = song_data[cur_song_spot+inc];
    leds[curLED] = CHSV((map(curLED, 0, NUM_LEDS, 0, MAX_CHSV_ANGLE) + shift) % MAX_CHSV_ANGLE, SATURATION, BRIGHTNESS);
  }
  
  FastLED.show();

  //TODO: ADJUST THIS
  delay(77);
}

//Function to update our FSM 
state update_fsm(state cur_state) {
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  state next_state;
  
  switch(cur_state) {
  case sDEFAULT_PATTERN:
    if (rec_button_pressed){
      wait_for_receive();
      next_state = sRECIEVE_CONNECTION;
    } else {
      next_state = sDEFAULT_PATTERN;
      display_default();
    }
    break;
  case sRECIEVE_CONNECTION:
    if (music_received){
      Serial.println("leaving recieve music");
      music_playing = true;
      rec_button_pressed = false;
      Serial.write("playmusic");
      next_state = sMUSIC_PATTERN;
    } else {
      receive_music();
      next_state = sRECIEVE_CONNECTION;
    }
  break;
  case sMUSIC_PATTERN:
    if (music_playing){
      display_pattern();
      music_playing = (cur_song_spot + FREQS_PER_TIME) < song_length; //make sure theres more music to be played
      cur_song_spot = cur_song_spot + FREQS_PER_TIME;                 //increment to the next "chunk" of the song
      next_state = sMUSIC_PATTERN;

      if(rec_button_pressed){                                         //if we have pressed the button during a song
        wait_for_receive();
        next_state = sRECIEVE_CONNECTION;
      }
    } else {
       next_state = sDEFAULT_PATTERN;                                 //revert to the default display pattern
    }
   break;
  }

  return next_state;
}


//Watchdog timer warning function
void WDT_Handler() {
  WDT->INTFLAG.reg = WDT_INTFLAG_EW;
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  Serial.println("might reset");
}


//
//#ifndef TESTING
///* ACTUAL HELPER FUNCTIONS - NOT MOCKS */
//
//#else
///* MOCKED UP FUNCTIONS FOR TESTING */
