
#include <ezLCDLib.h>

#define LED_PIN 13

ezLCD3 lcd;

int xPos = 35; // horizontal position
int yPos = 50; // vertical position
int width = 250;
int height = 120;
int option = 4; // 1=left,2=disabled,3=right,4=center.

void setup()
{
  lcd.begin( EZM_BAUD_RATE );

  lcd.string( 1, "Left Align");
  lcd.string( 2, "Disabled");
  lcd.string( 3, "Right Align");
  lcd.string( 4, "Center Align");
  
  lcd.fontw( 1, "sans24" );
  lcd.theme( 1, 155, 152,  3, 3, 1, 0, 1, 0, 0, 0 );
 
}

void loop()
{
  lcd.cls( 0 );
  lcd.groupBox(1, xPos, yPos, width, height, 1,  1, 1 );
  delay(1000);
  lcd.cls( 0 );
  lcd.groupBox(1, xPos, yPos, width, height, 2,  1, 2 );
  delay(1000);
  lcd.cls( 0 );
  lcd.groupBox(1, xPos, yPos, width, height, 3,  1, 3 );
  delay(1000);
  lcd.cls( 0 );
  lcd.groupBox(1, xPos, yPos, width, height, 4,  1, 4 );
  delay(1000);
}

