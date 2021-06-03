
#include "Arduino.h"
#include "DS18B20.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

//level

void level() {
  long LevelMax = 17;
  const int trigPin = 12;
  const int echoPin = 5;
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);

  long duration = pulseIn(echoPin, HIGH);
  long distance = microsecondsToCentimeters(duration);
  long Level = LevelMax - distance;

  Serial.println();
  Serial.print("Level: ");
  Serial.print(Level);
  Serial.print("[cm]");
  Serial.println();

  lcd.print(Level);
  lcd.print("cm");
}

long microsecondsToCentimeters(long microseconds) {
  return (microseconds / 29) / 2;
}


// temperature
#define DS18B20WP_PIN_DQ  2
DS18B20 ds18b20wp(DS18B20WP_PIN_DQ);

void temperature() {

  float temperature = ds18b20wp.readTempC();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" [C]");

  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C ");

}

// movement
#define ledGREEN 8
#define pir 3
#define ledRED 13
#define buzzer 7

void movement() {
  int val = digitalRead(pir);
  if (val == LOW) {
    Serial.println("No motion");
    digitalWrite(ledGREEN, LOW);
    digitalWrite(ledRED, HIGH);

    tone(buzzer, 1000);

    delay(10);
    digitalWrite(ledGREEN, LOW);
    digitalWrite(ledRED, LOW);
    noTone(buzzer);

  } else {
    Serial.println("Motion detected");
    digitalWrite(ledRED, LOW);
    digitalWrite(ledGREEN, HIGH);
    noTone(buzzer);


    delay(100);
    digitalWrite(ledGREEN, LOW);
    digitalWrite(ledRED, LOW);
  }
}

// food
#define SERVOPIN 10
Servo servo;
int angle = 10;

void feed() {
  Serial.println("Feeding");

  for (angle = 10; angle < 180; angle++)
  {

    servo.write(angle);
  }
  for (angle = 180; angle > 10; angle--)
  {

    servo.write(angle);
  }

}

//turbidity
void turbidity() {
  int sensorValue = analogRead(A0);
  float volt = sensorValue * (5.0 / 1024.0);
  float turbidity = -1120.4 * square(volt) + 5742.3 * volt - 4352.9;
  Serial.print("Turbidity ");
  Serial.print(turbidity);
  Serial.print(" [NTU]");
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print(turbidity);
  lcd.print(" NTU");
}

void setup() {
  pinMode(ledGREEN, OUTPUT);
  pinMode(ledRED, OUTPUT);
  pinMode(pir, INPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(ledGREEN, LOW);
  digitalWrite(ledRED, LOW);
  //led
  pinMode(A0, INPUT);
  lcd.begin();
  lcd.backlight();
  lcd.home();

  //servomor
  servo.attach(SERVOPIN);
  servo.write(angle);

  Serial.begin(9600);
}




void loop() {
  lcd.clear();
  movement();
  turbidity();
  temperature();
  feed();
  level();
  delay(4000);
}
