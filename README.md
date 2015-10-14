README.md
=====

Milkcocoa Arduino SDK


## How To Use


```
//client is Ethenet Client
Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID); 

void setup() {
	//"on" API was able to call in setup
	milkcocoa->on("datastore name", "push", onpush);
}

void loop() {
	//milkcocoa.loop must be called in loop
	milkcocoa.loop();

	//push
	DataElement elem = DataElement();
	elem.setValue("name", "Name");
	elem.setValue("age", 35);
	milkcocoa.push("datastore name", elem);
}

void onpush(DataElement elem) {
  Serial.println(elem.getString("name"));
  Serial.println(elem.getInt("age"));
};
```

