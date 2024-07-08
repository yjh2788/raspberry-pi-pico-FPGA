#include "ov7670.h"
#include "pico/error.h"
#include "hardware/clocks.h"
// 초기화 설정 값 예제 (필수 레지스터 설정)
const uint8_t OV7670_INIT[][2] = {
    {0x12, 0x80}, // COM7 - Reset
    {0x11, 0x01}, // CLKRC - Internal clock prescaler
    {0x6B, 0x0A}, // PLL Control
    {0x3A, 0x04}, // TSLB - Set UV ordering, do not auto-reset window
    {0x12, 0x14}, // COM7 - Output format: RGB
    {0x8C, 0x00}, // RGB444 - Disable RGB444
    {0x04, 0x00}, // COM1 - No CCIR601
    {0x40, 0xC0}, // COM15 - Full range output, RGB 565
    {0x14, 0x38}, // COM9 - AGC gain ceiling
    {0x4F, 0xBB}, // MTX1 - Matrix coefficients
    {0x50, 0x9C}, // MTX2 - Matrix coefficients
    {0x51, 0x2D}, // MTX3 - Matrix coefficients
    {0x52, 0x91}, // MTX4 - Matrix coefficients
    {0x53, 0x00}, // MTX5 - Matrix coefficients
    {0x54, 0x1E}, // MTX6 - Matrix coefficients
    {0x58, 0x9E}, // MTXS - Matrix coefficients
    {0x3D, 0xC0}, // COM13 - Turn on GAMMA and UV Auto adjust
    {0x17, 0x11}, // HSTART - Horizontal frame start
    {0x18, 0x43}, // HSTOP - Horizontal frame end
    {0x32, 0x09}, // HREF - Edge offset
    {0x19, 0x00}, // VSTART - Vertical frame start
    {0x1A, 0x4B}, // VSTOP - Vertical frame end
    {0x03, 0x0A}, // VREF - Vertical frame control
    {0x0E, 0x61}, // COM5 - Reserved
    {0x0F, 0x4B}, // COM6 - Reset timings
    {0x16, 0x02}, // RSVD - Reserved
    {0x1E, 0x07}, // MVFP - Mirror/VFlip Enable
    {0x21, 0x02}, // ADCCTR1 - ADC Control
    {0x22, 0x91}, // ADCCTR2 - ADC Control
    // {0x29, 0x07}, // RSVD - Reserved
    // {0x33, 0x0B}, // CHLF - Reserved
    // {0x35, 0x0B}, // RSVD - Reserved
    // {0x37, 0x1D}, // ADC - Reserved
    // {0x38, 0x71}, // RSVD - Reserved
    // {0x39, 0x2A}, // RSVD - Reserved
    // {0x3C, 0x78}, // COM12 - Reserved
    // {0x4D, 0x40}, // RSVD - Reserved
    // {0x4E, 0x20}, // RSVD - Reserved
    {0x69, 0x00}, // GFIX - Fix gain control
    {0x6B, 0x4A}, // PLL - PLL control
    {0x74, 0x19}, // REG74 - Digital gain
    // {0x8D, 0x4F}, // RSVD - Reserved
    // {0x8E, 0x00}, // RSVD - Reserved
    // {0x8F, 0x00}, // RSVD - Reserved
    // {0x90, 0x00}, // RSVD - Reserved
    // {0x91, 0x00}, // RSVD - Reserved
    // {0x96, 0x00}, // RSVD - Reserved
    // {0x9A, 0x80}, // RSVD - Reserved
    // {0xB0, 0x84}, // RSVD - Reserved
    {0xB1, 0x0C}, // ABLC1 - ABLC control
    // {0xB2, 0x0E}, // RSVD - Reserved
    // {0xB3, 0x82}, // THL_ST - Reserved
    // {0xB8, 0x0A}, // RSVD - Reserved
    // {0x43, 0x14}, // RSVD - Reserved
    // {0x44, 0xF0}, // RSVD - Reserved
    // {0x45, 0x34}, // RSVD - Reserved
    // {0x46, 0x58}, // RSVD - Reserved
    // {0x47, 0x28}, // RSVD - Reserved
    // {0x48, 0x3A}, // RSVD - Reserved
    // {0x59, 0x88}, // RSVD - Reserved
    // {0x5A, 0x88}, // RSVD - Reserved
    // {0x5B, 0x44}, // RSVD - Reserved
    // {0x5C, 0x67}, // RSVD - Reserved
    // {0x5D, 0x49}, // RSVD - Reserved
    // {0x5E, 0x0E}, // RSVD - Reserved
    // {0x64, 0x04}, // RSVD - Reserved
    // {0x65, 0x20}, // RSVD - Reserved
    // {0x66, 0x05}, // RSVD - Reserved
    // {0x94, 0x04}, // RSVD - Reserved
    // {0x95, 0x08}, // RSVD - Reserved
    // {0x6C, 0x0A}, // RSVD - Reserved
    // {0x6D, 0x55}, // RSVD - Reserved
    // {0x6E, 0x11}, // RSVD - Reserved
    // {0x6F, 0x9F}, // RSVD - Reserved
    // {0x6A, 0x40}, // RSVD - Reserved
    {0x01, 0x40}, // BLUE - Blue channel gain
    {0x02, 0x40}, // RED - Red channel gain
    {0x13, 0x99}, // COM8 - Banding filter, AGC, AWB
    {0x15, 0x00}, // COM10 - RSVD
    {0x4F, 0xB3}, // MTX1 - Matrix coefficients
    {0x50, 0xB3}, // MTX2 - Matrix coefficients
    {0x51, 0x00}, // MTX3 - Matrix coefficients
    {0x52, 0x3D}, // MTX4 - Matrix coefficients
    {0x53, 0xA7}, // MTX5 - Matrix coefficients
    {0x54, 0xE4}, // MTX6 - Matrix coefficients
    {0x58, 0x9E}, // MTXS - Matrix coefficients
    {0x41, 0x08}, // COM16 - Edge enhancement, denoise
    {0x3F, 0x00}, // EDGE - RSVD
    {0x75, 0x05}, // REG75 - RGB565 format
    {0x76, 0xE1}, // RGB444 - Disable RGB444
    {0x4C, 0x00}, // COM24 - Disable gamma, edge enhancement, color matrix
    {0x77, 0x01}, // COM24 - Gamma enable
    {0x3B, 0x0A}, // COM11 - Night mode
    // {0x33, 0x00}, // CHLF - Reserved
    // {0x3C, 0x78}, // COM12 - Reserved
    {0x5A, 0x23}, // BD50ST - 50Hz banding filter
    {0x23, 0xC1}, // BD60ST - 60Hz banding filter
    {0x12, 0x14}, // COM7 - Output format: RGB
    {0x17, 0x16}, // HSTART - Horizontal frame start
    {0x18, 0x04}, // HSTOP - Horizontal frame end
    {0x19, 0x02}, // VSTART - Vertical frame start
    {0x1A, 0x7A}, // VSTOP - Vertical frame end
    {0x03, 0x0A}, // VREF - Vertical frame control
    {0xFF, 0xFF}  // End of settings marker
};

