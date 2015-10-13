#ifndef _MILKCOCOA_H_
#define _MILKCOCOA_H_

#include "Client.h"
#include "include/Adafruit/Adafruit_MQTT.h"
#include "include/Adafruit/Adafruit_MQTT_Client.h"
#include "include/aJson/aJson.h"

#define MILKCOCOA_SUBSCRIBERS 8

class DataElement {
  public:
    DataElement();
    DataElement(char *json_string);
    void setValue(const char *key, const char *v);
    void setValue(const char *key, int v);
    void setValue(const char *key, double v);
    char *toCharArray();
    char *getString(const char *key);
    int getInt(const char *key);
    float getFloat(const char *key);

  private:
    aJsonObject *params;
};

typedef void (*GeneralFunction) (DataElement elem);

class MilkcocoaSubscriber {
	public:
		Adafruit_MQTT_Subscribe *mqtt_sub;
		GeneralFunction cb;
		MilkcocoaSubscriber(Adafruit_MQTT_Subscribe *_mqtt_sub, GeneralFunction _cb);
};

class Milkcocoa {
 public:
  Milkcocoa(Client *client, const char *host, const char *app_id, const char *client_id);

  void connect();
  void loop();
  bool push(const char *topic, DataElement dataelement);
  bool send(const char *topic, DataElement dataelement);
  bool on(const char *topic, GeneralFunction cb);


private:
  Adafruit_MQTT_Client *mqtt;
  MilkcocoaSubscriber *milkcocoaSubscribers[MILKCOCOA_SUBSCRIBERS];
};


#endif
