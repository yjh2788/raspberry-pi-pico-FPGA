#include "TFT.h"
#include "myfunc.h"
#include "Array.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "TFT_font.h"
#include "pico/binary_info/code.h"


unsigned char ScreenMode = 'P';         // screen mode(P=portrait, L=landscape)
unsigned char LineLimit = 40;           // character length of line (30 or 40)
unsigned char KoreanBuffer[32] = {0};   // 32 byte Korean font buffer
unsigned char xcharacter, ycharacter;   // xcharacter(0-29), ycharacter(0-39) for portrait
                                        // xcharacter(0-39), ycharacter(0-29) for landscape
unsigned int foreground, background;    // foreground and background color
unsigned char cursor_flag = 0;          // 0 = cursor off, 1 = cursor on
unsigned char xcursor = 0, ycursor = 0; // cursor position
unsigned int cursor = 0;                // cursor color
unsigned char outline_flag = 0;         // 0 = outline off, 1 = outline on
unsigned int outline = 0;               // outline color

TFT_LCD::TFT_LCD()
{
}
TFT_LCD::~TFT_LCD()
{
}
#ifdef TFT_SPI
//***********************************************************************************
// TFT_SPI
inline void TFT_LCD::cs_select()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_CS, 0); // Active low
  asm volatile("nop \n nop \n nop");
}

inline void TFT_LCD::cs_deselect()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_CS, 1);
  asm volatile("nop \n nop \n nop");
}

inline void TFT_LCD::DC_DATA()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_DC, 1); // data->high
  asm volatile("nop \n nop \n nop");
  sleep_us(100);
}

inline void TFT_LCD::DC_COMMAND()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_DC, 0); // command -> low
  asm volatile("nop \n nop \n nop");
  sleep_us(100);
}

inline void TFT_LCD::spi_transfer_byte(uint8_t byte)
{
  spi_write_blocking(m_spi, &byte, 1);
}

inline void TFT_LCD::spi_transfer_word(uint16_t word)
{
  uint8_t tmp = word >> 8;
  uint8_t tmp2 = word & 0xff;
  spi_write_blocking(m_spi, &tmp, 1);
  spi_write_blocking(m_spi, &tmp2, 1);
  // spi_write16_blocking(m_spi, &word, 1);
}

void TFT_LCD::sendWord(uint8_t IR, uint16_t data)
{
  cs_select();
  DC_COMMAND();
  spi_transfer_byte(IR);
  DC_DATA();
  spi_transfer_word(data);
  cs_deselect();
}
void TFT_LCD::sendByte(uint8_t IR, uint8_t data)
{
  cs_select();
  DC_COMMAND();
  spi_transfer_byte(IR);
  DC_DATA();
  spi_transfer_byte(data);
  cs_deselect();
}

void TFT_LCD::sendcommand(uint8_t IR)
{
  cs_select();
  DC_COMMAND();
  spi_transfer_byte(IR);
  cs_deselect();
}
void TFT_LCD::sendDataByte(uint8_t data)
{
  cs_select();
  DC_DATA();
  spi_transfer_byte(data);
  cs_deselect();
}
void TFT_LCD::sendDataWord(uint16_t data)
{
  cs_select();
  DC_DATA();
  spi_transfer_word(data);
  cs_deselect();
}

//*****************------TFT initial-------------------------------
void TFT_LCD::init(spi_inst_t *spi, uint32_t baud) /* initialize TFT-LCD with HX8347 */
{
  m_spi = spi;
  m_baud = baud;
  m_degree = TFT_degree90;
  m_color = TFT_RGB;

  spi_init(m_spi, m_baud);

  gpio_set_function(TFT_MISO, GPIO_FUNC_SPI);
  gpio_set_function(TFT_SCL, GPIO_FUNC_SPI);
  gpio_set_function(TFT_MOSI, GPIO_FUNC_SPI);
  // Make the SPI pins available to picotool
  bi_decl(bi_3pins_with_func(TFT_MISO, TFT_MOSI, TFT_SCL, GPIO_FUNC_SPI));

  // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(TFT_CS);
  gpio_set_dir(TFT_CS, GPIO_OUT);
  gpio_put(TFT_CS, 1);

  gpio_init(TFT_DC);
  gpio_set_dir(TFT_DC, GPIO_OUT);
  gpio_put(TFT_DC, 0);

  gpio_init(TFT_RST);
  gpio_set_dir(TFT_RST, GPIO_OUT);
  gpio_put(TFT_RST, 1);

  // TFT_LCD reset
  gpio_put(TFT_RST, 0);
  sleep_ms(100);
  gpio_put(TFT_RST, 1);
  // Make the CS pin available to picotool
  bi_decl(bi_1pin_with_name(TFT_CS, "SPI CS"));
  reg_init();
}
void TFT_LCD::imshow(Array<uint8_t> arr, uint8_t res)
{
  int width = 0;
  int height = 0;
  switch (res)
  {
  case resolution::QVGA:
    width=QVGA_width;
    height=QVGA_height;
    break;
  case resolution::QQVGA:
    width=QQVGA_width;
    height=QQVGA_height;
    break;
  case resolution::CIF:
    width=CIF_width;
    height=CIF_height;
    break;
  case resolution::QCIF:
    width=QCIF_width;
    height=QCIF_height;
    break;

  }
  uint8_t *buf = arr.getbuf();
  int t_width=width*2;
  setAddrWindow(0, 0, width, height);
  cs_select();
  DC_DATA();
  for (int j = 0; j < height; j++)
  {
    for (int i = 0; i <t_width; i++)
    {
      spi_transfer_byte(buf[i+j*t_width]);
    }
  }
  cs_deselect();
}

