/**
 * @author Ken Segler
 * @author ken@earthlcd.com
 * @copyright 2013 Ken Segler
 * @version 1.0
 * @date 
*/

#include "ezLCDLib.h"

char buf[64];
 
ezLCD3::~ezLCD3()
{
}
 
ezLCD3::ezLCD3(void) //: Serial(void)
{
}

void ezLCD3::begin( long baud )
{
    timeOutCount = 665000;
    Serial.begin( baud );
    Serial.setTimeout(0xf0000000);
    while(Serial.available())
        Serial.read();
        while(!sync());
}

/* itoa:  convert n to characters in s */
void ezLCD3::itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;
    int sign;

    sign = (radix == 10 && value < 0);
    if (sign)   v = -value;
    else    v = (unsigned)value;

    while (v || tp == tmp) {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
            *tp++ = i+'0';
        else
            *tp++ = i + 'a' - 10;
    }

    if (sign)
        *sp++ = '-';
    while (tp > tmp)
        *sp++ = *--tp;
}

bool ezLCD3::sync( void )
{
    char c=0;
    unsigned long timeOut=0;
    timedOut = false;
    while(Serial.available())
        Serial.read();
        Serial.write('\r');
        Serial.flush();
    while( c!='\r' && ++timeOut < timeOutCount) // wait for char or timeout
        c=Serial.read();   
    if(timeOut > timeOutCount || c!='\r') {
        timedOut = true;        
        return false;
    }else
        return true;
}

bool ezLCD3::waitForCRNTO( void )
{
    char c=0;
    timeOut=0;
    timedOut=false;
    error=false;
    while( c!='\r' && c!=0x31 )
        c=Serial.read(); 
    if(c==0x31) {
		while(!Serial.available());
		c=Serial.read(); 
		error = true;
    }else 
		return true;
}

bool ezLCD3::waitForCR( void )
{
    char c=0;
    timeOut=0;
    timedOut=false;
    error=false;
    while( c!='\r' && c!=0x31 && ++timeOut < timeOutCount)
        c=Serial.read(); 
    if( timeOut > timeOutCount || c!='\r' || c==0x31 ) {
		if(c==0x31) {
			while(!Serial.available());
			c=Serial.read(); 
			error = true;
		}
		timedOut = true;
		while(!sync());
    }else 
		return true;
}

void ezLCD3::sendInt( int i )
{
    char value[5]= {0, 0, 0, 0, 0};

    itoa(i, value, 10);
    if(value[0]) Serial.write(value[0]);
    if(value[1]) Serial.write(value[1]);
    if(value[2]) Serial.write(value[2]);
    if(value[3]) Serial.write(value[3]);
    if(value[4]) Serial.write(value[4]);
}

