#include <LiquidCrystal_I2C.h>

#include <Wire.h> 


// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.setCursor(0,0);  // (col,row)
  
  lcd.print("قوة");
}

void loop()
{
  // Do nothing here...
}
