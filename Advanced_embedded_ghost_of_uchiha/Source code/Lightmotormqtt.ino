#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <Servo.h>

#include "Arduino_secrets.h"

//mosquitto_sub -h test.mosquitto.org -t arduino/topic (Mosquito code to acess topic data)
// Sensor pins
//myservo.attach(9); attaches the servo on pin 9 to the servo object




///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                                // your network key Index number
int status = WL_IDLE_STATUS;                     // the Wifi radio's status

Servo myservo;  // create servo object to control a servo
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "arduino/topic";
const char topic2[]  = "arduino/topic";
const char topic3[]  = "arduino/topic";
const char topic4[]  = "arduino/topic";
const char topic5[]  = "arduino/topic";

//set interval for sending messages (milliseconds)
const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;
// Value for storing water level
int val = 0;

int pos = 0;    // variable to store the servo position


int lightPin=A1;
int lightVal;
int del=250;
int redPin=6;
int state=0;
int windows;


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  pinMode(lightPin,INPUT);
  pinMode(redPin,OUTPUT);

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
   
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
 }


  if (WiFi.status() == WL_NO_MODULE) {

    Serial.println("Communication with WiFi module failed!");

    // don't continue

    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {

    Serial.println("Please upgrade the firmware");

  }

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, keyIndex, pass);

    // wait 10 seconds for connection:

    delay(10000);

  }

  // once you are connected :

  Serial.print("");
//  printCurrentNet();



 
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();


  
}


void loop() {



  // check the network connection once every 2 seconds:

  delay(2000);
//  printCurrentNet();


  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();

//    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    myservo.write(pos);  // tell servo to go to position in variable 'pos'
//    windows=1;
//   delay(15);                       // waits 15ms for the servo to reach the position
//  }
//
//
//else{
//  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    windows=0;
//delay(15);                       // waits 15ms for the servo to reach the position
//  }
//}

lightVal=analogRead(lightPin);
//Serial.println(lightVal);
delay(del);
if(lightVal<350)
{
  digitalWrite(redPin,HIGH);
  myservo.write(90);
  state=1;
}
  
else{  digitalWrite(redPin,LOW);
       myservo.write(0);
       state=0;}
  

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //record value from sensors
  
    int Rvalue4 = state;
   int Rvalue5 = state;

  

     Serial.print("Light State: ");
    Serial.println();
    Serial.println(Rvalue4);

     Serial.print("windows: ");
    Serial.println();
    Serial.println(Rvalue5);

    // send message, the Print interface can be used to set the message contents

    mqttClient.beginMessage(topic4);
    mqttClient.print("Light State: ");
    mqttClient.print(Rvalue4);
    mqttClient.endMessage();

   mqttClient.beginMessage(topic5);
   mqttClient.print("Windows are: ");
   mqttClient.print(Rvalue5);
   mqttClient.endMessage();
    
    Serial.println();
  }
}

void printCurrentNet() {

  Serial.print("You're connected to : ");
  Serial.println(WiFi.SSID());
  byte bssid[6];
  WiFi.BSSID(bssid);
  long rssi = WiFi.RSSI();
  byte encryption = WiFi.encryptionType();
  Serial.println();
}