void TFT_LCD::imshow(uint8_t* arr, uint8_t res)
{
  int width = 0;
  int height = 0;
  switch (res)
  {
  case resolution::QVGA:
    width=QVGA_width;
    height=QVGA_height;
    break;
  case resolution::QQVGA:
    width=QQVGA_width;
    height=QQVGA_height;
    break;
  case resolution::CIF:
    width=CIF_width;
    height=CIF_height;
    break;
  case resolution::QCIF:
    width=QCIF_width;
    height=QCIF_height;
    break;

  }
  
  int t_width=width*2;
  setAddrWindow(0, 0, width, height);
  cs_select();
  DC_DATA();
  for (int j = 0; j < height; j++)
  {
    for (int i = 0; i <t_width; i++)
    {
      spi_transfer_byte(arr[i+j*t_width]);
    }
  }
  cs_deselect();
}

void TFT_LCD::imshow(uint8_t* arr, int width, int height)
{
  int total=width*2*height;
  //setAddrWindow(0, 0, width, height);
  sendcommand(HX8357_RAMWR);
  cs_select();
  DC_DATA();
  for (int j = 0; j <total; j++)
  {
    
    spi_transfer_byte(arr[j]);
    
  }
  cs_deselect();
}

void TFT_LCD::imshow(uint8_t* arr, int x, int y, int width, int height)
{
  int total=width*2*height;
  setAddrWindow(x, y, width, height);
  sendcommand(HX8357_RAMWR);
  cs_select();
  DC_DATA();
  for (int j = 0; j <total; j++)
  {
    
    spi_transfer_byte(arr[j]);
    
  }
  cs_deselect();
}


void TFT_LCD::color_screen(U16 color) /* TFT-LCD full screen color */
{
  GRAM_address(479, 319);
  cs_select();
  DC_DATA();
  for (int j = 0; j < TFTHEIGHT; j++)
  {
    for (int i = 0; i < TFTWIDTH; i++)
    {
      spi_transfer_byte(color >> 8);
      spi_transfer_byte(color & 0xff);
    }
  }
  cs_deselect();
}

#elif defined(TFT_8BIT)
//***********************************************************************************
// TFT_8BIT

inline void TFT_LCD::cs_select()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_CS, 0); // Active low
  asm volatile("nop \n nop \n nop");
}

inline void TFT_LCD::cs_deselect()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_CS, 1);
  asm volatile("nop \n nop \n nop");
}

inline void TFT_LCD::DC_DATA()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_DC, 1); // data->high
  asm volatile("nop \n nop \n nop");
  //sleep_us(100);
}

inline void TFT_LCD::DC_COMMAND()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_DC, 0); // command -> low
  asm volatile("nop \n nop \n nop");
  //sleep_us(100);
}

inline void TFT_LCD::WR_LOW()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_WR, 0); // command -> low
  asm volatile("nop \n nop \n nop");
  //sleep_us(100);
}

inline void TFT_LCD::WR_HIGH()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(TFT_WR, 1); // command -> low
  asm volatile("nop \n nop \n nop");
  //sleep_us(100);
}

inline void TFT_LCD::write8(uint8_t data)
{
  gpio_put_masked(0xff, data);
  WR_LOW();
  WR_HIGH();
}
inline void TFT_LCD::write16(uint16_t data)
{
  uint8_t h = data >> 8;
  uint8_t l = data & 0xff;
  write8(h);
  write8(l);
}

void TFT_LCD::sendWord(uint8_t IR, uint16_t data)
{
  cs_select();
  DC_COMMAND();
  write8(IR);
  DC_DATA();
  write16(data);
  cs_deselect();
}
void TFT_LCD::sendByte(uint8_t IR, uint8_t data)
{
  cs_select();
  DC_COMMAND();
  write8(IR);
  DC_DATA();
  write8(data);
  cs_deselect();
}

