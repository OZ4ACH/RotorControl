#include <ezLCDLib.h>
ezLCD3 lcd; // create lcd object

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(); 
}

void loop()
{
  lcd.color(random(0,168));
  int x = random(0,319); 
  int y = random(0,239);
  lcd.plot(x,y);  
}

