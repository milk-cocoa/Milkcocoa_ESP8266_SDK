/**
 * NOTE:
 * When you use this sketch,
 * please connect WAKE-pin(ESP-WROOM-02's IO16-pin) to RESET-pin.
 *
**/

#include <ESP8266WiFi.h>
#include <Milkcocoa.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "...SSID..."
#define WLAN_PASS       "...PASS..."


/************************* Your Milkcocoa Setup *********************************/

#define MILKCOCOA_APP_ID      "...YOUR_MILKCOCOA_APP_ID..."
#define MILKCOCOA_DATASTORE   "esp8266/tout/half-hour"

/************* Milkcocoa Setup (you don't need to change this!) ******************/

#define MILKCOCOA_SERVERPORT  1883

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);


/**
 * Functions
**/
void setupWiFi() {
  Serial.println(); Serial.println();
  Serial.println("WiFi Connection->AP==================");
  Serial.print("Connecting to "); Serial.print(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  int wifiCounter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(450);
    Serial.print(".");
    if(wifiCounter > 20) {
      Serial.println("Failed to connect WiFi. Reset now.");
      delay(500);
      ESP.deepSleep(2 * 1000 * 1000);
      delay(1000);
    }
    delay(50);
    wifiCounter++;
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  Serial.println("=====================================");
};

void connectToMilkcocoa() {
  if(!milkcocoa.loop(10*5000)){
    Serial.println("Failed to connect Milkcocoa. Reset now.");
    delay(500);
    ESP.deepSleep(2 * 1000 * 1000);
    delay(1000);
  }
};

void onpush(DataElement *elem) {
  Serial.println("Onpush comming.");
  Serial.print("Got value: "); Serial.println(elem->getInt("v"));

  Serial.println("Sleep start, reset after 30 minutes.");
  delay(500);
  ESP.deepSleep(1800000000);
  delay(1000);
};


/**
 * Main
**/
void setup() {
  Serial.begin(115200);
  delay(10);

  setupWiFi();
  Serial.println();

  if( milkcocoa.on(MILKCOCOA_DATASTORE, "push", onpush) ) Serial.println("Milkcocoa on sucesss");
  else Serial.println("Milkcocoa on failure");
  Serial.println();
};

int loopCounter = 0;

void loop() {
  connectToMilkcocoa();

  int sensorValue = analogRead(A0);
  Serial.println("Read sensor value: " + String(sensorValue));

  DataElement elem = DataElement();
  elem.setValue("v", sensorValue);
  Serial.println();

  if(loopCounter == 0) milkcocoa.push(MILKCOCOA_DATASTORE, &elem);

  delay(100);

  if(loopCounter > 50){
    Serial.println("Failed to push data to Milkcocoa. Reset now.");
    delay(500);
    ESP.deepSleep(2 * 1000 * 1000);
    delay(1000);
  }
  loopCounter++;
};