void TFT_LCD::sendcommand(uint8_t IR)
{
  cs_select();
  DC_COMMAND();
  write8(IR);
  cs_deselect();
}
void TFT_LCD::sendDataByte(uint8_t data)
{
  cs_select();
  DC_DATA();
  write8(data);
  cs_deselect();
}
void TFT_LCD::sendDataWord(uint16_t data)
{
  cs_select();
  DC_DATA();
  write16(data);
  cs_deselect();
}
//*****************------TFT initial-------------------------------
void TFT_LCD::init() /* initialize TFT-LCD with HX8347 */
{
  gpio_init_mask(0xff);
  gpio_set_dir(D0, GPIO_OUT);
  gpio_set_dir(D1, GPIO_OUT);
  gpio_set_dir(D2, GPIO_OUT);
  gpio_set_dir(D3, GPIO_OUT);
  gpio_set_dir(D4, GPIO_OUT);
  gpio_set_dir(D5, GPIO_OUT);
  gpio_set_dir(D6, GPIO_OUT);
  gpio_set_dir(D7, GPIO_OUT);
  m_degree=TFT_degree90;
  m_color=TFT_RGB;

  // Chip select is active-low, so we'll initialise it to a driven-high state
  gpio_init(TFT_CS);
  gpio_set_dir(TFT_CS, GPIO_OUT);
  gpio_put(TFT_CS, 1);

  gpio_init(TFT_DC);
  gpio_set_dir(TFT_DC, GPIO_OUT);
  gpio_put(TFT_DC, 0);

  gpio_init(TFT_WR);
  gpio_set_dir(TFT_WR, GPIO_OUT);
  gpio_put(TFT_WR, 0);

  gpio_init(TFT_RST);
  gpio_set_dir(TFT_RST, GPIO_OUT);
  gpio_put(TFT_RST, 1);

  // TFT_LCD reset
  gpio_put(TFT_RST, 0);
  sleep_ms(100);
  gpio_put(TFT_RST, 1);
  reg_init();
}

void TFT_LCD::imshow(Array<uint8_t> arr, uint8_t res)
{
  int width = 0;
  int height = 0;
  switch (res)
  {
  case resolution::QVGA:
    width=QVGA_width;
    height=QVGA_height;
    break;
  case resolution::QQVGA:
    width=QQVGA_width;
    height=QQVGA_height;
    break;
  case resolution::CIF:
    width=CIF_width;
    height=CIF_height;
    break;
  case resolution::QCIF:
    width=QCIF_width;
    height=QCIF_height;
    break;

  }
  uint8_t *buf = arr.getbuf();
  int t_width=width*2;
  setAddrWindow(0, 0, width, height);
  cs_select();
  DC_DATA();
  for (int j = 0; j < height; j++)
  {
    for (int i = 0; i <t_width; i++)
    {
      write8(buf[i+j*t_width]);
    }
  }
  cs_deselect();
}

void TFT_LCD::imshow(uint8_t* arr, uint8_t res)
{
  int width = 0;
  int height = 0;
  switch (res)
  {
  case resolution::QVGA:
    width=QVGA_width;
    height=QVGA_height;
    break;
  case resolution::QQVGA:
    width=QQVGA_width;
    height=QQVGA_height;
    break;
  case resolution::CIF:
    width=CIF_width;
    height=CIF_height;
    break;
  case resolution::QCIF:
    width=QCIF_width;
    height=QCIF_height;
    break;

  }
  
  int t_width=width*2;
  setAddrWindow(0, 0, width, height);
  cs_select();
  DC_DATA();
  for (int j = 0; j < height; j++)
  {
    for (int i = 0; i <t_width; i++)
    {
      write8(arr[i+j*t_width]);
    }
  }
  cs_deselect();
}

void TFT_LCD::imshow(uint8_t* arr, int width, int height)
{
  int total=width*2*height;
  //setAddrWindow(0, 0, width, height);
  sendcommand(HX8357_RAMWR);
  cs_select();
  DC_DATA();
  for (int j = 0; j <total; j++)
  {
    
    write8(arr[j]);
    
  }
  cs_deselect();
}

void TFT_LCD::imshow(uint8_t* arr, int x, int y, int width, int height)
{
  int total=width*2*height;
  setAddrWindow(x, y, width, height);
  sendcommand(HX8357_RAMWR);
  cs_select();
  DC_DATA();
  for (int j = 0; j <total; j++)
  {
    
    write8(arr[j]);
    
  }
  cs_deselect();
}


void TFT_LCD::color_screen(U16 color) /* TFT-LCD full screen color */
{
  GRAM_address(479, 319);
  cs_select();
  DC_DATA();
  for (int j = 0; j < TFTHEIGHT; j++)
  {
    for (int i = 0; i < TFTWIDTH; i++)
    {
      write8(color >> 8);
      write8(color & 0xff);
    }
  }
  cs_deselect();
}

#endif

