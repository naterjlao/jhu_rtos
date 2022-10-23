#if 1
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(128,32,&Wire,-1);

unsigned int baz;
float foo, bar;

void setup()   
{                
	// initialize with the I2C addr 0x3C SSD1306_SWITCHCAPVCC
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

	// Clear the buffer.
	display.clearDisplay();

  baz = 0;
	foo = 0.0;
  bar = 0.0;
}

void loop() {	
  
	display.clearDisplay();
  display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.println("192.168.123.111 - OK");
  display.print("Sample: ");
  display.println(baz);
  display.print("Temperature: ");
  display.print(foo);
  display.println("F");
  display.print("Humidity:    ");
	display.print(bar);
  display.println("%");
  display.display();

  baz++;
  foo += 1.5;
  bar += 1.0;
  if (foo > 150.0) foo = 0.0;
  if (bar > 100.0) bar = 0.0;

  //delay(1000);
}
#endif

#if 0
#include <DHT.h>

DHT dht(12, DHT11);

void setup()
{
    Serial.begin(9600);
    dht.begin();
}

void loop()
{
    float t = dht.readTemperature(true);
    float h = dht.readHumidity();

    if (!isnan(t)) Serial.println(t);
    if (!isnan(h)) Serial.println(h);
}


#endif
#if 0
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <WiFiUdp.h>

#include "protocol.hpp"

uint8_t activity_led = LOW;

// ----- PROTOCOL ----- //
PROTOCOL::WEATHER_DATAGRAM datagram;
char payload[sizeof(datagram)];

// ----- TRANSMISSION SETTINGS ----- //
WiFiUDP socket;
const int port = 8266;
IPAddress broadcast = IPAddress(224, 0, 1, 3);

// ----- WiFi SETTINGS ----- //
const char* ssid     = "lao-mesh";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "11531Seneca!";     // The password of the Wi-Fi network

void setup() {
    pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer
}

void loop()
{
    digitalWrite(LED_BUILTIN, activity_led);
    activity_led = (activity_led == HIGH) ? LOW : HIGH;

    PROTOCOL::load_datagram(&datagram, 5, 75.45, 42.87);
    memcpy(payload, &datagram, sizeof(datagram));

    Serial.print("beginPacketMulticast");
    Serial.println(socket.beginPacketMulticast(broadcast,port,WiFi.localIP()));
    socket.write(payload,sizeof(payload));
    Serial.print("send packet");
    Serial.println(socket.endPacket());
}

#endif










































