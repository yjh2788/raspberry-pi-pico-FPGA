#include "ov7670.h"
#include "pico/error.h"
#include "hardware/clocks.h"

typedef struct
{
    uint8_t hstart;
    uint8_t hstop;
    uint8_t href;
    uint8_t vstart;
    uint8_t vstop;
    uint8_t vref;
} OV7670_window_Reg;

OV7670_window_Reg Calc_Window_reg(int hstart_position, int vstart_position, int horizontal_length, int vertical_length)
{
    OV7670_window_Reg regs;

    // horizontal reg calc
    int hstop_position = hstart_position + horizontal_length;

    regs.hstart = uint8_t((hstart_position >> 3) & 0xFF);
    regs.hstop = uint8_t((hstop_position >> 3) & 0xFF);

    uint8_t href_start_adjust = hstart_position & 0x07;
    uint8_t href_stop_adjust = hstop_position & 0x07;

    // href[2:0]=hstart[2:0], href[5:3]=hstop[2:0]
    regs.href = href_start_adjust | (href_stop_adjust << 3);

    // vertical reg calc
    int vstop_position = vstart_position + vertical_length;

    regs.vstart = uint8_t((vstart_position >> 2) & 0xFF);
    regs.vstop = uint8_t((vstop_position >> 2) & 0xFF);

    uint8_t vstart_adjust = vstart_position & 0x03;
    uint8_t vstop_adjust = vstop_position & 0x03;
    // vref[1:0]=hstart[1:0], href[3:2]=hstop[1:0]
    regs.vref = vstart_adjust | (vstop_adjust << 2);

    return regs;
}

const uint8_t qvga_ov7670[][2] = {
    {REG_COM14, 0x19},
    {0x72, 0x11},
    {0x73, 0xf1},

    {REG_HSTART, 0x16},
    {REG_HSTOP, 0x04},
    {REG_HREF, 0xa4},
    {REG_VSTART, 0x02},
    {REG_VSTOP, 0x7a},
    {REG_VREF, 0x0a},
    {0xff, 0xff}, /* END MARKER */
};

