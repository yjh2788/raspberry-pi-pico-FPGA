#include "ov7670.h"
#include "pico/error.h"
#include "hardware/clocks.h"
#include "myfunc.h"

const uint8_t vga_ov7670[][2] = {
    {REG_CLKRC,0x01},
    {REG_COM7,COM7_FMT_VGA},
    {REG_COM3,0x0},
	{REG_COM14, 0},	// divide by 4
    {REG_SCALING_XSC,0x3A},
    {REG_SCALING_YSC,0x35},
	{REG_SCALING_DCWCTR, 0x11},		// downsample by 4
	{REG_SCALING_PCLK_DIV, 0xf0},		// divide by 4
	{REG_SCALING_PCLK_DELAY, 0x02},
	{REG_HREF,0xF6},	// was B6  
	{REG_HSTART,0x13},		// HSTART
	{REG_HSTOP,0x01},		// HSTOP
	{REG_VSTART,0x02},		// VSTART
	{REG_VSTOP,0x7a},		// VSTOP
	{REG_VREF,0x0a},	// VREF
	{0xff, 0xff},		/* END MARKER */
};

const uint8_t qvga_ov7670[][2] = {
    {REG_CLKRC,0x01},//01
    {REG_COM7,COM7_FMT_QVGA},
    {REG_COM3,COM3_DCWEN},
	{REG_COM14, 0x19},	// divide by 4
    {REG_SCALING_XSC,0x3A},
    {REG_SCALING_YSC,0x35},
	{REG_SCALING_DCWCTR, 0x11},		// downsample by 4
	{REG_SCALING_PCLK_DIV, 0xf1},		// divide by 4
	{REG_SCALING_PCLK_DELAY, 0x02}, //02
	{REG_HSTART,0x16},
	{REG_HSTOP,0x04},
	{REG_HREF,0x24},
	{REG_VSTART,0x02},
	{REG_VSTOP,0x7a},
	{REG_VREF,0x0a},
	{0xff, 0xff},	/* END MARKER */
};

const uint8_t qqvga_ov7670[][2] = {
    {REG_CLKRC,0x00},
    {REG_COM7,COM7_FMT_VGA},
    {REG_COM3,COM3_DCWEN},
	{REG_COM14, 0x1a},	// divide by 4
    {REG_SCALING_XSC,0x3A},
    {REG_SCALING_YSC,0x35},
	{REG_SCALING_DCWCTR, 0x22},		// downsample by 4
	{REG_SCALING_PCLK_DIV, 0xf2},		// divide by 4 
	{REG_SCALING_PCLK_DELAY, 0x00},
    {REG_HSTART,0x16},
	{REG_HSTOP,0x04 },
	{REG_HREF,0xa4},		//a4   
	{REG_VSTART,0x02},
	{REG_VSTOP,0x7a},
	{REG_VREF,0x0a},

	{0xff, 0xff},	/* END MARKER */
};

const uint8_t qqvga_ov7670_2[][2]={
    {REG_COM14, 0x1a},	// divide by 4
	{0x72, 0x22},		// downsample by 4
	{0x73, 0xf2},		// divide by 4
	{REG_HSTART,0x16},
	{REG_HSTOP,0x04},
	{REG_HREF,0xa4},		   
	{REG_VSTART,0x02},
	{REG_VSTOP,0x7a},
	{REG_VREF,0x0a},
    
	{0xff, 0xff},	/* END MARKER */
};

const uint8_t cif_ov7670[][2] = {
    {REG_CLKRC,0x01},
    {REG_COM7,COM7_FMT_VGA},
    {REG_COM3,COM3_DCWEN},
	{REG_COM14, 0x1a},	// divide by 4
    {REG_SCALING_XSC,0x3A},
    {REG_SCALING_YSC,0x35},
	{REG_SCALING_DCWCTR, 0x22},		// downsample by 4
	{REG_SCALING_PCLK_DIV, 0xf2},		// divide by 4
	{REG_SCALING_PCLK_DELAY, 0x02},
	//{ REG_COM3, COM3_DCWEN },
	//{ REG_COM14, COM14_DCWEN | 0x01},
	{ 0x7b, 0x1c },
	{ 0x7c, 0x28 },
	{ 0x7d, 0x3c },
	{ 0x7f, 0x69 },
	{ REG_COM9, 0x38 },
	{ 0xa1, 0x0b },
	{ 0x74, 0x19 },
	{ 0x9a, 0x80 },
	{ 0x43, 0x14 },
	{ REG_COM13, 0xc0 },
	{ 0xff, 0xff },
};