void TFT_LCD::reg_init()
{
  sendByte(HX8357_SWRESET, 0x01); // window setting
  sleep_ms(10);

  sendWord(HX8357D_SETC, 0xFF83);
  sendDataByte(0x57);
  sendDataByte(0xFF);
  sleep_ms(300);

  sendWord(HX8357_SETRGB, 0x8000);
  sendDataWord(0x0606);
  sendByte(HX8357D_SETCOM, 0x25);
  sendByte(HX8357_SETOSC, 0x68); // 0x68
  sendByte(HX8357_SETPANEL, 0x05);
  sendWord(HX8357_SETPWR1, 0x0015);
  sendDataWord(0x1C1C);
  sendDataWord(0x83AA);
  sendWord(HX8357D_SETSTBA, 0x5050);
  sendDataWord(0x013C);
  sendDataWord(0x1E08);
  sendWord(HX8357D_SETCYC, 0x0240);
  sendDataWord(0x002A);
  sendDataWord(0x2A0D);
  sendDataWord(0x78);            // sendDataByte(0x78)
  sendcommand(HX8357D_SETGAMMA); // setting gamma
  sendDataWord(0x020A);
  sendDataWord(0x111d);
  sendDataWord(0x2335);
  sendDataWord(0x414b);
  sendDataWord(0x4b42);
  sendDataWord(0x3A27);
  sendDataWord(0x1b08);
  sendDataWord(0x0903);
  sendDataWord(0x020A);
  sendDataWord(0x111d);
  sendDataWord(0x2335);
  sendDataWord(0x414b);
  sendDataWord(0x4b42);
  sendDataWord(0x3A27);
  sendDataWord(0x1b08);
  sendDataWord(0x0903);
  sendDataWord(0x0001);

  sendByte(HX8357_COLMOD, 0x55);
  sendByte(HX8357_MADCTL, 0x20);
  sendByte(HX8357_TEON, 0x00);
  sendByte(HX8357_TEARLINE, 0x0002);
  sendByte(HX8357_SLPOUT, 0x11);
  sendByte(HX8357_DISPON, 0x29);
}

//*****************************************************************************
/* ---------------------------------------------------------------------------- */
/*	 출력제어 함수					*/
/* ---------------------------------------------------------------------------- */
void TFT_LCD::setBGR()
{
  uint8_t madctl=m_degree|TFT_BGR;
  sendcommand(HX8357_MADCTL);
  sendDataByte(madctl);  
  m_color=TFT_BGR;
}
void TFT_LCD::setRGB()
{
  uint8_t madctl=m_degree|TFT_RGB;
  sendcommand(HX8357_MADCTL);
  sendDataByte(madctl);  
  m_color=TFT_RGB;

}
void TFT_LCD::setRotation(uint8_t m)
{
  uint8_t madctl = 0;

  switch (m)
  {
  case 0:
    madctl = TFT_degree0|m_color; // 0 degree rotation (default)
    m_degree=TFT_degree0;
    break;
  case 1:
    madctl = TFT_degree90|m_color; // 90 degree rotation
    m_degree=TFT_degree90;
    break;
  case 2:
    madctl = TFT_degree180|m_color; // 180 degree rotation
    m_degree=TFT_degree180;
    break;
  case 3:
    madctl = TFT_degree270|m_color; // 270 degree rotation
    m_degree=TFT_degree270;
    break;
  }
  sendcommand(HX8357_MADCTL); // MADCTL command
  sendDataByte(madctl);       // Set the rotation
}

void TFT_LCD::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h)
{
  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
  sendWord(HX8357_CASET, x1); // xPos = 0~239
  sendDataWord(x2);
  sendWord(HX8357_PASET, y1); // yPos = 0~319
  sendDataWord(y2);
  sendcommand(HX8357_RAMWR);
}
void TFT_LCD::GRAM_address(U16 xPos, U16 yPos) /* set GRAM address of TFT-LCD */
{
  // if((xPos > 239) || (yPos > 319)) return;
  sendWord(HX8357_CASET, 0x0000); // xPos = 0~239
  sendDataWord(xPos);
  sendWord(HX8357_PASET, 0x0000); // yPos = 0~319
  sendDataWord(yPos);
  sendcommand(HX8357_RAMWR);
  // cs_deselect();
}

void TFT_LCD::clear_screen(void) /* TFT-LCD clear screen with black color */
{
  TFT_LCD::color_screen(Black);
}

void TFT_LCD::screen_mode(uint8_t mode)
{
}

void TFT_LCD::TFT_pixel(U16 xPos, U16 yPos, U16 color) /* write a pixel */
{
  // drawPixel (xPos , yPos, color);
  sendcommand(HX8357_CASET); // Column address set
  sendDataWord(xPos);
  sendDataWord(xPos + 1);
  sendcommand(HX8357_PASET); // Row address set
  sendDataWord(yPos);
  sendDataWord(yPos + 1);
  sendcommand(HX8357_RAMWR); // Write to RAM
  for (int i = 0; i <= 4; i++)
    sendDataWord(color);
  cs_deselect();
}




void TFT_LCD::xy(U08 xChar, U08 yChar) /* set character position (x,y) */
{
  xcharacter = xChar;
  ycharacter = yChar;
}

void TFT_LCD::color(U16 colorfore, U16 colorback) /* set foreground and background color */
{
  foreground = colorfore;
  background = colorback;
}

