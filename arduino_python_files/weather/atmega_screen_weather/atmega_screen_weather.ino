#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String readString; // readstring is for collecting the serial buffer code
String temp_c;  // string that stores temp
String feels;  // string that stores feels like temp
String weather;  // string that stores weather forcast
String lowest_temp;  // string that stores lowest temp
String wind;  // string that stores wind speed
String lowest_time;  // Time last lowest
String highest_temp;  // string that stores highest temp
String highest_time;   // string that stores highest temp time

uint8_t degree[8] = {0x8,0xf4,0x8,0x43,0x4,0x4,0x43,0x0};

// Button
const int button = 5; // Set button to pin 5
int buttonPushCounter1 = 8;    // counts the button pushes
int buttonState1 = 0;    // tracks the button state
int lastButtonState1 = 0;    // last state of the button

// Debounce
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;  // last button state set to low
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Timing for the display cycle times
long waitUntilcycle1 = 0;
long waitUntilcycle2 = 5000;
long waitUntilcycle3 = 10000;
long waitUntilcycle4 = 15000;
long waitUntilcycle5 = 20000;
long waitUntilcycle6 = 25000;
long waitUntilcycle7 = 30000;

// RGB LED
int redPin = 12; // Red led set to pin 12
int greenPin = 11; // Green led set to pin 11
int bluePin = 10; // Blue led set to pin 10
//#define COMMON_ANODE

// RTC 
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


void setup() {
  Wire.begin(); // Start the wire liberty
  lcd.backlight(); // Turn on the lcd backlight
  lcd.init(); // Start up the lcd
  lcd.begin(16, 2); // Set up the lcd to have 16 char on 2 lines
  pinMode(button, INPUT); // Set the button as input
  digitalWrite(button, HIGH); // initiate the internal pull up resistor
  lcd.createChar(0, degree);
  Serial.begin(9600); // Begin serial at 9600 baud
  Serial.println("Waiting"); // Print to serial 
}