const uint8_t qcif_ov7670[][2] = {
    { REG_COM3, COM3_SCALEEN|COM3_DCWEN },
	{ REG_COM3, COM3_DCWEN },
	//{ REG_COM14, COM14_DCWEN | 0x01},
	{REG_COM14, 0x1a},	// divide by 4
	{0x72, 0x22},		// downsample by 4
	{0x73, 0xf2},		// divide by 4
	{ 0xa2, 0x52 },
	{ 0x7b, 0x1c },
	{ 0x7c, 0x28 },
	{ 0x7d, 0x3c },
	{ 0x7f, 0x69 },
	{ REG_COM9, 0x38 },
	{ 0xa1, 0x0b },
	{ 0x74, 0x19 },
	{ 0x9a, 0x80 },
	{ 0x43, 0x14 },
	{ REG_COM13, 0xc0 },
	{ 0xff, 0xff },
};

const uint8_t yuv422_ov7670[][2] = {
    {REG_TSLB,TSLB_YFIRST},
	{REG_COM7, COM7_YUV},	/* Selects YUV mode */
	{REG_RGB444, 0},	/* No RGB444 please */
	{REG_COM1, 0},
	{REG_COM15, COM15_R00FF},
	{REG_COM9, 0x6A},	/* 128x gain ceiling; 0x8 is reserved bit */
	{0x4f, 0x80},		/* "matrix coefficient 1" */
	{0x50, 0x80},		/* "matrix coefficient 2" */
	{0x51, 0},		/* vb */
	{0x52, 0x22},		/* "matrix coefficient 4" */
	{0x53, 0x5e},		/* "matrix coefficient 5" */
	{0x54, 0x80},		/* "matrix coefficient 6" */
	{REG_COM13,/*COM13_GAMMA|*/COM13_UVSAT},
	{0xff, 0xff},		/* END MARKER */
};

const uint8_t rgb565_ov7670[][2] = {
	{REG_COM7, COM7_RGB}, /* Selects RGB mode */
	{REG_RGB444, 0},	  /* No RGB444 please */
	{REG_COM1, 0x0},
	{REG_COM15, COM15_RGB565|COM15_R00FF},
	{REG_COM9, 0x6A},	 /* 128x gain ceiling; 0x8 is reserved bit */
	{0x4f, 0xb3},		 /* "matrix coefficient 1" */
	{0x50, 0xb3},		 /* "matrix coefficient 2" */
	{0x51, 0},		 /* vb */
	{0x52, 0x3d},		 /* "matrix coefficient 4" */
	{0x53, 0xa7},		 /* "matrix coefficient 5" */
	{0x54, 0xe4},		 /* "matrix coefficient 6" */
	{REG_COM13, /*COM13_GAMMA|*/COM13_UVSAT},
	{0xff, 0xff},	/* END MARKER */
};

const uint8_t bayerRGB_ov7670[][2] = {
	{REG_COM7, COM7_BAYER},
	{REG_COM13, 0x08}, /* No gamma, magic rsvd bit */
	{REG_COM16, 0x3d}, /* Edge enhancement, denoise */
	{REG_REG76, 0xe1}, /* Pix correction, magic rsvd */
	{0xff, 0xff},	/* END MARKER */
};

