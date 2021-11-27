#include <SPI.h>
#include <WiFi101.h>
//#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "WMBD4";        // your network SSID (name)
char pass[] = "1801701128";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)
char server[] = "192.168.1.174";
//IPAddress 

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

WiFiClient client;

void setup() {

  //Initialize serial and wait for port to open:

  Serial.begin(115200);

  while (!Serial) {
 // wait for serial port to connect. Needed for native USB port only

  }

  // check for the presence of the shield:

  if (WiFi.status() == WL_NO_SHIELD) {

    Serial.println("WiFi shield not present");

    // don't continue:

    while (true);

  }

  // attempt to connect to WiFi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:

    delay(10000);

  }

  Serial.println("Connected to wifi");

  printWiFiStatus();

  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:

  if (client.connect(server, 5000)) {

    Serial.println("connected to server");

    // Make a HTTP request:

//    client.println("GET /search?q=arduino HTTP/1.1");
//
//    client.println("Host: www.google.com");
    client.println("GET /data HTTP/1.1");

    client.println("Connection: close");

    client.println();

  }
}

String readString;
union ArrayToInteger {
  byte array[2];
  uint32_t integer;
};

void loop() {
  uint8_t mybuf[7000];
  uint8_t garbage[156];
  ArrayToInteger arrsize;

  // if there are incoming bytes available

  // from the server, read them and print them:
  int counter = 0;
  uint8_t* readptr = &(mybuf[0]);
  while (client.connected()) {
      if (client.available()) {
        int c = client.read(readptr, 6157);
//        char c = client.read();
        readptr = &(readptr[c+1]);
        Serial.println(c);
        counter += 1;
      }
  }
//  Serial.println(readString);
  Serial.println(counter);
  Serial.println("HELLO");
//  Serial.println(mybuf[1000], HEX);
//  Serial.println(mybuf[2000], HEX);
  int i;
  for(i = 0; i<7000;i++){
    Serial.println(mybuf[i], HEX);
  } 
//  Serial.println("split");
//  uint8_t* newb = &(mybuf[137]);
//  for(i = 0; i<5863;i++){
//    Serial.println(newb[i], HEX);
//  }

  // if the server's disconnected, stop the client:

  if (!client.connected()) {

    Serial.println();

    Serial.println("disconnecting from server.");

    client.stop();


    // do nothing forevermore:

    while (true);

  }
}

void printWiFiStatus() {

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}


//#include <EasyButton.h>
//#include <ArduinoHttpClient.h>
//#include <WiFi101.h>
//#include <SPI.h>
//#define REC_BTN_PIN 2
//
//char ssid[] = "WMBD4";  // network SSID (name)
//char pass[] = "1801701128"; // for networks that require a password
//int status = WL_IDLE_STATUS;  // the WiFi radio's status
//
//WiFiClient client;
//HttpClient client2 = HttpClient(client, "192.168.1.174", 5000);
//String current_line="";
//
//String sensorReadings;
//
//
//char servername[]="192.168.1.174";
//IPAddress server(192,168,1,174);
//
//
//
//EasyButton recButton(REC_BTN_PIN);
//
//void setup() {
//  Serial.begin(115200);
//  while (!Serial);
//  // put your setup code here, to run once:
//  Serial.println("hello");
//  
//  recButton.begin();
//  recButton.onPressed(recButtonHandle);
//  setup_wifi();
//}
//
//void recButtonHandle() {
//  Serial.println("button pressed");
//  Serial.println("\nStarting connection...");
////  // if you get a connection, report back via serial:
////  sensorReadings = httpGETRequest("http://192.168.1.174:5000/");
////  Serial.println("a");
////  Serial.println(sensorReadings);
//////  if (client.connect(servername, 5000)) {
//////    Serial.println("connected");
//////    // Make a HTTP request:
//////    client.println("GET / HTTP/1.1");
//////    client.println();
//////  }
//
//  
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
////  Serial.println("here");
////  delay(1000);
//  recButton.read();
//
//}
//
//
//
//
//void setup_wifi() {
//  // attempt to connect to WiFi network:
//  while ( status != WL_CONNECTED) {
//    Serial.print("Attempting to connect to: ");
//    Serial.println(ssid);
//    status = WiFi.begin(ssid,pass); // WiFi.begin(ssid, pass) for password
//    delay(10000);
//  }
//  Serial.println("Connected!");
////  if (connect_to_webpage()) {
////    Serial.println("fetched webpage");
////  } else {
////    Serial.println("failed to fetch webpage");
////  }
////  Serial.println();
//}
//
//bool connect_to_webpage() {
//  if (client.connect("cs.brown.edu", 80)) {
//    client.println("GET /courses/csci1600/labs/lab7LED.txt HTTP/1.0");
//    client.println("Host: cs.brown.edu");
//    client.println();
//    return true;
//  } else {
//    Serial.println("Failed to fetch webpage");
//    return false;
//  }
//}
//
//void read_webpage() {
//  // Check for bytes to read
//  int len = client.available();
//  if (len > 0) {
//    byte buffer[80];
//    if (len > 80) len = 80;
//    client.read(buffer, len);
//    for (int i = 0; i < len; i++) {
//      // Process and print data line by line
//      if ((char) (buffer[i]) != '\n') {
//        current_line += (char) buffer[i];
//      } else {
//        
//        Serial.println(current_line);
//        interrupts();
//        current_line = "";
//      }
//    }
//  }
//  /*
//   * LAB STEP 3b: change above code to only print RED, GREEN, or BLUE as it is read
//   * LAB STEP 3c: change above code to send 'r', 'g', or 'b' via Serial1
//   * LAB STEP 4e: change above code to put 'r', 'b', or 'g' in s_buf for sending on UART
//   */
//}
