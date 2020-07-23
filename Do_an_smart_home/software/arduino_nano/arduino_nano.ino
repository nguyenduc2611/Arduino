#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <TimerOne.h>
#include <EEPROM.h>
#include <Servo.h> 
#include <SPI.h>
#include <RFID.h>


#define SS_PIN 10
#define RST_PIN 7

RFID rfid(SS_PIN, RST_PIN);
unsigned char reading_card[5];
int card_pass[5] = {228, 172, 216, 42, 186};

/*mode của password*/
#define MODE_RUN 0
#define MODE_CHANGE 1
#define MODE_CANCEL 2
#define MODE_INPUT_PASS 3

#define BUZZER    8


#define CHECKING  2
#define NOT_OK    1
#define OK        0

#define ENABLE    1
#define DISABLE   0

#define ON  1
#define OFF 2
/*global variable*/
char key = 0;
char mode_pass = MODE_RUN;
char count_key = 0;
char count_key_in = 0;
char buffer_pass[5] = {0};
char new_pass1[5] = {0};
char new_pass2[5] = {0};
char count_enter_pass = 0;
/*cau hinh servo*/
#define SERVO_PIN 9
Servo gServo;


char run_CheckPass(void);

/*cấu hình LCD*/
LiquidCrystal_I2C lcd(0x27,16,2);
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000, 
  0B00000,
  0B00000
};

/*cấu hình bàn phím*/
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
char keys[rows][columns] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
  };
byte rowPins[rows] = {A1, A2, A3, A6};
byte columnPins[columns] = {6, 5, 4, A0};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);
char data[5] = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.createChar(1, degree);
  gServo.attach(SERVO_PIN);
  gServo.write(100);
  delay(500);
  gServo.detach();
  SPI.begin();
  rfid.init();


  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  
}
/*check bàn phím*/
void get_key(void)
{
  char buffer_key = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (buffer_key != 0) {
      key = buffer_key;
      Serial.println("key:");
      Serial.println(key);
    }
  }

  /* set up password*/   
  if((key == '*')&&(mode_pass == MODE_RUN))
  {
      mode_pass = MODE_CHANGE;
      Serial.println("change pass");
  }
  else
  {
    
  }

  buffer_key = 0;
}


void RFID_Reader(void)
{
  int i, j = 0;
  char correct = 0;
  if (rfid.isCard())
  {
    if (rfid.readCardSerial())
    {
      for (i = 0; i < 5; i++)
      {
        reading_card[i] = rfid.serNum[i];
        if(reading_card[i] != card_pass[i])
        {
          correct = 1;
        }
      }
      if(correct == 1)
      {
        for(int i = 0; i < 3;i++)
        {
          digitalWrite(BUZZER, HIGH);
          delay(100);
          digitalWrite(BUZZER, LOW);
          delay(100);
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access Denied");
        delay(1000);
        lcd.clear();
      }
      else
      {
        for(int i = 0; i < 2;i++)
        {
          digitalWrite(BUZZER, HIGH);
          delay(100);
          digitalWrite(BUZZER, LOW);
          delay(100);
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Access");
        delay(1000);
        lcd.clear();
        gServo.attach(SERVO_PIN);
      gServo.write(180);
      delay(500);
      gServo.detach();
      delay(3000);
      gServo.attach(SERVO_PIN);
      gServo.write(100);
      delay(500);
      gServo.detach();
      }
      
    }
    rfid.halt();
  }

  
}


/*chạy tại mode normal*/
void run_Normal(void)
{
  /*RFID*/
  RFID_Reader();
  
    char check_pass = run_CheckPass();
    if(check_pass == OK)
    {
      for(int i = 0; i < 2;i++)
        {
          digitalWrite(BUZZER, HIGH);
          delay(100);
          digitalWrite(BUZZER, LOW);
          delay(100);
        }
      Serial.println("ok");
      count_enter_pass = 0;
      gServo.attach(SERVO_PIN);
      gServo.write(180);
      delay(500);
      gServo.detach();
      delay(3000);
      gServo.attach(SERVO_PIN);
      gServo.write(100);
      delay(500);
      gServo.detach();
    }
    else if(check_pass == NOT_OK)
    {
      for(int i = 0; i < 3;i++)
        {
          digitalWrite(BUZZER, HIGH);
          delay(100);
          digitalWrite(BUZZER, LOW);
          delay(100);
        }
        
      count_enter_pass++;
      if(count_enter_pass == 3)
      {
        char flag = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Thu Lai Sau:");
        for(int i = 110; i > 0;i--)
        {
          if((i<100)&&(flag == 0))
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Thu Lai Sau:");
            flag = 1;
          }
          lcd.setCursor(4,1);
          lcd.print(i);
          delay(1000);
        }
        digitalWrite(BUZZER, HIGH);
        delay(2000);
        digitalWrite(BUZZER, LOW);
        count_enter_pass = 0;
      }
    }
    else
    {
        /*Mở của dựa cảm biến hồng ngoại*/
    }
  
}



/*chạy mode đổi password*/
void run_ChangePass(void)
{
  lcd.setCursor(0,0);
  lcd.print("Doi Mat Khau:  ");
  lcd.setCursor(0,1);
  lcd.print("Nhap:");

  char status_pass = run_CheckPass();
  if(status_pass == OK)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("xac nhan mat khau");
    mode_pass = MODE_INPUT_PASS;
  }
  else if(status_pass == NOT_OK)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("mat khau sai");
    delay(3000);
    lcd.clear();
    mode_pass = MODE_RUN;
  }
  else
  {
    /* code */
  }
  
}

