#include <ezLCDLib.h>

ezLCD3 lcd;

void setup()
{
  lcd.begin( EZM_BAUD_RATE );
  lcd.cls(BLACK);
  lcd.color(WHITE);
  lcd.font( 0 );
  lcd.println("hello, world!");
  lcd.println(65);
  lcd.println();
  lcd.println(65);
  lcd.printString("65 in Dec ");
  lcd.println(65,DEC);
  lcd.printString("65 in Hex ");
  lcd.println(65,HEX);
  lcd.printString("65 in Oct ");
  lcd.println(65,OCT);
  lcd.printString("65 in Bin ");
  lcd.println(65,BIN);
  lcd.printString("Pi ");
  lcd.println(3.14159);
}

void loop() {}
