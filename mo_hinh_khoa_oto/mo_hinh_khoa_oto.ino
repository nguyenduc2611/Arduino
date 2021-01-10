#define D0_IN 5
#define D1_IN 6 
#define RUNG 12
#define LOA 7

#define RELAY 9
char unlock = 0;
char mode = 0;
char status_mode = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(D0_IN, INPUT);
  pinMode(D1_IN, INPUT);
  pinMode(RUNG, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LOA, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  char lock = digitalRead(D0_IN);
  char protect = digitalRead(D1_IN);
  delay(50);
  char sw = digitalRead(RUNG);
  
  if(lock == HIGH)
  {
    /*Mở Khóa*/
    unlock = 1;
    digitalWrite(RELAY, HIGH);
  }
  else
  {
    unlock = 0;
    digitalWrite(RELAY, LOW);
  }



  if((protect == HIGH)&&(!unlock))
  {
    if(!status_mode)
    {
      for(int i = 0;i<5;i++)
      {
        digitalWrite(LOA, HIGH);
        delay(200);
        digitalWrite(LOA, LOW);
        delay(200);
      }
      status_mode = 1;
    }
    mode = 1;
  }
  else
  {
    mode = 0;
    status_mode = 0;
  }




  if(mode)
  {
    if(sw == HIGH)
    {
      digitalWrite(LOA, HIGH);
      for(int i = 0; i < 500;i++)
      {
        delay(10);
        protect = digitalRead(D1_IN);
        if(protect == LOW)
        {
          mode = 0;
          status_mode = 0;
          break;
        }
      }

      digitalWrite(LOA, LOW);
      mode = 0;
      status_mode = 0;
    }
  }
  else
  {
    digitalWrite(LOA, LOW);
  }
}
