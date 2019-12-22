int relay1 = 3;  // Grey
int high = 2;
int high2 = 5;
void setup() {
  // put your setup code here, to run once:

  pinMode(relay1, OUTPUT);
  pinMode(high, OUTPUT);
  digitalWrite(high, HIGH); 
  pinMode(high2, OUTPUT);
  digitalWrite(high2, HIGH); 

}

void loop() {
  // put your main code here, to run repeatedly:


  digitalWrite(relay1, HIGH); 
  delay(1000);
  
  digitalWrite(relay1, LOW); 
  delay(1000);
}
