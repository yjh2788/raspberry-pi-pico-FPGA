#include "ImageProcessing.h"



Mat::Mat(uint16_t width, uint16_t height, IMG_Type Itype)
{
    this->cols=width;
    this->rows=height;
    this->img_type=Itype;
    if( Itype==IMG_Type::RGB565)
    {
        this->data=new uint16_t[width*height];
    }
    else if((Itype==IMG_Type::RGB888))
    {
        this->blue=new uint8_t[width*height];
        this->red=new uint8_t[width*height];
        this->green=new uint8_t[width*height];
    }
        
    else if((Itype==IMG_Type::GRAY))
    {
        this->gray= new uint8_t[width*height];
    }
    
}

Mat::~Mat()
{
    if(_msize(this->data)!=0) delete []this->data;
    if(_msize(this->red)!=0) delete []this->red;
    if(_msize(this->blue)!=0) delete []this->blue;
    if(_msize(this->green)!=0) delete []this->green;
    if(_msize(this->gray)!=0) delete []this->gray;

}

// RGB565 to RGB888 conversion function
void Mat::rgb565_to_rgb888(uint16_t rgb565_color, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    // Red channel extention
    red = ((rgb565_color >> 11) & 0x1F) << 3;

    // Green channel extention
    green = ((rgb565_color >> 5) & 0x3F) << 2;

    // Blue channel extention
    blue = ((rgb565_color >> 0) & 0x1F) << 3;
}

void Mat::rgb565_to_rgb888()
{
    // Red channel extention
    for(int i=0;i<this->cols*this->rows;i++)
    {
        this->red[i] = ((data[i] >> 11) & 0x1F) << 3;

        // Green channel extention
        this->green[i] = ((data[i] >> 5) & 0x3F) << 2;

        // Blue channel extention
        this->blue[i] = ((data[i] >> 0) & 0x1F) << 3;
    }    
}