const uint8_t qvga_ov7670[][2] = {
  { REG_COM14, 0x19 },
  { 0x72, 0x11 },
  { 0x73, 0xf1 },

  { REG_HSTART, 0x16 },
  { REG_HSTOP, 0x04 },
  { REG_HREF, 0xa4 },
  { REG_VSTART, 0x02 },
  { REG_VSTOP, 0x7a },
  { REG_VREF, 0x0a },


/*  { REG_HSTART, 0x16 },
  { REG_HSTOP, 0x04 },
  { REG_HREF, 0x24 },
  { REG_VSTART, 0x02 },
  { REG_VSTOP, 0x7a },
  { REG_VREF, 0x0a },*/
  { 0xff, 0xff }, /* END MARKER */
};


const uint8_t reg_vals[][2] = {//from the linux driver
  { REG_COM7, COM7_RESET },
  { REG_TSLB, 0x04 }, /* OV */
  { REG_COM7, 0x40 },  /* VGA */
  /*
  * Set the hardware window.  These values from OV don't entirely
  * make sense - hstop is less than hstart.  But they work...
  */
  { REG_HSTART, 0x13 }, { REG_HSTOP, 0x01 },
  { REG_HREF, 0xb6 }, { REG_VSTART, 0x02 },
  { REG_VSTOP, 0x7a }, { REG_VREF, 0x0a },

  { REG_COM3, 0 }, { REG_COM14, 0 },
  /* Mystery scaling numbers */
  { 0x70, 0x3a }, { 0x71, 0x35 },
  { 0x72, 0x11 }, { 0x73, 0xf0 },
  { 0xa2,/* 0x02 changed to 1*/1 }, { REG_COM10, 0x0 },
  /* Gamma curve values */
  { 0x7a, 0x20 }, { 0x7b, 0x10 },
  { 0x7c, 0x1e }, { 0x7d, 0x35 },
  { 0x7e, 0x5a }, { 0x7f, 0x69 },
  { 0x80, 0x76 }, { 0x81, 0x80 },
  { 0x82, 0x88 }, { 0x83, 0x8f },
  { 0x84, 0x96 }, { 0x85, 0xa3 },
  { 0x86, 0xaf }, { 0x87, 0xc4 },
  { 0x88, 0xd7 }, { 0x89, 0xe8 },
  /* AGC and AEC parameters.  Note we start by disabling those features,
  then turn them only after tweaking the values. */
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP },
  { REG_GAIN, 0 }, { REG_AECH, 0 },
  { REG_COM4, 0x40 }, /* magic reserved bit */
  { REG_COM9, 0x18 }, /* 4x gain + magic rsvd bit */
  { REG_BD50MAX, 0x05 }, { REG_BD60MAX, 0x07 },
  { REG_AEW, 0x95 }, { REG_AEB, 0x33 },
  { REG_VPT, 0xe3 }, { REG_HAECC1, 0x78 },
  { REG_HAECC2, 0x68 }, { 0xa1, 0x03 }, /* magic */
  { REG_HAECC3, 0xd8 }, { REG_HAECC4, 0xd8 },
  { REG_HAECC5, 0xf0 }, { REG_HAECC6, 0x90 },
  { REG_HAECC7, 0x94 },
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC },
  { 0x30, 0 }, { 0x31, 0 },//disable some delays
  /* Almost all of these are magic "reserved" values.  */
  { REG_COM5, 0x61 }, { REG_COM6, 0x4b },
  { 0x16, 0x02 }, { REG_MVFP, 0x07 },
  { 0x21, 0x02 }, { 0x22, 0x91 },
  { 0x29, 0x07 }, { 0x33, 0x0b },
  { 0x35, 0x0b }, { 0x37, 0x1d },
  { 0x38, 0x71 }, { 0x39, 0x2a },
  { REG_COM12, 0x78 }, { 0x4d, 0x40 },
  { 0x4e, 0x20 }, { REG_GFIX, 0 },
  /*{0x6b, 0x4a},*/{ 0x74, 0x10 },
  { 0x8d, 0x4f }, { 0x8e, 0 },
  { 0x8f, 0 }, { 0x90, 0 },
  { 0x91, 0 }, { 0x96, 0 },
  { 0x9a, 0 }, { 0xb0, 0x84 },
  { 0xb1, 0x0c }, { 0xb2, 0x0e },
  { 0xb3, 0x82 }, { 0xb8, 0x0a },

  /* More reserved magic, some of which tweaks white balance */
  { 0x43, 0x0a }, { 0x44, 0xf0 },
  { 0x45, 0x34 }, { 0x46, 0x58 },
  { 0x47, 0x28 }, { 0x48, 0x3a },
  { 0x59, 0x88 }, { 0x5a, 0x88 },
  { 0x5b, 0x44 }, { 0x5c, 0x67 },
  { 0x5d, 0x49 }, { 0x5e, 0x0e },
  { 0x6c, 0x0a }, { 0x6d, 0x55 },
  { 0x6e, 0x11 }, { 0x6f, 0x9e }, /* it was 0x9F "9e for advance AWB" */
  { 0x6a, 0x40 }, { REG_BLUE, 0x40 },
  { REG_RED, 0x60 },
  { REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC | COM8_AWB },

  /* Matrix coefficients */
  { 0x4f, 0x80 }, { 0x50, 0x80 },
  { 0x51, 0 },    { 0x52, 0x22 },
  { 0x53, 0x5e }, { 0x54, 0x80 },
  { 0x58, 0x9e },

  { REG_COM16, COM16_AWBGAIN }, { REG_EDGE, 0 },
  { 0x75, 0x05 }, { REG_REG76, 0xe1 },
  { 0x4c, 0 },     { 0x77, 0x01 },
  { REG_COM13, /*0xc3*/0x48 }, { 0x4b, 0x09 },
  { 0xc9, 0x60 },   /*{REG_COM16, 0x38},*/
  { 0x56, 0x40 },

  { 0x34, 0x11 }, { REG_COM11, COM11_EXP | COM11_HZAUTO },
  { 0xa4, 0x82/*Was 0x88*/ }, { 0x96, 0 },
  { 0x97, 0x30 }, { 0x98, 0x20 },
  { 0x99, 0x30 }, { 0x9a, 0x84 },
  { 0x9b, 0x29 }, { 0x9c, 0x03 },
  { 0x9d, 0x4c }, { 0x9e, 0x3f },
  { 0x78, 0x04 },

  /* Extra-weird stuff.  Some sort of multiplexor register */
  { 0x79, 0x01 }, { 0xc8, 0xf0 },
  { 0x79, 0x0f }, { 0xc8, 0x00 },
  { 0x79, 0x10 }, { 0xc8, 0x7e },
  { 0x79, 0x0a }, { 0xc8, 0x80 },
  { 0x79, 0x0b }, { 0xc8, 0x01 },
  { 0x79, 0x0c }, { 0xc8, 0x0f },
  { 0x79, 0x0d }, { 0xc8, 0x20 },
  { 0x79, 0x09 }, { 0xc8, 0x80 },
  { 0x79, 0x02 }, { 0xc8, 0xc0 },
  { 0x79, 0x03 }, { 0xc8, 0x40 },
  { 0x79, 0x05 }, { 0xc8, 0x30 },
  { 0x79, 0x26 },
  { 0xff, 0xff }, /* END MARKER */
};

