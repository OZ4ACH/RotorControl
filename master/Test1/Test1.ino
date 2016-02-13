#include <ezLCDLib.h>

ezLCD3 lcd; // create lcd object

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(); 
  lcd.light(70); 
  lcd.picture(0,0,"Map2.gif"); 
  
}

void loop(){






}

