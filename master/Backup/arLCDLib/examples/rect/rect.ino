#include <ezLCDLib.h>

ezLCD3 lcd; // create lcd object

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(); 
 }

void loop(){
   lcd.cls();
   int x=2;
  int y=2; 
  int width = 300;
  int height = 200;
 for(int i=0; i < 100; i++)
  {
     lcd.color(i);  
     lcd.xy(x, y);
     lcd.box( width, height, NOFILL );  //draw box from the previous xy location
     x = x + 2;
     y = y + 2;
     width = width -4;
     height = height -4;
     delay(100);
  } 

}

