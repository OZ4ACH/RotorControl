#include <ezLCDLib.h>

ezLCD3 lcd; // create lcd object

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(); 
}

void loop(){
  lcd.picture(0,0,"tulips.gif"); 
  delay(100);
  lcd.picture(0,0,"winter.gif"); 
  delay(100);
  lcd.picture(0,0,"Sunset.gif"); 
  delay(100);
}

