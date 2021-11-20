#include <TimedAction.h>
#include <LiquidCrystal_I2C.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

int lcdColumns = 20;
int lcdRows    =  4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 




// this constant won't change:
const int  Up_buttonPinT   = 27;    // change the pin that the pushbutton is attached to
const int  Down_buttonPinT = 26;
const int  Up_buttonPinH   = 25;
const int  Down_buttonPinH = 33;


float buttonPushCounterT = 30;   // counter for the number of button presses
float up_buttonStateT = 0;         // current state of the up button
float up_lastButtonStateT = 0;     // previous state of the up button

float buttonPushCounterH = 30;   // counter for the number of button presses
float up_buttonStateH = 0;         // current state of the up button
float up_lastButtonStateH = 0; 

float down_buttonStateT = 0;         // current state of the up button
float down_lastButtonStateT = 0;     // previous state of the up button

float down_buttonStateH = 0;         // current state of the up button
float down_lastButtonStateH = 0; 
bool bPress = false;



void checkUp()
{
  up_buttonStateT = digitalRead(Up_buttonPinT);

  up_buttonStateH = digitalRead(Up_buttonPinH);
      
  // compare the buttonState to its previous state
  if (up_buttonStateT != up_lastButtonStateT) {
    // if the state has changed, increment the counter
    if (up_buttonStateT == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounterT += 0.5;
    } else {
      // if the current state is LOW then the button went from on to off:
      //Serial.println("off");
    }

    
      //Serial.println(buttonPushCounterT);
  }

  if (up_buttonStateH != up_lastButtonStateH) {
    // if the state has changed, increment the counter
    if (up_buttonStateH == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounterH += 0.5;
    } else {
      // if the current state is LOW then the button went from on to off:
      //Serial.println("off");
    }

    
      Serial.println(buttonPushCounterT);
  }
  // save the current state as the last state, for next time through the loop
  up_lastButtonStateT = up_buttonStateT;

  up_lastButtonStateH = up_buttonStateH;
}
void checkDown()
{
  down_buttonStateT = digitalRead(Down_buttonPinT);

  down_buttonStateH = digitalRead(Down_buttonPinH);

  // compare the buttonState to its previous state
  if (down_buttonStateT != down_lastButtonStateT) {
    // if the state has changed, increment the counter
    if (down_buttonStateT == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounterT -= 0.5;
    } 
    }

      if (down_buttonStateH != down_lastButtonStateH) {
    // if the state has changed, increment the counter
    if (down_buttonStateH == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounterH -= 0.5;
    } 
    }
      
  
  // save the current state as the last state, for next time through the loop
  down_lastButtonStateT = down_buttonStateT;

  down_lastButtonStateH = down_buttonStateH;
}

TimedAction checkUpThread = TimedAction(50,checkUp);
TimedAction checkDownThread = TimedAction(50,checkDown);


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  
  float temp;    // when called store temperature in it

  float hum;
  
} struct_message;


// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  //Serial.print("temp=");
  //Serial.println(myData.temp);
  
  //Serial.print(" RH%=");
  //Serial.println(myData.hum);

 



  
}
void displayDataToLcd(){

  lcd.setCursor(0,0);
  lcd.print("Room Temp= ");
  lcd.setCursor(11,0);
  lcd.print(myData.temp);
  lcd.setCursor(16,0);
  lcd.print("C");
  


  lcd.setCursor(0,1);
  lcd.print("user Temp= ");
  lcd.setCursor(11,1);
  lcd.print(buttonPushCounterT);
  lcd.setCursor(16,1);
  lcd.print("C");

  lcd.setCursor(0,2);
  lcd.print("Room %RH = ");
  lcd.setCursor(11,2);
  lcd.print(myData.hum);
  lcd.setCursor(16,2);
  lcd.print("%");
  
  lcd.setCursor(0,3);
  lcd.print("user %RH = ");
  lcd.setCursor(11,3);
  lcd.print(buttonPushCounterH);
  lcd.setCursor(16,3);
  lcd.print("%");

  
 
  /* if( bPress){
       bPress = false;
      //lcd.setCursor(11,1);
      lcd.print(buttonPushCounterT);
   }  */
  }

TimedAction displayDataToLcdThred = TimedAction(50,displayDataToLcd);

void setup() {
  
  Serial.begin(115200);
  lcd.begin(); //lcd.init();

  Wire.setClock(10000);         // Bush added to try to get rid of LCD crashing 10/23/2021
          // got this error 'class TwoWire' has no member named 'SetClock'


  pinMode( Up_buttonPinT , INPUT_PULLUP);
  pinMode( Down_buttonPinT , INPUT_PULLUP);
  
  pinMode( Up_buttonPinH , INPUT_PULLUP);
  pinMode( Down_buttonPinH , INPUT_PULLUP);

  lcd.backlight();

   WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
 
  checkUpThread.check();
  checkDownThread.check();
  displayDataToLcdThred.check();
}
