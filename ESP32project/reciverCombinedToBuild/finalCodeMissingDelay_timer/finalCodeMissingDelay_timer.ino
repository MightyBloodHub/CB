#include <TimedAction.h>
#include <LiquidCrystal_I2C.h>
#include <esp_now.h>
#include <WiFi.h>


int lcdColumns = 20;
int lcdRows    =  4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 


int relay1 = 5;
int relay2 = 18;  
int relay3 = 19;



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

    
      //Serial.println(buttonPushCounterT);
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

 
  }

TimedAction displayDataToLcdThred = TimedAction(50,displayDataToLcd);

void switchAllOnIfGreatorAllOffIFLess(){
  //actualTemp = sensors.getTempCByIndex(0);
  int tempInt = 0;
  int humInt  = 0;
  
  if((myData.temp -(int)myData.temp)< 0.5){
    tempInt = (int)myData.temp;
    
  }else{
    tempInt = ((int)myData.temp) + 1;   
  }
  
    if((myData.hum -(int)myData.hum)< 0.5){
    humInt = (int)myData.hum;
  }else{
    humInt = ((int)myData.hum) + 1;   
  }
 
  if ((buttonPushCounterT < tempInt)|| (buttonPushCounterH < humInt)) {
    //delay(1000);  // added this delay to avoid instant relays kick on!!
    // will need the delays here in this void!! using millis()
    switchRelay(1,0); //switch relay1 off
    switchRelay(2,0); //switch relay2 off
    switchRelay(3,0); //switch relay3 off
    
    }
  else {
    switchRelay(1,1); //switch relay1 on
    switchRelay(2,1); //switch relay2 on
    switchRelay(3,1); //switch relay3 on
    
      }
}



TimedAction switchAllOnIfGreatorAllOffIFLessThread = TimedAction(1000,switchAllOnIfGreatorAllOffIFLess);
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
         if(C){digitalWrite(relay2, HIGH);}else{digitalWrite(relay2, LOW);}
         break;
      case 3 :
         if(C){digitalWrite(relay3, HIGH);}else{digitalWrite(relay3, LOW);}
         break;
       
   }

  
  }




void setup() {
  Serial.begin(115200);
  lcd.begin(); 
  
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);


  switchRelay(1,1); //switch relay1 on
  switchRelay(2,1); //switch relay2 on
  switchRelay(3,1); //switch relay3 on

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
  switchAllOnIfGreatorAllOffIFLessThread.check();


}