const uint8_t reg_vals[][2] = {
	{REG_TSLB,  0x00},	/* OV */
	{REG_COM7, 0},	/* VGA */
	/*
	 * Set the hardware window.  These values from OV don't entirely
	 * make sense - hstop is less than hstart.  But they work...
	 */
	{REG_HSTART, 0x13},	{REG_HSTOP, 0x01},
	{REG_HREF, 0xb6},	{REG_VSTART, 0x02},
	{REG_VSTOP, 0x7a},	{REG_VREF, 0x0a},
    { DBLV, 0x0a | DBLV_CLK4 },	
	{REG_COM3, 0},	{REG_COM14, 0},
	/* Mystery scaling numbers */
	{0x70, 0x3a},		{0x71, 0x35},
	{0x72, 0x11},		{0x73, 0xf0},
	{0xa2,/* 0x02 changed to 1*/1},{REG_COM10, COM10_VS_NEG},
	/* Gamma curve values */
	{0x7a, 0x20},		{0x7b, 0x10},
	{0x7c, 0x1e},		{0x7d, 0x35},
	{0x7e, 0x5a},		{0x7f, 0x69},
	{0x80, 0x76},		{0x81, 0x80},
	{0x82, 0x88},		{0x83, 0x8f},
	{0x84, 0x96},		{0x85, 0xa3},
	{0x86, 0xaf},		{0x87, 0xc4},
	{0x88, 0xd7},		{0x89, 0xe8},
    
	/* AGC and AEC parameters.  Note we start by disabling those features,
	   then turn them only after tweaking the values. */
	{REG_COM8, COM8_FASTAEC | COM8_AECSTEP},
	
	{REG_COM4, 0x40}, /* magic reserved bit */
	{REG_COM9, 0x18}, /* 4x gain + magic rsvd bit */
	{REG_BD50MAX, 0x05},	{REG_BD60MAX, 0x07},
	{REG_AEW, 0x95},	{REG_AEB, 0x33},
	{REG_VPT, 0xe3},	{REG_HAECC1, 0x78},
	{REG_HAECC2, 0x68},	{0xa1, 0x03}, /* magic */
	{REG_HAECC3, 0xd8},	{REG_HAECC4, 0xd8},
	{REG_HAECC5, 0xf0},	{REG_HAECC6, 0x90},
	{REG_HAECC7, 0x94},
	{REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AGC|COM8_AEC},
	{0x30,0},{0x31,0},//disable some delays
	/* Almost all of these are magic "reserved" values.  */
	{REG_COM5, 0x61},	{REG_COM6, 0x4b},
	{0x16, 0x02},		{REG_MVFP, 0x07},
	{0x21, 0x02},		{0x22, 0x91},
	{0x29, 0x07},		{0x33, 0x0b},
	{0x35, 0x0b},		{0x37, 0x1d},
	{0x38, 0x71},		{0x39, 0x2a},
	{REG_COM12, 0x78},	{0x4d, 0x40},
	{0x4e, 0x20},		{REG_GFIX, 0},
	/*{0x6b, 0x4a},*/		
	{0x8d, 0x4f},		{0x8e, 0},
	{0x8f, 0},		{0x90, 0},
	{0x91, 0},		{0x96, 0},
	{0x9a, 0},		{0xb0, 0x84},
	{0xb1, 0x0c},		{0xb2, 0x0e},
	{0xb3, 0x82},		{0xb8, 0x0a},

	/* More reserved magic, some of which tweaks white balance */
	{0x43, 0x0a},		{0x44, 0xf0},
	{0x45, 0x34},		{0x46, 0x58},
	{0x47, 0x28},		{0x48, 0x3a},
	{0x59, 0x88},		{0x5a, 0x88},
	{0x5b, 0x44},		{0x5c, 0x67},
	{0x5d, 0x49},		{0x5e, 0x0e},
	{0x6c, 0x0a},		{0x6d, 0x55},
	{0x6e, 0x11},		{0x6f, 0x9e}, /* it was 0x9F "9e for advance AWB" */
	{0x6a, 0x40},	

	//gain	
	{REG_BLUE, 0x80/*0x40*/},
	{REG_RED, 0x80/*0x06*/},
	{REG_GAIN,0x10}, 	{REG_AECH, 0x0},//0x10//exposure time
	//{REG_GAIN, 0},	{REG_AECH, 0},
	//
	{REG_COM8, COM8_FASTAEC|COM8_AECSTEP|COM8_AGC|COM8_AEC|COM8_AWB},

	/* Matrix coefficients */
	{0x4f, 0x80},		{0x50, 0x80},
	{0x51, 0},		    {0x52, 0x22},
	{0x53, 0x5e},		{0x54, 0x80},
	{0x58, 0x9e},
	{REG_COM16, COM16_AWBGAIN},	{REG_EDGE, 0},
	{0x74,0x10/*0x10*/},        {0x75, 0x0f/*0x05*/},		
    {REG_REG76, 0xe1},
	{0x4c, 0},		    {0x77, 0x01},
	{REG_COM13, /*0xc3*/0x48},	{0x4b, 0x09},
	{0xc9, 0x60},		/*{REG_COM16, 0x38},*/
	{0x56, 0x40},

	{0x34, 0x11},		{REG_COM11, COM11_EXP|COM11_HZAUTO},
	{0xa4, 0x82/*Was 0x88*/},		{0x96, 0},
	{0x97, 0x30},		{0x98, 0x20},
	{0x99, 0x30},		{0x9a, 0x84},
	{0x9b, 0x29},		{0x9c, 0x03},
	{0x9d, 0x4c},		{0x9e, 0x3f},
	{0x78, 0x04},

	/* Extra-weird stuff.  Some sort of multiplexor register */
	{0x79, 0x01},		{0xc8, 0xf0},
	{0x79, 0x0f},		{0xc8, 0x00},
	{0x79, 0x10},		{0xc8, 0x7e},
	{0x79, 0x0a},		{0xc8, 0x80},
	{0x79, 0x0b},		{0xc8, 0x01},
	{0x79, 0x0c},		{0xc8, 0x0f},
	{0x79, 0x0d},		{0xc8, 0x20},
	{0x79, 0x09},		{0xc8, 0x80},
	{0x79, 0x02},		{0xc8, 0xc0},
	{0x79, 0x03},		{0xc8, 0x40},
	{0x79, 0x05},		{0xc8, 0x30},
	{0x79, 0x26},       //{REG_COM10,32}

};

