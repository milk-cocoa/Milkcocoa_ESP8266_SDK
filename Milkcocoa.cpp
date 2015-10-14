#include "Milkcocoa.h"
#include "stdio.h"



DataElement::DataElement() {
  params = aJson.createObject();
}

DataElement::DataElement(char *json_string) {
  aJsonObject* obj = aJson.parse(json_string);
  params = aJson.getObjectItem(obj, "params");
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
  if(obj == NULL)
    Serial.println("obj is NULL");
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

Milkcocoa::Milkcocoa(Client *client, const char *host, uint16_t port, const char *_app_id, const char *client_id) {
  app_id = _app_id;
  mqtt = new Adafruit_MQTT_Client(client, host, port, client_id, "sdammy", app_id);

  for (uint8_t i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
    milkcocoaSubscribers[i] = 0;
  }

}

Milkcocoa::Milkcocoa(Client *client, const char *host, uint16_t port, const char *_app_id, const char *client_id, const char *_session) {
  app_id = _app_id;
  sprintf(session, "s%s", _session);
  mqtt = new Adafruit_MQTT_Client(client, host, port, client_id, session, app_id);

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

bool Milkcocoa::push(const char *path, DataElement dataelement) {
  char topic[100];
  sprintf(topic, "%s/%s/push", app_id, path);
  Adafruit_MQTT_Publish pushPublisher = Adafruit_MQTT_Publish(mqtt, topic);
  pushPublisher.publish(dataelement.toCharArray());
}

bool Milkcocoa::send(const char *path, DataElement dataelement) {
  char topic[100];
  sprintf(topic, "%s/%s/send", app_id, path);
  Adafruit_MQTT_Publish pushPublisher = Adafruit_MQTT_Publish(mqtt, topic);
  pushPublisher.publish(dataelement.toCharArray());
}

void Milkcocoa::loop() {
  connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt->readSubscription(1000))) {
    for (uint8_t i=0; i<MILKCOCOA_SUBSCRIBERS; i++) {
      if(milkcocoaSubscribers[i] == 0) continue;
      if (subscription == (milkcocoaSubscribers[i]->mqtt_sub) ) {
        DataElement de = DataElement((char *)milkcocoaSubscribers[i]->mqtt_sub->lastread);
        milkcocoaSubscribers[i]->cb( de );
      }
    }
  }
}

bool Milkcocoa::on(const char *path, const char *event, GeneralFunction cb) {
  MilkcocoaSubscriber *sub = new MilkcocoaSubscriber(cb);
  sprintf(sub->topic, "%s/%s/%s", app_id, path, event);

  uint8_t i;
  Adafruit_MQTT_Subscribe *mqtt_sub = new Adafruit_MQTT_Subscribe(mqtt, sub->topic);
  sub->set_mqtt_sub(mqtt_sub);
  if(!mqtt->subscribe(mqtt_sub)) {
    return false;
  }
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

MilkcocoaSubscriber::MilkcocoaSubscriber(GeneralFunction _cb) {
  cb = _cb;
}

void MilkcocoaSubscriber::set_mqtt_sub(Adafruit_MQTT_Subscribe *_mqtt_sub) {
  mqtt_sub = _mqtt_sub;
}


