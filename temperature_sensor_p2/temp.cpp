


#include "temp.hpp"

#define DHTPIN 2
#define DHTTYPE DHT11

TEMP::Sensor::Sensor()
{
    this->dht = new DHT(DHTPIN,DHTTYPE);
}