// void TFT_LCD::string_size(U08 xChar, U08 yChar, U16 colorfore, U16 colorback, char  *str,U08 width, U08 height )	/* write TFT-LCD string */
// {
//  drawstring(xChar,yChar,str,colorfore,colorback,width,height);
// }
// void TFT_LCD::string(U08 xChar, U08 yChar, U16 colorfore, U16 colorback, char *str)	/* write TFT-LCD string */
// {
// 	drawstring(xChar,yChar,str,colorfore,colorback,1,1);
// }

void TFT_LCD::TFT_English(U08 code) /* write a English ASCII character */
{
  unsigned char data, x, y;
  unsigned int pixel[8][16];
  unsigned int dot0, dot1, dot2, dot3, dot4;

  for (x = 0; x < 8; x++) // read English ASCII font
  {
    data = E_font[code][x];
    for (y = 0; y < 8; y++)
    {
      if (data & 0x01)
        pixel[x][y] = foreground;
      else
        pixel[x][y] = background;
      data = data >> 1;
    }
  }

  for (x = 0; x < 8; x++)
  {
    data = E_font[code][x + 8];
    for (y = 0; y < 8; y++)
    {
      if (data & 0x01)
        pixel[x][y + 8] = foreground;
      else
        pixel[x][y + 8] = background;
      data = data >> 1;
    }
  }

  if (outline_flag == 1) // display outline
    for (x = 0; x < 8; x++)
    {
      dot0 = E_font[code][x] + E_font[code][x + 8] * 256;
      dot1 = dot0 >> 1;                                       // up side
      dot2 = dot0;                                            // down side
      dot3 = E_font[code][x + 1] + E_font[code][x + 8] * 256; // left side
      dot4 = E_font[code][x - 1] + E_font[code][x + 7] * 256; // right side

      for (y = 0; y < 15; y++)
      {
        if (!(dot0 & 0x0001))
        {
          if (dot1 & 0x0001)
            pixel[x][y] = outline;
          if (dot2 & 0x0001)
            pixel[x][y] = outline;
          if ((dot3 & 0x0001) && (x < 7))
            pixel[x][y] = outline;
          if ((dot4 & 0x0001) && (x > 0))
            pixel[x][y] = outline;
        }

        dot1 >>= 1;
        dot2 = dot0;
        dot0 >>= 1;
        dot3 >>= 1;
        dot4 >>= 1;
      }
    }

  if ((cursor_flag == 1) && (xcharacter == xcursor) && (ycharacter == ycursor))
  {
    for (x = 0; x < 8; x++) // display cursor
    {
      pixel[x][14] = cursor;
      pixel[x][15] = cursor;
    }
  }

  if (ScreenMode == 'P')
  {
    for (y = 0; y < 16; y++) // write in portrait mode
      for (x = 0; x < 8; x++)
        TFT_pixel(xcharacter * 8 + x, ycharacter * 8 + y, pixel[x][y]);
  }
  else
  {
    for (y = 0; y < 16; y++) // write in landscape mode
      for (x = 0; x < 8; x++)
        TFT_pixel((29 - ycharacter) * 8 + 7 - y, xcharacter * 8 + x, pixel[x][y]);
  }

  xcharacter += 1;
  if (xcharacter >= LineLimit) // end of line ?
  {
    xcharacter = 0;
    ycharacter += 2;
  }
}

unsigned int TFT_LCD::KS_code_conversion(U16 KSSM) /* convert KSSM(�ϼ���) to KS(������) */
{
  unsigned char HB, LB;
  unsigned int index, KS;

  HB = KSSM >> 8;
  LB = KSSM & 0x00FF;

  if (KSSM >= 0xB0A1 && KSSM <= 0xC8FE)
  {
    index = (HB - 0xB0) * 94 + LB - 0xA1;
    KS = KS_Table[index][0] * 256;
    KS |= KS_Table[index][1];

    return KS;
  }
  else
    return -1;
}

