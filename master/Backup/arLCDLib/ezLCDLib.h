/* 
 * @author Ken Segler
 * @author ken@earthlcd.com
 * @copyright 2013 Ken Segler
 * @version 1.0
 * @date 
 */

#ifndef _EZLCDLIB_H
#define _EZLCDLIB_H


#include <Arduino.h>
#include "Print.h"

#define EZM_BAUD_RATE 115200  //default rate for coms link to GPU, this must match startup.ezm

#define FIFO    0
#define LIFO    1
#define CLEAR   2
#define PEEK    3

#define DELETE  0
#define ENABLE  1
#define DISABLE 2
#define REDRAW  3
#define FILL    1
#define NOFILL 0
#define PRESSED 4
#define RELEASED 1
#define CANCEL  2
#define ON 1
#define OFF 0


 /** Enum for standard colors */
enum { BLACK,GRAY,SILVER,WHITE,RED,MAROON,YELLOW,OLIVE,
		LIME,GREEN,AQUA,TEAL,BLUE,NAVY,FUCHSIA,PURPLE,
		INDIANRED,LIGHTCORAL,SALMON,DARKSALMON,LIGHTSALMON,RED2,CRIMSON,FIREBRICK,
		DARKRED,PINK,LIGHTPINK,HOTPINK,DEEPPINK,MEDIUMVIOLETRED,PALEVIOLETRED,LIGHTSALMON2,
		CORAL,TOMATO,ORANGERED,DARKORANGE,ORANGE,GOLD,YELLOW2,LIGHTYELLOW,
		LEMONCHIFFON,LIGHTGOLDENRODYELLOW,PAPAYAWHIP,MOCCASIN,PEACHPUFF,PALEGOLDENROD,KHAKI,DARKKHAKI,
		LAVENDER,THISTLE,PLUM,VIOLET,ORCHID,FUCHSIA2,MAGENTA,MEDIUMORCHID,
		MEDIUMPURPLE,BLUEVIOLET,DARKVIOLET,DARKORCHID,DARKMAGENTA,PURPLE2,INDIGO,SLATEBLUE,
		DARKSLATEBLUE,GREENYELLOW,CHARTREUSE,LAWNGREEN,LIME2,LIMEGREEN,PALEGREEN,LIGHTGREEN,
		MEDIUMSPRINGGREEN,SPRINGGREEN,MEDIUMSEAGREEN,SEAGREEN,FORESTGREEN,GREEN2,DARKGREEN,YELLOWGREEN,
		OLIVEDRAB,OLIVE2,DARKOLIVEGREEN,MEDIUMAQUAMARINE,DARKSEAGREEN,LIGHTSEAGREEN,DARKCYAN,TEAL2,
		AQUA2,CYAN2,LIGHTCYAN,PALETURQUOISE,AQUAMARINE,TURQUOISE,MEDIUMTURQUISE2,DARKTURQUOISE,
		CADETBLUE,STEELBLUE,LIGHTSTEELBLUE,POWDERBLUE,LIGHTBLUE,SKYBLUE,LIGHTSKYBLUE,DEEPSKYBLUE,
		DODGERBLUE,CORNFLOWERBLUE,MEDIUMSLATEBLUE,ROYALBLUE,BLUE2,MEDIUMBLUE,DARKBLUE,NAVY2,
		MIDNIGHTBLUE,CORNSILK,BLANCHEDALMOND,BISQUE,NAVAJOWHITE,WHEAT,BURLYWOOD,TAN,
		ROSYBROWN,SANDYBROWN,GOLDENROD,DARKGOLDENROD,PERU,CHOCOLATE,SADDLEBROWN,SIENNA,BROWN,MAROON2,
		WHITE2,SNOW,HONEYDEW,MINTCREAM,AZURE,ALICEBLUE,GHOSTWHITE,WHITESMOKE,
		SEASHELL,BEIGE,OLDLACE,FLORALWHITE,IVORY,ANTIQUEWHITE,LINEN,LAVENDERBLUSH,
		MISTYROSE,GAINSBORO,LIGHTGREY,SILVER2,DARKGRAY,GRAY2,DIMGRAY,LIGHTSLATEGRAY,
		SLATEGRAY,DARKSLATEGRAY,BLACK2};
        
