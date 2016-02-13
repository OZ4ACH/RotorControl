#include <ezLCDLib.h>

#define LED_PIN 13

ezLCD3 lcd; // create lcd object using pins 10 & 11

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.fontw( 0, "sans24" );
  lcd.theme( 2, 5, 20, 3, 3, 3, 4, 4, 4, 2, 1 );
  
  pinMode( LED_PIN, OUTPUT );
  digitalWrite( LED_PIN, LOW );
}
  
void loop()
{
  lcd.cls(BLACK);
 
  int result = lcd.choice( "\"Got Milk\"", 1 );  
  if(result == 1 )
     digitalWrite( LED_PIN, HIGH ); // turn LED on
  else if(result == 0 ) 
     digitalWrite( LED_PIN, LOW ); // turn LED off
  lcd.cls(BLACK);
  lcd.xy(50,100);
  if(result ==1) lcd.printString("Pressed YES");
  if(result ==0) lcd.printString("Pressed NO");  
  if(result ==255) lcd.printString("Pressed CANCEL");  
  delay(2000);
}