void TFT_LCD::TFT_Korean(U16 code) /* write a Korean character */
{
  unsigned char cho_5bit, joong_5bit, jong_5bit;
  unsigned char cho_bul, joong_bul, jong_bul = 0, i, jong_flag;
  unsigned int ch;

  cho_5bit = table_cho[(code >> 10) & 0x001F];    // get 5bit(14-10) of chosung
  joong_5bit = table_joong[(code >> 5) & 0x001F]; // get 5bit(09-05) of joongsung
  jong_5bit = table_jong[code & 0x001F];          // get 5bit(04-00) of jongsung

  if (jong_5bit == 0) // if jongsung not exist
  {
    jong_flag = 0;
    cho_bul = bul_cho1[joong_5bit];
    if ((cho_5bit == 1) || (cho_5bit == 16))
      joong_bul = 0;
    else
      joong_bul = 1;
  }
  else // if jongsung exist
  {
    jong_flag = 1;
    cho_bul = bul_cho2[joong_5bit];
    if ((cho_5bit == 1) || (cho_5bit == 16))
      joong_bul = 2;
    else
      joong_bul = 3;
    jong_bul = bul_jong[joong_5bit];
  }

  ch = cho_bul * 20 + cho_5bit; // get chosung font
  for (i = 0; i < 32; i++)
    KoreanBuffer[i] = K_font[ch][i];
  ch = 8 * 20 + joong_bul * 22 + joong_5bit; // OR joongsung font
  for (i = 0; i < 32; i++)
    KoreanBuffer[i] |= K_font[ch][i];
  if (jong_flag) // OR jongsung font
  {
    ch = 8 * 20 + 4 * 22 + jong_bul * 28 + jong_5bit;
    for (i = 0; i < 32; i++)
      KoreanBuffer[i] |= K_font[ch][i];
  }

  unsigned char data, x, y;
  unsigned int pixel[16][16];
  unsigned int dot0, dot1, dot2, dot3, dot4;

  for (x = 0; x < 16; x++) // read Korean font
  {
    data = KoreanBuffer[x];
    for (y = 0; y < 8; y++)
    {
      if (data & 0x01)
        pixel[x][y] = foreground;
      else
        pixel[x][y] = background;
      data = data >> 1;
    }
  }

  for (x = 0; x < 16; x++)
  {
    data = KoreanBuffer[x + 16];
    for (y = 0; y < 8; y++)
    {
      if (data & 0x01)
        pixel[x][y + 8] = foreground;
      else
        pixel[x][y + 8] = background;
      data = data >> 1;
    }
  }

  if (outline_flag == 1) // display outline
    for (x = 0; x < 16; x++)
    {
      dot0 = KoreanBuffer[x] + KoreanBuffer[x + 16] * 256;
      dot1 = dot0 >> 1;                                        // up side
      dot2 = dot0;                                             // down side
      dot3 = KoreanBuffer[x + 1] + KoreanBuffer[x + 16] * 256; // left side
      dot4 = KoreanBuffer[x - 1] + KoreanBuffer[x + 15] * 256; // right side

      for (y = 0; y < 16; y++)
      {
        if (!(dot0 & 0x0001))
        {
          if (dot1 & 0x0001)
            pixel[x][y] = outline;
          if (dot2 & 0x0001)
            pixel[x][y] = outline;
          if ((dot3 & 0x0001) && (x < 15))
            pixel[x][y] = outline;
          if ((dot4 & 0x0001) && (x > 0))
            pixel[x][y] = outline;
        }

        dot1 >>= 1;
        dot2 = dot0;
        dot0 >>= 1;
        dot3 >>= 1;
        dot4 >>= 1;
      }
    }

  if ((cursor_flag == 1) && (xcharacter == xcursor) && (ycharacter == ycursor))
  {
    for (x = 0; x < 16; x++) // display cursor
    {
      pixel[x][14] = cursor;
      pixel[x][15] = cursor;
    }
  }

  if (xcharacter >= (LineLimit - 1)) // end of line ?
  {
    xcharacter = 0;
    ycharacter += 2;
  }

  if (ScreenMode == 'P') // write in portrait mode
  {
    for (y = 0; y < 16; y++)
      for (x = 0; x < 16; x++)
        TFT_pixel(xcharacter * 8 + x, ycharacter * 8 + y, pixel[x][y]);
  }
  else // write in landscape mode
  {
    for (y = 0; y < 16; y++)
      for (x = 0; x < 16; x++)
        TFT_pixel((29 - ycharacter) * 8 + 7 - y, xcharacter * 8 + x, pixel[x][y]);
  }

  xcharacter += 2;
  if (xcharacter >= LineLimit) // end of line ?
  {
    xcharacter = 0;
    ycharacter += 2;
  }
}

void TFT_cursor(U16 cursor_color) /* set cursor and color */
{
  if (cursor_color == Transparent) // disable cursor
    cursor_flag = 0;
  else // enable cursor
  {
    cursor_flag = 1;
    cursor = cursor_color;
  }
}

void TFT_LCD::TFT_cursor(U16 cursor_color) /* set cursor and color */
{
  if (cursor_color == Transparent) // disable cursor
    cursor_flag = 0;
  else // enable cursor
  {
    cursor_flag = 1;
    cursor = cursor_color;
  }
}
void TFT_LCD::TFT_outline(U16 outline_color) /* set outline and color */
{
  if (outline_color == Transparent) // disable outline
    outline_flag = 0;
  else // enable outline
  {
    outline_flag = 1;
    outline = outline_color;
  }
}

unsigned int TFT_LCD::Unicode_to_KS(U16 unicode) // convert Unicode(�����ڵ�) to KS(������)
{
  unsigned char cho = 0, joong = 0, jong = 0;
  unsigned int value;

  value = unicode - 0xAC00; // �����ڵ忡�� '��'�� �ش��ϴ� ���� ����.

  jong = value % 28; // �����ڵ带 �ʼ�, �߼�, �������� �и�
  joong = ((value - jong) / 28) % 21;
  cho = ((value - jong) / 28) / 21;

  cho += 2; // �ʼ� + ������

  if (joong < 5)
    joong += 3; // �߼� + ������
  else if (joong < 11)
    joong += 5;
  else if (joong < 17)
    joong += 7;
  else
    joong += 9;

  if (jong < 17)
    jong++; // ���� + ������
  else
    jong += 2;

  return 0x8000 | (cho << 10) | (joong << 5) | jong; // ������ �ڵ�
}