class Stream;       /* Forward declaration of Stream to be used
                     * in the EzLCD3 abstract class */

/** Interface to the EarthLCD ezLCD3xx line of smart displays
* Derived class from Serial so that you
* can conveniently printf(), putc(), etc to the display.
*/
class ezLCD3: public Print
{
public:
    volatile unsigned int currentWidget;
    volatile unsigned int currentInfo;
    volatile unsigned int currentData;
    volatile unsigned int timedOut;    
    volatile unsigned int error;        
    volatile unsigned long timeOutCount;  
    volatile unsigned long timeOut;
    volatile unsigned int X;    
    volatile unsigned int Y;        
    /** Create a new interface to a ezLCD3xx display
     */
    ezLCD3(void);

    /** Class destructor */
    virtual ~ezLCD3();

    /**
    * @param[in] baud baud rate defined here EZM_BAUD_RATE 115200
    *
    */
    void begin( long baud );

/************************ Library Internal Functions *****************/
    /** required for classes derived from Print
    * This allows using build in arduino print functions 
    * like println(test,HEX);
    */
    virtual size_t write(uint8_t);
    
    /**
    *
    *
    */    
    void itoa(int value, char *sp, int radix);

    /**
    *
    *
    */
    void stripSpace(char *str);

    /**
    *
    *
    */
    bool sync( void );

    /** Send a integer to the display as command
    * @param[in] i integer to send
    */
    void sendInt( int i );

    /** Get a integer from the display
    * @param[out] str string to put the data in
    *
    */
    int getInt( char *str );

    /** Get a string from the display
    * @param[out] str string to put the data in
    *
    */
    bool getString( char *str );

    /** Get a string from the display
    * @param[out] str string to put the data in
    *
    */
    bool getStringToSpace( char *str );

    /** Waits for a CR from display
    *
    */
    bool waitForCRNTO( void );
    bool waitForCR( void );

/************************ Library Internal Functions *****************/
    
    /**
     * 
     */ 
    void calibrate( void );

    /**
     * 
     */     
    unsigned char choice( char *str, unsigned char c  );
    
    /** clear the screen to black
    *
    */
    void cls(void);

    /** clear the screen with background color
    * @param[in] bcolor Background color
    */
    void cls(int bColor);

    /** clear the screen with background color and drawing color
    *
    * @param[in] bColor background color
    * @param[in] fColor drawing color
    */
    void cls(int bColor, int fColor);

    /** Set drawing color
    * @param[in] color color to draw with
    */
    void color( int color );

    /** Set drawing color
    * @param[in] color color to draw with
    */
    void colorID( unsigned char  ID, unsigned char r, unsigned char g, unsigned char b );

    /** set x y position for drawing and text
     *
     * @param[in] x is the x coordinate
     * @param[in] y is the y coordinate
     */
    void xy(int x, int y);
    
    void xy( void );
    
    unsigned int getX( void );

    unsigned int getY( void );
    
    /** set pixel at current x y
     *
     */
    void plot(void);
	void point(void);

    /** set pixel in current draw color at x y
     *
     * @param[in] x is the x coordinate
     * @param[in] y is the y coordinate
     */
    void plot(int x, int y);
    void point(int x, int y);

    /** draw line from current x y to new x y
     *
     * @param[in] x is the x coordinate
     * @param[in] y is the y coordinate
     */
    void line(int x, int y);

    /** Set line drawing type
    * @param[in] type Options: 0 = solid, 1= dotted (1 pixel spacing between dots), 2 = dashed (2 pixel spacing between dashes)
    */
    void lineType( int type );

    /** Set line drawing width
    * @param[in] width in pixels
    */
    void lineWidth( int width );

    /** Draw circle in current color at current x y
     *
     * @param[in] x location of circle
     * @param[in] y location of circle
     * @param[in] radius diameter of circle
     * @param[in] filled  true for a filled box outline if false
     */
    void circle(int radius, bool filled );
	void circle(int x, int y, int radius, bool filled );