ov7670::ov7670()
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
    m_resolution = QQVGA;
    m_img_format = IMG_Type::RGB565;

}
ov7670::ov7670(IMG_Type Itype)
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
    m_resolution = QQVGA;
    m_img_format = Itype;
}
ov7670::ov7670(uint32_t width, uint32_t height, IMG_Type Itype)
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
    m_resolution = QQVGA;
    m_img_format = Itype;
}

void ov7670::ov7670_xclk_init(uint32_t xclk_freq,int duty)
{
    gpio_set_function(m_XLK, GPIO_FUNC_PWM);
    uint32_t chan;
    uint32_t slice_num = pwm_gpio_to_slice_num(m_XLK);
    if (m_XLK % 2 == 0)
        chan = PWM_CHAN_A;
    else
        chan = PWM_CHAN_B;
    // Set the PWM running
    pwm_set_freq_duty(slice_num,chan,xclk_freq,duty);
}

void ov7670::ov7670_PIO_pin_init()
{
    this->pin_mask = MASK(m_SDA) | MASK(m_SCL) | MASK(m_VS) | MASK(m_HS) |
                     MASK(m_PLK) | MASK(m_XLK) | MASK(m_D0) | MASK(m_D1) |
                     MASK(m_D2) | MASK(m_D3) | MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    this->data_mask = MASK(m_D0) | MASK(m_D1) | MASK(m_D2) | MASK(m_D3) |
                      MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    gpio_init_mask(this->pin_mask);
    gpio_set_dir(m_SCL, GPIO_OUT);
    gpio_set_dir(m_SDA, GPIO_OUT); 

    //xclk setting
    ov7670_xclk_init(8*MHz,50);
}

