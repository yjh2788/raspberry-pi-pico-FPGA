#include "SIMD.h"
#include "BUS_control.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"



#if defined(SIMD_V1)
inline void cs_select()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_CS, 1); // Active high
  asm volatile("nop \n nop \n nop");
}

inline void cs_deselect()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_CS, 0);
  asm volatile("nop \n nop \n nop");
}

inline void AD_DATA()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_AD, 0);
  asm volatile("nop \n nop \n nop");
}

inline void AD_ADDR()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_AD, 1); 
  asm volatile("nop \n nop \n nop");
}

inline void WR_LOW()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_WR, 0); 
  asm volatile("nop \n nop \n nop");
}

inline void WR_HIGH()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_WR, 1); 
  asm volatile("nop \n nop \n nop");
}

inline void RD_LOW()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_RD, 0); 
  asm volatile("nop \n nop \n nop");
}

inline void RD_HIGH()
{
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_RD, 1);
  asm volatile("nop \n nop \n nop \n nop \n nop \n nop");
}
inline void EXCUTE()
{
  //asm volatile("nop \n nop \n nop");
  //gpio_put(SIMD_EXCUTE, 0); 
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_EXCUTE, 1); 
  asm volatile("nop \n nop \n nop");
  gpio_put(SIMD_EXCUTE, 0); 
}

inline void write8(uint8_t data)
{
  gpio_put_masked(0xff, data); 
  WR_LOW();
  WR_HIGH();
}
void SIMD_init()
{
    gpio_init(SIMD_RD);
    gpio_set_dir(SIMD_RD, GPIO_OUT);
    gpio_put(SIMD_RD, 0);
    
    gpio_init(SIMD_WR);
    gpio_set_dir(SIMD_WR, GPIO_OUT);
    gpio_put(SIMD_WR, 0);

    gpio_init(SIMD_AD);
    gpio_set_dir(SIMD_AD, GPIO_OUT);
    gpio_put(SIMD_AD, 0);

    gpio_init(SIMD_CS);
    gpio_set_dir(SIMD_CS, GPIO_OUT);
    gpio_put(SIMD_CS, 0);

    gpio_init(SIMD_EXCUTE);
    gpio_set_dir(SIMD_EXCUTE, GPIO_OUT);
    gpio_put(SIMD_EXCUTE, 0);
}

void SIMD_write(uint8_t addr,uint8_t data)
{
    //cs_select();
    AD_ADDR();
    write8(addr);
    AD_DATA();
    write8(data);
    //cs_deselect();
}

void SIMD_read(uint8_t n,uint8_t* arr)
{
    //cs_select();
    uint8_t tmp=0;
    SIMD2PICO();
    for (int i=0;i<n;i++)
    {
        arr[i] = uint8_t(gpio_get_all()&0xff);
        //tmp=arr[i];
        //printf("*");
        //printf("res[i]=*%d\n",arr[i]);
        RD_LOW();
        RD_HIGH(); 
        //RD_LOW();
    }
   // cs_deselect();
}

_256_uint8_t _256_load_opA_uint8_t(uint8_t* op_A)
{
    _256_uint8_t a;
    cs_select();
    for(int i=0;i<32;i++)
    {
        a.uint8_data[i] = op_A[i];
        SIMD_write(i,op_A[31-i]);
    }
    cs_deselect();
    return a;
}
_128_uint8_t _128_load_opA_uint8_t(uint8_t* op_A)
{
    _128_uint8_t a;
    cs_select();
   for(int i=0;i<16;i++)
    {
        a.uint8_data[i] = op_A[i];
        SIMD_write(i+16,op_A[15-i]);
    }
    cs_deselect();
    return a;
}
_64_uint8_t _64_load_opA_uint8_t(uint8_t* op_A)
{
    _64_uint8_t a;
    cs_select();
    for(int i=0;i<8;i++)
    {
        a.uint8_data[i] = op_A[i];
        SIMD_write(i+24,op_A[7-i]);
    }
    cs_deselect();
    return a;
}
_256_uint8_t _256_load_opB_uint8_t(uint8_t* op_B)
{
    _256_uint8_t a;
    cs_select();
    for(int i=0;i<32;i++)
    {
        a.uint8_data[i] = op_B[i];
        SIMD_write(i+32,op_B[31-i]);
    }
    cs_deselect();
    return a;
}
_128_uint8_t _128_load_opB_uint8_t(uint8_t* op_B)
{
    _128_uint8_t a;
    cs_select();
    for(int i=0;i<16;i++)
    {
        a.uint8_data[i] = op_B[i];
        SIMD_write(i+48,op_B[15-i]);
    }
    cs_deselect();
    return a;
}

