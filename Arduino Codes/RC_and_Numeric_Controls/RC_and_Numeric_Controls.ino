//leds are used for showing conditions and alerts.
#define Led_red 2
#define Led_blue 3
#define Led_white 4

//Ir sensor is used for accident prevention.
#define IR_pin 8

//Motors for up-down and left-right motion are connected to MM1.
#define MM1_ENA 22
#define MM1_IN1 24
#define MM1_IN2 26
#define MM1_IN3 28
#define MM1_IN4 30
#define MM1_ENB 32

//Motors for clockwise and anti-clockwise motion are connected to MM2.
#define MM2_ENA 23
#define MM2_IN1 25
#define MM2_IN2 27
#define MM2_IN3 29
#define MM2_IN4 31
#define MM2_ENB 33

//Bluetooth is connected to rx,tx -> 18,19 .. i.e Serial1 of Mega.

//Setting EEPROM to set values to non-volatile memory.
#include <EEPROM.h>

//positions for motors.
long mm1_posA = 0;
long mm1_posB = 0;
long mm2_pos = 0;

long last_mm1_posA = 0;
long last_mm1_posB = 0;
long last_mm2_pos = 0;

//address map for eeprom.
#define EEPROM_MM1_POS_A_ADDR 0
#define EEPROM_MM1_POS_B_ADDR sizeof(long)
#define EEPROM_MM2_POS_ADDR (EEPROM_MM1_POS_B_ADDR + sizeof(long))

#define EEPROM_MAGIC_ADDR 100
#define EEPROM_MAGIC_VAL 123

byte checkVal;

//HX711 is included to set up load cell.
#include "HX711.h"

#define DOUT 11
#define CLK 12

// HX711 scale(DOUT, CLK);
HX711 scale;

int a=0;
int b=0;
int c=0;
int d=0;
int e=0;
int f=0;

void setup() {
  //Serial and Bluetooth Communcation.
  Serial.begin(9600);
  Serial1.begin(9600);

  //IR Sensor.
  pinMode(IR_pin, INPUT);

  //LEDs.
  pinMode(Led_red,OUTPUT);
  pinMode(Led_blue,OUTPUT);
  pinMode(Led_white,OUTPUT);

  //Motor Module 1
  pinMode(MM1_ENA,OUTPUT);
  pinMode(MM1_IN1,OUTPUT);
  pinMode(MM1_IN2,OUTPUT);
  pinMode(MM1_IN3,OUTPUT);
  pinMode(MM1_IN4,OUTPUT);
  pinMode(MM1_ENB,OUTPUT);

  //Motor Module 2
  pinMode(MM2_ENA,OUTPUT);
  pinMode(MM2_IN1,OUTPUT);
  pinMode(MM2_IN2,OUTPUT);
  pinMode(MM2_IN3,OUTPUT);
  pinMode(MM2_IN4,OUTPUT);
  pinMode(MM2_ENB,OUTPUT);

  //EEPROM.
  EEPROM.get(EEPROM_MAGIC_ADDR, checkVal);
  if (checkVal != EEPROM_MAGIC_VAL) {
    mm1_posA = 0;
    mm1_posB = 0;
    mm2_pos = 0;
    EEPROM.put(EEPROM_MM1_POS_A_ADDR, mm1_posA);
    EEPROM.put(EEPROM_MM1_POS_B_ADDR, mm1_posB);
    EEPROM.put(EEPROM_MM2_POS_ADDR, mm2_pos);

    EEPROM.put(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VAL);
    Serial.println("First boot — position initialized.");
  }
  else
  {
    EEPROM.get(EEPROM_MM1_POS_A_ADDR, mm1_posA);
    EEPROM.get(EEPROM_MM1_POS_B_ADDR, mm1_posB);
    EEPROM.get(EEPROM_MM2_POS_ADDR, mm2_pos);

    Serial.println("Positions loaded from EEPROM.");
  }
  last_mm1_posA = mm1_posA;
  last_mm1_posB = mm1_posB;
  last_mm2_pos = mm2_pos;

  Serial.print("Horizontal Position : ");
  Serial.println(mm1_posA);
  Serial.print("Vertical Position : ");
  Serial.println(mm1_posB);
  Serial.print("Angular Position : ");
  Serial.println(mm2_pos);

  //Load Cell.
  scale.begin(DOUT, CLK);
  
  // Tare the scale (zero it)
  scale.set_scale();
  scale.tare();
  
  // Set the scale calibration factor (this will depend on your load cell)

  scale.set_scale(600.f);  // You will need to calibrate this value based on your load cell

  long initial_reading = scale.get_units(10);
  Serial.print("Load Cell Reading : ");
  Serial.println(initial_reading);
  
  
  Serial1.print("Horizontal Position : ");
  Serial1.println(mm1_posA);
  Serial1.print("Vertical Position : ");
  Serial1.println(mm1_posB);
  Serial1.print("Angular Position : ");
  Serial1.println(mm2_pos);
  Serial1.print("Load Cell Reading : ");
  Serial1.println(initial_reading);
}

void left()
{
  if (mm1_posA>0)
  {
    digitalWrite(Led_blue,HIGH);
    digitalWrite(MM1_IN1,HIGH);
    digitalWrite(MM1_IN2,LOW);
    analogWrite(MM1_ENA,225);
    savePositionIfChanged();
  }
  else
  {
    digitalWrite(Led_red,HIGH);
    delay(500);
    digitalWrite(Led_red,LOW);
  }
}

void right()
{
  if (mm1_posA<10)
  {
    digitalWrite(Led_blue,HIGH);
    digitalWrite(MM1_IN1,LOW);
    digitalWrite(MM1_IN2,HIGH);
    analogWrite(MM1_ENA,225);
    savePositionIfChanged();
  }
  else
  {
    digitalWrite(Led_red,HIGH);
    delay(500);
    digitalWrite(Led_red,LOW);
  }
}

