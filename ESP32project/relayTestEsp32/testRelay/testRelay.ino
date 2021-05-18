

// &&  logical AND
// || logical OR
// ! logical NOT
// != not euqal 
// == equal 
// <= >= 
const int relay = 26;

const int temp = 14;

const int uTemp = 15;

const int hum = 14;

const int uHum = 15;

const int maxtime = 12;

const int nowTime = 3;


void setup() {
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
}

void loop() {

  if (((uTemp <= temp) || (uHum <= hum)) && (nowTime <= maxtime)){
      digitalWrite(relay, LOW);
  Serial.println("Current Flowing");
  delay(5000); 
  }
  else {digitalWrite(relay, HIGH);
  Serial.println("Current not Flowing");
  delay(5000);

  }
    
  
  /*
  // Normally Open configuration, send LOW signal to let current flow
  // (if you're usong Normally Closed configuration send HIGH signal)
  digitalWrite(relay, LOW);
  Serial.println("Current Flowing");
  delay(5000); 
  
  // Normally Open configuration, send HIGH signal stop current flow
  // (if you're usong Normally Closed configuration send LOW signal)
  digitalWrite(relay, HIGH);
  Serial.println("Current not Flowing");
  delay(5000);*/
}
