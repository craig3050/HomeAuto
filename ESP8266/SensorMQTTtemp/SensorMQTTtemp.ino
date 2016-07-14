#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <DHT.h>

//Device Setup
const char *area = "LivingRoom";
const char *ssid = "CraigDC";
const char *pass = "32e75c752f";

//setup variables used in the sketch
unsigned long currentMillis = millis();
unsigned long lastMillis = 0; //used for the delay loop
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
//  dht.begin(); //initialise temp sensor

  //wifi + MQTT Setup
  Serial.begin(115200); //begin serial monitor for debugging
  WiFi.begin(ssid, pass); //start wifi module
  client.begin("192.168.0.10", esp12); //begin connection to MQTT server
  connect(); //run connect loop
}


void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
//


  if (currentMillis - lastMillis >= 10000000) {
    lastMillis = currentMillis;  
    
    //go to sleep for 10 seconds
    ESP.deepSleep(100000000, WAKE_RF_DEFAULT); //ESP.deepSleep([microseconds], [mode])
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
  
    if(payload == "UP"){
      digitalWrite(5, HIGH);
    }
  
    if(payload == "DOWN"){
      digitalWrite(5, LOW);
    }
  
}


//void takeReadings() {
//    //take tempReading from DHT22 sensor;
//    tempReading = dht.readTemperature();  
//    //take humidityReading from DHT22 sensor;
//    humidityReading = dht.readHumidity();
//    Serial.print(tempReading);
//    Serial.print(humidityReading);
    
//    client.publish("area/temperature", tempReading);
//    client.publish("area/humidity", humidityReading);

//}