void ezLCD3::stripSpace(char* str)
{
    char* i = str;
    char* j = str;
    while(*j != 0) {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0;
}

int ezLCD3::getInt( char *str )
{
    stripSpace(str);
    return atoi(str );
}

bool ezLCD3::getString( char *str )
{
    char c=0;
    unsigned long timeOut=0;
    timedOut =false ;
    do {
    if(Serial.available()) {
        c = Serial.read();
        *str++ = c;
        }
        timeOut++;
    } while( c!='\r' && timeOut!=timeOutCount );
    *str--;
    *str = 0;
    if(timeOut > timeOutCount)
        timedOut = true;
    if( c=='\r' ||  timeOut!=timeOutCount)
        return true;
    else
        return false;
}

bool ezLCD3::getStringToSpace( char *str )
{
    char c=0;
    unsigned long timeOut=0;
    timedOut =false ;
    do {
    if(Serial.available()) {
        c = Serial.read();
        *str++ = c;
        }
        timeOut++;
    } while(c!='\r' && c!=' ' && timeOut!=timeOutCount);
    *str--;
    *str = 0;
    if(timeOut > timeOutCount)
        timedOut = true;
    if( c=='\r' || c==' ' || timeOut!=timeOutCount)
        return true;
    else
        return false;
}

void ezLCD3::calibrate( void ) {
    sendInt(Calibrate);
    Serial.write('\r');
    waitForCRNTO();
}

void ezLCD3::cls()
{
    sendInt(Clr_Screen);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::cls(int bColor)
{
    sendInt(Clr_Screen);
    Serial.write(' ');
    sendInt(bColor);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::cls(int bColor, int fColor)
{
    sendInt(Clr_Screen);
    Serial.write(' ');
    sendInt(bColor);
    Serial.write(' ');
    sendInt(fColor);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::color(int fColor)
{
    sendInt(Color);
    Serial.write(' ');
    sendInt(fColor);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::colorID( unsigned char  ID, unsigned char r, unsigned char g, unsigned char b )
{
    sendInt(eColor_ID);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(r);
    Serial.write(' ');
    sendInt(g);
    Serial.write(' ');
    sendInt(b);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::xy(int x, int y)
{
    sendInt(XY);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::xy( void )
{
    sendInt(XY);
    Serial.write('\r');
    getStringToSpace( buf );
    X = getInt( buf );    
    getStringToSpace( buf );
    Y = getInt( buf );
}

unsigned int ezLCD3::getX( void )
{
    sendInt(XY);
    Serial.write('\r');
    getStringToSpace( buf );
    X = getInt( buf );    
    getStringToSpace( buf );
    Y = getInt( buf );
    return X;
}

unsigned int ezLCD3::getY( void )
{
    sendInt(XY);
    Serial.write('\r');
    getStringToSpace( buf );
    X = getInt( buf );    
    getStringToSpace( buf );
    Y = getInt( buf );
    return Y;
}

void ezLCD3::plot( void )
{
    sendInt(Plot);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::point( void )
{
    sendInt(Plot);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::plot( int x, int y )
{
    sendInt(Plot);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::point( int x, int y )
{
    sendInt(Plot);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::line(int x, int y )
{
    sendInt(Line);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::lineType( int type )
{
    sendInt(Line_Type);
    Serial.write(' ');
    sendInt(type);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::lineWidth( int width )
{
    sendInt(Line_Width);
    Serial.write(' ');
    sendInt(width);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::arc( uint16_t radius, int16_t start, int16_t end, bool fill ) 
{
    sendInt(Arc);
    Serial.write(' ');
    sendInt(radius);
    Serial.write(' ');
    sendInt(start);
    Serial.write(' ');
    sendInt(end);
    Serial.write(' ');
    sendInt(fill);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::circle(int radius, bool filled )
{
    sendInt(Circle);
    Serial.write(' ');
    sendInt(radius);
    if(filled) {
        Serial.write(' ');
        Serial.write('f');
    }
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::circle(int x, int y, int radius, bool filled )
{
	sendInt(XY);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write('\r');
    waitForCR();

    sendInt(Circle);
    Serial.write(' ');
    sendInt(radius);
    if(filled) {
        Serial.write(' ');
        Serial.write('f');
    }
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::box(int width, int height, bool filled)
{
    sendInt(Box);
    Serial.write(' ');
    sendInt(width);
    Serial.write(' ');
    sendInt(height);
    if(filled) {
        Serial.write(' ');
        Serial.write('f');
    }
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::rect(int x, int y, int width, int height, bool filled)
{
	sendInt(XY);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write('\r');
    waitForCR();

	sendInt(Box);
    Serial.write(' ');
    sendInt(width);
    Serial.write(' ');
    sendInt(height);
    if(filled) {
        Serial.write(' ');
        Serial.write('f');
    }
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::pie( uint16_t radius, int16_t start, int16_t end )
{
    sendInt(Pie);
    Serial.write(' ');
    sendInt(radius);
    Serial.write(' ');
    sendInt(start);
    Serial.write(' ');
    sendInt(end);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::light(int level)
{
     sendInt( Light );
     Serial.write(' ');
     sendInt( level );
     Serial.write('\r');
     waitForCR();
}

int ezLCD3::light( void )
{
    sendInt( Light );
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}

void ezLCD3::light( int level, unsigned long timeOut, int timeOutBrightness )
{
     sendInt( Light );
     Serial.write(' ');
     sendInt( level );
     Serial.write(' ');
     sendInt( timeOut );
     Serial.write(' ');
     sendInt( timeOutBrightness );
     Serial.write('\r');
     waitForCR();
}

int ezLCD3::getXmax( void )
{
    sendInt(Xmax);
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}

int ezLCD3::getYmax( void )
{
    sendInt(Ymax);
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}

int ezLCD3::touchX( void )
{
    sendInt(Xtouch);
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}

int ezLCD3::touchY( void )
{
    sendInt(Ytouch);
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}

int ezLCD3::touchS( void )
{
    sendInt(Stouch);
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}

void ezLCD3::sendString( char *str )
{
    unsigned char c;
    while( (c = *str++) )
        Serial.write(c);
}

void ezLCD3::string( int ID, char *str )
{
    sendInt(StringID);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    Serial.write('\"');
    sendString(str);
    Serial.write('\"');
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::getStringID( int ID , char *str)
{
    sendInt(StringID);
    Serial.write(' ');
    sendInt(ID);
    Serial.write('\r');
    getString(str);
}

void ezLCD3::printString( char *str )
{
    sendInt(Print);
    Serial.write(' ');
    Serial.write('\"');
    sendString(str);
    Serial.write('\"');
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::printStringID( char ID )
{
    sendInt(Print);
    Serial.write(' ');
    sendInt(ID);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::fontw( int ID, char *str)
{
    sendInt(Fontw);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendString(str);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::fontw( int ID, int font)
{
    sendInt(Fontw);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(font);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::font( char *str)
{
    sendInt(Font);
    Serial.write(' ');
    sendString(str);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::font( int font )
{
    sendInt(Font);
    Serial.write(' ');
    sendInt(font);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::fontO( bool dir )
{
    sendInt(Font_Orient);
    Serial.write(' ');
    if(dir)
        Serial.write('1');
    else
        Serial.write('0');
    Serial.write('\r');
    waitForCR();
}

unsigned char ezLCD3::choice( char *str, unsigned char c ) {
    sendInt(Choice);
    Serial.write(' ');
    sendString(str);
    Serial.write(' ');
    sendInt(c);
    Serial.write(' ');    
    Serial.write('\r');
    timeOutCount = 0xFFFFFFFF;
    do {
		getString( buf );
		currentData=strlen(buf);
    }while(currentData == 0);
    timeOutCount = 65000;
    return getInt( buf );
}

void ezLCD3::clipArea( int l, int t, int r, int b)
{
    sendInt(ClipArea);
    Serial.write(' ');
    sendInt(l);
    Serial.write(' ');
    sendInt(t);
    Serial.write(' ');
    sendInt(r);
    Serial.write(' ');
    sendInt(b);
    Serial.write(' ');
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::clipEnable( bool enable)
{
    sendInt(ClipEnable);
    Serial.write(' ');
    if(enable)
        Serial.write('1');
    else
        Serial.write('0');
    Serial.write('\r');
}

void ezLCD3::drawLed( char dim, int x, int y, unsigned char colorLed, unsigned char colorHigh) 
{
  xy(x,y);
  color(DIMGRAY);
  circle(dim,NOFILL);
  color(WHITE);
  arc(dim+1, 145,270,NOFILL); 
 
  color(colorLed);
  circle(dim-2,FILL);
  color(colorHigh);
  arc(dim-4,180,270,NOFILL);
  arc(dim-5,180,270,NOFILL);
}

void ezLCD3::theme(int ID, int EmbossDkColor, int  EmbossLtColor, int TextColor0, int TextColor1, int TextColorDisabled, int Color0, int Color1, int ColorDisabled, int CommonBkColor, int Fontw)
{
    sendInt(Widget_Theme);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(EmbossDkColor);
    Serial.write(' ');
    sendInt(EmbossLtColor);
    Serial.write(' ');
    sendInt(TextColor0);
    Serial.write(' ');
    sendInt(TextColor1);
    Serial.write(' ');
    sendInt(TextColorDisabled);
    Serial.write(' ');
    sendInt(Color0);
    Serial.write(' ');
    sendInt(Color1);
    Serial.write(' ');
    sendInt(ColorDisabled);
    Serial.write(' ');
    sendInt(CommonBkColor);
    Serial.write(' ');
    sendInt(Fontw);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::groupBox( int ID, int x, int y, int w, int h, int options, int theme, int stringID)
{
    sendInt(Set_Gbox);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(options);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::checkBox( int ID, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                 uint16_t options, int theme, int stringID )
{
    sendInt(Set_CheckBox);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(options);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();    
}

void ezLCD3::radioButton( int ID, uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                 uint16_t options, int theme, int stringID )
{
    sendInt(Set_RadioButton);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(options);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();    
}

void ezLCD3::analogMeter( int ID, int x, int y, int w, int h, int options, int value, int min, int max, int theme, int stringID, int type)
{
    sendInt(Set_AMeter);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(options);
    Serial.write(' ');
    sendInt(value);
    Serial.write(' ');
    sendInt(min);
    Serial.write(' ');
    sendInt(max);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write(' ');
    sendInt(type);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::touchZone( int ID, int x, int y, int w, int h, bool option)
{
    sendInt(Set_TouchZone);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    if(option)
        sendInt('1');
    else
        sendInt('0');
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::button( int ID, int x, int y, int w, int h, int option, int align, int radius, int theme, int stringID )
{
    sendInt(Set_Button);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(align);
    Serial.write(' ');
    sendInt(radius);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::dial( int ID, uint16_t x, uint16_t y, uint16_t radius, uint16_t option, 
                    int resolution, int initial, int max, int theme )
{
    sendInt(Set_Dial);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(radius);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(resolution);
    Serial.write(' ');
    sendInt(initial);
    Serial.write(' ');
    sendInt(max);
    Serial.write(' ');
    sendInt(theme);
    Serial.write('\r');
    waitForCR();
}

bool ezLCD3::picture( char *str )
{
    sendInt(Picture);
    Serial.write(' ');
    Serial.write('\"');
    sendString(str);
    Serial.write('\"');
    Serial.write('\r');
    timeOutCount = 2000000;
    waitForCR();
    timeOutCount = 65000;
    return timedOut;
}

bool ezLCD3::picture( int x, int y, char *str)
{
    sendInt(Picture);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendString(str);
    Serial.write('\r');
    timeOutCount = 2000000;
    waitForCR();
    timeOutCount = 65000;
    return timedOut;
}

bool ezLCD3::picture( int x, int y, int options, char *str)
{
    sendInt(Picture);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(options);
    Serial.write(' ');
    sendString(str);
    Serial.write('\r');
    timeOutCount = 2000000;
    waitForCR();
    timeOutCount = 65000;
    return timedOut;
}

void ezLCD3::progressBar(int ID, int x, int y, int w, int h, int option, int value, int max, int theme, int stringID)
{
    sendInt(Set_Progress);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(value);
    Serial.write(' ');
    sendInt(max);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::gauge(int ID, int x, int y, int w, int h, int option, int initial, int min, int max, int theme, int stringID)
{
    sendInt(Set_Gauge);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(initial);
    Serial.write(' ');
    sendInt(min);
    Serial.write(' ');
    sendInt(max);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::slider(int ID, int x, int y, int w, int h, int option, int range, int res, int value, int theme)
{
    sendInt(Set_Slider);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(range);
    Serial.write(' ');
    sendInt(res);
    Serial.write(' ');
    sendInt(value);
    Serial.write(' ');
    sendInt(theme);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::staticText( int ID, int x, int y, int w, int h, int option, int theme, int stringID)
{
    sendInt(Set_StaticText);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(theme);
    Serial.write(' ');
    sendInt(stringID);
    Serial.write('\r');
    waitForCR();
}

void ezLCD3::digitalMeter( int ID, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t option, int initial,int digits, int dotpos,int theme )
{    
	sendInt(Set_DMeter);
    Serial.write(' ');
    sendInt(ID);
    Serial.write(' ');
    sendInt(x);
    Serial.write(' ');
    sendInt(y);
    Serial.write(' ');
    sendInt(w);
    Serial.write(' ');
    sendInt(h);
    Serial.write(' ');
    sendInt(option);
    Serial.write(' ');
    sendInt(initial);
    Serial.write(' ');
    sendInt(digits);
    Serial.write(' ');
    sendInt(dotpos);
    Serial.write(' ');
    sendInt(theme);
	Serial.write('\r');
    waitForCR();
}

unsigned int ezLCD3::wstack( int type )
{
    unsigned int temp=0;
    sendInt( Wstack );
    Serial.write(' ');
    sendInt( type );
    Serial.write('\r');
    getStringToSpace( buf );
    currentWidget = getInt( buf );
    getStringToSpace( buf );
    currentInfo = getInt( buf );
    getStringToSpace( buf );
    currentData = getInt( buf );
    return currentWidget;
}

void ezLCD3::wquiet( void )
{
     sendInt( Wquiet);
     Serial.write('\r');   
     waitForCR();
}

unsigned int ezLCD3::wvalue( int ID )
{
    sendInt( GetWidget_Values );
    Serial.write(' ');
    sendInt( ID );
    Serial.write('\r');
    getString( buf );
    return getInt( buf );
}
    
void ezLCD3::wvalue( int ID, int value)
{
    sendInt( Widget_Values );
    Serial.write(' ');
    sendInt( ID );
    Serial.write(' ');
    sendInt( value );
    Serial.write('\r');
    waitForCR();
}

unsigned int ezLCD3::wstate( int ID)
{
    sendInt( Widget_State );
    Serial.write(' ');
    sendInt( ID );
    Serial.write('\r');
    getString( buf );
    return strtoul(buf,NULL,16); 
}

void ezLCD3::wstate( int ID, int option )
{
    sendInt( Widget_State );
    Serial.write(' ');
    sendInt( ID );
    Serial.write(' ');
    sendInt( option );
    Serial.write('\r');
	waitForCR();
}

void ezLCD3::sendCommand(char *str)
{
    sendString(str);
    Serial.write('\r');
    waitForCR();
}

size_t ezLCD3::write( uint8_t c)
{
	sendInt(Print);
    Serial.write(' ');
    Serial.write('\"');
    if(c =='\n') {
		Serial.write(0x5c);
		Serial.write(0x6e);
		}
	else if (c=='\r') {
		Serial.write(0x5c);
		Serial.write(0x72);
		}
	else {
	Serial.write(c);
	}
    Serial.write('\"');
    Serial.write('\r');
    waitForCR();    
    return(c);
}