const uint8_t reg_vals[][2] = {
    {REG_COM7, COM7_RESET},
    //{REG_TSLB, 0x04}, /* OV */
    //{REG_COM7, COM7_FMT_QVGA | COM7_RGB}, /* QVGA */
    {REG_COM7, COM7_FMT_QCIF | COM7_YUV}, // QCIF, YUV
    // Set the hardware window.
    {REG_HSTART, 0x00},
    {REG_HSTOP, 0x16},
    {REG_HREF, HREF_BOTH_ADJUSTED | 0x25},
    {REG_VSTART, 0x01},
    {REG_VSTOP, 0x28},
    {REG_VREF, 0x03}, // 0011

    {REG_COM3, 0},
    {REG_COM14, 0},
    /*  scaling numbers */
    {0x70, 0x3a},
    {0x71, 0x35},
    {0x72, 0x11},
    {0x73, 0xf0},
    {0xa2, /* 0x02 changed to 1*/ 1},
    {REG_COM10, 0x0},
    /* Gamma curve values */
    {REG_COM13, /*0x48*/ 0xc0}, // gamma enable//YUYV
    {0x7a, 0x20},
    {0x7b, 0x10},
    {0x7c, 0x1e},
    {0x7d, 0x35},
    {0x7e, 0x5a},
    {0x7f, 0x69},
    {0x80, 0x76},
    {0x81, 0x80},
    {0x82, 0x88},
    {0x83, 0x8f},
    {0x84, 0x96},
    {0x85, 0xa3},
    {0x86, 0xaf},
    {0x87, 0xc4},
    {0x88, 0xd7},
    {0x89, 0xe8},
    /* AGC and AEC parameters. */
    {REG_COM8, COM8_FASTAEC | COM8_AECSTEP},
    {REG_GAIN, 0},
    {REG_AECH, 0},
    {REG_COM4, 0x40}, /* magic reserved bit */
    {REG_COM9, 0x18}, /* 4x gain + magic rsvd bit */
    {REG_BD50MAX, 0x05},
    {REG_BD60MAX, 0x07},
    {REG_AEW, 0x95},
    {REG_AEB, 0x33},
    {REG_VPT, 0xe3},
    {REG_HAECC1, 0x78},
    {REG_HAECC2, 0x68},
    {0xa1, 0x03}, /* magic */
    {REG_HAECC3, 0xd8},
    {REG_HAECC4, 0xd8},
    {REG_HAECC5, 0xf0},
    {REG_HAECC6, 0x90},
    {REG_HAECC7, 0x94},
    {REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC},
    {0x30, 0},
    {0x31, 0}, // disable some delays
    /* Almost all of these are magic "reserved" values.  */
    {REG_COM5, 0x61},
    {REG_COM6, 0x4b},
    {0x16, 0x02},
    {REG_MVFP, 0x07},
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {REG_COM12, 0x78},
    {0x4d, 0x40},
    {0x4e, 0x20},
    {REG_GFIX, 0},
    /*{0x6b, 0x4a},*/ {0x74, 0x10},
    {0x8d, 0x4f},
    {0x8e, 0},
    {0x8f, 0},
    {0x90, 0},
    {0x91, 0},
    {0x96, 0},
    {0x9a, 0},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x82},
    {0xb8, 0x0a},

    /* More reserved magic, some of which tweaks white balance */
    {0x43, 0x0a},
    {0x44, 0xf0},
    {0x45, 0x34},
    {0x46, 0x58},
    {0x47, 0x28},
    {0x48, 0x3a},
    {0x59, 0x88},
    {0x5a, 0x88},
    {0x5b, 0x44},
    {0x5c, 0x67},
    {0x5d, 0x49},
    {0x5e, 0x0e},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9e}, /* it was 0x9F "9e for advance AWB" */
    {0x6a, 0x40},
    {REG_BLUE, 0x40},
    {REG_RED, 0x60},
    {REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_AGC | COM8_AEC | COM8_AWB},

    /* Matrix coefficients */
    {0x4f, 0x80},
    {0x50, 0x80},
    {0x51, 0},
    {0x52, 0x22},
    {0x53, 0x5e},
    {0x54, 0x80},
    {0x58, 0x9e},

    {REG_COM16, COM16_AWBGAIN},
    {REG_EDGE, 0},
    {0x75, 0x05},
    {REG_REG76, 0xe1},
    {0x4c, 0},
    {0x77, 0x01},

    {0x4b, 0x09},
    {0xc9, 0x60}, /*{REG_COM16, 0x38},*/
    {0x56, 0x40},

    {0x34, 0x11},
    {REG_COM11, COM11_EXP | COM11_HZAUTO},
    {0xa4, 0x82 /*Was 0x88*/},
    {0x96, 0},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x4c},
    {0x9e, 0x3f},
    {0x78, 0x04},

    /* Extra-weird stuff.  Some sort of multiplexor register */
    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},
    {0xff, 0xff}, /* END MARKER */
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
const uint8_t yuv422_ov7670[][2] = {
    {REG_COM7, 0x0}, /* Selects YUV mode */
    {REG_RGB444, 0}, /* disable RGB444 */
    {REG_COM1, 0},
    {REG_COM15, COM15_R00FF},
    {REG_COM9, 0x6A}, /* 128x gain ceiling; 0x8 is reserved bit */
    {0x4f, 0x80},     /* "matrix coefficient 1" */
    {0x50, 0x80},     /* "matrix coefficient 2" */
    {0x51, 0},        /* vb */
    {0x52, 0x22},     /* "matrix coefficient 4" */
    {0x53, 0x5e},     /* "matrix coefficient 5" */
    {0x54, 0x80},     /* "matrix coefficient 6" */
    {REG_COM13, COM13_UVSAT},
    {0xff, 0xff}, /* END MARKER */
};

ov7670::ov7670()
    : m_img(QCIF_width, QCIF_height, default_Itype)
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
    : m_img(width, height, Itype)
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
    m_resolution = QVGA;
    m_img_format = Itype;
}