_64_uint8_t _64_load_opB_uint8_t(uint8_t* op_B)
{
     _64_uint8_t a;
     cs_select();
    for(int i=0;i<8;i++)
    {
        a.uint8_data[i] = op_B[i];
        SIMD_write(i+56,op_B[7-i]);
    }
    cs_deselect();
    return a;
}
//op=0->op_A, op=1->op_B
_256_uint8_t _256_load_uint8_t(bool op,uint8_t* ops)
{
    _256_uint8_t a;
    cs_select();
    uint8_t offset;
    if(op==opA) offset=0;
    else if(op==opB) offset=32;
    for(int i=0;i<32;i++)
    {
        a.uint8_data[i] = ops[i];
        SIMD_write(i+offset,ops[31-i]);
    }
    cs_deselect();
    return a;
}
//op=0->op_A, op=1->op_B
_128_uint8_t _128_load_uint8_t(bool op,uint8_t* ops)
{
    _128_uint8_t a;
    cs_select();
    uint8_t offset;
    if(op==opA) offset=0;
    else if(op==opB) offset=32;
    for(int i=0;i<16;i++)
    {
        a.uint8_data[i] = ops[i];
        SIMD_write(i+offset+16,ops[15-i]);
    }
    cs_deselect();
    return a;
}
//op=0->op_A, op=1->op_B
_64_uint8_t _64_load_uint8_t(bool op,uint8_t* ops)
{
    _64_uint8_t a;
    cs_select();
    uint8_t offset;
    if(op==opA) offset=0;
    else if(op==opB) offset=32;
    for(int i=0;i<8;i++)
    {
        a.uint8_data[i] = ops[i];
        SIMD_write(i+offset+24,ops[7-i]);
    }
    cs_deselect();
    return a;
}

void _256_load_ops_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    PICO2SIMD();
    //cs_select(); 
    for(int i=0;i<32;i++)
    {
        //SIMD_write(i,op_A[31-i]);
        //SIMD_write(i+32,op_B[31-i]);
        AD_ADDR();
        write8(i);
        AD_DATA();
        write8(op_A[31-i]);
        AD_ADDR();
        write8(i+32);
        AD_DATA();
        write8(op_B[31-i]);
    }
    //cs_deselect();

}
void _128_load_ops_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    PICO2SIMD();
    //cs_select();
    for(int i=16;i<32;i++)
    {
        //SIMD_write(i+16,op_A[15-i]);
        //SIMD_write(i+48,op_B[15-i]);
        AD_ADDR();
        write8(i);
        AD_DATA();
        write8(op_A[31-i]);
        AD_ADDR();
        write8(i+32);
        AD_DATA();
        write8(op_B[31-i]);
    }
    //cs_deselect();
}
void _64_load_ops_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    PICO2SIMD();
    //cs_select();
    for(int i=24;i<32;i++)
    {
        //SIMD_write(i+24,op_A[7-i]);
        //SIMD_write(i+56,op_B[7-i]);
        AD_ADDR();
        write8(i);
        AD_DATA();
        write8(op_A[31-i]);
        AD_ADDR();
        write8(i+32);
        AD_DATA();
        write8(op_B[31-i]);
    }
    //cs_deselect();
}

_256_uint8_t _256_ADD_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    _256_uint8_t a;
    cs_select();
    _256_load_ops_uint8_t(op_A,op_B); 
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(32,a.uint8_data);
    cs_deselect();
    return a;
}
_128_uint8_t _128_ADD_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    _128_uint8_t a;
    cs_select();
    _128_load_ops_uint8_t(op_A,op_B);  
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(16,a.uint8_data);
    cs_deselect();
    return a;
}

