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
    char topic[100];
		MilkcocoaSubscriber(GeneralFunction _cb);
    void set_mqtt_sub(Adafruit_MQTT_Subscribe *_mqtt_sub);
};

class Milkcocoa {
 public:
  Milkcocoa(Client *client, const char *host, uint16_t port, const char *_app_id, const char *client_id);
  Milkcocoa(Client *client, const char *host, uint16_t port, const char *_app_id, const char *client_id, const char *_session);

  void connect();
  void loop();
  bool push(const char *path, DataElement dataelement);
  bool send(const char *path, DataElement dataelement);
  bool on(const char *path, const char *event, GeneralFunction cb);


private:
  const char *app_id;
  char session[128];
  Adafruit_MQTT_Client *mqtt;
  MilkcocoaSubscriber *milkcocoaSubscribers[MILKCOCOA_SUBSCRIBERS];
};


#endif
