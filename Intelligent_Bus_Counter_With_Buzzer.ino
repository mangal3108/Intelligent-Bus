#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if needed

// Pins
const int sensorIn = 2;    // IR Entry sensor
const int sensorOut = 3;   // IR Exit sensor
const int buzzer = 4;      // Buzzer pin

// Variables
int count = 0;
int maxCapacity = 30;

int inState = HIGH;
int outState = HIGH;

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Passenger Count");

  pinMode(sensorIn, INPUT);
  pinMode(sensorOut, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  delay(2000);
  lcd.clear();
  updateLCD();
}

void loop() {
  int currentIn = digitalRead(sensorIn);
  int currentOut = digitalRead(sensorOut);

  // Detect Entry
  if (inState == HIGH && currentIn == LOW) {
    count++;
    updateLCD();
    delay(500);  // Debounce delay
  }
  inState = currentIn;

  // Detect Exit
  if (outState == HIGH && currentOut == LOW) {
    if (count > 0) count--;
    updateLCD();
    delay(500);
  }
  outState = currentOut;

  // Buzzer Alert if Overcapacity
  if (count > maxCapacity) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("On Board: ");
  lcd.print(count);
  
  if (count > maxCapacity) {
    lcd.setCursor(0, 1);
    lcd.print("** OVER LIMIT **");
  }
}