const uint8_t reg_vals2[][2] = {
    {0x12, 0x80}, // Soft reset
    //{0xFF, 0xFF}, // delay
    {0x11, 0x80}, // Internal clock pre-scaler
    {0x12, 0x14}, // Output format: RGB
    {0x17, 0x16}, // HSTART
    {0x18, 0x04}, // HSTOP
    {0x19, 0x02}, // VSTART
    {0x1A, 0x7A}, // VSTOP
    {0x40, 0xD0}, // RGB565
    {0x7A, 0x20}, // SLOP
    {0x7B, 0x10},
    {0x7C, 0x1E},
    {0x6F, 0x9F}, // Simple white balance
    {0xA5, 0x05}, // BD50MAX
    {0xAB, 0x07}, // BD60MAX
    {0x3A, 0x04}, // TSLB
    {0x15, 0x00}, // PCLK does not toggle on HBLANK
};

ov7670::ov7670()
:m_img(default_width, default_height, default_Itype)
{
    m_SCL = SCL;
    m_SDA = SDA;
    m_VS = VS;
    m_HS = HS;
    m_PLK = PLK;
    m_XLK = XLK;
    m_D0 = D0;
    m_D1 = D1;
    m_D2 = D2;
    m_D3 = D3;
    m_D4 = D4;
    m_D5 = D5;
    m_D6 = D6;
    m_D7 = D7;
}
ov7670::ov7670(uint width, uint height, IMG_Type Itype)
:m_img(width, height, Itype)
{
    m_SCL = SCL;
    m_SDA = SDA;
    m_VS = VS;
    m_HS = HS;
    m_PLK = PLK;
    m_XLK = XLK;
    m_D0 = D0;
    m_D1 = D1;
    m_D2 = D2;
    m_D3 = D3;
    m_D4 = D4;
    m_D5 = D5;
    m_D6 = D6;
    m_D7 = D7;
}

