#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int DS1307 = 0x68; // Address of DS1307 see data sheets
const char* days[] =
{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // converts the numbers from the days of week to names

// Initializes all values: 
byte second = 0; // Setting up bytes for seconds
byte minute = 0; // Setting up bytes for minutes
byte hour = 0; // Setting up bytes for hours
byte weekday = 0; // Setting up bytes for weekday
byte monthday = 0; // Setting up bytes for monthday
byte month = 0; // Setting up bytes for month
byte year = 0;  // Setting up bytes for year

// Button red stuff
const int button_top_red = 7; // Button set to pin 5
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

void setup() {
  Wire.begin(); // Start the wire liberty
  lcd.backlight(); // Turn on the lcd backlight
  lcd.init(); // Start up the lcd
  lcd.begin(16, 2); // Set up the lcd to have 16 char on 2 lines
  pinMode(button_top_red, INPUT); // Set the button as input
  digitalWrite(button_top_red, HIGH); // initiate the internal pull up resistor
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {

  // -------------- Debound code button top red --------------

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  reading = digitalRead(button_top_red);

  if (reading == HIGH && previous == LOW && millis() - time > debounce) {
    if (state == HIGH) {
      state = LOW;
      lcd.backlight();
    }
    else {
      state = HIGH;
      lcd.noBacklight();
    }

    time = millis();    
  }
  
  previous = reading;
   
  
  char buffer_minutes[3]; // Creates a char buffer of 3 characters for minutes
  char buffer_seconds[3]; // Creates a char buffer of 3 characters for seconds
  lcd.setCursor(0,0); // Set lcd cursor to start of first line
  readTime(); // Runs readTime to get RTC data
  lcd.print(days[weekday-1]);
  lcd.setCursor(0,1); // Set lcd cursor to start of second line
  lcd.print(monthday);
  lcd.print("/");
  lcd.print(month);
  lcd.print("/");
  lcd.print(year);
  lcd.print(" ");
  lcd.print(hour);
  lcd.print(":");
  sprintf(buffer_minutes, "%02d", minute); // Floats the minutes to only 2 decimal points stores them on buffer
  lcd.print(buffer_minutes);
  lcd.print(":");
  sprintf(buffer_seconds, "%02d", second); // Floats the seconds to only 2 decimal points stores them on buffer
  lcd.print(buffer_seconds);
  }
  
// RTC data
void readTime() {
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0));
  Wire.endTransmission();
  Wire.requestFrom(DS1307, 7);
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read());
  weekday = bcdToDec(Wire.read());
  monthday = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

byte decToBcd(byte val) {
  return ((val/10*16) + (val%10));
}
byte bcdToDec(byte val) {
  return ((val/16*10) + (val%16));
}


