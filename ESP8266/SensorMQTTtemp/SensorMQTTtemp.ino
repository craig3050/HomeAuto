#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DHT.h>

//###########################################################
//WIFI & MQTT definitions & global variables
const char *area = "LivingRoom";
const char *ssid = "3Com";
const char *pass = "32e75c752f";
WiFiClient esp12;
MQTTClient client;
void connect(); // <- predefine connect() for setup()
//###########################################################

//###########################################################
//DHT22 setup area
float tempReading = 0;
float humidityReading = 0;
#define DHTTYPE DHT22
#define DHTPIN 5 //data wire plugged in to pin 5
//initialise DHT Sensor
DHT dht(DHTPIN, DHTTYPE); 
//###########################################################

//###########################################################
//MISC
unsigned long lastMillis = 0; //used for the delay loop
int ledPin = 4; //define ledPin as number 12
int analogPin = A0;
int voltageReading = 0;
//###########################################################


void setup() {
  //Sensor pin setup
  dht.begin(); //initialise temp sensor
  pinMode(ledPin, OUTPUT); //define ledPin as an output
  
  //wifi + MQTT Setup
  Serial.begin(57600); //begin serial monitor for debugging
  WiFi.begin(ssid, pass); //start wifi module
  client.begin("192.168.0.10", esp12); //begin connection to MQTT server
  connect(); //run connect loop
}

//###########################################################

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  //if client not connected, reconnect
  if (!client.connected()) { 
    connect();
  }


//Loop to put the microcontroller to sleep
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= 10000) {
    lastMillis = currentMillis;  

    //take tempReading from DHT22 sensor;
    Serial.println("taking readings");
    tempReading = dht.readTemperature();  //read temperature from DHT22
    //take humidityReading from DHT22 sensor;
    humidityReading = dht.readHumidity(); //read humidity from DHT22
    String temp_send = String(tempReading); //convert reading from Float to String
    client.publish("area/temperature", temp_send); //send reading to mqtt
    String humidity_send = String(humidityReading); //convert reading from Float to String
    client.publish("area/humidity", humidity_send); //send reading to mqtt
    Serial.println(tempReading);
    Serial.println(humidityReading);

    //take voltage reading
    voltageReading = analogRead(analogPin);
    String voltage_send = String(voltageReading); //convert reading from Float to String
    client.publish("area/voltage", voltage_send); //send reading to mqtt
    
    //go to sleep for 10 seconds
//    ESP.deepSleep(100000000, WAKE_RF_DEFAULT); //ESP.deepSleep([microseconds], [mode])
    }
}


//###########################################################


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
  
    if(payload == "ON"){
      digitalWrite(ledPin, HIGH);
      Serial.println("PIN HIGH");
    }
  
    if(payload == "OFF"){
      digitalWrite(ledPin, LOW);
      Serial.println("PIN LOW");
    }
}
//###########################################################
