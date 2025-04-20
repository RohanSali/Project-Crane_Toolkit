#define Led_red 2
#define Led_blue 3
#define Led_white 4

#define IR_pin 53

#define MM1_ENA 22
#define MM1_IN1 24
#define MM1_IN2 26
#define MM1_IN3 28
#define MM1_IN4 30
#define MM1_ENB 32

#define MM2_ENA 23
#define MM2_IN1 25
#define MM2_IN2 27
#define MM2_IN3 29
#define MM2_IN4 31
#define MM2_ENB 33

//Bluetooth is connected to rx,tx -> 18,19 .. i.e Serial1 of Mega.

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(IR_pin, INPUT);

  pinMode(Led_red,OUTPUT);
  pinMode(Led_blue,OUTPUT);
  pinMode(Led_white,OUTPUT);

  pinMode(MM1_ENA,OUTPUT);
  pinMode(MM1_IN1,OUTPUT);
  pinMode(MM1_IN2,OUTPUT);
  pinMode(MM1_IN3,OUTPUT);
  pinMode(MM1_IN4,OUTPUT);
  pinMode(MM1_ENB,OUTPUT);

  pinMode(MM2_ENA,OUTPUT);
  pinMode(MM2_IN1,OUTPUT);
  pinMode(MM2_IN2,OUTPUT);
  pinMode(MM2_IN3,OUTPUT);
  pinMode(MM2_IN4,OUTPUT);
  pinMode(MM2_ENB,OUTPUT);

}

void left()
{
  Serial1.println("In Up Function...");
  digitalWrite(MM1_IN1,HIGH);
  digitalWrite(MM1_IN2,LOW);
  analogWrite(MM1_ENA,225);
  delay(1000);
  stop();
}

void right()
{
  Serial1.println("In Down Function...");
  digitalWrite(MM1_IN1,LOW);
  digitalWrite(MM1_IN2,HIGH);
  analogWrite(MM1_ENA,225);
  delay(1000);
  stop();
}

void up()
{
  Serial1.println("In Right Function...");
  digitalWrite(MM1_IN3,HIGH);
  digitalWrite(MM1_IN4,LOW);
  analogWrite(MM1_ENB,225);
  delay(1000);
  stop();
}

void down()
{
  Serial1.println("In Left Function...");
  digitalWrite(MM1_IN3,LOW);
  digitalWrite(MM1_IN4,HIGH);
  analogWrite(MM1_ENB,225);
  delay(1000);
  stop();
}

void clockwise()
{
  Serial1.println("In AntiClockwise...");
  digitalWrite(MM2_IN1,LOW);
  digitalWrite(MM2_IN2,HIGH);
  digitalWrite(MM2_IN3,LOW);
  digitalWrite(MM2_IN4,HIGH);
  analogWrite(MM2_ENA,255);
  analogWrite(MM2_ENB,255);
  delay(1000);
  stop();
}

void anticlockwise()
{
  Serial1.println("In Clockwise...");
  digitalWrite(MM2_IN1,HIGH);
  digitalWrite(MM2_IN2,LOW);
  digitalWrite(MM2_IN3,HIGH);
  digitalWrite(MM2_IN4,LOW);
  analogWrite(MM2_ENA,255);
  analogWrite(MM2_ENB,255);
  delay(1000);
  stop();
}

void stop()
{
  Serial1.println("Stoping...");
  digitalWrite(MM1_IN1,HIGH);
  digitalWrite(MM1_IN2,LOW);
  analogWrite(MM1_ENA,0);
  digitalWrite(MM1_IN3,HIGH);
  digitalWrite(MM1_IN4,LOW);
  analogWrite(MM1_ENB,0);

  digitalWrite(MM2_IN1,HIGH);
  digitalWrite(MM2_IN2,LOW);
  analogWrite(MM2_ENA,0);
  digitalWrite(MM2_IN3,HIGH);
  digitalWrite(MM2_IN4,LOW);
  analogWrite(MM2_ENB,0);
}

void loop() {
  if(Serial1.available()>0)
  {
    Serial1.println("Enter your command : ");
    char command=Serial1.read();
    Serial.println(command);
    

    if(command=='1')
    {
      digitalWrite(Led_red,HIGH);
      digitalWrite(Led_blue,LOW);
      digitalWrite(Led_white,LOW);
      delay(2000);
      //Running Motor 1 for left motion
      up();
      digitalWrite(Led_red,LOW);
    }
    else if(command=='2')
    {
      digitalWrite(Led_red,LOW);
      digitalWrite(Led_blue,HIGH);
      digitalWrite(Led_white,LOW);
      delay(2000);
      //Running Motor 1 for left motion
      down();
      digitalWrite(Led_blue,LOW);
    }
    else if(command=='3')
    {
      digitalWrite(Led_red,LOW);
      digitalWrite(Led_blue,LOW);
      digitalWrite(Led_white,HIGH);
      delay(2000);
      //Running Motor 2 for up motion
      left();
      digitalWrite(Led_white,LOW);
    }
    else if(command=='4')
    {
      digitalWrite(Led_red,HIGH);
      digitalWrite(Led_blue,HIGH);
      digitalWrite(Led_white,HIGH);
      delay(2000);
      //Running Motor 2 for up motion
      right();
    }
    else if(command=='5')
    {
      digitalWrite(Led_red,HIGH);
      digitalWrite(Led_blue,HIGH);
      digitalWrite(Led_white,LOW);
      delay(2000);
      //Running Motor 2 for up motion
      clockwise();
    }
    else if(command=='6')
    {
      digitalWrite(Led_red,LOW);
      digitalWrite(Led_blue,HIGH);
      digitalWrite(Led_white,HIGH);
      delay(2000);
      //Running Motor 2 for up motion
      anticlockwise();
    }
    else if(command=='0')
    {
      digitalWrite(Led_red,LOW);
      digitalWrite(Led_blue,LOW);
      digitalWrite(Led_white,LOW);
      delay(2000);
      stop();
    }
    else
    {
      Serial.println("Invlaid Input !!!");
    }
  }
  delay(100);
}