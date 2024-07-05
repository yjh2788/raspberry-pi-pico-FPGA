#include "ov7670.h"


uint8_t reg_vals[][2] = {
    {0x12, 0x80}, // Soft reset
    {0xFF, 0xFF}, // delay
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

    gpio_init_mask(this->pin_mask);
    gpio_set_dir(m_SCL, GPIO_OUT);
    gpio_set_dir(m_SDA, GPIO_OUT); // INOUT
    gpio_set_dir(m_VS, GPIO_IN);
    gpio_set_dir(m_HS, GPIO_IN);
    gpio_set_dir(m_PLK, GPIO_IN);
    gpio_set_dir(m_XLK, GPIO_OUT);
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
    // get pwm slice nuber of XCLK_PIN
    uint slice_num = pwm_gpio_to_slice_num(m_XLK);
    // PWM frequency setting
    pwm_set_clkdiv(slice_num, 12.5f); // 10MHz--> system clk frequency=125MHz/12.5=10MHz
    // PWM period setting
    pwm_set_wrap(slice_num, 1);
    // PWM enable
    pwm_set_enabled(slice_num, true);
}
void ov7670::ov7670_init(i2c_inst_t *i2c, uint32_t baudrate)
{
    ov7670_pin_init();

    i2c_init(i2c, baudrate);
    // gpio_set_function(m_SDA, GPIO_FUNC_I2C);
    // gpio_set_function(m_SCL, GPIO_FUNC_I2C);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    debug_print(debug, "reg_vals = %d\n", sizeof(reg_vals));
    // printf("reg_vals = %d\n",sizeof(reg_vals));
    for (size_t i = 0; i < sizeof(reg_vals) / sizeof(reg_vals[0]); i++)
    {
        i2c_write_blocking(i2c, reg_vals[i][0], &reg_vals[i][1], 1, true);
    }
}

Mat ov7670::getFrame()
{
    uint16_t pixel=0;
    int16_t rows = this->m_img.rows;
    int16_t cols = this->m_img.cols;

    // wait until frame start(vertical data start)
    while (!(gpio_get(m_VS)));
        
    for (int height = 0; height < rows; height++)
    {
        // wait until horizontal data start
        while (!(gpio_get(m_HS)));
            
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
        }
        

    }
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
    int count=0;
    for(uint i = 0; i < 32; i++)
    {
        if(d_mask & 1ul)
            word_data=word_data | ((uint8_t)1 << count);
        d_mask >> 1;
        count++;
    }
    debug_print(debug,"8bit data count = %d bits\n",count);
    return word_data;
}
