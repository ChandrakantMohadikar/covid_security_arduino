#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>
//programmed By Mr. Sumit Sajan 
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define echoPin 7 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 6 //attach pin D3 Arduino to pin Trig of HC-SR04
#define LED 8
#define BUZ 10
// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

int IRSensor = 9;
#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Time at which the last beat occurred
uint32_t tsLastReport = 0;
  int j = 0;
  int k=0;
// Callback routine is executed when a pulse is detected
void onBeatDetected() {

  Serial.println("Beat!");
  if(k<1){
  lcd.setCursor(0, 0);
  lcd.print("                  ");
  lcd.setCursor(0, 1);
  lcd.print("                  ");
  k++;
  }
  if(j<8){
  if(k<2){
  lcd.setCursor(0, 0);
  k++;
  }
    lcd.print("B!");
  j++;
  }
  if(j>=8){
  if(k<3){
    lcd.setCursor(0, 1);
    k++;
  }
  lcd.print("B!");
  j++;
  }
}
int i = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);


  lcd.setCursor(0, 0);
  lcd.print("----WELCOME---- ");
  delay(5000);
  lcd.setCursor(0, 0);
  lcd.print("--PRIYADARSHINI--");
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("--POLY. NAGPUR--");
  delay(5000);
  lcd.setCursor(0, 0);
  lcd.print("  SMART HEALTH  ");
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print("MONITORINGSYSTEM");
  delay(5000);
  lcd.setCursor(0, 0);
  lcd.print("                  ");
  lcd.setCursor(0, 1);
  lcd.print("                  ");
  // Print a message to the LCD.
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  // Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  // Serial.println("with Arduino UNO R3");

  pinMode (IRSensor, INPUT);
  pinMode(BUZ, OUTPUT);
  Serial.print("Initializing pulse oximeter..");

  // Initialize sensor
  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");
  }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback routine
  pox.setOnBeatDetectedCallback(onBeatDetected);
  Serial.begin(9600);
  while (!Serial);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  };
}

void loop() {

  // Clears the trigPin condition
while(distance==0){
  digitalWrite(trigPin, LOW);
  delay(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print("distance:                ");
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(distance);
  lcd.print("cm");
  lcd.print("                       ");  
  
if(distance>120){
      pinMode(LED, OUTPUT);
      digitalWrite(LED, LOW);
}

  // Read from the sensor
  if (distance < 120)
  {
   pox.update();
  while (i < 1) {
      pox.update();

      // Grab the updated heart rate and SpO2 levels
      if (pox.getHeartRate() > 60 & pox.getHeartRate()<100 & pox.getSpO2()>0 ) {
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
          Serial.print("Heart rate:");
          Serial.print(pox.getHeartRate());
          Serial.print("bpm / SpO2:");
          Serial.print(pox.getSpO2());
          Serial.println("%");
          i++;
         tsLastReport = millis();
        }
    digitalWrite(BUZ, HIGH);
    delay(300);
    digitalWrite(BUZ, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Heart:");
    lcd.print(pox.getHeartRate());
    lcd.print("            ");
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print("SpO2:");
    lcd.print(pox.getSpO2());
    lcd.print("%");
    lcd.print("               ");
    delay(3000);
      }
  }
        

      lcd.setCursor(0, 0);
      lcd.print("Put  Hand  Below ");
      lcd.setCursor(0, 1);
      lcd.print("  Temp. Sensor   ");
      delay(3000);
int statusSensor = digitalRead (IRSensor);
while(statusSensor == 0){
    while (i < 2) {

      float AmbientTempC = mlx.readAmbientTempC();
      float ObjectTempC = mlx.readObjectTempC();
      float AmbientTempF = mlx.readAmbientTempF();
      float ObjectTempF = mlx.readObjectTempF();
      i++;
      

      Serial.print("Ambient = "); Serial.print(AmbientTempC);
      Serial.print("*C\tObject = "); Serial.print(ObjectTempC); Serial.println("*C");
      Serial.print("Ambient = "); Serial.print(AmbientTempF);
      Serial.print("*F\tObject = "); Serial.print(ObjectTempF); Serial.println("*F");

      Serial.println();
      digitalWrite(BUZ, HIGH);
      delay(200);
      digitalWrite(BUZ, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Ambient=");
      lcd.print(AmbientTempC);
      lcd.print("*C  ");
      lcd.setCursor(0, 1);
      lcd.print("Object=");
      lcd.print(ObjectTempC);
      lcd.print("*C  ");
      delay(5000);
      lcd.setCursor(0, 0);
      lcd.print("Ambient=");
      lcd.print(AmbientTempF);
      lcd.print("*F  ");
      lcd.setCursor(0, 1);
      lcd.print("Object=");
      lcd.print(ObjectTempF);
      lcd.print("*F  ");
      delay(5000);
      lcd.setCursor(0, 0);
      lcd.print("   THANK YOU!   ");
      lcd.setCursor(0, 1);
      // print the number of seconds since reset:
      lcd.print("  VISIT  AGAIN  ");
      delay(5000);
      lcd.setCursor(0, 0);
      lcd.print("HAVE A NICE DAY ");
      lcd.setCursor(0, 1);
      // print the number of seconds since reset:
      lcd.print("-By Sumit Sajan-");
      delay(5000);
    }
      pinMode(LED, OUTPUT);
      digitalWrite(LED, LOW);
  }
  }
}