void up()
{
  if (mm1_posB>0)
  {
    digitalWrite(Led_blue,HIGH);
    digitalWrite(MM1_IN3,HIGH);
    digitalWrite(MM1_IN4,LOW);
    analogWrite(MM1_ENB,225);
    savePositionIfChanged();
  }
  else
  {
    digitalWrite(Led_red,HIGH);
    delay(500);
    digitalWrite(Led_red,LOW);
  }
}

void down()
{
  if (mm1_posB<35)
  {
    digitalWrite(Led_blue,HIGH);
    digitalWrite(MM1_IN3,LOW);
    digitalWrite(MM1_IN4,HIGH);
    analogWrite(MM1_ENB,225);
    savePositionIfChanged();
  }
  else
  {
    digitalWrite(Led_red,HIGH);
    delay(500);
    digitalWrite(Led_red,LOW);
  }
}

void clockwise()
{
  if (mm2_pos<12)
  {
    digitalWrite(Led_white,HIGH);
    digitalWrite(MM2_IN1,LOW);
    digitalWrite(MM2_IN2,HIGH);
    digitalWrite(MM2_IN3,LOW);
    digitalWrite(MM2_IN4,HIGH);
    analogWrite(MM2_ENA,255);
    analogWrite(MM2_ENB,255);
    savePositionIfChanged();
  }
  else
  {
    digitalWrite(Led_red,HIGH);
    delay(500);
    digitalWrite(Led_red,LOW);
  }
}

void anticlockwise()
{
  if (mm2_pos>-12)
  {
    digitalWrite(Led_white,HIGH);
    digitalWrite(MM2_IN1,HIGH);
    digitalWrite(MM2_IN2,LOW);
    digitalWrite(MM2_IN3,HIGH);
    digitalWrite(MM2_IN4,LOW);
    analogWrite(MM2_ENA,255);
    analogWrite(MM2_ENB,255);
    savePositionIfChanged();
  }
  else
  {
    digitalWrite(Led_red,HIGH);
    delay(500);
    digitalWrite(Led_red,LOW);
  }
}

void stop()
{
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

  digitalWrite(Led_red,LOW);
  digitalWrite(Led_blue,LOW);
  digitalWrite(Led_white,LOW);
}


void savePositionIfChanged() {
  Serial.println("");
  if (mm1_posA != last_mm1_posA) {
    EEPROM.put(EEPROM_MM1_POS_A_ADDR, mm1_posA);
    last_mm1_posA = mm1_posA;
    Serial.print("mm1_posA updated: "); Serial.println(mm1_posA);
  }

  if (mm1_posB != last_mm1_posB) {
    EEPROM.put(EEPROM_MM1_POS_B_ADDR, mm1_posB);
    last_mm1_posB = mm1_posB;
    Serial.print("mm1_posB updated: "); Serial.println(mm1_posB);
  }

  if (mm2_pos != last_mm2_pos) {
    EEPROM.put(EEPROM_MM2_POS_ADDR, mm2_pos);
    last_mm2_pos = mm2_pos;
    Serial.print("mm2_pos updated: "); Serial.println(mm2_pos);
  }

  Serial.print("a = ");Serial.println(a);
  Serial.print("b = ");Serial.println(b);
  if(a==499) mm1_posA--;
  if(b==499) mm1_posA++;
  if(c==499) mm1_posB--;
  if(d==499) mm1_posB++;
  if(e==2199) mm2_pos++;
  if(f==2199) mm2_pos--;
}


void loop() {
  if(Serial1.available()>0)
  {
    char command=Serial1.read();
    Serial.print("Command Received : ");
    Serial.println(command);
    
    long reading = scale.get_units(10);
    long r=reading;
    if(reading<50)
    {
      reading=0;
    }
    else if(reading<100)
    {
      reading+=70;
    }
    else if(reading<150)
    {
      reading+=50;
    }
    else if(reading<200)
    {
      reading+=20;
    }
    else if(reading>300)
    {
      reading-=50;
    }
    else if(reading>400)
    {
      reading-=100;
    }
    else if(reading>500)
    {
      reading-=150;
    }


    switch(command)
    {
      case 'l':
        left();
        a=(a+1)%500;
        break;
      case 'r':
        right();
        b=(b+1)%500;
        break;
      case 'u':
        up();
        c=(c+1)%500;
        break;
      case 'd':
        down();
        d=(d+1)%500;
        break;
      case 'c':
        clockwise();
        e=(e+1)%2200;
        break;
      case 'a':
        anticlockwise();
        f=(f+1)%2200;
        break;
      case 's':
        stop();
        break;
      case '1':
        Serial.print("Reading: ");
        Serial.println(reading);
        Serial1.print("Reading: ");
        Serial1.println(reading);
        break;
      default:
        //Serial.println("Invalid Input !!!");
        break;
    }
  }


  if(Serial.available())
  {
    char val=Serial.read();

    long reading = scale.get_units(10);
    long r=reading;
    if(reading<50)
    {
      reading=0;
    }
    else if(reading<100)
    {
      reading+=70;
    }
    else if(reading<150)
    {
      reading+=50;
    }
    else if(reading<200)
    {
      reading+=20;
    }
    else if(reading>300)
    {
      reading-=50;
    }
    else if(reading>400)
    {
      reading-=100;
    }
    else if(reading>500)
    {
      reading-=150;
    }

    if(val=='1')
    {
      Serial.print("Reading: ");
      Serial.print(reading);
      Serial.print(" | ");
      Serial.println(r);
    }
  }
}