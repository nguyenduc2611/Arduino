#include <LiquidCrystal_I2C.h>

#define LED1 3
#define LED2 8
#define LED3 9
#define LED4 10
#define LED5 11
#define LED6 12
#define LED7 A1
#define LED8 A2

#define BUTTON1 4
#define BUTTON2 5
#define BUTTON3 6
#define BUTTON4 7

LiquidCrystal_I2C lcd(0x20,16,2);

char pin_inter = 2;
int i = 0;
char print_lcd = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t = 0;
int v = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(pin_inter, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_inter), call_inter, FALLING);
  lcd.begin();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(BUTTON4, INPUT);
  Serial.begin(9600);
}

void call_inter(void)
{
    if(i == 29)
    {
      print_lcd = 1;
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
  if(print_lcd)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    t2 = millis();
    t = t2 - t1;
    v = 60000/t;
    lcd.print(v);
    Serial.println(v);
    i = 0;
    print_lcd = 0;
  }
  if(HIGH == digitalRead(BUTTON1))
  {
    digitalWrite(LED1, HIGH);
  }
  else
  {
    digitalWrite(LED1, LOW);
  }

  if(HIGH == digitalRead(BUTTON2))
  {
    digitalWrite(LED2, HIGH);
  }
  else
  {
    digitalWrite(LED2, LOW);
  }

  if(HIGH == digitalRead(BUTTON3))
  {
    digitalWrite(LED3, HIGH);
  }
  else
  {
    digitalWrite(LED3, LOW);
  }

  if(HIGH == digitalRead(BUTTON4))
  {
    if((v>0)&&(v<20))
    {
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      analogWrite(LED7, 0);
      analogWrite(LED8, 0);
    }
    else if((v>20)&&(v<40))
    {
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, HIGH);
      digitalWrite(LED6, LOW);
      analogWrite(LED7, 0);
      analogWrite(LED8, 0);
    }
    else if((v>40)&&(v<80))
    {
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, HIGH);
      analogWrite(LED7, 0);
      analogWrite(LED8, 0);
    }
    else if((v>80)&&(v<200))
    {
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      analogWrite(LED7, 1024);
      analogWrite(LED8, 0);
    }
    else if(v>200)
    {
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      analogWrite(LED7, 0);
      analogWrite(LED8, 1024);
    }
    else
    {
      
    }
  }
  else
  {
      digitalWrite(LED4, LOW);
      digitalWrite(LED5, LOW);
      digitalWrite(LED6, LOW);
      analogWrite(LED7, 0);
      analogWrite(LED8, 0);
  }
}
