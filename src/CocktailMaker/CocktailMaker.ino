#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonNext = 13;    
const int buttonSelect = 12; 

//relay pins
#define PIN_RELAY_1  5 
#define PIN_RELAY_2  6 
#define PIN_RELAY_3  7 

//ultrasonic sensor pins
const int trigPin = 9;
const int echoPin = 10;

//Cup Detetion LED indicator pin
int ultLED = 8;

// for ultrasonic sensor
long duration;
int distance;
const int thresholdDistance = 10;  // Distance in cm to detect the cup

// for Menu
int menuIndex = 0;
int flavorIndex = 0;
int amountIndex = 0;

String flavors[] = {"Apple", "Strawberry"};
int flavorCount = sizeof(flavors) / sizeof(flavors[0]);

String amounts[] = {"Small", "Large"};
int amountCount = sizeof(amounts) / sizeof(amounts[0]);

// Timing variables for millis();
unsigned long lastCheck = 0;
unsigned long lastButtonPress = 0;
unsigned long nextButtonPressTime = 0;    // For medium-press detection on next button
unsigned long selectButtonPressTime = 0;  // For long-press detection on select button
const int debounceDelay = 50;             // Short debounce delay for both buttons
const int mediumPressDuration = 300;      // Hold the next button for 300 ms
const int longPressDuration = 500;        // Hold the select button for 500 ms

void setup() {

  // Initialize the LCD
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Flavor:");

  // Set button pins 
  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);

  // Set Ultrasonic sensor LED
  pinMode(ultLED, OUTPUT);

  // Set relay pins
  pinMode(PIN_RELAY_1, OUTPUT);
  pinMode(PIN_RELAY_2, OUTPUT);
  pinMode(PIN_RELAY_3, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Start relays in OFF state
  digitalWrite(PIN_RELAY_1, HIGH);
  digitalWrite(PIN_RELAY_2, HIGH);
  digitalWrite(PIN_RELAY_3, HIGH);

}

void loop() {

  // Check for cup detection
  if (millis() - lastCheck >= 1000) {
    detectCup();  // Check the cup every 1 sec
    lastCheck = millis();
  }

  // Menu - Flvor and Amount Selection.
  if (menuIndex == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Select Flavor:    ");
    lcd.setCursor(0, 1);
    lcd.print(flavors[flavorIndex]);

    // flavor selection
    if (millis() - lastButtonPress >= debounceDelay) {
      if (digitalRead(buttonNext) == LOW) {
        if (nextButtonPressTime == 0) {
          nextButtonPressTime = millis();  // Start timing when button is pressed
        }
        
        if (millis() - nextButtonPressTime >= mediumPressDuration) {
          lastButtonPress = millis();
          flavorIndex++;
          if (flavorIndex >= flavorCount) {
            flavorIndex = 0;
          }
          lcd.setCursor(0, 1);
          lcd.print("                ");  // Clear the previous flavor
          lcd.setCursor(0, 1);
          lcd.print(flavors[flavorIndex]); 
          nextButtonPressTime = 0;
        }

      } else {
        nextButtonPressTime = 0;  // Reset timing if the button is released early
      }

      // Check for long press on select button
      if (digitalRead(buttonSelect) == LOW) {
        if (selectButtonPressTime == 0) {
          selectButtonPressTime = millis();  // Start timing when button is pressed
        }

        // If the button is held for longer than the longPressDuration
        if (millis() - selectButtonPressTime >= longPressDuration) {
          lastButtonPress = millis();
          menuIndex = 1;  // Move to amount selection
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Select Size:");
          selectButtonPressTime = 0;  // Reset timing after the long press
        }

      } else {
        selectButtonPressTime = 0;  // Reset time if the button is released early
      }

    }
  } 
  else if (menuIndex == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Select Size:     ");  // Clear any previous characters
    lcd.setCursor(0, 1);
    lcd.print(amounts[amountIndex]);

    // Button press logic for amount selection
    if (millis() - lastButtonPress >= debounceDelay) {
      // Check for medium press on next button
      if (digitalRead(buttonNext) == LOW) {
        if (nextButtonPressTime == 0) {
          nextButtonPressTime = millis();  // Start timing when button is pressed
        }

        if (millis() - nextButtonPressTime >= mediumPressDuration) {
          lastButtonPress = millis();
          amountIndex++;

          if (amountIndex >= amountCount) {
            amountIndex = 0;
          }

          lcd.setCursor(0, 1);
          lcd.print("                ");  // Clear the previous amount
          lcd.setCursor(0, 1);
          lcd.print(amounts[amountIndex]);
          nextButtonPressTime = 0;  // Reset the next button press timer
        }

      } else {
        nextButtonPressTime = 0;  // Reset timing if the button is released early
      }

      // Check for long press on select button
      if (digitalRead(buttonSelect) == LOW) {
        if (selectButtonPressTime == 0) {
          selectButtonPressTime = millis();  // Start timing when button is pressed
        }

        if (millis() - selectButtonPressTime >= longPressDuration) {
          lastButtonPress = millis();

          if (detectCup()) {
            dispense(flavorIndex, amounts[amountIndex]);
          } else {
            lcd.clear();
            lcd.print("No cup detected!");
            lcd.setCursor(0, 1);
            lcd.print("Place your Cup!");
            delay(2000); 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Select Flavor:");
          }

          menuIndex = 0;  // Return to flavor selection
          selectButtonPressTime = 0;  // Reset timing after the long press

        }

      } else {
        selectButtonPressTime = 0;  // Reset timing if the button released early
      }

    }
  }
}