void ov7670::ov7670_pin_init()
{
    this->pin_mask = MASK(m_SDA) | MASK(m_SCL) | MASK(m_VS) | MASK(m_HS) |
                     MASK(m_PLK) | MASK(m_XLK) | MASK(m_D0) | MASK(m_D1) |
                     MASK(m_D2) | MASK(m_D3) | MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    this->data_mask = MASK(m_D0) | MASK(m_D1) | MASK(m_D2) | MASK(m_D3) |
                      MASK(m_D4) | MASK(m_D5) | MASK(m_D6) | MASK(m_D7);

    printf("%32b\n", this->data_mask);
    printf("%32b\n", MASK(m_D6));
    printf("%32b\n", MASK(m_D4));
    printf("%32b\n", MASK(m_D2));
    printf("%32b\n", MASK(m_D0));
    printf("%32b\n", MASK(m_D1));
    printf("%32b\n", MASK(m_D3));
    printf("%32b\n", MASK(m_D5));
    printf("%32b\n", MASK(m_D7));
    blink(15);
    gpio_init_mask(this->pin_mask);
    gpio_set_dir(m_SCL, GPIO_OUT);
    gpio_set_dir(m_SDA, GPIO_OUT); // INOUT
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

    // PWM setting on GPIO xlk pin
    gpio_set_function(m_XLK, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    uint slice_num = pwm_gpio_to_slice_num(m_XLK);
    uint32_t sys_clk_freq = clock_get_hz(clk_sys); // 시스템 클럭 주파수 얻기 (보통 125 MHz)
    uint16_t wrap = (sys_clk_freq / 24000000) - 1;
    pwm_set_wrap(slice_num, wrap);
    // Setting duty cycle. if pin num is odd-> chan A, even -> chan B
    if (m_XLK % 2 == 0)
        pwm_set_chan_level(slice_num, PWM_CHAN_A, wrap / 2);
    else
        pwm_set_chan_level(slice_num, PWM_CHAN_B, wrap / 2);
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

    m_i2c = i2c;
    i2c_init(i2c, baudrate);
    int num = sizeof(reg_vals);

    debug_print(debug, "reg_vals = %d\n", num);
    // printf("reg_vals = %d\n",sizeof(reg_vals));
    for (size_t i = 0; i < num / sizeof(reg_vals[0]); i++)
    {
        debug_print(debug, "sending(%d)\n", i);
        debug_print(debug, "adress=%x,data=%x\n", reg_vals[i][0], reg_vals[i][1]);
    }
    debug_print(debug, "finished ov7670 initializing\n");
}

Mat ov7670::getFrame()
{
    uint16_t pixel = 0;
    int16_t rows = this->m_img.rows;
    int16_t cols = this->m_img.cols;

    // wait until frame start(vertical data start)
    debug_print(debug, "getFrame start\n");
    while (!(gpio_get(m_VS)));
    debug_print(debug, "vs high\n");
    for (int height = 0; height < rows; height++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)));
        debug_print(debug, "hs high\n");

        for (int width = 0; width < cols; width++)
        {
            wait_for_posedge(m_PLK);
            pixel = static_cast<uint16_t>(get_word_data()) << 8; // high
            wait_for_negedge(m_PLK);

            wait_for_posedge(m_PLK);
            pixel = pixel | static_cast<uint16_t>(get_word_data()); // low
            wait_for_negedge(m_PLK);

            // save pixel values in data array
            m_img.data[width + cols * height] = pixel;
            // debug_print(debug,"(%x)pixel saved.\n",pixel);
        }
         while (gpio_get(m_HS));

    }
    blink(4);
    return m_img;
}

Mat ov7670::getFrame(uint16_t width, uint16_t height, IMG_Type Itype)
{
    uint16_t pixel = 0;

    // wait until frame start(vertical data start)
    while (!(gpio_get(m_VS)))
        ;

    for (int i = 0; i < height; i++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)))
            ;

        for (int j = 0; j < width; j++)
        {
            wait_for_posedge(m_PLK);
            pixel = static_cast<uint16_t>(get_word_data()) << 8; // high
            wait_for_negedge(m_PLK);

            wait_for_posedge(m_PLK);
            pixel = pixel | static_cast<uint16_t>(get_word_data()); // low
            wait_for_negedge(m_PLK);

            // save pixel values in data array
            m_img.data[j + width * i] = pixel;
        }
    }
    return m_img;
}

uint8_t ov7670::get_word_data()
{
    uint8_t word_data = 0;
    uint32_t d_mask = this->data_mask;
    uint32_t raw_data = gpio_get_all();
    uint32_t data = raw_data & d_mask;
    int count = 0;
    // printf("d_mask=%08x\n",d_mask);
    debug_print(debug, "starting get word data\n");
    for (uint i = 0; i < 32; i++)
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
    if (count >= 8)
        printf("data error, got more then 8bits\n");
    return word_data;
}

