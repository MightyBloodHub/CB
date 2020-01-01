#include <TimedAction.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 2 
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  

// (not just Maxim/Dallas temperature ICs) 

OneWire oneWire(ONE_WIRE_BUS); 
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 

DallasTemperature sensors(&oneWire);
/********************************************************************/

// this constant won't change:
const int  Up_buttonPin   = 9;    // change the pin that the pushbutton is attached to
const int  Down_buttonPin = 8;

// Variables will change:
float buttonPushCounter = 20;   // counter for the number of button presses
float up_buttonState = 0;         // current state of the up button
float up_lastButtonState = 0;     // previous state of the up button

float down_buttonState = 0;         // current state of the up button
float down_lastButtonState = 0;     // previous state of the up button
bool bPress = false;


int source5v = 46;  // for LCD power



void pushButton(){
  
  //Serial.print("i = ");
  //Serial.println(i);
  lcd.setCursor(0,1);
  lcd.print("user Temp=");
  lcd.setCursor(11,1);
  lcd.print(buttonPushCounter);
  checkUp();
  checkDown();

   if( bPress){
       bPress = false;
      //lcd.setCursor(2,1);
      //lcd.print("               ");
      lcd.setCursor(2,1);
      lcd.print(buttonPushCounter);
   }
  }

  void tempSensor(){
  sensors.requestTemperatures(); // Send the command to get temperature readings 
  lcd.setCursor(0,0);  // (col,row)
  lcd.print("now Temp=");
  //Serial.begin(9600) ;
  //sensors.begin();
  //Serial.print(sensors.getTempCByIndex(0)); 
  //Serial.print('\n');
  lcd.setCursor(9,0);
  lcd.print(sensors.getTempCByIndex(0)); // Why "byIndex"?  
   // You can have more than one DS18B20 on the same bus.  
   // 0 refers to the first IC on the wire 
   delay(1000);
  
    
    }



TimedAction tempThread = TimedAction(5000,tempSensor);
TimedAction pushThread = TimedAction(1,pushButton);



void setup() {
  // put your setup code here, to run once:
  pinMode(source5v, OUTPUT);
  digitalWrite(source5v, HIGH);

  lcd.init(); 

  Serial.begin(9600) ;
  sensors.begin();

  pinMode( Up_buttonPin , INPUT_PULLUP);
  pinMode( Down_buttonPin , INPUT_PULLUP);

  lcd.backlight();
  
 
}

void loop() {
  // put your main code here, to run repeatedly:

  tempThread.check();
  pushThread.check();
  Serial.print(sensors.getTempCByIndex(0)+'\n'+'\n');
  //pushButton();
 //tempSensor();
}



void checkUp()
{
  up_buttonState = digitalRead(Up_buttonPin);

  // compare the buttonState to its previous state
  if (up_buttonState != up_lastButtonState) {
    // if the state has changed, increment the counter
    if (up_buttonState == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter += 0.5;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  up_lastButtonState = up_buttonState;
}
void checkDown()
{
  down_buttonState = digitalRead(Down_buttonPin);

  // compare the buttonState to its previous state
  if (down_buttonState != down_lastButtonState) {
    // if the state has changed, increment the counter
    if (down_buttonState == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter -= 0.5;
     
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(buttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  down_lastButtonState = down_buttonState;
}
  
