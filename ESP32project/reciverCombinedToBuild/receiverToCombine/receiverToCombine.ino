
#include <LiquidCrystal_I2C.h>

int lcdColumns = 20;
int lcdRows    =  4;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 




#include <esp_now.h>
#include <WiFi.h>



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

  Serial.print("temp=");
  Serial.println(myData.temp);
  
  Serial.print(" RH%=");
  Serial.println(myData.hum);

  // test dumping lcd output here!!



  
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

    // initialize LCD
  lcd.begin();     //lcd.init();  this is used for older lcd libraries
   // turn on LCD backlight                      
  lcd.backlight();
  
  // Set device as a Wi-Fi Station
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



  
  lcd.setCursor(0,0);

  lcd.print(F("RH%="));
  lcd.print(myData.hum);
 
  lcd.setCursor(10,0);
  lcd.print(F(" T="));
  lcd.print(myData.temp);
  lcd.print(F("C"));

}
