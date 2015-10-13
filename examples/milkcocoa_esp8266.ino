#include <ESP8266WiFi.h>
#include <Milkcocoa.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"


/************************* Your Milkcocoa Setup *********************************/

#define MILKCOCOA_APP_ID      "uniib1d489g"
#define MILKCOCOA_DATASTORE   "esp8266"
// Of course, you can use multiple datastores
// #define MILKCOCOA_DATASTORE2   "your_milkcocoa_datastore_name2"

/************* Milkcocoa Setup (you don't need to change this!) ******************/

#define MILKCOCOA_SERVERPORT  1883
#define MILKCOCOA_USERNAME    "sdammy"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_USERNAME;

Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_APP_ID, MQTT_CLIENTID);

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  milkcocoa.connect();
  milkcocoa.on("uniib1d489g/esp8266/push", onpush);
};

void loop() {
  milkcocoa.loop();
  DataElement a = DataElement();
  a.setValue("v", 1);
  milkcocoa.push("uniib1d489g/esp8266/push", a);
  delay(5000);
};

void onpush(DataElement elem) {
  Serial.println(elem.getInt("v"));
};

