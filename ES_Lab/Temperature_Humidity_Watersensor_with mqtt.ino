#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <SPI.h>
#include <DHT.h>

#include "Arduino_secrets.h"

//mosquitto_sub -h test.mosquitto.org -t arduino/topic (Mosquito code to acess topic data)
// Sensor pins
#define sensorPower 7
#define sensorPin A0
#define Type DHT11


///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                                // your network key Index number
int status = WL_IDLE_STATUS;                     // the Wifi radio's status

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "arduino/topic";
const char topic2[]  = "arduino/topic";
const char topic3[]  = "arduino/topic";

//set interval for sending messages (milliseconds)
const long interval = 5000;
unsigned long previousMillis = 0;

int count = 0;
// Value for storing water level
int val = 0;



//Value for storing temperature and humidity
int sensPin=2;
int setTime=500;
float humidity;
float tempC;
DHT HT(sensPin,Type);



void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only

//Set temperature pin
     HT.begin();
  delay(setTime);

  
 // Set D7 as an OUTPUT
  pinMode(sensorPower, OUTPUT);
  
  // Set to LOW so no power flows through the sensor
  digitalWrite(sensorPower, LOW);
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

  // check the network connection once every 5 seconds:

  delay(5000);
//  printCurrentNet();


  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  unsigned long currentMillis = millis();


  humidity=HT.readHumidity();
  tempC=HT.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" Temperature: ");
  Serial.print(tempC);
  Serial.print("C");


 //get the reading from the function below and print it
  int level = readSensor();
  
  Serial.print(" Water level: ");
  Serial.println(level);
  
  delay(1000);

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //record value from sensors
    int Rvalue = val;
    int Rvalue2 = humidity;
    int Rvalue3 = tempC;

    Serial.print("Water leve: ");
    Serial.println();
    Serial.println(Rvalue);

    Serial.print("Humidity: ");
    Serial.println();
    Serial.println(Rvalue2);

    Serial.print("Temperature: ");
    Serial.println();
    Serial.println(Rvalue3);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print("Water level:");
    mqttClient.print(Rvalue);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic2);
    mqttClient.print("Humidity:");
    mqttClient.print(Rvalue2);
    mqttClient.endMessage();

    mqttClient.beginMessage(topic3);
    mqttClient.print("Temperatue:");
    mqttClient.print(Rvalue3);
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

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}
