#include <string>
#include <stdio.h>
#include "display_control.h"
#include "pico/types.h"
#include "SIMD.h"


int main()
{
    debug=false;
    Display disp(Green);
    
    stdio_init_all();
    bus_ctrl_pin_init();
    SIMD_init();
    
    blink(10);
    // uint8_t op_a[32]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    //                   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    //                   0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
    //                   0xff,0xff,
    //                   };
    //  uint8_t op_b[32]={0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
    //                   0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
    //                   0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,
    //                   0x0f,0x0f,
    //                   };
    uint8_t op_a[32]={1,2,3,4,5,6,7,8,9,10,
                      50,51,52,53,54,55,56,57,58,59,
                      101,102,103,104,105,106,107,108,109,110,
                      156,210,
                      };
     uint8_t op_b[32]={11,12,13,14,15,16,17,18,19,20,
                      40,41,42,43,44,45,46,47,48,49,
                      81,82,83,84,85,86,87,88,89,80,
                      178,25,
                      };

    _256_uint8_t result;
    uint8_t result_arr[32]={0,};
    //SIMD time
    absolute_time_t start_time = get_absolute_time(); 
    result =_256_ADD_uint8_t(op_a,op_b);
    //_256_load_ops_uint8_t(op_a,op_b);
    absolute_time_t end_time = get_absolute_time(); // tick end
    float simd_time=to_us_since_boot(end_time) - to_us_since_boot(start_time); 
    
    //normal calc time 
    start_time = get_absolute_time(); 
    for(int i=0;i<32;i++)
    {
        result_arr[i]=op_a[i]+op_b[i];
    }
    end_time = get_absolute_time(); // tick end
    float calc_time=to_us_since_boot(end_time) - to_us_since_boot(start_time); 
    
    printf("SIMD calc time= %f, SIMD result=[",simd_time);
    for(int i=0;i<32;i++)
    {
        printf("%d, ",result.uint8_data[i]);
    }
    printf("]\n");
    printf("normal calc time= %f, calc result=[",calc_time);
    for(int i=0;i<32;i++)
    {
        printf("%d, ",result_arr[i]);
    }
    printf("]\n");

}   