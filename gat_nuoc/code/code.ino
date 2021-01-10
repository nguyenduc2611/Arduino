#define BUTTON_1 3
#define BUTTON_2 4

#define MOTOR_1 11
#define MOTOR_2 12

#define FAST_SLOW 5
char speed_mode = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(FAST_SLOW, INPUT);
  
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);

  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
    delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
 int a = analogRead(A1);
Serial.println(a);
 char button_1 = digitalRead(BUTTON_1);
 char button_2 = digitalRead(BUTTON_2);

 char fast_slow = digitalRead(FAST_SLOW);

 if(fast_slow == HIGH)
 {
  speed_mode = 1;
 }
 else
 {
  speed_mode = 0;
 }
 
 if(a < 512)
 {
  motor_quay(speed_mode);
 }
 else
 {
  digitalWrite(MOTOR_1, LOW);
 }

if(button_2 == HIGH) 
{
  digitalWrite(MOTOR_2, HIGH);
  motor_quay(speed_mode);
}
else if(a > 512)
{
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_1, LOW);
}

if((button_1 == HIGH)||(button_2 == HIGH))
{
  motor_quay(speed_mode);
}
else if(a > 512)
{
  digitalWrite(MOTOR_1, LOW);
}
else
{
  
}
}

void motor_quay(char speed_motor)
{
  if(speed_motor)
  {
    digitalWrite(MOTOR_1, HIGH);
  }
  else
  {
    digitalWrite(MOTOR_1, LOW);
    delay(100);
    digitalWrite(MOTOR_1, HIGH);
    delay(10);
  }
}
