
#define upButt 13


void setup() {
  Serial.begin(115200);

  pinMode (upButt, INPUT_PULLUP) ;
  
  
  }

  void loop () {
  

    int upButtState = digitalRead(upButt); 
    
      Serial.println(upButtState);
      delay(1000);
  /*  if (upButtState == LOW) 
    {
        Serial.println("up pushed");
      delay(1000);
      }

      else 
      {
        Serial.println("waiting"); 
        }
          delay(1000);
*/
    }
