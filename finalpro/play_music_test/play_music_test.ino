#include <EasyButton.h>
#define REC_BTN_PIN 5
EasyButton recButton(REC_BTN_PIN);

bool isplaying = false;

//Function that gets called when the button is pressed
void recButtonHandle() {
  //TODO update a button counter somewhere to note that a button has been pressed and that we should recieve music now from serial/wifi
  if(isplaying){
    Serial.write("stopmusic");
  } else {
    Serial.write("playmusic");
  }
  isplaying = !isplaying;
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  recButton.onPressed(recButtonHandle);
  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  recButton.read();
  delay(10);
}
