#include "Milkcocoa.h"




DataElement::DataElement() {
  params = aJson.createObject();
}

DataElement::DataElement(char *json_string) {
  params = aJson.parse(json_string);
}

void DataElement::setValue(const char *key, const char *v) {
  aJson.addStringToObject(params, key, v);
}

void DataElement::setValue(const char *key, int v) {
  aJson.addNumberToObject(params, key, v);
}

void DataElement::setValue(const char *key, double v) {
  aJson.addNumberToObject(params, key, v);
}

char *DataElement::getString(const char *key) {
  aJsonObject* obj = aJson.getObjectItem(params, key);
  return obj->valuestring;
}

int DataElement::getInt(const char *key) {
  aJsonObject* obj = aJson.getObjectItem(params, key);
  return obj->valueint;
}

float DataElement::getFloat(const char *key) {
  aJsonObject* obj = aJson.getObjectItem(params, key);
  return obj->valuefloat;
}


char *DataElement::toCharArray() {
  aJsonObject *data = aJson.createObject();
  aJson.addItemToObject(data, "params", params);
  aJson.print(data);
}

Milkcocoa::Milkcocoa(Client *client, const char *host, const char *app_id, const char *client_id) {
  mqtt = new Adafruit_MQTT_Client(client, host, 1883, client_id, "sdammy", app_id);

  for (uint8_t i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
    milkcocoaSubscribers[i] = 0;
  }

}

void Milkcocoa::connect() {
  int ret;

  if (mqtt->connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt->connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt->connectErrorString(ret));
       Serial.println(ret);
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt->disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}

bool Milkcocoa::push(const char *topic, DataElement dataelement) {
  Adafruit_MQTT_Publish pushPublisher = Adafruit_MQTT_Publish(mqtt, topic);
  pushPublisher.publish(dataelement.toCharArray());
}

bool Milkcocoa::send(const char *topic, DataElement dataelement) {
}

void Milkcocoa::loop() {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt->readSubscription(1000))) {
    for (uint8_t i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
      if (subscription == (milkcocoaSubscribers[i]->mqtt_sub) ) {
        DataElement de = DataElement();
        de.setValue("v", 1);
        milkcocoaSubscribers[i]->cb((char *)milkcocoaSubscribers[i]->mqtt_sub->lastread);
      }
    }
  }
}

bool Milkcocoa::on(const char *topic, GeneralFunction cb) {
  uint8_t i;
  MilkcocoaSubscriber *sub = new MilkcocoaSubscriber(new Adafruit_MQTT_Subscribe(mqtt, topic), cb);
  for (i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
    if (milkcocoaSubscribers[i] == sub) {
      return false;
    }
  }
  for (i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
    if (milkcocoaSubscribers[i] == 0) {
      milkcocoaSubscribers[i] = sub;
      return true;
    }
  }
}

MilkcocoaSubscriber::MilkcocoaSubscriber(Adafruit_MQTT_Subscribe *_mqtt_sub, GeneralFunction _cb) {
  mqtt_sub = _mqtt_sub;
  cb = _cb;
}