    /**
     * Draw a pie with the specified radius, start angle and end angle.
     * \param[in]  radius    pie radius in pixels.
     * \param[in]  start     Start angle in degrees.
     * \param[in]  end       End angle in degrees.
     */
    void pie( uint16_t radius, int16_t start, int16_t end );
    
    /**
     * Draw an arc with the specified radius, start angle and end angle.
     * \param[in]  radius    Arc radius in pixels.
     * \param[in]  start     Start angle in degrees.
     * \param[in]  end       End angle in degrees.
     */
    void arc( uint16_t radius, int16_t start, int16_t end , bool fill);
    
    /** draw a box from current x y of width and heigth
     *
     * @param[in] width
     * @param[in] heigth
     * @param[in] filled  true for a filled box outline if false
     */
    void box(int width, int height, bool filled);
	void rect(int x, int y, int width, int height, bool filled);

    /**
    * Return current brightness setting.
    * \return    Brightness in 0-100.
    */
    int light();

    /** set backlight brightness
     *
     * @param[in] level is brightness 0=off 100=full in steps of 32
     */
    void light(int level);

   /**
     * Set brightness & timeout
     * \param[in]  level Brightness in 0-100.
     * \param[in]  timeout    Timeout value in minutes before dimming
     * \param[in]  timeOutBrightness 
     */
     void light( int level, unsigned long timeout, int timeOutBrightness );
    
    /** Send a command direct to display
    * @param str command string
    *
    * Example:\n
    * @code
    * lcd.sendCommand( "cls" ); // clear display
    * @endcode
    */
    void sendCommand( char *str );

    /** Return Xmax of display
    * @return int Xmax of display
    *
    */
    int getXmax( void );

    /** Return Ymax of display
    * @return int Ymax of display
    *
    */
    int getYmax( void );

    /** Return last touch x
    * @return int 
    *
    */
    int touchX( void );

    /** Return last touch y
    * @return int 
    *
    */
    int touchY( void );
    
    /** Return last touch status
    * @return int touch status
    *
    */
    int touchS( void );
    
    /** Send a string of data to display
    *
    */
    void sendString( char *str );

    /** Set stringID to display
    * @param ID string ID to write
    * @param str string to write
    *
    */
    void string( int ID, char *str );

    /** Get stringID from Display
    * @param ID string ID to read
    * @param str string to put data in
    *
    * Example:
    * @code
    * lcd.getStringID(66, ezVer);   // get ezLCD Firmware version
    * lcd.print(ezVer);             // print version
    * @endcode
    */
    void getStringID( int ID, char *str );

    /** print string at current x y
    * @param str string prints directly to display
    */
    void printString( char *str);
    
    /** print stringID at current x y
    * @param str string prints directly to display
    */
    void printStringID( char ID );

    
    /** Set widget font
    * Fonts are located in the flash drive of the display\n
    * font 0 and font 1 are builtin fonts and are much faster to draw\n
    * in the /EZUSERS/FONTS and /EZSYS/FONTS directory\n
    * use the ezLCD3xx font convert utilty to convert true type fonts
    * @param str font name
    */
    void fontw( int id, char *str);
    /**
    *
    *
    *
    */
    
    void fontw( int ID, int font);
    /**
    *
    *
    *
    */

    void font(char *str);
    /**
    *
    *
    *
    */

    void font(int font);
    /**
    *
    *
    *
    */

    void fontO( bool dir );

    /**
    *
    *
    *
    */
    void clipArea( int l, int t, int r, int b);

    /**
    *
    *
    *
    */
    void clipEnable( bool enable );

    /** Sets color themes for widgets
    *
    *
    */
    void theme(int ID, int EmbossDkColor, int  EmbossLtColor, int TextColor0, int TextColor1, int TextColorDisabled, int Color0, int Color1, int ColorDisabled, int CommonBkColor, int Fontw);

