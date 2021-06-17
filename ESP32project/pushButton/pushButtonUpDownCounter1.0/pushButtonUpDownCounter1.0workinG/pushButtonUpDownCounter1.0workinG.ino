#include <TimedAction.h>
#include <LiquidCrystal_I2C.h>

int lcdColumns = 20;
int lcdRows    =  4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 




// this constant won't change:
const int  Up_buttonPinT   = 27;    // change the pin that the pushbutton is attached to
const int  Down_buttonPinT = 26;
const int  Up_buttonPinH   = 25;
const int  Down_buttonPinH = 33;

// Variables will change:
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



void displayDataToLcd(){



  lcd.setCursor(0,1);
  lcd.print("user Temp= ");
  lcd.setCursor(11,1);
  lcd.print(buttonPushCounterT);

  lcd.setCursor(0,2);
  lcd.print("user %RH= ");
  lcd.setCursor(11,2);
  lcd.print(buttonPushCounterH);


  
 
   if( bPress){
       bPress = false;
      lcd.setCursor(11,1);
      lcd.print(buttonPushCounterT);
   }
  }

TimedAction displayDataToLcdThred = TimedAction(50,displayDataToLcd);

void setup() {
  
  Serial.begin(115200);
  lcd.begin(); //lcd.init();

  

  pinMode( Up_buttonPinT , INPUT_PULLUP);
  pinMode( Down_buttonPinT , INPUT_PULLUP);
  
  pinMode( Up_buttonPinH , INPUT_PULLUP);
  pinMode( Down_buttonPinH , INPUT_PULLUP);

  lcd.backlight();
}

void loop() {
 
  checkUpThread.check();
  checkDownThread.check();
  displayDataToLcdThred.check();
}