// cup detection function
bool detectCup() {
  // Send a pulse from trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // time took for soundwave to return
  duration = pulseIn(echoPin, HIGH);

  // Convert to centimeters
  // speed of sound in air is 0.0343 centimeters per microsecond
  // so the distance in Centimeters is,

  distance = duration * 0.034 / 2;  

  if (distance <= thresholdDistance) {
    // turn on the led indicator
    digitalWrite(ultLED, HIGH);
    return true;
  } else {
    digitalWrite(ultLED, LOW);
    return false;
  }
}

// dispensing logic function
void dispense(int flavor, String amount) {

  if (flavor == 0 && amount == "Small") {
    //Dispensing logic for Apple - Small");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Making Apple");
    lcd.setCursor(0, 1);
    lcd.print("Flavor Mocktail");

    //Dispensing Sprite
    digitalWrite(PIN_RELAY_1, LOW);
    delay(4000);
    digitalWrite(PIN_RELAY_1, HIGH);

    //Dispensing Sunquick
    digitalWrite(PIN_RELAY_2, LOW);
    delay(2000);
    digitalWrite(PIN_RELAY_2, HIGH);

  } else if (flavor == 1 && amount == "Small") {
    //Dispensing logic for Strawberry - Small

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Making Stawberry");
    lcd.setCursor(0, 1);
    lcd.print("& Guava Mocktail");

    //Dispensing Sprite
    digitalWrite(PIN_RELAY_1, LOW);
    delay(4000);
    digitalWrite(PIN_RELAY_1, HIGH);

    //Dispensing Sunquick
    digitalWrite(PIN_RELAY_3, LOW);
    delay(2000);
    digitalWrite(PIN_RELAY_3, HIGH);

  } else if (flavor == 0 && amount == "Large") {
    //Dispensing logic for Apple - Large

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Making Apple");
    lcd.setCursor(0, 1);
    lcd.print("Flavor Mocktail");

    //Dispensing Sprite
    digitalWrite(PIN_RELAY_1, LOW);
    delay(10000);
    digitalWrite(PIN_RELAY_1, HIGH);

    //Dispensing Sunquick
    digitalWrite(PIN_RELAY_2, LOW);
    delay(3000);
    digitalWrite(PIN_RELAY_2, HIGH);

  } else if (flavor == 1 && amount == "Large") {
    //Dispensing logic for Strawberry - Large

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Making Stawberry");
    lcd.setCursor(0, 1);
    lcd.print("& Guava Mocktail");

    //Dispensing Sprite
    digitalWrite(PIN_RELAY_1, LOW);
    delay(10000);
    digitalWrite(PIN_RELAY_1, HIGH);

    //Dispensing Sunquick
    digitalWrite(PIN_RELAY_3, LOW);
    delay(3000);
    digitalWrite(PIN_RELAY_3, HIGH);
    
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("All Done!");
  lcd.setCursor(0, 1);
  lcd.print("Cheers & Enjoy");
  delay(2000);
  lcd.clear();

}