    /** 
    *
    *
    *
    */
    void drawLed( char dim, int x, int y, unsigned char colorLed, unsigned char colorHigh) ;
    /**
    *
    *
    *
    */
    void groupBox( int ID, int x, int y, int width, int height, int options, int theme, int stringID) ;
    /**
     * Draw/alter a checkBox.
     * \param[in]  id        Widget ID to assign.
     * \param[in]  x         Starting x-coordinate in pixels.
     * \param[in]  y         Starting y-coordinate in pixels.
     * \param[in]  width     Width in pixels.
     * \param[in]  height    Height in pixels.
     * \param[in]  option    Options 1=draw unchecked, 2=draw disabled, 3=draw checked, 4=redraw    
     * \param[in]  theme     Theme ID to use.
     * \param[in]  strid     String ID to use for text.
     */
    void checkBox( int id, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 uint16_t option, int theme, int strid );
    /**
     * Draw/alter a radioButton.
     * \param[in]  id        Widget ID to assign.
     * \param[in]  x         Starting x-coordinate in pixels.
     * \param[in]  y         Starting y-coordinate in pixels.
     * \param[in]  width     Width in pixels.
     * \param[in]  height    Height in pixels.
     * \param[in]  option    Options 1=draw unchecked, 2=draw disabled, 3=draw checked, 4=redraw    
     * \param[in]  theme     Theme ID to use.
     * \param[in]  strid     String ID to use for text.
     */
    void radioButton( int id, uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                 uint16_t option, int theme, int strid );

    /** Analog Meter Widget
    * Draw Analog Meter.
     * \param[in]  id        Widget ID to assign.
     * \param[in]  x         Starting x-coordinate in pixels.
     * \param[in]  y         Starting y-coordinate in pixels.
     * \param[in]  w		 Width in pixels.
     * \param[in]  h         Height in pixels.
     * \param[in]  options   Options 
	 * \param[in]  value	 Current Value
	 * \param[in]  min   	 Minimum Value
	 * \param[in]  max	     Maximum Value
     * \param[in]  theme     Theme ID to use.
     * \param[in]  strid     String ID to use for text.
	 * \param[in]  type		 Meter Value 0=360, 1=180, 2=90
    */
    void analogMeter( int ID, int x, int y, int w, int h, int options, int value, int min, int max, int theme, int stringID, int type);

    /** Touchzone Widget
    *
    *
    */
    void touchZone( int ID, int x, int y, int w, int h, bool option);

    /** Button Widget
    *
    *
    */
    void button( int ID, int x, int y, int w, int h, int option, int align, int radius, int theme, int stringID );

    /**
     * Create/alter a dial widget.
     * \param[in]  id         Widget ID to assign.
     * \param[in]  theme      Theme ID to use.
     * \param[in]  x          Starting x-coordinate in pixels.
     * \param[in]  y          Starting y-coordinate in pixels.
     * \param[in]  radius     Radius of the dial in pixels.
     * \param[in]  resolution Resolution of the dial in degrees.
     * \param[in]  initial    Initial numeric position of the dial. 
     * \param[in]  max        Maximum numeric position of the dial. 
     * \param[in]  option     1=draw, 2=disabled, 3=ring, 4=accuracy
     */
    void dial( int id, uint16_t x, uint16_t y, uint16_t radius, uint16_t option,
             	int resolution, int initial, int max, int theme );

    /** Display Bitmap
    * Display Bitmap at current x y \n
    * supports GIF BMP JPG \n
    * images are located in the /EZUSERS/FONTS and /EZSYS/FONTS directory
    * @param str filename
    */
    bool picture( char *str );

    /** Display Bitmap
    * Display Bitmap at specfied x y \n
    * supports GIF BMP JPG \n
    * images are located in the /EZUSERS/FONTS and /EZSYS/FONTS directory
    * @param x x location to start bitmap
    * @param y y location to start bitmap
    * @param str filename
    */
    bool picture(  int x, int y ,char *str);
 
	/** Display Bitmap
    * Display Bitmap at specfied x y \n
    * supports GIF BMP JPG \n
    * images are located in the /EZUSERS/FONTS and /EZSYS/FONTS directory
    * @param x x location to start bitmap
    * @param y y location to start bitmap
	* @param options 1=align center 2=downscale 3= both
    * @param str filename
    */
    bool picture(  int x, int y, int options, char *str);

    /** StaticBox Widget
    * @param ID the Widget ID to create 
    * @param x
    * @param y
    * @param w
    * @param h
    * @param option
    * @param theme
    * @param stringID
    */
    void staticText( int ID, int x, int y, int w, int h, int option, int theme, int stringID);