void ov7670::ov7670_pin_init()
{
    this->pin_mask = MASK(m_SDA) | MASK(m_SCL) | MASK(m_VS) | MASK(m_HS) |
                     MASK(m_PLK) | MASK(m_XLK) | MASK(m_D0) | MASK(m_D1) |
                     MASK(m_D2) | MASK(m_D3) | MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    this->data_mask = MASK(m_D0) | MASK(m_D1) | MASK(m_D2) | MASK(m_D3) | 
                      MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    printf("%32b\n",this->data_mask);
    printf("%32b\n",MASK(m_D6));
    printf("%32b\n",MASK(m_D4));
    printf("%32b\n",MASK(m_D2));
    printf("%32b\n",MASK(m_D0));
    printf("%32b\n",MASK(m_D1));
    printf("%32b\n",MASK(m_D3));
    printf("%32b\n",MASK(m_D5));
    printf("%32b\n",MASK(m_D7));
    blink(15);
    gpio_init_mask(this->pin_mask);
    gpio_set_dir(m_SCL, GPIO_OUT);
    gpio_set_dir(m_SDA, GPIO_OUT); // INOUT
    gpio_set_dir(m_VS, GPIO_IN);
    gpio_set_dir(m_HS, GPIO_IN);
    gpio_set_dir(m_PLK, GPIO_IN);
    //gpio_set_dir(m_XLK, GPIO_OUT);
    gpio_set_dir(m_D0, GPIO_IN);
    gpio_set_dir(m_D1, GPIO_IN);
    gpio_set_dir(m_D2, GPIO_IN);
    gpio_set_dir(m_D3, GPIO_IN);
    gpio_set_dir(m_D4, GPIO_IN);
    gpio_set_dir(m_D5, GPIO_IN);
    gpio_set_dir(m_D6, GPIO_IN);
    gpio_set_dir(m_D7, GPIO_IN);

    // PWM setting on GPIO xlk pin
    gpio_set_function(m_XLK, GPIO_FUNC_PWM);   

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(m_XLK);
    uint32_t sys_clk_freq = clock_get_hz(clk_sys); // 시스템 클럭 주파수 얻기 (보통 125 MHz)
    uint16_t wrap = (sys_clk_freq / 24000000) - 1; 
    pwm_set_wrap(slice_num, wrap);
    // Setting duty cycle. if pin num is odd-> chan A, even -> chan B
    if(m_XLK%2==0)
        pwm_set_chan_level(slice_num, PWM_CHAN_A, wrap/2);
    else pwm_set_chan_level(slice_num, PWM_CHAN_B, wrap/2);
    // Set the PWM running
    pwm_set_enabled(slice_num, true);
}
void ov7670::ov7670_init(i2c_inst_t *i2c, uint32_t baudrate)
{
    ov7670_pin_init();
    gpio_set_function(m_SDA, GPIO_FUNC_I2C);
    gpio_set_function(m_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(m_SCL);
    gpio_pull_up(m_SDA);
    bi_decl(bi_2pins_with_func(m_SDA, m_SCL, GPIO_FUNC_I2C));

    i2c_init(i2c, baudrate);
    int num=sizeof(reg_vals);

    debug_print(debug, "reg_vals = %d\n",num);
    // printf("reg_vals = %d\n",sizeof(reg_vals));
    for (size_t i = 0; i < num / sizeof(reg_vals[0]); i++)
    {
        debug_print(debug,"sending(%d)\n",i);
        debug_print(debug,"adress=%x,data=%x\n",reg_vals[i][0],reg_vals[i][1]);
        int message=i2c_write_blocking(i2c, ov7670_I2C_ADDRESS , reg_vals[i], 2, false);
        if (message==PICO_ERROR_GENERIC)
            debug_print(debug,"genereic\n");
        else if( message==PICO_ERROR_TIMEOUT)
            debug_print(debug,"timeout\n");
        else if (message==PICO_ERROR_NO_DATA)
            debug_print(debug,"no data\n");
        else debug_print(debug,"ok\n");
    }
    debug_print(debug, "finished ov7670 initializing\n");
}

Mat ov7670::getFrame()
{
    uint16_t pixel=0;
    int16_t rows = this->m_img.rows;
    int16_t cols = this->m_img.cols;

    // wait until frame start(vertical data start)
    debug_print(debug,"getFrame start\n");
    while (!(gpio_get(m_VS)));
    debug_print(debug,"vs high\n");
     for (int height = 0; height < rows; height++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)));
        debug_print(debug,"hs high\n");
            
        for (int width = 0; width < cols; width++)
        {
            wait_for_posedge(m_PLK);
            pixel = static_cast<uint16_t>(get_word_data()) << 8;//high
            wait_for_negedge(m_PLK);

            wait_for_posedge(m_PLK);
            pixel=pixel|static_cast<uint16_t>(get_word_data());//low
            wait_for_negedge(m_PLK);
            
            //save pixel values in data array
            m_img.data[width+cols*height]=pixel;
             //debug_print(debug,"(%x)pixel saved.\n",pixel);
        }
        

    }
    blink(4);
    return m_img;
}

