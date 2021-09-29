// This project shows how to read DHT sensors and  send data from an ESP32 board to flespi MQTT broker
/*************************     Includes      *********************************/
#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <WiFi.h> /* WiFi library needed for connecting to WiFi on ESP32 */
#include <PubSubClient.h> /* PubSubClient needed for MQTT based API */
#include <HardwareSerial.h>  /* HardwareSerial needed to assign relavant pins on the board for UART communication */
#include <Ticker.h> 
#include <ArduinoJson.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <NewPing.h>
#include <string.h>

/************************* MAIN DEFINITIONS *********************************/

const char* ssid = "Sahar PC";
const char* password = "sahar123" ;        /* Required WiFi PARAMETERS to connect to WiFi */

const char* mqttserver = "mqtt.flespi.io";
const char* mqttUser = "cGM4ycol5E1uL1fmOubwOuFqJ4yGdAmd6W6tYkERMv9lkewXzxhbRMppELVGb7J6";      // if you don't have MQTT Username, no need input
const char* mqttPassword = "";  // if you don't have MQTT Password, no need input
const int mqttPort = 1883; /* Establishing MQTT connection parammeters */

/********************* Global connection instances **************************/

DynamicJsonDocument jsonDoc(256);
JsonObject payload = jsonDoc.to<JsonObject>();
JsonObject status = payload.createNestedObject("d");
char msg[128];

WiFiClient espClient;  /* Creating WiFiClient to use the full WIFI API */
PubSubClient client(espClient);  /* Creates pub */

/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Ticker for temperature reading */
Ticker tempTicker;
/** Comfort profile */
ComfortState cf;
/** Flag if task should run */
bool tasksEnabled = false;
/** Pin number for DHT11 data pin */

#define TRIGGER_PIN 2
#define ECHO_PIN 4
#define MAX_DISTANCE 400

#define dhtPin 5 //DHT-11 Output Pin connection

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
DHTesp dht;


String Temp , Humid , Dist;
float humidity, temperature ;

/**
 * Task to reads temperature from DHT11 sensor
 * @param pvParameters
 *    pointer to task parameters
 */
void tempTask(void *pvParameters) {
  Serial.println("tempTask loop started");
  while (1) // tempTask loop
  {
    if (tasksEnabled) {
      // Get temperature values
      getTemperature();
    }
    // Got sleep again
    vTaskSuspend(NULL);
  }
}


/**
 * initTemp
 * Setup DHT library
 * Setup task and timer for repeated measurement
 * @return bool
 *    true if task and timer are started
 *    false if task or timer couldn't be started
 */
bool initTemp() {
  byte resultValue = 0;
  // Initialize temperature sensor
	dht.setup(dhtPin, DHTesp::DHT11);
	Serial.println("DHT initiated");

  // Start task to get temperature
	xTaskCreatePinnedToCore(
			tempTask,                       /* Function to implement the task */
			"tempTask ",                    /* Name of the task */
			4000,                           /* Stack size in words */
			NULL,                           /* Task input parameter */
			5,                              /* Priority of the task */
			&tempTaskHandle,                /* Task handle. */
			1);                             /* Core where the task should run */

  if (tempTaskHandle == NULL) {
    Serial.println("Failed to start task for temperature update");
    return false;
  } else {
    // Start update of environment data every 20 seconds
    tempTicker.attach(20, triggerGetTemp);
  }
  return true;
}

/**
 * triggerGetTemp
 * Sets flag dhtUpdated to true for handling in loop()
 * called by Ticker getTempTimer
 */
void triggerGetTemp() {
  if (tempTaskHandle != NULL) {
	   xTaskResumeFromISR(tempTaskHandle);
  }
}

/**
 * getTemperature
 * Reads temperature from DHT11 sensor
 * @return bool
 *    true if temperature could be aquired
 *    false if aquisition failed
*/
bool getTemperature() {
  
    float distance, duration;
    float soundsp , soundcm;
    int iterations = 5 ;
   // Reading temperature for humidity takes about 250 milliseconds!
   // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
   TempAndHumidity newValues = dht.getTempAndHumidity();
   // Check if any reads failed and exit early (to try again).
   if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
    return false;
  }
    humidity=newValues.humidity;
    temperature= newValues.temperature ; 
   
    Temp=String(newValues.temperature);
    Humid= String(newValues.humidity/100);
   
  	return true;
  }
/**
 * getdistance
 * Reads temperature from DHT11 sensor
 * @return bool
 *    true if temperature could be aquired
 *    false if aquisition failed
*/
bool getDistance() {
    float distance, duration;
    float soundsp , soundcm;
    int iterations = 5 ;
    soundsp = 331.4 + (0.606 * temperature) + (0.0124 * humidity ) ;
    soundcm = soundsp / 10000 ;
    
    duration = sonar.ping_median(iterations); //distance = sonar.ping_cm();
    distance = (duration/2) * soundcm ;

      if (distance >= 400 || distance <= 2){
       Dist="Out of range  ";
    }
    else {
        Dist=String(distance);
       }
   return true;
    }
/**
 * callback
 * The PubSubClient MQTT API allows you to define how the system responds to a MQTT message, known as the 'callback' response,
this is where we will react to incoming data and instructions 
 */
void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
  



}

/*************************** Sketch Code ************************************/

void setup()
{
  Serial.begin(115200);
  /*Connect to WiFi first*/
  WiFi.begin(ssid, password);
  /*Loop until connected*/
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 // establish MQTT connection
   client.setServer(mqttserver, mqttPort);
  client.setCallback(callback);
   /*Loop until connected*/
   while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32", mqttUser, mqttPassword )) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  Serial.println();
  Serial.println("DHT ESP32 example with tasks");
  initTemp();
  // Signal end of setup() to tasks
  tasksEnabled = true;
}

void loop() {
   DynamicJsonDocument jsonDoc(256);
   JsonObject payload = jsonDoc.to<JsonObject>();
   JsonObject status = payload.createNestedObject("d");
   char msg[128];
  getTemperature();
  getDistance();
  status["temperature"]=Temp;
  status["humid"]=Humid;
  status["dist"]=Dist;
  Serial.println( Temp+"  "+Humid+"  "+Dist) ;
  serializeJson(jsonDoc,msg);
  Serial.println(msg);
  client.publish("ESP32",msg);
  memset(msg,0,sizeof(msg)) ;
  Serial.println(msg);
  jsonDoc.clear();
  delay(1000);  
  
  
}
  