    /** ProgressBar Widget
    * @param ID
    * @param x
    * @param y
    * @param w
    * @param h
    * @param option
    * @param value
    * @param max
    * @param theme
    * @param stringID
    */
   
    void progressBar(int ID, int x, int y, int w, int h, int option, int value, int max, int theme, int stringID);

    /** gauge Widget
    * @param ID
    * @param x
    * @param y
    * @param w
    * @param h
    * @param option
    * @param initial
    * @param min
    * @param max
    * @param theme
    * @param stringID
    *
    *
    */
    void gauge(int ID, int x, int y, int w, int h, int option, int initial, int min, int max, int theme, int stringID);
    
    /** Slider Widget
    * @param ID
    * @param x
    * @param y
    * @param w
    * @param h
    * @param option
    * @param range
    * @param res
    * @param value
    * @param theme
    *
    */
    void slider(int ID, int x, int y, int w, int h, int option, int range, int res, int value, int theme);
    
    void digitalMeter( int id, uint16_t x, uint16_t y, uint16_t width,
                      uint16_t height, uint16_t option, int initial,
                      int digits, int dotpos,int theme );
    /** wstate
    *
    *
    *
    */
    unsigned int wstate( int ID );

    /** wstate
    *
    *
    *
    */
    void wstate( int ID, int option );

    
    /** wquiet
    *   will disable the default data comming back from the GPU
    *   on widget events
    *
    */
    void wquiet( void );
    
    /** Wvalue set widget values
    * @param ID widget ID to update
    * @param value widget value to update
    */
    void wvalue( int ID, int value);

    /** Wvalue set widget values
    * @param ID widget ID to update
    * @return widget value
    */
    unsigned int wvalue( int ID );
    
    /** Wstack is a 32 level stack of widget events
    * @param type LIFO  - Get the last widget event off the stack
    * @param type FIFO  - Get the first widget event off the stack
    * @param type CLEAR - Clear stack
    * @returns widget id and even touch, release
    */
    unsigned int wstack( int type );

    /**
     * Numerical values for the EarthSEMPL commands.
     */

    enum Commands {
        Command=             0,     /**< Direct command. */
        Clr_Screen=          2,     /**< Clear to provided color. */
        Ping=                3,     /**< Return Pong */
        zBeep=               4,     /**< Beep provided duration
                                      *(frequency fixed) */
        Light=               5,     /**< \c 0 (off) to \c 100 (on) */
        Color=               6,
        eColor_ID=           7,
        Font=                10,    /**< Font number. */
        Fontw=               11,    /**< Font number widget. */
        Font_Orient=         12,    /**< Horizontal or vertical. */
        Line_Width=          13,    /**< 1 or 3. */
        Line_Type=           14,    /**< 1=dot dot 2=dash dash. */
        XY=                  15,    /**< X and Y. */
        StringID=            16,    /**< SID ASCII String or File Name that
                                      * ends with 0. */
        Plot=                17,    /**< Place Pixel at X and Y. */
        Line=                18,    /**< Draw a line to X and Y. */
        Box=                 19,    /**< Draws a Box to X and Y optional
                                      * fill. */
        Circle=              20,    /**< Draws a Circle with Radius optional
                                      * fill */
        Arc=                 21,    /**< Draws an Arc with Radius and Begin
                                      * Angle to End Angle. */
        Pie=                 22,    /**< Draws a Pie figure with Radius and
                                      * Begin Angle to End Angle and fills
                                      * it. */
        Picture=             24,    /**< Places a Picture on display. */
        Print=               25,    /**< Places the string on display which
                                      * ends with 0. */
        Beep_Freq=           26,    /**< Set the beeper frequency. */
		Get_Pixel=			 27,    /**< Get Pixel. */
		Calibrate=           28,    /**< Calibrate touch screen. */
        zReset=              29,    /**< Reset. */
        Rec_Macro=           30,    /**< Record Macro to flash drive. */
        Play_Macro=          31,    /**< Play Macro. */
        Stop_Macro=          32,    /**< Stop Macro. */
        Pause_Macro=         33,    /**< Pause n msec. */
        Loop_Macro=          34,    /**< Loop on Macro. */
        Speed_Macro=         35,    /**< Set the macro speed. */
        ConfigIO=            37,
        IO=                  38,	/**< Set/Get IO pin state */
        Bridge=              39,	/**< Set Bridge Command. */
        Security=            40,    /**< Set drive security string. */
        Location=            41,    /**< LID Location Value. */
        Upgrade=             43,
        Run_Macro=           44,
        Parameters=          45,
        ClipEnable=          46,    /**< Set clip Enable. */
        ClipArea=            47,    /**< Set clip area. */
		Snapshot=			 48,	/**< Snapshot the screen area. */
		Cmd=				 49,	/**< Sets the Command Port. */
        /* Filesystem operations */
		Comment=			 50,     /**< Comment */
		Fsgetcwd=			 51,     /**< Get current working directory */
		Fschdir=			 52,     /**< Change directory */
		Fsmkdir=			 53,     /**< Make directory */
		Fsrmdir=			 54,     /**< Remove directory */
		Fsdir=				 55,     /**< Display directory */
		Fscopy=				 56,     /**< File copy */
		Fsrename=			 57,     /**< File rename */
		Fsremove=			 58,     /**< File remove */
		Fsmore=				 59,     /**< File dump */
		Fsattrib=            60,     /**< File Attributes */
		Fsopen=              61,     /**< File Open */
		Fswrite=             62,     /**< File Write */
		Fsread=              63,     /**< File Close */
		Fsclose=             64,     /**< File Read */
		Fsrewind=            65,     /**< File Rewind */
		Fserror=             66,     /**< File Error */
		Fsseek=              67,     /**< File Seek */
		Fseof=               68,     /**< File EOF */
		Fstell=              69,     /**< File Tell */
		
