#include <Wire.h>
void setup() {
Serial.begin (9600);
Serial.println ("Scanning I2C device...");
Wire.begin();
for (byte i = 0; i <50; i++)
{
Wire.beginTransmission (i);
if (Wire.endTransmission () == 0)
{
Serial.print ("Address found->");
Serial.print (" (0x");
Serial.print (i, HEX);
Serial.println (")");
Count++;
}
Serial.print ("Found ");
Serial.print (count, DEC);
Serial.println (" device");
} 
void loop() {}