Mat ov7670::getFrame(uint16_t width, uint16_t height, IMG_Type Itype)
{
    uint16_t pixel=0;

    // wait until frame start(vertical data start)
    while (!(gpio_get(m_VS)));
        
    for (int i = 0; i < height; i++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)));
            
        for (int j = 0; j < width; j++)
        {
            wait_for_posedge(m_PLK);
            pixel = static_cast<uint16_t>(get_word_data()) << 8;//high
            wait_for_negedge(m_PLK);

            wait_for_posedge(m_PLK);
            pixel=pixel | static_cast<uint16_t>(get_word_data());//low
            wait_for_negedge(m_PLK);
            
            //save pixel values in data array
            m_img.data[j+width*i]=pixel;
        }
        

    }
    return m_img;
}

uint8_t ov7670::get_word_data()
{
    uint8_t word_data=0;
    uint32_t d_mask=this->data_mask;
    uint32_t raw_data= gpio_get_all();
    uint32_t data=raw_data&d_mask;
    int count=0;
    //printf("d_mask=%08x\n",d_mask);
    debug_print(debug,"starting get word data\n");
    for(uint i = 0; i < 32; i++)
    {
        //printf("data=%08x\n",data);
        //printf("data&1ul =%8x\n",data & 1ul);
        if(data & 1ul)
        {
            //blink(1);
            count++;
            word_data=word_data | ((uint8_t)1 << count);
        }
        data = data >> 1;
        
    }
    //debug_print(debug,"8bit data count = %d bits\n",count);
    if(count>=8) printf("data error, got more then 8bits\n");
    return word_data;
}