        /* Widget commands */
        Set_Button=          70,    /**< Widget Button. */
        Set_CheckBox=        71,    /**< Widget Checkbox. */
        Set_Gbox=            72,    /**< Widget Group Box. */
        Set_RadioButton=     73,    /**< Widget Radio Button. */
        Set_DMeter=          74,    /**< Widget Digital Meter. */
        DMeter_Value=        75,    /**< Set DMeter value. */
        Set_AMeter=          76,    /**< Widget Analog Meter. */
        AMeter_Value=        77,    /**< Set AMeter value. */
        AMeter_Color=        78,    /**< Set AMeter color */
        Set_TouchZone=       79,    /**< touch zone       */
        Set_Dial=            80,    /**< Widget RoundDial. */
		Draw_LED=			 81,    /**< Draw LED. */
        Set_Slider=          82,    /**< Widget Slider. */
        Set_Progress=        85,    /**< Widget Progress bar. */
        Progress_Value=      86,    /**< Progress value. */
        Set_StaticText=      87,    /**< Widget Static text. */
        StaticText_Value=    88,    /**< Static text Value. */
        Choice=              89,    /**< Widget get choice. */
        Widget_Theme=        90,    /**< Widget Scheme. */
        Widget_Values=       91,    /**< Widget Values (Slider and Dial in this version).*/
        Widget_State=        92,    /**< Widget State (Button, checkbox, radiobutton in this version).*/
		Set_Gauge=			 93,	/**< Widget Gauge. */
		Gauge_Value=		 94,	/**< Set Gauge Value. */
		GetWidget_Values=    95,    /**< Get widget values. */        
        // no id returns the id of the last touched
        Xmax=                100,   /**< Return Xmax width. */
        Ymax=                101,   /**< Return Ymax height. */
        Wait=                102,   /**< Wait for touch. */
        Waitn=               103,   /**< Wait for no touch. */
        Waitt=               104,   /**< Wait for touch. */
        Threshold=           105,   /**< Touch threshold. */
        Verbose=             106,   /**< Controls the verbose mode. */
        Lecho=               107,   /**< Controls the echo mode. */
        Xtouch=              110,   /**< return touchX. */
        Ytouch=              111,   /**< return touchY. */
        Stouch=              112,   /**< return touchS. */
        Wquiet=              113,
        Wstack=              114,
        Fshelp=              254
    };
protected:
private:
};

#endif