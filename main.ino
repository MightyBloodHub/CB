#include <TimedAction.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

// Pin Definitions
const int ONE_WIRE_BUS = 2;
const int UP_BUTTON_PIN = 9;
const int DOWN_BUTTON_PIN = 8;
const int RELAY_PINS[] = {3, 4, 5, 6};
const int ALARM_SETTING_ONE_SWITCH = 10;
const int ALARM_SETTING_TWO_SWITCH = 11;
const int ALARM_OUT = 12;
const int R1_SOURCE_5V = 44;
const int R2_SOURCE_5V = 45;
const int SOURCE_5V = 46;

// Constants
const float TEMP_STEP = 0.5;
const float HYSTERESIS = 1.0;
const unsigned long BUTTON_DEBOUNCE_DELAY = 50;
const unsigned long TEMP_UPDATE_INTERVAL = 20000;
const unsigned long DISPLAY_UPDATE_INTERVAL = 500;
const unsigned long CONTROL_UPDATE_INTERVAL = 1000;
const int EEPROM_TEMP_ADDR = 0;

// Global Variables
float setTemp = 30.0;
float currentTemp = 0.0;
bool heatingMode = false;
unsigned long lastTempUpdate = 0;
unsigned long lastDisplayUpdate = 0;
unsigned long lastControlUpdate = 0;
unsigned long lastDebounceTime = 0;
int buttonState = HIGH;
int lastButtonState = HIGH;

// Objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function Prototypes
void updateTemperature();
void updateDisplay();
void controlSystem();
void handleButtons();
void switchRelay(int relay, bool state);
void switchAllRelays(bool state);
void saveSetTemp();
void loadSetTemp();

void setup() {
  // Initialize pins
  for (int pin : RELAY_PINS) {
    pinMode(pin, OUTPUT);
  }
  pinMode(SOURCE_5V, OUTPUT);
  pinMode(R1_SOURCE_5V, OUTPUT);
  pinMode(R2_SOURCE_5V, OUTPUT);
  pinMode(ALARM_OUT, OUTPUT);
  pinMode(ALARM_SETTING_ONE_SWITCH, INPUT_PULLUP);
  pinMode(ALARM_SETTING_TWO_SWITCH, INPUT_PULLUP);
  pinMode(UP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(DOWN_BUTTON_PIN, INPUT_PULLUP);

  // Initialize 5V sources
  digitalWrite(SOURCE_5V, HIGH);
  digitalWrite(R1_SOURCE_5V, HIGH);
  digitalWrite(R2_SOURCE_5V, HIGH);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize sensors
  sensors.begin();

  // Load saved temperature setpoint
  loadSetTemp();

  // Initial temperature reading
  updateTemperature();
}

void loop() {
  unsigned long currentMillis = millis();

  // Update temperature reading
  if (currentMillis - lastTempUpdate >= TEMP_UPDATE_INTERVAL) {
    updateTemperature();
    lastTempUpdate = currentMillis;
  }

  // Update display
  if (currentMillis - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    updateDisplay();
    lastDisplayUpdate = currentMillis;
  }

  // Control system
  if (currentMillis - lastControlUpdate >= CONTROL_UPDATE_INTERVAL) {
    controlSystem();
    lastControlUpdate = currentMillis;
  }

  // Handle button presses
  handleButtons();
}

void updateTemperature() {
  sensors.requestTemperatures();
  currentTemp = sensors.getTempCByIndex(0);
}

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Now: ");
  lcd.print(currentTemp, 1);
  lcd.print("C ");
  lcd.print(heatingMode ? "Heat" : "Cool");
  
  lcd.setCursor(0, 1);
  lcd.print("Set: ");
  lcd.print(setTemp, 1);
  lcd.print("C ");
  lcd.print(currentTemp > setTemp ? "^" : (currentTemp < setTemp ? "v" : "-"));
}

void controlSystem() {
  if (currentTemp > setTemp + HYSTERESIS) {
    switchAllRelays(true);  // Turn on cooling
    heatingMode = false;
  } else if (currentTemp < setTemp - HYSTERESIS) {
    switchAllRelays(false);  // Turn off cooling
    heatingMode = true;
  }
}

void handleButtons() {
  int reading = digitalRead(UP_BUTTON_PIN) & digitalRead(DOWN_BUTTON_PIN);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        if (digitalRead(UP_BUTTON_PIN) == LOW) {
          setTemp += TEMP_STEP;
        } else if (digitalRead(DOWN_BUTTON_PIN) == LOW) {
          setTemp -= TEMP_STEP;
        }
        saveSetTemp();
      }
    }
  }
  
  lastButtonState = reading;
}

void switchRelay(int relay, bool state) {
  digitalWrite(RELAY_PINS[relay], state ? HIGH : LOW);
}

void switchAllRelays(bool state) {
  for (int i = 0; i < sizeof(RELAY_PINS) / sizeof(RELAY_PINS[0]); i++) {
    switchRelay(i, state);
  }
}

void saveSetTemp() {
  EEPROM.put(EEPROM_TEMP_ADDR, setTemp);
}

void loadSetTemp() {
  EEPROM.get(EEPROM_TEMP_ADDR, setTemp);
  if (isnan(setTemp) || setTemp < 0 || setTemp > 100) {
    setTemp = 30.0;  // Default value if EEPROM data is invalid
  }
}