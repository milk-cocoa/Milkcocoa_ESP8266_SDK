Milkcocoa ESP8266 SDK
=====

[Milkcocoa](https://mlkcca.com/) SDK working on [Arduino core for ESP8266](https://github.com/esp8266/Arduino).

Works with the ESP8266 Arduino platforms, and anything that supports Arduino's Client interface.


## How To Use

Include library(`#include <Milkcocoa.h>`), and write a code like below.

```
// 'client' is Ethernet/WiFi Client
Milkcocoa milkcocoa = Milkcocoa(&client, "milkcocoa_app_id.mlkcca.com", 1883, "milkcocoa_app_id", "mqtt_client_id");

void setup() {
 	//"on" API was able to call in setup
	milkcocoa.on("milkcocoa_datastore_name", "push", onpush);
}

void loop() {
	//milkcocoa.loop must be called in loop()
	milkcocoa.loop();

	//push
	DataElement elem = DataElement();
	elem.setValue("name", "Milk");
	elem.setValue("age", 35);
	milkcocoa.push("milkcocoa_datastore_name", &elem);

	delay(10000);
}

void onpush(DataElement *elem) {
  Serial.println(elem->getString("name"));
  Serial.println(elem->getInt("age"));
  // Output:
  // Milk
  // 35
};
```

### Using API Key

If you use Milkcocoa API Key Authantication, please use `createWithApiKey`.

```
Milkcocoa *milkcocoa = Milkcocoa::createWithApiKey(&client, "milkcocoa_app_id.mlkcca.com", 1883, "milkcocoa_app_id", "mqtt_client_id", "API_KEY", "API_SECRET");
```


## Examples

- [Simple ESP8266 Example](https://github.com/milk-cocoa/Milkcocoa_ESP8266_SDK/blob/master/examples/milkcocoa_esp8266/milkcocoa_esp8266.ino): simple test of `push()` and `on("push")`.
- [ESP8266 TOUT Example](https://github.com/milk-cocoa/Milkcocoa_ESP8266_SDK/blob/master/examples/milkcocoa_esp8266_tout/milkcocoa_esp8266_tout.ino): get sensor data from TOUT of ESP8266
- [ESP8266 API Key Authantication Example](https://github.com/milk-cocoa/Milkcocoa_ESP8266_SDK/blob/master/examples/milkcocoa_esp8266_apikey_auth/milkcocoa_esp8266_apikey_auth.ino): auth with Milkcocoa API Key


## LICENSE

MIT



以下はCopyright (c) 2015 Technical Rockstars.

- Milkcocoa.h
- Milkcocoa.cpp

### Using

- https://github.com/adafruit/Adafruit_MQTT_Library
- https://github.com/interactive-matter/aJson

