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
float buttonPushCounter = 30;   // counter for the number of button presses
float up_buttonState = 0;         // current state of the up button
float up_lastButtonState = 0;     // previous state of the up button

float down_buttonState = 0;         // current state of the up button
float down_lastButtonState = 0;     // previous state of the up button
bool bPress = false;

    // relay 1 and 2 controlls FAN1
int relay1 = 3; //   Grey
int relay2 = 4;  // Purple
    // relay 3 and 4 controlls FAN2
int relay3 = 5;  //blue
int relay4 = 6;  // green
    // relay 5 and 6 controlls water pump 1
// int relay5 = 10;  blue
 //int relay6 = 11;  green

int alarmSettingOneSwitch = 10;
int alarmSettingTwoSwitch = 11 ;
int	alarmOut = 12;
int alarmSettingOne = 28;
int alarmSettingTwo = 36;

int R1source5v = 44;
int R2source5v = 45;
int source5v   = 46;

float actualTemp = 0.0;


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
    } else {
      // if the current state is LOW then the button went from on to off:
      //Serial.println("off");
    }
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
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
  }
  // save the current state as the last state, for next time through the loop
  down_lastButtonState = down_buttonState;
}
TimedAction checkUpThread = TimedAction(50,checkUp);
TimedAction checkDownThread = TimedAction(50,checkDown);

void displayDataToLcd(){

  //Serial.print("i = ");
  //Serial.println(i);
  lcd.setCursor(0,0);  // (col,row)
  lcd.print("now Temp= ");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.setCursor(0,1);
  lcd.print("user Temp= ");
  lcd.setCursor(11,1);
  lcd.print(buttonPushCounter);

  
 
   if( bPress){
       bPress = false;
      lcd.setCursor(11,1);
      lcd.print(buttonPushCounter);
   }
  }

TimedAction displayDataToLcdThred = TimedAction(500,displayDataToLcd);



void updateTempFromSensor(){
  sensors.requestTemperatures();
  Serial.println(sensors.getTempCByIndex(0));
}

void switchAllOnIfGreatorAllOffIFLess(){
  //actualTemp = sensors.getTempCByIndex(0);
  int tempInt = 0;
  if((sensors.getTempCByIndex(0)-(int)sensors.getTempCByIndex(0))< 0.5){
    tempInt = (int)sensors.getTempCByIndex(0);
  }else{
    tempInt = (int)sensors.getTempCByIndex(0) + 1;
  }
  
  
 
  if (buttonPushCounter<tempInt) {
    //delay(1000);  // added this delay to avoid instant relays kick on!!
    switchRelay(1,0); //switch relay1 off
    switchRelay(2,0); //switch relay2 off
    switchRelay(3,0); //switch relay3 off
    switchRelay(4,0); //switch relay4 off
    switchRelay(5,0); //switch relay5 off
    switchRelay(6,0); //switch relay6 off
    }
  else {
    switchRelay(1,1); //switch relay1 on
    switchRelay(2,1); //switch relay2 on
    switchRelay(3,1); //switch relay3 on
    switchRelay(4,1); //switch relay4 on
    switchRelay(5,1); //switch relay5 on
    switchRelay(6,1); //switch relay6 on
      }
}


TimedAction updateTempFromSensorThread = TimedAction(20000,updateTempFromSensor);
TimedAction switchAllOnIfGreatorAllOffIFLessThread = TimedAction(1000,switchAllOnIfGreatorAllOffIFLess);






void setup() {
  // put your setup code here, to run once:
  pinMode(source5v, OUTPUT);
  digitalWrite(source5v, HIGH);

  pinMode(R1source5v, OUTPUT);
  digitalWrite(R1source5v, HIGH);

  pinMode(R2source5v, OUTPUT);
  digitalWrite(R2source5v, HIGH);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  //pinMode(relay6, OUTPUT);
  //pinMode(relay5, OUTPUT);
  pinMode(relay4, OUTPUT);

   

  pinMode(alarmOut, OUTPUT);
  pinMode(alarmSettingOneSwitch, INPUT_PULLUP);
  pinMode(alarmSettingTwoSwitch, INPUT_PULLUP);

  switchRelay(1,1); //switch relay1 on
  switchRelay(2,1); //switch relay2 on
  switchRelay(3,1); //switch relay3 on
  switchRelay(4,1); //switch relay4 on
  switchRelay(5,1); //switch relay5 on
  switchRelay(6,1); //switch relay6 on

  switchAlarm(0); //alarm  off

  lcd.init();

  lcd.init();

  Serial.begin(9600) ;
  sensors.begin();

  pinMode( Up_buttonPin , INPUT_PULLUP);
  pinMode( Down_buttonPin , INPUT_PULLUP);

  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkUpThread.check();
  checkDownThread.check();
  displayDataToLcdThred.check();
  updateTempFromSensorThread.check();
  switchAllOnIfGreatorAllOffIFLessThread.check();


  //Test alarm
  //switchAlarm(1) //alarm  onn
  //switchAlarm(0) //alarm  off

  
  

}

bool isGreator()
{
  //cheack if actual greator than desiared temp
  //leave empty for now as this for cleaning the code later
}

void switchRelay(int N, bool C)
{
  switch(N) {
      case 1 :
        if(C){digitalWrite(relay1, HIGH);}else{digitalWrite(relay1, LOW);}
         break;
      case 2 :
         if(C){digitalWrite(relay2, HIGH);}else{digitalWrite(relay3, LOW);}
         break;
      case 3 :
         if(C){digitalWrite(relay3, HIGH);}else{digitalWrite(relay3, LOW);}
         break;
        /* case 4 :
           if(C){digitalWrite(relay4, HIGH);}else{digitalWrite(relay4, LOW);}
            break;
         case 5 :
            if(C){digitalWrite(relay5, HIGH);}else{digitalWrite(relay5, LOW);}
            break;
         case 6 :
            if(C){digitalWrite(relay6, HIGH);}else{digitalWrite(relay6, LOW);}
            break; */
   }

  
  }
  void switchAlarm(bool C)
  {
    if (C)
    {
      digitalWrite(alarmOut, HIGH);
    }
    else
    {
      digitalWrite(alarmOut, LOW);
    }

}
