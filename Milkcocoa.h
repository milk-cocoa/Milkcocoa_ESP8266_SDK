/*
The MIT License (MIT)

Copyright (c) 2015 Technical Rockstars
Copyright (C) 2015 Embedded and Real-Time Systems Laboratory
              Graduate School of Information Science, Nagoya Univ., JAPAN
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
    ~DataElement();
    void setValue(const char *key, const char *v);
    void setValue(const char *key, int v);
    void setValue(const char *key, double v);
    char *toCharArray();
    char *getString(const char *key);
    int getInt(const char *key);
    float getFloat(const char *key);

  private:
    aJsonObject *params;
    aJsonObject *paJsonObj;
};

typedef void (*GeneralFunction) (DataElement *pelem);

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
  static Milkcocoa* createWithApiKey(Client *client, const char *host, uint16_t port, const char *app_id, const char *client_id, const char *key, const char *secret);
  bool connect(uint16_t timeout);
  bool loop(uint16_t timeout = 0);
  bool push(const char *path, DataElement *pdataelement);
  bool send(const char *path, DataElement *pdataelement);
  bool on(const char *path, const char *event, GeneralFunction cb);


private:
  const char *app_id;
  char session[128];
  Adafruit_MQTT_Client *mqtt;
  MilkcocoaSubscriber *milkcocoaSubscribers[MILKCOCOA_SUBSCRIBERS];
};


#endif