void loop() {
  lcd.setCursor(0,0); // Set lcd cursor to the start of the first row
  if (Serial.available())  { // Check to see if serial is available
    char c = Serial.read();  //gets one byte from serial buffer
    
    if (c == ';') { // Checks to see if c contains ;
      if (readString.length() >0) { // Checks that the length of readString is over 0
        if (readString.startsWith("#1")) { // Checks readString if it starts with #1
          temp_c = readString.substring(2); // Adds the readString less the first 2 characters to temp_c
          Serial.println(temp_c); // Prints temp_c to serial
        }
        
        else if (readString.startsWith("#2")) { // Checks readString if it starts with #2
          feels = readString.substring(2); // Adds the readString less the first 2 characters to feels
          Serial.println(feels); // Prints feels to serial
        }
        
        else if (readString.startsWith("#3")) { // Checks readString if it starts with #3
          weather = readString.substring(2); // Adds the readString less the first 2 characters to weather
          Serial.println(weather); // Prints weather to serial
        }
        
        else if (readString.startsWith("#4")) { // Checks readString if it starts with #4
          lowest_temp = readString.substring(2); // Adds the readString less the first 2 characters to lowest_temp
          Serial.println(lowest_temp); // Prints lowest_temp to serial
        }
        
        else if (readString.startsWith("#5")) { // Checks readString if it starts with #5
          wind = readString.substring(2); // Adds the readString less the first 2 characters to wind
          Serial.println(wind); // Prints wind to serial
        }
        
        else if (readString.startsWith("#6")) { // Checks readString if it starts with #6
          lowest_time = readString.substring(2); // Adds the readString less the first 2 characters to lowest_time
          Serial.println(lowest_time); // Prints lowest_time to serial
        }
        
        else if (readString.startsWith("#7")) { // Checks readString if it starts with #7
          highest_temp = readString.substring(2); // Adds the readString less the first 2 characters to highest_temp
          Serial.println(highest_temp); // Prints highest_temp to serial
        }
        
        else if (readString.startsWith("#8")) { // Checks readString if it starts with #8
          highest_time = readString.substring(2); // Adds the readString less the first 2 characters to highest_time
          Serial.println(highest_time); // Prints highest_time to serial
        }
        readString=""; //clears variable for new input
      }
    }
    
    else if (c =='@') { // Check if c contains @
      if (readString.length() >0) { // Checks that the length of readString is over 0
        if (readString == "blue") { // Check if readString contains blue
          setColor(0, 0, 255); // Sets the RGB colour to blue
        }
        else if (readString == "green") { // Check if readString contains green
          setColor(0, 255, 0); // Sets the RGB colour to green
        }
        else if (readString == "red") { // Check if readString contains red
          setColor(255, 0, 0);  // Sets the RGB colour to red
        }
        else if (readString == "yellow") { // Check if readString contains yellow
          setColor(255, 255, 0); // Sets the RGB colour to yellow
        }
        else if (readString == "purple") { // Check if readString contains purple
          setColor(175, 0, 175); // Sets the RGB colour to purple
        }
        else if (readString == "aqua") { // Check if readString contains aqua
          setColor(0, 255, 255); // Sets the RGB colour to aqua
          
    }
    }
    readString=""; //clears variable for new input
    }
    
    else {     
      readString += c; //makes the string readString
    }
  }
  // -------------- Debound code start --------------
  int reading = digitalRead(button); // Load button stat to int reading
  buttonState1 = digitalRead(button); // Load button stat into buttonState1

  if (reading != lastButtonState) { // Check if reading doesnt equal lastButtonState
    lastDebounceTime = millis(); 
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading; // 
      
        if (buttonState1 != lastButtonState1) { // Check buttonState1 doesnt equal lastButtonState1
          
          if (buttonState1 == LOW) { // Check if buttonStata1 == LOW
            buttonPushCounter1++; // Increment buttonPushCounter1
            
            if (buttonPushCounter1 == 9) { // Check buttonPushCounter1 is == 9
              buttonPushCounter1 = 1;} // If buttonPushCounter == 9 then set buttonPushCounter1 to 1
              lcd.clear(); // Clear the lcd
          }
          else {
            Serial.println("off"); 
          }
        }
          lastButtonState1 = buttonState1; 
    }
  }
  lastButtonState = reading;
  // -------------- Debound code end --------------
  
    if (buttonPushCounter1 == 1) {
      lcd.print("--Current temp--");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(temp_c); // Lcd print temp_c string
      lcd.print((char)0);
    }
    
    if (buttonPushCounter1 == 2) {
      lcd.print("---Feels like---");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(feels); // Lcd print feels string
      lcd.print((char)0);
    }

    if (buttonPushCounter1 == 3) {
      lcd.print("-----Weather----");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(weather); // Lcd print weather String
    }

    if (buttonPushCounter1 == 4) {
      lcd.print("---Wind speed---");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(wind); // Lcd print wind string
    }

    if (buttonPushCounter1 == 5) {
      lcd.print("--Lowest temp---");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(lowest_time); // Lcd print lowest_time string
      lcd.print(" ");
      lcd.print(lowest_temp); // Lcd print lowest_temp string
      lcd.print((char)0);
    }
    
    if (buttonPushCounter1 == 6) {
      lcd.print("--Highest temp--");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(highest_time); // Lcd print highest_time string
      lcd.print(" ");
      lcd.print(highest_temp); // Lcd print highest_temp string
      lcd.print((char)0);
    }
    

  if (buttonPushCounter1 == 7) {
          
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
  
  
    if (buttonPushCounter1 == 8) {
      if (millis() >= waitUntilcycle1) {
      lcd.clear();
      lcd.print("--Current temp--");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(temp_c); // Lcd print temp_c string
      lcd.print((char)0);
      waitUntilcycle1 += 35000; // Addes 35000 to waitUntilcycle1
      }
    if (millis() >= waitUntilcycle2) {
      lcd.clear();
      lcd.print("---Feels like---"); // Lcd print feels string
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(feels); // Lcd print feels String
      lcd.print((char)0);
      waitUntilcycle2 += 35000;
     }
    if (millis() >= waitUntilcycle3) {
      lcd.clear();
      lcd.print("-----Weather----");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(weather); // Lcd print weather String
      waitUntilcycle3 += 35000;
     }
    if (millis() >= waitUntilcycle4) {
      lcd.clear();
      lcd.print("---Wind speed---");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(wind); // Lcd print wind string
      waitUntilcycle4 += 35000;
    }
    if (millis() >= waitUntilcycle5) {
      lcd.clear();
      lcd.print("--Lowest temp---");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(lowest_time); // Lcd print lowest_time string
      lcd.print(" ");
      lcd.print(lowest_temp); // Lcd print lowest_temp string
      lcd.print((char)0);
      waitUntilcycle5 += 35000;
    }
    if (millis() >= waitUntilcycle6) {
      lcd.clear();
      lcd.print("--Highest temp--");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(highest_time); // Lcd print highest_time string
      lcd.print(" ");
      lcd.print(highest_temp); // Lcd print highest_temp string
      lcd.print((char)0);
      waitUntilcycle6 += 35000;
    }
    if (millis() >= waitUntilcycle7) {
      char buffer_minutes[3]; // Creates a char buffer of 3 characters for minutes
      char buffer_seconds[3]; // Creates a char buffer of 3 characters for seconds
      lcd.clear();
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
      waitUntilcycle7 += 35000;
  }
  }
}

// Set RGB colours
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
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