_64_uint8_t _64_ADD_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    _64_uint8_t a;
    cs_select();
    _64_load_ops_uint8_t(op_A,op_B); 
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(8,a.uint8_data);
    cs_deselect();
    return a;
}

_256_uint8_t _256_ADD_uint8_t(_256_uint8_t op_A, _256_uint8_t op_B)
{
    _256_uint8_t a;
    cs_select();
    _256_load_ops_uint8_t(op_A.uint8_data,op_B.uint8_data);
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(32,a.uint8_data);
    cs_deselect();
    return a;
}

_128_uint8_t _128_ADD_uint8_t(_128_uint8_t op_A, _128_uint8_t op_B)
{
    _128_uint8_t a;
    cs_select();
    _128_load_ops_uint8_t(op_A.uint8_data,op_B.uint8_data);
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(16,a.uint8_data);
    cs_deselect();
    return a;
}
_64_uint8_t _64_ADD_uint8_t(_64_uint8_t op_A, _64_uint8_t op_B)
{
    _64_uint8_t a;
    cs_select();
    _64_load_ops_uint8_t(op_A.uint8_data,op_B.uint8_data); 
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(8,a.uint8_data);
    cs_deselect();
    return a;
}
_256_uint8_t _256_ADD_uint8_t()
{
    _256_uint8_t a;
    cs_select();
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(32,a.uint8_data);
    cs_deselect();
    return a;
}
_128_uint8_t _128_ADD_uint8_t()
{
    _128_uint8_t a;
    cs_select();
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(16,a.uint8_data);
    cs_deselect();
    return a;
}
_64_uint8_t _64_ADD_uint8_t()
{
    _64_uint8_t a;
    cs_select();
    SIMD_write(reg_MODE,ADD);
    EXCUTE();
    SIMD_read(8,a.uint8_data);
    cs_deselect();
    return a;
}
_256_uint8_t _256_mul_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    _256_uint8_t a;
    cs_select();
    _256_load_ops_uint8_t(op_A,op_B); 
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(32,a.uint8_data);
    cs_deselect();
    return a;
}
_128_uint8_t _128_mul_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    _128_uint8_t a;
    cs_select();
    _128_load_ops_uint8_t(op_A,op_B);  
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(16,a.uint8_data);
    cs_deselect();
    return a;
}
_64_uint8_t _64_mul_uint8_t(uint8_t* op_A, uint8_t* op_B)
{
    _64_uint8_t a;
    cs_select();
    _64_load_ops_uint8_t(op_A,op_B); 
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(8,a.uint8_data);
    cs_deselect();
    return a;
}
_256_uint8_t _256_mul_uint8_t(_256_uint8_t op_A, _256_uint8_t op_B)
{
    _256_uint8_t a;
    cs_select();
    _256_load_ops_uint8_t(op_A.uint8_data,op_B.uint8_data); 
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(32,a.uint8_data);
    cs_deselect();
    return a;
}
_128_uint8_t _128_mul_uint8_t(_128_uint8_t op_A, _128_uint8_t op_B)
{
    _128_uint8_t a;
    cs_select();
    _128_load_ops_uint8_t(op_A.uint8_data,op_B.uint8_data);  
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(16,a.uint8_data);
    cs_deselect();
    return a;
}
_64_uint8_t _64_mul_uint8_t(_64_uint8_t op_A, _64_uint8_t op_B)
{
    _64_uint8_t a;
    cs_select();
    _64_load_ops_uint8_t(op_A.uint8_data,op_B.uint8_data);  
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(8,a.uint8_data);
    cs_deselect();
    return a;
}

_256_uint8_t _256_mul_uint8_t()
{
    _256_uint8_t a;
    cs_select();
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(32,a.uint8_data);
    cs_deselect();
    return a;
}
_128_uint8_t _128_mul_uint8_t()
{
    _128_uint8_t a;
    cs_select();
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(16,a.uint8_data);
    cs_deselect();
    return a;
}
_64_uint8_t _64_mul_uint8_t()
{
    _64_uint8_t a;
    cs_select();
    SIMD_write(reg_MODE,MUL);
    EXCUTE();
    SIMD_read(8,a.uint8_data);
    cs_deselect();
    return a;
}


#endif 