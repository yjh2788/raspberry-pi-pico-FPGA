#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

#include "ov7670.h"
#include "TFT.h"

class Display{
public:
    TFT_LCD tft;
    ov7670 cam;
    
    Display();
    void setRes_type(resolution res,IMG_Type type);
    
};



#endif