void TFT_LCD::unsigned_decimal(U32 number, U08 zerofill, U08 digit) /* display unsigned decimal number */
{
  unsigned char zero_flag, character, count = 0;
  unsigned long div;

  if ((digit == 0) || (digit > 9))
    return;

  div = 1;
  while (--digit)
    div *= 10;

  zero_flag = zerofill;
  while (div > 0) // display number
  {
    count++;
    xcharacter += 5 * count;
    character = number / div;
    if ((character == 0) && (zero_flag == 0) && (div != 1))
      TFT_English(character + ' ');
    else
    {
      zero_flag = 1;
      TFT_English(character + '0');
    }
    number %= div;
    div /= 10;
  }
}

void TFT_LCD::signed_decimal(S32 number, U08 zerofill, U08 digit) /* display signed decimal number */
{
  unsigned char zero_flag, character;
  unsigned long div;

  if ((digit == 0) || (digit > 9))
    return;

  if (number >= 0) // display sign
    TFT_English('+');
  else
  {
    TFT_English('-');
    number = -number;
  }

  div = 1;
  while (--digit)
    div *= 10;

  zero_flag = zerofill;
  while (div > 0) // display number
  {
    character = number / div;
    if ((character == 0) && (zero_flag == 0) && (div != 1))
      TFT_English(character + ' ');
    else
    {
      zero_flag = 1;
      TFT_English(character + '0');
    }
    number %= div;
    div /= 10;
  }
}

void TFT_LCD::hexadecimal(U32 number, U08 digit) /* display hexadecimal number */
{
  unsigned char i, character, count = 0;

  if ((digit == 0) || (digit > 8))
    return;

  for (i = digit; i > 0; i--)
  {
    count++;
    character = (number >> 4 * (i - 1)) & 0x0F;
    xcharacter += 5 * count;
    if (xcharacter > 230)
    {
      xcharacter = 0;
      ycharacter += 16;
    }
    if (character < 10)
      TFT_English(character + '0'); //
    else
      TFT_English(character - 10 + 'A'); //
    xcharacter += 8;
  }
  xcharacter += 8;
}

void TFT_LCD::_0x_hexadecimal(U32 number, U08 digit) /* display hexadecimal number with 0x */
{
  unsigned char i, character;

  if ((digit == 0) || (digit > 8))
    return;

  TFT_English('0');
  TFT_English('x');

  for (i = digit; i > 0; i--)
  {
    character = (number >> 4 * (i - 1)) & 0x0F;
    if (character < 10)
      TFT_English(character + '0');
    else
      TFT_English(character - 10 + 'A');
  }
}

void TFT_LCD::unsigned_float(float number, U08 integral, U08 fractional) /* display unsigned floating-point number */
{
  unsigned char zero_flag, digit, character; // integral = digits of integral part
  unsigned long div, integer;                // fractional = digits of fractional part

  digit = integral + fractional;
  if ((integral == 0) || (fractional == 0) || (digit > 9))
    return;

  div = 1;
  while (--digit)
    div *= 10;

  while (fractional--)
    number *= 10.;
  integer = (U32)(number + 0.5);

  zero_flag = 0;
  digit = 1;
  while (div > 0) // display number
  {
    character = integer / div;
    if ((character == 0) && (zero_flag == 0) && (digit != integral))
      TFT_English(character + ' ');
    else
    {
      zero_flag = 1;
      TFT_English(character + '0');
    }
    integer %= div;
    div /= 10;

    if (digit == integral)
      TFT_English('.');
    digit++;
  }
}

void TFT_LCD::signed_float(float number, U08 integral, U08 fractional) /* display signed floating-point number */
{
  unsigned char zero_flag, digit, character;
  unsigned long div, integer;

  digit = integral + fractional;
  if ((integral == 0) || (fractional == 0) || (digit > 9))
    return;

  if (number >= 0) // display sign
    TFT_English('+');
  else
  {
    TFT_English('-');
    number = -number;
  }

  div = 1;
  while (--digit)
    div *= 10;

  while (fractional--)
    number *= 10.;
  integer = (U32)(number + 0.5);

  zero_flag = 0;
  digit = 1;
  while (div > 0) // display number
  {
    character = integer / div;
    if ((character == 0) && (zero_flag == 0) && (digit != integral))
      TFT_English(character + ' ');
    else
    {
      zero_flag = 1;
      TFT_English(character + '0');
    }
    integer %= div;
    div /= 10;

    if (digit == integral)
      TFT_English('.');
    digit++;
  }
}

/* ---------------------------------------------------------------------------- */
/*		�׷��� �Լ�							*/
/* ---------------------------------------------------------------------------- */

