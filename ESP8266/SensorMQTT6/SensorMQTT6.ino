#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DHT.h>
#include <Ticker.h>

//###########################################################

//WIFI & MQTT definitions & global variables
const char *area = "LivingRoom";
const char *ssid = "3Com";
const char *pass = "32e75c752f";
WiFiClient esp12;
MQTTClient client;
void connect(); // <- predefine connect() for setup()

//###########################################################

//DHT22 setup area
float tempReading = 0;
float humidityReading = 0;
#define DHTTYPE DHT22
#define DHTPIN 5 //data wire plugged in to pin 5
//initialise DHT Sensor
DHT dht(DHTPIN, DHTTYPE); 
//###########################################################

//MISC
int analogPin = A0;
int voltageReading = 0;
Ticker sleepTicker; 

//###########################################################

void readSensors() {
  //take tempReading from DHT22 sensor;
  Serial.println("taking readings");
  tempReading = dht.readTemperature();  //read temperature from DHT22
  String temp_send = String(tempReading); //convert reading from Float to String
  client.publish("LivingRoom/temperature", temp_send); //send reading to mqtt  
  
  //take humidityReading from DHT22 sensor;
  humidityReading = dht.readHumidity(); //read humidity from DHT22
  String humidity_send = String(humidityReading); //convert reading from Float to String
  client.publish("LivingRoom/humidity", humidity_send); //send reading to mqtt
  
//  Serial.println(tempReading);
//  Serial.println(humidityReading);

  //take voltage reading
  voltageReading = analogRead(analogPin);
  voltageReading = voltageReading - 584;
  String voltage_send = String(voltageReading); //convert reading from Float to String
  client.publish("LivingRoom/voltage", voltage_send); //send reading to mqtt
}

//#############################################################

//loop to connect to wifi & MQTT Server
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("?");
    delay(500);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "try", "try")) {
    Serial.print(".");
  }

  Serial.println("\nconnected!");

  //subscribe to MQTT server
  client.subscribe("/example"); // client.unsubscribe("/example");
}

//###########################################################

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}

//###########################################################

void sleepFunc() {

  WiFi.disconnect();
  
  Serial.print("Going to sleep");
  //go to sleep for 25 mins
  ESP.deepSleep(1500000000, WAKE_RF_DEFAULT); //ESP.deepSleep([microseconds], [mode]) 25000000 = 25 seconds, 1500000000 = 25 mins, 2500000000 = 41 minutes

}

//###########################################################

void setup() {
  sleepTicker.once_ms(60000, &sleepFunc); //120000 = 120 seconds. 60000 = 60 seconds
  
  //Sensor pin setup
  dht.begin(); //initialise temp sensor
  
  //wifi + MQTT Setup
  Serial.begin(57600); //begin serial monitor for debugging
  
  //start WiFi
  WiFi.begin(ssid, pass); //start wifi module

  //start connection to MQTT Server
  client.begin("192.168.0.14", esp12); //begin connection to MQTT server
  connect(); //run connect loop
  Serial.print("Runningh through setup");
}

//###########################################################

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  //if client not connected, reconnect
  if (!client.connected()) { 
    connect();
  }
  Serial.print("Main Loop");
  readSensors();
  sleepFunc();
}

//############################################################

