#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include "string.h"
#include "stdlib.h"

SoftwareSerial mySerial(10, 11);

static const int RXPin = 2, TXPin = 3;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
char status_sms = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
float lo = 0;
float la = 0;
char buff_sms[255] = {0};
char content_sms[100] = {0};
char sim_AT[100] = {0};
char STD[12] = {0};
char confirm[8] = {'L','o','c','a','t','i','o','n'};
int count = 0;
int c = 0;
char flag_content = 0;
char flag_location = 0;
char flag_2 = 0;
char channel1_off = 0;
char channel2_flag = 0;
char car = 0;
char flag_den = 0;
char count_sw = 0;
char count_gps = 0;
#define CHANNEL1 9
#define CHANNEL2 8
#define CHANNEL3 7
#define CHANNEL4 6
#define RELAY1 5
#define RELAY2 4

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    /*Sim module*/
    Serial.println("ret");
  mySerial.begin(9600);
  delay(500);
  mySerial.write("AT+CMGF=1");
  mySerial.write(10);
  delay(500);
  mySerial.write("AT+CNMI=2,2");
  mySerial.write(10);
  delay(3000);
  while(mySerial.available())
  {
    sim_AT[c] = mySerial.read();
    Serial.println(sim_AT[c]);
    c++;
  }

 /*GPS module*/
  //ss.begin(GPSBaud);
  
  pinMode(CHANNEL1, INPUT);
  pinMode(CHANNEL2, INPUT);
  pinMode(CHANNEL3, INPUT);
  pinMode(CHANNEL4, INPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, LOW);

  /*while(lo== 0)
  {
    ss.begin(GPSBaud);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  delay(500);
  }*/

  mySerial.begin(9600);
  digitalWrite(RELAY1, LOW);
  delay(1000);
  digitalWrite(RELAY1, HIGH);
}

void loop() {
  int sw = analogRead(A0);
  //Serial.println(sw);
  //delay(500);
  if((sw > 1000)&&(car == 0))
  {
    count_sw++;
    if(count_sw == 10)
    {
      digitalWrite(RELAY1, LOW);
      for(int i = 0; i < 10;i++)
      {
    digitalWrite(RELAY2, LOW);
    delay(500);
    digitalWrite(RELAY2, HIGH);
    delay(500);
    }
    digitalWrite(RELAY1, HIGH);
    count_sw = 0;
    }
  }
  sms_rev();
//mySerial.begin(9600);
  while((flag_content)&&(count_gps < 100))
  {
    Serial.println("doc");
    ss.begin(GPSBaud);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  delay(100);
  count_gps++;
    count = 0;
    if(lo!= 0)
    {
      Serial.println("gui");
      send_sms();
      for(int l = 0; l < 12;l++)
      {
        STD[l] = 0;
      }
      flag_content = 0;
      count = 0;
    }
  }
  count_gps = 0;

 char channel1 = digitalRead(CHANNEL1);
  char channel2 = digitalRead(CHANNEL2);
  char channel3 = digitalRead(CHANNEL3);
  char channel4 = digitalRead(CHANNEL4);

if(channel1 == LOW)
{
  channel1_off++;
  if(channel1_off > 100)
  {
    //Serial.println("channel1");
    flag_den = 1;
    digitalWrite(RELAY2, LOW);
  }
  
}
else
{
  channel1_off = 0;
  flag_den = 0;
}


if((channel2 == HIGH)&&(!channel2_flag))
{
  //Serial.println("channel2");
  digitalWrite(RELAY1, LOW);
  for(int i = 0; i < 5;i++)
  {
    digitalWrite(RELAY2, LOW);
    delay(500);
    digitalWrite(RELAY2, HIGH);
    delay(500);
  }
  digitalWrite(RELAY1, HIGH);
  channel2_flag = 1;
}
else if((channel2_flag)&&(channel2 == LOW))
{
  //Serial.println("channel3");
  digitalWrite(RELAY1, LOW);
  for(int i = 0; i < 5;i++)
  {
    digitalWrite(RELAY2, LOW);
    delay(500);
    digitalWrite(RELAY2, HIGH);
    delay(500);
  }
  digitalWrite(RELAY1, HIGH);
  channel2_flag = 0;
}
else
{
  
}


if((channel4 == HIGH)&&(car == 1))
{
    digitalWrite(RELAY2, LOW);
    car = 0;
}
else if((channel4 == LOW)&&(!flag_den)&&(car == 0))
{
    digitalWrite(RELAY2, HIGH);
    car = 1;
}
else
{
}

}
void sms_rev(void)
{
  char j, p, k = 0;
  char flag = 0;
  char flag_std = 0;
  count = 0;
  while(mySerial.available()>0)
  {
    buff_sms[count] = mySerial.read();
    Serial.print(buff_sms[count]);
    count++;
    delay(10);
  }
  if(count > 0)
  {
    Serial.println("count");
    Serial.println(count);
    for(int g = 0; g < count;g++)
    {
      if((buff_sms[g] == '8')&&(buff_sms[g+1] == '4'))
      {
        flag_std = 1;
      }
      if((flag_std)&&(buff_sms[g] == 34))
      {
        flag_std = 0;
      }
      if(flag_std)
      {
        if(buff_sms[g] > 47)
        {
        STD[k] = buff_sms[g];
        k++;
        }
      }

      if((buff_sms[g] == 'L'))
      {
         flag = 1;
      }

      if(flag)
      {
        content_sms[j] = buff_sms[g];
        Serial.print(content_sms[j]);
        j++;
      }
    }
    //Serial.println(content_sms);
    //Serial.println(STD);
    while(flag)
    {
      Serial.println("get content");
      flag_content = 1;
      flag = 0;
    }
  }

}



static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid) //neu 0 bt dc toa do
  {
    while (len-- > 1); //show len debug kí tu *
  }
  else //bat dc toa do
  {
    lo = gps.location.lng();
    la = gps.location.lat();
  }
  smartDelay(0);
}

void send_sms(void)
{
    mySerial.begin(9600);
    Serial.println("SEND ok");
    mySerial.write("AT");
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGF=1"); //cai dat module sim che do nhan tin
    mySerial.write(10);
    delay(1000);
    mySerial.write("AT+CMGS="); //batdau cu phap text(AT+CMGS="sdt"
    mySerial.write(34);
    mySerial.write('+');
    mySerial.write(STD);
    mySerial.write(34);
    mySerial.write(10); //dau enter
    delay(1000);
    mySerial.print(gps.location.lng(),6); //so nguyen toa do
    mySerial.write(",");
    mySerial.print(gps.location.lat(),6);
    delay(1000);
    mySerial.write(26); //ket thuc text =send
}