/*lay du lieu pass va chec*/
char run_CheckPass(void)
{
  char key_pass[5] = {0};
  char status_pass = CHECKING;
  int i = 0;

  if((key < 58)&&(key > 47))
  {
    if(count_key == 0)
    {
    lcd.clear();
    }
    if(count_key < 4)
    {
      buffer_pass[count_key] =  key - 48;
      count_key++;
      lcd.setCursor((count_key + 6),1);
      lcd.print("*");
      key = 0;
    }

    if(count_key == 4)
    {
      delay(500);
      lcd.clear();
      count_key = 0;
      key_pass[0] = EEPROM.read(0);
      key_pass[1] = EEPROM.read(1);
      key_pass[2] = EEPROM.read(2);
      key_pass[3] = EEPROM.read(3);

      for(i = 0; i < 4; i++)
      {
        if(key_pass[i] != buffer_pass[i])
        {
          break;
        }
      }

      if(i == 4)
      {
        status_pass = OK;
      }
      else
      {
        status_pass = NOT_OK;
      }
      
    }
  }

  return status_pass;
}

/*get new password*/
void run_InputPass(void)
{
  int i = 0;

  if(count_key_in < 4)
  {
    lcd.setCursor(0,0);
    lcd.print("Nhap password 1:");
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print("Nhap password 2:");
  }
  if((key < 58)&&(key > 47))
  {
    if(count_key_in < 4)
    {
      new_pass1[count_key_in] =  key - 48;
      count_key_in++;
      lcd.setCursor((count_key_in + 6),1);
      lcd.print("*");
      if(count_key_in == 4)
      {
        lcd.clear();
      }
      key = 0;
    }
    else
    {
      new_pass2[count_key_in - 4] =  key - 48;
      count_key_in++;
      lcd.setCursor((count_key_in + 2),1);
      lcd.print("*");
      key = 0;
    }
    
    if(count_key_in == 8)
    {
      delay(500);
      lcd.clear();
      count_key_in = 0;
      for(i = 0; i < 4; i++)
      {
        if(new_pass1[i] != new_pass2[i])
        {
          break;
        }
      }
      
      if(i == 4)
      {
        EEPROM.write(0,new_pass1[0]);
        EEPROM.write(1,new_pass1[1]);
        EEPROM.write(2,new_pass1[2]);
        EEPROM.write(3,new_pass1[3]);
        lcd.setCursor(0,0);
        lcd.print("Doi thanh cong  ");
      }
      else
      {
        lcd.setCursor(0,0);
        lcd.print("  khong khop       ");
      }
      delay(2000);
      lcd.clear();
      mode_pass = MODE_RUN;
    }
  }

}
void loop() {
  get_key();
  switch(mode_pass)
  {
    case MODE_RUN:
    
    lcd.setCursor(4,0);
    lcd.print("Welcome!");
        run_Normal();
        break;
    case MODE_CHANGE:
        run_ChangePass();
        break;
    case MODE_INPUT_PASS:
        run_InputPass();
        break;
    default:
        break;
  }

  

}
