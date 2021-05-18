#include <TimedAction.h>
#include <LiquidCrystal_I2C.h>

int lcdColumns = 20;
int lcdRows    =  4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 




// this constant won't change:
const int  Up_buttonPin   = 36;    // change the pin that the pushbutton is attached to
const int  Down_buttonPin = 39;

// Variables will change:
float buttonPushCounter = 30;   // counter for the number of button presses
float up_buttonState = 0;         // current state of the up button
float up_lastButtonState = 0;     // previous state of the up button

float down_buttonState = 0;         // current state of the up button
float down_lastButtonState = 0;     // previous state of the up button
bool bPress = false;



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
    
      Serial.println(buttonPushCounter);
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
    
      Serial.println(buttonPushCounter);
  }
  // save the current state as the last state, for next time through the loop
  down_lastButtonState = down_buttonState;
}

TimedAction checkUpThread = TimedAction(50,checkUp);
TimedAction checkDownThread = TimedAction(50,checkDown);



void displayDataToLcd(){

  //Serial.print("i = ");
  //Serial.println(i);
// lcd.setCursor(0,0);  // (col,row)
//  lcd.print("now Temp= ");
//  lcd.print(sensors.getTempCByIndex(0));
  lcd.setCursor(0,1);
  lcd.print("user Temp= ");
  lcd.setCursor(11,1);
  lcd.print(buttonPushCounter);
//   lcd.setCursor(0,2);
//  lcd.print("hum");

  
 
   if( bPress){
       bPress = false;
      lcd.setCursor(11,1);
      lcd.print(buttonPushCounter);
   }
  }

TimedAction displayDataToLcdThred = TimedAction(50,displayDataToLcd);
void setup() {
  // put your setup code here, to run once:

  lcd.begin(); //lcd.init();

  

  pinMode( Up_buttonPin , INPUT_PULLUP);
  pinMode( Down_buttonPin , INPUT_PULLUP);

  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  checkUpThread.check();
  checkDownThread.check();
  displayDataToLcdThred.check();
}
