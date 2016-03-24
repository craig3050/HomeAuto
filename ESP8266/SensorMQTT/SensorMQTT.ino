#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DHT.h>

//Device Setup
const char *area = "LivingRoom";
const char *ssid = "3Com";
const char *pass = "32e75c752f";

//setup variables used in the sketch
unsigned long lastMillis = 0; //used for the delay loop
const int ledTest = 5; //set up pin for test LED to pin 5
const int sensorSwitch = 6; //output pin to power on sensor transistor
float tempReading = 0;
float humidityReading = 0;
#define DHTTYPE DHT22
#define DHTPIN 7 //data wire plugged in to pin 7
//initialise DHT Sensor
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

WiFiClient esp12;
MQTTClient client;

void connect(); // <- predefine connect() for setup()
void takeReadings(); // predefine takeReadings()

void setup() {
  //Sensor pin setup
  pinMode(ledTest, OUTPUT); //initialise ledTest as output
  pinMode(sensorSwitch, OUTPUT); //initialise sensorSwitch as output
  dht.begin(); //initialise temp sensor

  //wifi + MQTT Setup
  Serial.begin(9600); //begin serial monitor for debugging
  WiFi.begin(ssid, pass); //start wifi module
  client.begin("192.168.0.2", esp12); //begin connection to MQTT server
  connect(); //run connect loop
}


void loop() {


  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  //power on the sensors
  digitalWrite(sensorSwitch, HIGH); //DHT22 requires min 2 seconds to stabilise

  //keep alive for 10 seconds so any messages can be retrieved
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();

    digitalWrite(ledTest, HIGH); //used during debug only

    takeReadings(); //launch the loop to take the readings
    digitalWrite(sensorSwitch, LOW); //Power down the sensors

    //go to sleep after 10 seconds
    ESP.deepSleep(600000000, WAKE_RF_DEFAULT); //ESP.deepSleep([microseconds], [mode])
  }

}


//connect to wifi
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


void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  //
  //  if(payload == "UP"){
  //    digitalWrite(5, HIGH);
  //  }
  //
  //  if(payload == "DOWN"){
  //    digitalWrite(5, LOW);
  //  }
  //
}


void takeReadings() {

  tempReading = dht.readTemperature();
  humidityReading = dht.readHumidity();
    //take tempReading from DHT22 sensor;
    //take humidityReading from DHT22 sensor;
    //client.publish("/area/temperature", tempReading);
    //client.publish("/area/humidity", humidityReading);

}
