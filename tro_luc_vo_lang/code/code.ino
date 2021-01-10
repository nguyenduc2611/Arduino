#include <TimerOne.h>
#include <Servo.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define BUTTON1 3
#define BUTTON2 4
#define IN1 6
#define IN2 5

#define IN3 8
#define IN4 7
LiquidCrystal_I2C lcd(0x20,16,2);
unsigned long t = 0;
unsigned long t_delay = 0;
char i = 0;
char print_xung = 0;
int pin_inter = 2;
unsigned long t1 = 0;
unsigned long t2 = 0;
int h = 0;
int v = 0;
volatile int k = 0;
char right = 0;
char left = 0;

int pre_var = 512;
int pre_com_var = 512;

void setup() {
  // put your setup code here, to run once:
    pinMode(pin_inter, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_inter), get_key, FALLING);

    pinMode(BUTTON1, INPUT);
    pinMode(BUTTON2, INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,HIGH);
    Serial.begin(9600);
    lcd.begin();
    lcd.setCursor(0,0);
    lcd.print("hello");
}


void get_key(void)
{
    
    if(i == 29)
    {
      print_xung = 1;
    }
    else if(i == 0)
    {
      t1 = millis();
    }
    else
    {
      
    }
    i++;

}

void loop() {
  // put your main code here, to run repeatedly:

  int var = analogRead(A1);
  if(print_xung == 1)
  {
    t2 = millis();
    t = t2 - t1;
    t = 60000/t;
    v = t;
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print(var);
    t = 0;
    i = 0;
    print_xung = 0;
  }

if(((pre_var + 20)<var)&&((right)&&(!left)||((!right)&&(!left))))
{
  left = 0;
  right = 1;
  pre_var = pre_var + 20;
  if((pre_var + 20)>var)
  {
    pre_com_var = pre_var;
  }
  quay_motor(1, v);
}
else if(((pre_com_var + 20)>var)&&((right)&&(!left)))
{
  pre_var = var;
  left = 1;
  right = 0;
}
else
{
  
}

if(((var + 20)<pre_var)&&((!right)&&(left)||((!right)&&(!left))))
{
  left = 1;
  right = 0;
  pre_var = pre_var - 20;
  if((var + 20)>pre_var)
  {
    pre_com_var = pre_var;
  }
  quay_motor(0, v);
}
else if(((var + 20)>pre_com_var)&&((!right)&&(left)))
{
  pre_var = var;
  left = 0;
  right = 1;
}
else
{
 
}

}

void quay_motor(char dir, int v_quay)
{
  if(dir)
  {
    digitalWrite(IN2,HIGH);
    if(v_quay > 100)
    {
      for(int i = 0; i<2;i++)
      {
        digitalWrite(IN1,LOW);
        delay(10);
        digitalWrite(IN1,HIGH);
        delay(100);
      }
    }
    else
    {
      for(int i = 0; i<2;i++)
      {
        digitalWrite(IN1,HIGH);
        delay(10);
        digitalWrite(IN1,LOW);
        delay(100);
      }
    }
  }
  else
  {
    digitalWrite(IN2,LOW);
    if(v_quay > 100)
    {
      for(int i = 0; i<2;i++)
      {
        digitalWrite(IN1,HIGH);
        delay(10);
        digitalWrite(IN1,LOW);
        delay(100);
      }
    }
    else
    {
      for(int i = 0; i<2;i++)
      {
        digitalWrite(IN1,LOW);
        delay(10);
        digitalWrite(IN1,HIGH);
        delay(100);
      }
    }
  }

  digitalWrite(IN2,HIGH);
  digitalWrite(IN1,HIGH);
}
