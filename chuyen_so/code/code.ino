#include <LiquidCrystal_I2C.h>

#define BUTTON1 5
#define BUTTON2 6

LiquidCrystal_I2C lcd(0x27,16,2);

char pin_inter = 2;
int i = 0;
int so = 1;
char print_lcd = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t = 0;
int vsum = 0;
char var = 0;
int v = 0;
int next_so = 0;
int war = 0;
char motor = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(pin_inter, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin_inter), call_inter, FALLING);
  lcd.begin();

  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  lcd.print("Toc Do:");
}

void call_inter(void)
{
    if(i == 29)
    {
      print_lcd = 1;
      t2 = millis();
      t = t2 - t1;
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
    v = 3000/t;
    vsum = vsum + v;
    if(var == 4)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Toc Do:");
      lcd.print(vsum/5);
      lcd.setCursor(0,1);
      lcd.print("So:");
      lcd.print(so);
      lcd.setCursor(8,1);
      lcd.print("war:");
      lcd.print(war);
      vsum = 0;
      var = 0;
    }
    i = 0;
    print_lcd = 0;
    var++;
  }

  if(HIGH == digitalRead(BUTTON1))
  {
    next_so = chuyen_so(v, so, 1);
    so = next_so;
    war = 80;
    
    delay(1000);
  }

  if(HIGH == digitalRead(BUTTON2))
  {
    next_so = chuyen_so(v, so, 2);
    so = next_so;
    war = 90;
    delay(1000);
  }
  
}

int chuyen_so(int van_toc, int current_so, char alpha)
{
  int new_so = 0;
  switch(current_so)
  {
    case 1:
      if(alpha == 1)
      {
        if(van_toc > 32)
        {
          new_so = 2;
        }
      }
      else
      {
        if(van_toc > 38)
        {
          new_so = 2;
        }
      }
    break;
    case 2:
      if(alpha == 1)
      {
        if(van_toc > 72)
        {
          new_so = 3;
        }
        else if(van_toc < 22)
        {
          new_so = 1;
        }
        else
        {
        
        }
      }
      else
      {
        if(van_toc > 77)
        {
          new_so = 3;
        }
        else if(van_toc < 28)
        {
          new_so = 1;
        }
        else
        {
        
        }
      }
      
    break;
    case 3:
      if(alpha == 1)
      {
        if(van_toc > 105)
        {
          new_so = 4;
        }
        else if(van_toc < 58)
        {
          new_so = 2;
        }
        else
        {
        
        }
      }
      else
      {
        if(van_toc > 107)
        {
          new_so = 4;
        }
        else if(van_toc < 63)
        {
          new_so = 2;
        }
        else
        {
        
        }
      }
    break;
    case 4:
      if(alpha == 1)
      {
        if(van_toc < 90)
        {
          new_so = 3;
        }
      }
      else
      {
        if(van_toc < 95)
        {
          new_so = 3;
        }
      }
    break;
    default:
    break;
  }

  if(new_so == 0)
  {
    new_so = current_so;
  }
  return new_so;                                                                                                           ;
}
