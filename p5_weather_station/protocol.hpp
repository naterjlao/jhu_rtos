namespace PROTOCOL
{
    const unsigned int TRAFFIC_ID = 0x45535057;

    typedef struct weather_datagram
    {
        unsigned int traffic_id;
        unsigned int sample_num;
        unsigned int temp_f;
        unsigned int humdity;
    } WEATHER_DATAGRAM;

    void load_datagram(WEATHER_DATAGRAM* datagram,
        const unsigned int sample_num,
        const float temp_f,
        const float humidity);
}