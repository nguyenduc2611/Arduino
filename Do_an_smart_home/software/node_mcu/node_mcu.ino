#include <ESP8266WiFi.h>
#include <SimpleTimer.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"

#define DHTPIN D1   
#define AIR A0 

#define DHTTYPE DHT11  
#define RELAY1 D2
#define RELAY2 D3
#define RELAY3 D7
#define RELAY4 D8

char *ssid      = "AE-Cay-chuoi";               // Ten WiFi SSID
char *password  = "123456789";               // Mat khau wifi
char auth[] = "kZEITAMokBoYsofChTetud34c4Vlv7YG";    //AuthToken copy ở Blynk Project
float hum = 0;
float temp = 0;
char status_relay1 = 0;
char status_relay2 = 0;
char status_relay3 = 0;
char status_relay4 = 0;
int air = 0;
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void setup() {
  Serial.begin(9600);
  dht.begin();
  Blynk.begin(auth, ssid, password);
  timer.setInterval(500, sendUptime);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
}

BLYNK_WRITE(V2)
{
  status_relay1 = param.asInt();
}

BLYNK_WRITE(V3)
{
  status_relay2 = param.asInt();
}

BLYNK_WRITE(V4)
{
  status_relay3 = param.asInt();
}

BLYNK_WRITE(V5)
{
  status_relay4 = param.asInt();
}

void sendUptime()
{
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V6, air);
}
void loop() {

  hum = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int a = analogRead(A0);
  air = map(a,0,1023,0,100);
  if(status_relay1)
  {
    digitalWrite(RELAY1, HIGH);
  }
  else
  {
    digitalWrite(RELAY1, LOW);
  }

  if(status_relay2)
  {
    digitalWrite(RELAY2, HIGH);
  }
  else
  {
    digitalWrite(RELAY2, LOW);
  }

  if(status_relay3)
  {
    digitalWrite(RELAY3, HIGH);
  }
  else
  {
    digitalWrite(RELAY3, LOW);
  }

  if(status_relay4)
  {
    digitalWrite(RELAY4, HIGH);
  }
  else
  {
    digitalWrite(RELAY4, LOW);
  }
  
  Blynk.run();
  timer.run();
}