void ov7670::ov7670_pin_init()
{
    this->pin_mask = MASK(m_SDA) | MASK(m_SCL) | MASK(m_VS) | MASK(m_HS) |
                     MASK(m_PLK) | MASK(m_XLK) | MASK(m_D0) | MASK(m_D1) |
                     MASK(m_D2) | MASK(m_D3) | MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    this->data_mask = MASK(m_D0) | MASK(m_D1) | MASK(m_D2) | MASK(m_D3) |
                      MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    gpio_init_mask(this->pin_mask);
    gpio_set_dir(m_SCL, GPIO_OUT);
    gpio_set_dir(m_SDA, GPIO_OUT); 
    gpio_set_dir(m_VS, GPIO_IN);
    gpio_set_dir(m_HS, GPIO_IN);
    gpio_set_dir(m_PLK, GPIO_IN);
    // gpio_set_dir(m_XLK, GPIO_OUT);
    gpio_set_dir(m_D0, GPIO_IN);
    gpio_set_dir(m_D1, GPIO_IN);
    gpio_set_dir(m_D2, GPIO_IN);
    gpio_set_dir(m_D3, GPIO_IN);
    gpio_set_dir(m_D4, GPIO_IN);
    gpio_set_dir(m_D5, GPIO_IN);
    gpio_set_dir(m_D6, GPIO_IN);
    gpio_set_dir(m_D7, GPIO_IN);

    //xclk setting
    ov7670_xclk_init(8*MHz,50);
}
void ov7670::ov7670_init(i2c_inst_t *i2c, uint32_t baudrate)
{
    ov7670_pin_init();
    gpio_set_function(m_SDA, GPIO_FUNC_I2C);
    gpio_set_function(m_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(m_SCL);
    gpio_pull_up(m_SDA);
    bi_decl(bi_2pins_with_func(m_SDA, m_SCL, GPIO_FUNC_I2C));
    
    m_i2c = i2c;
    i2c_init(i2c, baudrate);
    sleep_ms(100);
    int num = sizeof(reg_vals)/sizeof(reg_vals[0]);
    sendCommand(0x12,0x80);
    sleep_ms(1);
    debug_print(debug, "reg_vals = %d\n", num);
    for (size_t i = 0; i < num ; i++)
    {
        sendCommand(reg_vals[i][0],reg_vals[i][1]);
        debug_print(debug, "sending(%d)\n", i);
        debug_print(debug, "adress=%x,data=%x\n", reg_vals[i][0], reg_vals[i][1]);
    }
    debug_print(debug, "finished ov7670 initializing\n");
}

void ov7670::ov7670_init(i2c_inst_t *i2c, uint32_t baudrate, bool PIO_enable)
{   
    if (PIO_enable) ov7670_PIO_pin_init(); 
    else ov7670_pin_init();

    gpio_set_function(m_SDA, GPIO_FUNC_I2C);
    gpio_set_function(m_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(m_SCL);
    gpio_pull_up(m_SDA);
    bi_decl(bi_2pins_with_func(m_SDA, m_SCL, GPIO_FUNC_I2C));
    
    m_i2c = i2c;
    i2c_init(i2c, baudrate);
    sleep_ms(100);
    int num = sizeof(reg_vals)/sizeof(reg_vals[0]);
    sendCommand(0x12,0x80);
    sleep_ms(1);
    debug_print(debug, "reg_vals = %d\n", num);
    for (size_t i = 0; i < num ; i++)
    {
        sendCommand(reg_vals[i][0],reg_vals[i][1]);
        debug_print(debug, "sending(%d)\n", i);
        debug_print(debug, "adress=%x,data=%x\n", reg_vals[i][0], reg_vals[i][1]);
    }
    debug_print(debug, "finished ov7670 initializing\n");
}



uint8_t ov7670::get_word_data()
{
    uint8_t pixel = uint8_t(gpio_get_all()&0xff);
    //debug_print(debug, "starting get word data\n");
    return pixel;
}

uint8_t ov7670::get_word_data2()
{
    uint32_t d_mask = this->data_mask;
    uint32_t input = gpio_get_all()&d_mask;
    uint8_t D0_ = uint8_t(gpio_get(m_D0));
    uint8_t D1_ = uint8_t(gpio_get(m_D1)) << 1;
    uint8_t D2_ = uint8_t(gpio_get(m_D2)) << 2;
    uint8_t D3_ = uint8_t(gpio_get(m_D3)) << 3;
    uint8_t D4_ = uint8_t(gpio_get(m_D4)) << 4;
    uint8_t D5_ = uint8_t(gpio_get(m_D5)) << 5;
    uint8_t D6_ = uint8_t(gpio_get(m_D6)) << 6;
    uint8_t D7_ = uint8_t(gpio_get(m_D7)) << 7;
    uint8_t pixel= D7_ | D6_ | D5_ |D4_ | D3_ | D2_ | D1_ | D0_;
  
    // printf("d_mask=%08x\n",d_mask);
    debug_print(debug, "pixel data: %x\n",pixel);
    
    return pixel;
}

uint8_t ov7670::get_word_data3()
{
    uint8_t word_data = 0;
    uint32_t d_mask = this->data_mask;
    uint32_t raw_data = gpio_get_all();
    uint32_t data = raw_data & d_mask;
    int count = 0;
    //printf("d_mask=%08x\n",d_mask);
    debug_print(debug, "starting get word data\n");
    for (uint32_t i = 0; i < 32; i++)
    {
        // printf("data=%08x\n",data);
        // printf("data&1ul =%8x\n",data & 1ul);
        if (data & 1ul)
        {
            // blink(1);
            count++;
            word_data = word_data | ((uint8_t)1 << count);
        }
        data = data >> 1;

    }
    // debug_print(debug,"8bit data count = %d bits\n",count);
    if (count > 8)
        printf("data error, got more then 8bits -> %d\n",count);
    return word_data;
}

uint8_t ov7670::sendCommand(const uint8_t *src)
{
    if (src == nullptr) {
        debug_print("Error: src is nullptr\n");
        return 1; // Error code for null pointer
    }
    uint8_t message = i2c_write_blocking(m_i2c, ov7670_I2C_ADDRESS, src, 2, false);

    if (message == PICO_ERROR_GENERIC)
        debug_print(debug, "genereic\n");
    else if (message == PICO_ERROR_TIMEOUT)
        debug_print(debug, "timeout\n");
    else if (message == PICO_ERROR_NO_DATA)
        debug_print(debug, "no data\n");
    else
        debug_print(debug, "ok\n");
    return message;
}

uint8_t ov7670::sendCommand_regs(const uint8_t src[][2])
{

    if (src == nullptr) {
        debug_print("Error: src is nullptr\n");
        return 1; // Error code for null pointer
    }
    int i=0;
    while(1)
    {
        if(src[i][1]==0xff || src[i][0]==0xff) break;
        int result = i2c_write_blocking(m_i2c, ov7670_I2C_ADDRESS, src[i], 2, false);
        if (result < 0) {
            debug_print("I2C write error: %d\n", result);
            return 2; // Error code for I2C failure
        }
        i++;
    }
    return 0;
}

uint8_t ov7670::sendCommand(const uint8_t reg, const uint8_t data)
{
    uint8_t message;
    uint8_t buf[2] = { reg, data };
    message = i2c_write_blocking(m_i2c, ov7670_I2C_ADDRESS, buf, 2, false);
    if (message == PICO_ERROR_GENERIC)
        debug_print(debug, "genereic\n");
    else if (message == PICO_ERROR_TIMEOUT)
        debug_print(debug, "timeout\n");
    else if (message == PICO_ERROR_NO_DATA)
        debug_print(debug, "no data\n");
    else
        debug_print(debug, "ok\n");
    return message;
}
uint8_t ov7670::readReg(const uint8_t reg)
{
    uint8_t data;
    i2c_write_blocking(m_i2c, ov7670_I2C_ADDRESS, &reg, 1, false);
    uint8_t message = i2c_read_blocking(m_i2c, ov7670_I2C_ADDRESS, &data, 1, false);

    if (message == PICO_ERROR_GENERIC)
        debug_print(debug, "genereic\n");
    else if (message == PICO_ERROR_TIMEOUT)
        debug_print(debug, "timeout\n");
    else if (message == PICO_ERROR_NO_DATA)
        debug_print(debug, "no data\n");
    else
        debug_print(debug, "ok\n");
    return data;
}

void ov7670::setResolution(resolution res)
{
    switch(res)
    {
        case resolution::VGA:
            m_resolution = VGA;
            sendCommand(REG_COM3,0);//disable scaling
            sendCommand_regs(vga_ov7670);
        break;

        case resolution::QVGA:
            m_resolution = QVGA;
            sendCommand(REG_COM3,4);//enable scaling
            sendCommand_regs(qvga_ov7670);
        break;

        case resolution::QQVGA:
            m_resolution = QQVGA;
            sendCommand(REG_COM3,4);//enable scaling
            //sendCommand_regs(qqvga_ov7670_2);
            sendCommand_regs(qqvga_ov7670);
        break;

        case resolution::CIF:
            m_resolution = CIF;
            sendCommand(REG_COM3,4);//enable scaling
            sendCommand_regs(cif_ov7670);
        break;

        case resolution::QCIF:
            m_resolution = QCIF;
            sendCommand(REG_COM3,4);//enable scaling
            sendCommand_regs(qcif_ov7670);
        break;
        
    }
    
}
void ov7670::setImageType(IMG_Type Itype)
{
    uint8_t freq;
    switch(Itype)
    {
        case IMG_Type::RGB565:
            sendCommand_regs(rgb565_ov7670);
            freq=readReg(0x11);
            sleep_ms(1);
            sendCommand(0x11,freq);
        break;
        case IMG_Type::GRAY:
            sendCommand_regs(yuv422_ov7670);
        break;
        case IMG_Type::YUV:
            sendCommand_regs(yuv422_ov7670);
        break;
        case IMG_Type::RAW_RGB:
            sendCommand_regs(bayerRGB_ov7670);
        break;
        case IMG_Type::p_RAW_RGB:
            sendCommand_regs(bayerRGB_ov7670);
        break;

    }
    
}


