// code is perfect and running!

#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;

int lcdRows    =  2;

LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); 

#include <esp_now.h>

#include <WiFi.h>

#include <Wire.h>

#include "DHT.h"

#define DHTPIN 4    

#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);


uint8_t broadcastAddress[] = {0xF0, 0x08, 0xD1, 0xD3, 0x1E, 0x0C};

//{0xF0, 0x08 , 0xD1 ,0xD8 , 0x4F , 0x50};


//0x3F

typedef struct struct_message {
  
  float temp;    // when called store temperature in it

  float hum;
  
} struct_message;


// Create a struct_message called myData

struct_message myData;


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
 
  
}



void setup() {
  // Init Serial Monitor
  
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    
    return;
  }
   dht.begin();
}
 
void loop() {

  delay(2000);
  
  float h = dht.readHumidity();
 
  float t = dht.readTemperature();

  myData.hum  = h;
  
  myData.temp = t;

  

  lcd.setCursor(0,0);

  lcd.print(F("H%="));
  lcd.print(h);
  /*lcd.print(F("% "));*/
  lcd.setCursor(9,0);
  lcd.print(F("T="));
  lcd.print(t);
  lcd.print(F("C"));

  Serial.println(t);

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
    
  }
  else {
    Serial.println("Error sending the data");
    
  }
  delay(2000);
}
