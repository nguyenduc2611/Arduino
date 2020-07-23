#define Q1 10
#define Q2 8
#define Q3 7
#define Q4 9 
unsigned long t = 0;
char i = 0;
int pin_inter = 2;
unsigned long t1 = 0;
unsigned long t2 = 0;
char motor = 0;
int t_de = 0;

void setup() {
	/*ngắt ngoài để đo xung*/
    pinMode(pin_inter, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin_inter), get_encoder, FALLING);
/*chân led*/
    pinMode(Q1, OUTPUT);
    pinMode(Q2, OUTPUT);
    pinMode(Q3, OUTPUT);
    pinMode(Q4, OUTPUT);
}
/*đo thời gian quay hết 1 vòng*/
void get_encoder(void)
{
    
    if(i == 23)
    {
      t2 = millis();
      /*tính thời gian đo 1 vòng encoder*/
      t_de = t2 - t1;
      motor = 1;
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
	/* nháy led theo độ delay*/
  if((t_de != 0)&&(motor == 1))
  {
    quet_led(t_de*2);
    motor = 0;
  }
    
  
}

void quet_led(int time_delay)
{
	/*led 1 sáng và tắt*/
    digitalWrite(Q1, HIGH);
    delay(time_delay);
    digitalWrite(Q1, LOW );

/*led 2 sáng và tắt*/
    digitalWrite(Q2, HIGH);
    delay(time_delay);
    digitalWrite(Q2, LOW );

/*led 3 sáng và tắt*/
     digitalWrite(Q3, HIGH);
     delay(time_delay);
    digitalWrite(Q3, LOW );

/*led 4 sáng và tắt*/
    digitalWrite(Q4, HIGH);
    delay(time_delay);
    digitalWrite(Q4, LOW );
}
