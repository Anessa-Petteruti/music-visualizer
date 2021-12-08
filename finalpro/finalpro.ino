/* IO IMPORTS/VARS */
#include <FastLED.h>
#include <EasyButton.h>
#define NUM_LEDS 60
#define LED_DATA_PIN 7
#define LED_TYPE WS2812B
#define BRIGHTNESS 50
#define SATURATION 255
#define BLUR_RATE BRIGHTNESS/3
#define BLUR_SPREAD 2
#define POTEN_PIN 4
#define REC_BTN_PIN 5
#define MAX_CHSV_ANGLE 360
#define MAX_SONG_LEN 10000
CRGB leds[NUM_LEDS];
EasyButton recButton(REC_BTN_PIN);

#define TESTING

/* WIFI IMPORTS/VARS - From Lab 7 */
#include <SPI.h>
#include <WiFi101.h>
WiFiClient client;

char ssid[] = "Brown-Guest";        // your network SSID (name)
char pass[] = "politephoenix279";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

char server[] = "192.168.1.170"; // your IPv4 address

/* SONG VARIABLES */
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

/* TEST VARIABLES */
bool serial_play_sent = false;
bool serial_stop_sent = false;


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

  all_tests();
}


void loop() {
  update_inputs();
  CURRENT_STATE = update_fsm(CURRENT_STATE);
  delay(10);
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

//Function that gets called when the button is pressed
void recButtonHandle() {
  rec_button_pressed = true;
}

//Function to display a moment of the default light show
void display_default() {
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV((map(i, 0, NUM_LEDS-1, 0, 240) + default_light_shift) % 240, SATURATION, BRIGHTNESS);
  }

  FastLED.show();
  default_light_shift = (default_light_shift + 10) % 240;
  delay(75);
}


//Function to update our user inputs
void update_inputs() {
  recButton.read();
}

//Function which waits to recieve music and updates variables accordingly
void wait_for_receive() {
  receive_music();
  rec_button_pressed = false;
}

//Function which displays one unit time piece of the song on the LED strip
void display_pattern() {
  FastLED.clear();
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  int inc = 0;
  int vol = get_volume();

   WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);

  int vol_brightness = map(vol, 0, 100, 5, 255);

  //read shift from potentiometer to calculate LED colors
  shift = map(read_poten_val(), 0, 1023, 0, MAX_CHSV_ANGLE);

  //for each frequency in this chunk
  for (inc = 0; inc < FREQS_PER_TIME; inc++) {
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
    int smear;
    uint8_t curLED = song_data[cur_song_spot+inc];
    for (smear=-1*BLUR_SPREAD; smear<=BLUR_SPREAD; smear++) {
      if (curLED+smear >= 0 and curLED+smear<NUM_LEDS) {
        //want to fade brightness and saturation across our "blur"
        int sat = SATURATION-(abs(smear)*SATURATION/BLUR_SPREAD);
        int bri = vol_brightness;
        bri = vol_brightness-(abs(smear)*bri/BLUR_SPREAD);
        int color = (map(curLED+smear, 0, NUM_LEDS, 0, MAX_CHSV_ANGLE) + shift) % MAX_CHSV_ANGLE;
        leds[curLED+smear] = CHSV(color, sat, bri);
      }
    }
  }

  FastLED.show();
  delay(53);
}

//Function to update our FSM
state update_fsm(state cur_state) {
  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
  state next_state;

  switch(cur_state) {
  case sDEFAULT_PATTERN:
    if (rec_button_pressed) {
      wait_for_receive();
      next_state = sRECIEVE_CONNECTION;
    } else {
      next_state = sDEFAULT_PATTERN;
      display_default();
    }
    break;
  case sRECIEVE_CONNECTION:
    if (music_received) {
      music_playing = true;
      rec_button_pressed = false;
      serial_play_music();
      next_state = sMUSIC_PATTERN;
      delay(450);   //delay so music player has time to react
    } else {
      receive_music();
      next_state = sRECIEVE_CONNECTION;
    }
  break;
  case sMUSIC_PATTERN:
    if (music_playing) {
      display_pattern();
      music_playing = (cur_song_spot + FREQS_PER_TIME) < song_length; //make sure theres more music to be played
      cur_song_spot = cur_song_spot + FREQS_PER_TIME;                 //increment to the next "chunk" of the song
      next_state = sMUSIC_PATTERN;

      if (rec_button_pressed) {                                         //if we have pressed the button during a song
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

//Function to send the GET request and read the music data from our server
void receive_music() {
  serial_stop_music();
  
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(0, SATURATION, BRIGHTNESS);
  }
  FastLED.show();

  WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);

  if (client.connect(server, 5000)) {
    WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);

    // Make a HTTP request:
    client.println(request_string());
    client.println("Connection: keep-alive");
    client.println();

  }

  int counter = 0;
  uint8_t* readptr = &(song_buf[0]);
  while (client.connected()) {
      WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
      if (client.available()) {
        WDT->CLEAR.reg = WDT_CLEAR_CLEAR(0xA5);
        int c = client.read(readptr, MAX_SONG_LEN);
        readptr = &(readptr[c+1]);
        counter += 1;
      }
  }

  int i;
  int breaker = 0;

  for (i=0; i<300; i++) { //header is certainly less than 300 bytes long
    if(song_buf[i] == 13 and song_buf[i+1] == 10 and song_buf[i+2] == 13 and song_buf[i+3] == 10) {
      //Sequence which signifies the end of the header and the beginning of the actual payload
      // -the index where our payload actually starts
      breaker = i+4;
    }
  }

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
  
  if (!client.connected()) {
    Serial.println("client disconnected");
    client.stop(); // disconnect from server
  }

  music_received = true;
}

#ifndef TESTING

/* ACTUAL HELPER FUNCTIONS - NOT MOCKS */

void serial_play_music() {
  Serial.write("playmusic");
}

void serial_stop_music() {
  Serial.write("stopmusic");
}

int read_poten_val() {
  return analogRead(POTEN_PIN);
}

char* request_string() {
  return "GET /data HTTP/1.1";
}

int get_volume() {
  int vol = 100;
  if(Serial.available() > 0) {
    String c = "";
    char f = Serial.read();
    c += f;
    f = Serial.read();
    c += f;

    int cur_vol;
    cur_vol = c[1];
    cur_vol = cur_vol *256;
    cur_vol = cur_vol | c[0];
    vol = cur_vol;
  }
  return vol;
}

#else

/* MOCKED UP FUNCTIONS FOR TESTING */

char* request_string() {
  return "GET /testdata HTTP/1.1";
}

int read_poten_val() {
  return 500;
}

void serial_play_music() {
  serial_play_sent = true;
}

void serial_stop_music() {
  serial_stop_sent = true;
}

int get_volume() {
  return 100;
}

#endif