uint8_t ov7670::sendCommand(const uint8_t *src)
{
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

uint8_t ov7670::sendCommand(const uint8_t reg, const uint8_t data)
{
    uint8_t message;
    i2c_write_blocking(m_i2c, ov7670_I2C_ADDRESS, &reg, 1, false);
    message = i2c_write_blocking(m_i2c, ov7670_I2C_ADDRESS, &data, 1, false);
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

void ov7670::setFormat(resolution res)
{
    OV7670_window_Reg window;
    if (res == resolution::VGA)
    {
        int width = 640;
        int height = 480;
        window = Calc_Window_reg(0x04, 0x04, width, height);
        sendCommand(REG_COM7, COM7_RGB | COM7_FMT_VGA);
    }
    else if (res == resolution::QVGA)
    {
        int width = 320;
        int height = 240;
        window = Calc_Window_reg(0x04, 0x04, width, height);
        sendCommand(REG_COM7, COM7_RGB | COM7_FMT_QVGA);
    }
    else if (res == resolution::CIF)
    {
        int width = 352;
        int height = 288;
        window = Calc_Window_reg(0x04, 0x04, width, height);
        sendCommand(REG_COM7, COM7_YUV | COM7_FMT_CIF);
    }
    else if (res == resolution::QCIF)
    {
        int width = 176;
        int height = 144;
        window = Calc_Window_reg(0x04, 0x04, width, height);
        sendCommand(REG_COM7, COM7_YUV | COM7_FMT_QCIF);
    }

    debug_print(debug, "hstart=%d\n", window.hstart);
    debug_print(debug, "hstop=%d\n", window.hstop);
    debug_print(debug, "href=%08x\n", window.href);
    debug_print(debug, "vstart=%d\n", window.vstart);
    debug_print(debug, "vstop=%d\n", window.vstop);
    debug_print(debug, "vref=%08x\n", window.vref);

    sendCommand(REG_HSTART, window.hstart);
    sendCommand(REG_HSTOP, window.hstop);
    sendCommand(REG_HREF, HREF_BOTH_ADJUSTED | window.href);
    sendCommand(REG_VSTART, window.vstart);
    sendCommand(REG_VSTOP, window.vstop);
    sendCommand(REG_VREF, window.vref);
}
void ov7670::setImageType(IMG_Type Itype)
{
    if (Itype == IMG_Type::RGB565)
    {
        uint8_t reg_com7 = readReg(REG_COM7);
        sendCommand(REG_COM7, reg_com7 | COM7_RGB);
        sendCommand(REG_COM15, COM15_RGB565 | COM15_DEFAULT);
        sendCommand(REG_RGB444, RGB444_DEFAULT);
    }
    else if (Itype == IMG_Type::GRAY)
    {

        sendCommand(0x12, 0x00); // COM7: RGB disable, YUV mode
        sendCommand(0x3d, 0x80); // COM13: UV fix
        sendCommand(0x3a, 0x04); // TSLB: YUV mode activate
    }
    else if (Itype == IMG_Type::YUV)
    {
        uint8_t reg_com7 = readReg(REG_COM7);
        sendCommand(REG_COM7, reg_com7 | COM7_YUV);
        sendCommand(REG_TSLB, TSLB_YFIRST | TSLB_AUTO_WINDOW_ENABLE);
        sendCommand(REG_COM13, COM13_GAMMA | COM13_UVSAT);
    }
    else if (Itype == IMG_Type::RAW_RGB)
    {
        uint8_t reg_com7 = readReg(REG_COM7);
        sendCommand(REG_COM7, reg_com7 | COM7_BAYER);
        sendCommand(REG_COM15, COM15_RGB565 | COM15_DEFAULT);
    }
    else if (Itype == IMG_Type::p_RAW_RGB)
    {
        uint8_t reg_com7 = readReg(REG_COM7);
        sendCommand(REG_COM7, reg_com7 | COM7_PBAYER);
        sendCommand(REG_COM15, COM15_RGB565 | COM15_DEFAULT);
    }
}