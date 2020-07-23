#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Stepper.h>

const int stepsPerRevolution = 4096;
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

int loop_count = 0;
LiquidCrystal_I2C lcd(0x20,16,2);
char key = 0;
char i = 0;
const byte rows = 4; //số hàng
const byte columns = 4; //số cột
char keys[rows][columns] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'},
  };
byte rowPins[rows] = {9, 8, 7, 6};
byte columnPins[columns] = {10, 11, 12};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rows, columns);

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(10000); 
  Timer1.attachInterrupt(get_key);
  lcd.begin();
  myStepper.setSpeed(60);
}

void get_key(void)
{
  char buffer_key = keypad.getKey();
  if ((int)keypad.getState() ==  PRESSED) {
    if (buffer_key != 0) {
      key = buffer_key;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if((key < 58)&&(key > 47))
  {
    lcd.setCursor(i,0);
    lcd.print(key);
    i++;
    loop_count = key - 48;
    key = 0;
  }

  if(key == '*')
  {
    lcd.clear();
    i = 0;
    key = 0;
  }
  if(key == '#')
  {
    key = 0;
  if(loop_count != 0)
  {
    for(int i = 0; i < loop_count; i++)
    {
      myStepper.step(stepsPerRevolution);
      delay(500);
    }

    loop_count = 0;
  }
  }
}
