

#include <EasyButton.h>
#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include <SPI.h>
#define REC_BTN_PIN 2

char ssid[] = "WMBD4";  // network SSID (name)
char pass[] = "1801701128"; // for networks that require a password
int status = WL_IDLE_STATUS;  // the WiFi radio's status

WiFiClient client;
HttpClient client2 = HttpClient(client, "192.168.1.174", 5000);
String current_line="";

String sensorReadings;

int counter = 0;

char servername[]="192.168.1.174";
IPAddress server(192,168,1,174);



EasyButton recButton(REC_BTN_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  // put your setup code here, to run once:
//  Serial.println("hello");
  
  recButton.begin();
  recButton.onPressed(recButtonHandle);
//  setup_wifi();
}

void recButtonHandle() {
  String s = "";
  Serial.write("a");
  delay(100);
  for(int i =0; i <20;i++){
    while(Serial.available()<1);
    s = Serial.readString();
    counter = counter + 1;
    Serial.write("b");
  }
  delay(1000);
  Serial.write("b");
  delay(1000);
  Serial.println(counter);
  Serial.println(s);
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("here");
//  delay(1000);
  recButton.read();

}




void setup_wifi() {
  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid,pass); // WiFi.begin(ssid, pass) for password
    delay(10000);
  }
  Serial.println("Connected!");
//  if (connect_to_webpage()) {
//    Serial.println("fetched webpage");
//  } else {
//    Serial.println("failed to fetch webpage");
//  }
//  Serial.println();
}

bool connect_to_webpage() {
  if (client.connect("cs.brown.edu", 80)) {
    client.println("GET /courses/csci1600/labs/lab7LED.txt HTTP/1.0");
    client.println("Host: cs.brown.edu");
    client.println();
    return true;
  } else {
    Serial.println("Failed to fetch webpage");
    return false;
  }
}

void read_webpage() {
  // Check for bytes to read
  int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    for (int i = 0; i < len; i++) {
      // Process and print data line by line
      if ((char) (buffer[i]) != '\n') {
        current_line += (char) buffer[i];
      } else {
        
        Serial.println(current_line);
        interrupts();
        current_line = "";
      }
    }
  }
  /*
   * LAB STEP 3b: change above code to only print RED, GREEN, or BLUE as it is read
   * LAB STEP 3c: change above code to send 'r', 'g', or 'b' via Serial1
   * LAB STEP 4e: change above code to put 'r', 'b', or 'g' in s_buf for sending on UART
   */
}
