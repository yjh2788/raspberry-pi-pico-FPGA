#ifndef TFT_h
#define TFT_h
#include "hardware/spi.h"
#include "myfunc.h"
#include <math.h>
#include <stdint.h>
#include "TFT_LCD_reg.h"
#include "ov7670.h"

#define __DELAY_BACKWARD_COMPATIBLE__
#define TFTWIDTH 320
#define TFTHEIGHT 480
#define TFTLCD_DELAY 250

#define TFT_8BIT
//#define TFT_SPI 

#ifdef TFT_8BIT
#include "BUS_control.h"

#define TFT_RD 16
#define TFT_WR 17
#define TFT_DC 18
#define TFT_CS 19 /// tft_lcd chip select
//#define TFT_RST 21


#elif defined(TFT_SPI)
#define TFT_RST 21
#define TFT_CS 22 /// tft_lcd chip select
#define TFT_DC 20
#define TFT_MISO 16
#define TFT_MOSI 19
#define TFT_SCL 18
#endif

#define TFT_RGB 0x08
#define TFT_BGR 0x00
#define TFT_degree0 0x00
#define TFT_degree90 0x60
#define TFT_degree180 0xD0
#define TFT_degree270 0xA0
#define TFT_Y_INVERT 0x80
#define TFT_X_INVERT 0x40
#define TFT_XY_EXCHANGE 0x20


#define READ_BIT 0x80

class TFT_LCD
{
private:
    inline void cs_select();
    inline void cs_deselect();
    inline void DC_DATA();
    inline void DC_COMMAND();
public:
    TFT_LCD();
    ~TFT_LCD();

#ifdef TFT_8BIT
private:
    inline void WR_LOW();
    inline void WR_HIGH();
    inline void write8(uint8_t data);
    inline void write16(uint16_t data);
public: 
    void init();

#elif defined(TFT_SPI)
private:
    inline void spi_transfer_byte(uint8_t byte);
    inline void spi_transfer_word(uint16_t word);
public:
    spi_inst_t *m_spi;
    uint32_t m_baud;
    void init(spi_inst_t *spi, uint32_t baud);    
#endif  
  
    uint8_t m_degree;
    uint8_t m_color;

    void reg_init();
    void setRotation(uint8_t m);
    void imshow(Array<uint8_t> arr, uint8_t res);
    void imshow(uint8_t* arr , uint8_t res);
    void imshow(uint8_t* arr, int width, int height);
    void imshow(uint8_t* arr, int x, int y, int width, int height);
    void sendWord(uint8_t IR, uint16_t data);
    void sendByte(uint8_t IR, uint8_t data);
    void sendcommand(uint8_t IR);
    void sendDataByte(uint8_t data);
    void sendDataWord(uint16_t data);
    void setBGR();
    void setRGB();
    // void TFT_clear();
    void clear_screen(void); // TFT-LCD clear screen with black color
    // void TFT_color_screen(U16 color);		// TFT-LCD full screen color
    void GRAM_address(U16 xPos, U16 yPos); // set GRAM address of TFT-LCD
    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);
    void color_screen(U16 color);
    void screen_mode(uint8_t mode); // set screen direction mode
    void TFT_pixel(U16 xPos, U16 yPos, U16 color);
    void writeFillRect(int16_t x, int16_t y, int16_t size_x, int16_t size_y, uint16_t color);
    void writeFastVLine(int16_t x, int16_t y, int16_t i, uint16_t bg);
    void xy(U08 xChar, U08 yChar);            // set character position
    void color(U16 colorfore, U16 colorback); // set foreground and background color
    void drawChar(int16_t x, int16_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y); 
    void drawstring(int16_t x, int16_t y, char* str, uint16_t text_color, uint16_t background_color, uint8_t size_x, uint8_t size_y);
    void string_size(int16_t xChar, int16_t yChar, int16_t colorfore, int16_t colorback, char  *str,int8_t width, int8_t height );
    void string(int16_t xChar, int16_t yChar, int16_t colorfore, int16_t colorback, char *str);	// write TFT-LCD string
    void TFT_English(U08 code);                // write a English ASCII character
    unsigned int KS_code_conversion(U16 KSSM); // convert KSSM(�ϼ���) to KS(������)
    void TFT_Korean(U16 code);                 // write a Korean character
    void TFT_cursor(U16 cursor_color);         // set cursor and color
    void TFT_outline(U16 outline_color);       // set outline and color
    unsigned int Unicode_to_KS(U16 unicode);
    void unsigned_decimal(U32 number, U08 zerofill, U08 digit);      // display unsigned decimal number
    void signed_decimal(S32 number, U08 zerofill, U08 digit);        // display signed decimal number
    void hexadecimal(U32 number, U08 digit);                         // display hexadecimal number
    void _0x_hexadecimal(U32 number, U08 digit);                     // display hexadecimal number with 0x
    void unsigned_float(float number, U08 integral, U08 fractional); // display unsigned floating-point number
    void signed_float(float number, U08 integral, U08 fractional);   // display signed floating-point number
    //
    void drawSLine(int16_t x, int16_t y, int16_t length, uint16_t color);
    void Line(S16 x1, S16 y1, S16 x2, S16 y2, U16 color);            // draw a straight line
    void Rectangle(S16 x1, S16 y1, S16 x2, S16 y2, U16 color);       // draw a rectangle
    void Block(S16 x1, S16 y1, S16 x2, S16 y2, U16 color, U16 fill); // draw a rectangle with filled color
    void Circle(S16 x1, S16 y1, S16 r, U16 color);                   // draw a circle
    void Sine(S16 peak, U08 mode, U16 color);
};

#endif