void TFT_LCD::Line(S16 x1, S16 y1, S16 x2, S16 y2, U16 color) /* draw a straight line */
{
  int x, y;
  int x_diff = x2 - x1;
  int y_diff = y2 - y1;

  if (y1 != y2) // if y1 != y2, y is variable
  {
    if (x1==x2)
    {
      sendcommand(HX8357_CASET); // Column address set
      sendDataWord(x1);
      sendDataWord(x2);
      sendcommand(HX8357_PASET); // Row address set
      sendDataWord(y1);
      sendDataWord(y2);
      sendcommand(HX8357_RAMWR); // Write to RAM
      for (int i = 0; i < y_diff; i++)
        sendDataWord(color);
      cs_deselect();
    }
    else
    {
      if (y1 < y2) //              x is function
      for (y = y1; y <= y2; y++)
      {
        x = x1 + (long)(y - y1) * (long)(x_diff) / y_diff;
        TFT_pixel(x, y, color);
      }
    else
      for (y = y1; y >= y2; y--)
      {
        x = x1 + (long)(y - y1) * (long)(x_diff) / y_diff;
        TFT_pixel(x, y, color);
      }
    }
    
  }
  else if (x1 != x2) // if x1 != x2, x is variable
  {
    if (y1==y2)
    {
      sendcommand(HX8357_CASET); // Column address set
      sendDataWord(x1);
      sendDataWord(x2);
      sendcommand(HX8357_PASET); // Row address set
      sendDataWord(y1);
      sendDataWord(y2);
      sendcommand(HX8357_RAMWR); // Write to RAM
      for (int i = 0; i < x_diff; i++)
        sendDataWord(color);
      cs_deselect();
    }
    else
    {
      if (x1 < x2) //              y is function
      for (x = x1; x <= x2; x++)
      {
        y = y1 + (long)(x - x1) * (long)(y_diff) / x_diff;
        TFT_pixel(x, y, color);
      }
    else
      for (x = x1; x >= x2; x--)
      {
        y = y1 + (long)(x - x1) * (long)(y_diff) / x_diff;
        TFT_pixel(x, y, color);
      }
    }
    
  }
  else // if x1 == x2 and y1 == y2, it is a dot
    TFT_pixel(x1, y1, color);
}

void TFT_LCD::Rectangle(S16 x1, S16 y1, S16 x2, S16 y2, U16 color) /* draw a rectangle */
{
  Line(x1, y1, x1, y2, color); // horizontal line
  Line(x2, y1, x2, y2, color);
  Line(x1, y1, x2, y1, color); // vertical line
  Line(x1, y2, x2, y2, color);
}

void TFT_LCD::Block(S16 x1, S16 y1, S16 x2, S16 y2, U16 color, U16 fill) /* draw a rectangle with filled color */
{
  int i;

  Line(x1, y1, x1, y2, color); // horizontal line
  Line(x2, y1, x2, y2, color);
  Line(x1, y1, x2, y1, color); // vertical line
  Line(x1, y2, x2, y2, color);

  if ((y1 < y2) && (x1 != x2)) // fill block
  {
    for (i = y1 + 1; i <= y2 - 1; i++)
      Line(x1 + 1, i, x2 - 1, i, fill);
  }
  else if ((y1 > y2) && (x1 != x2))
  {
    for (i = y2 + 1; i <= y1 - 1; i++)
      Line(x1 + 1, i, x2 - 1, i, fill);
  }
}

void TFT_LCD::Circle(S16 x1, S16 y1, S16 r, U16 color) /* draw a circle */
{
  int x, y;
  float s;

  for (y = y1 - r * 3 / 4; y <= y1 + r * 3 / 4; y++) // draw with y variable
  {
    s = sqrt((long)r * (long)r - (long)(y - y1) * (long)(y - y1)) + 0.5;
    x = x1 + (int)s;
    TFT_pixel(x, y, color);
    x = x1 - (int)s;
    TFT_pixel(x, y, color);
  }

  for (x = x1 - r * 3 / 4; x <= x1 + r * 3 / 4; x++) // draw with x variable
  {
    s = sqrt((long)r * (long)r - (long)(x - x1) * (long)(x - x1)) + 0.5;
    y = y1 + (int)s;
    TFT_pixel(x, y, color);
    y = y1 - (int)s;
    TFT_pixel(x, y, color);
  }
}

void TFT_LCD::Sine(S16 peak, U08 mode, U16 color) /* draw a sine curve */
{
  int x, y;

  if (mode == 0)
    for (y = 0; y <= 319; y++)
    {
      x = 120 - (int)(sin((float)y * 1.6875 * M_PI / 180.) * peak + 0.5);
      TFT_pixel(x, y, color);
    }
  else
    for (y = 0; y <= 319; y++)
    {
      x = 120 + (int)(sin((float)y * 1.6875 * M_PI / 180.) * peak + 0.5);
      TFT_pixel(x, y, color);
    }
}