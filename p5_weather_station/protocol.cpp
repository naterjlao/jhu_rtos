#include <WiFiUdp.h>
#include "protocol.hpp"

//----------------------------------------------------------------
/// @note The ESP8266 is Little Endian. Host-to-Network conversions
/// must be performed.
//----------------------------------------------------------------
void PROTOCOL::load_datagram(WEATHER_DATAGRAM* datagram,
    const unsigned int sample_num,
    const float temp_f,
    const float humidity)
{
    unsigned int buffer = 0x0;

    // ----- TRAFFIC ID ----- //
    datagram->traffic_id = htonl(PROTOCOL::TRAFFIC_ID);

    // ----- SAMPLE NUMBER ----- //
    datagram->sample_num = htonl(sample_num);

    // ----- TEMPERATURE ----- //
    memcpy(&buffer, &temp_f, sizeof(buffer));
    buffer = htonl(buffer);
    datagram->temp_f = buffer;

    // ----- HUMIDITY ----- //
    memcpy(&buffer, &humidity, sizeof(humidity));
    buffer = htonl(buffer);
    datagram->humdity = buffer;
}