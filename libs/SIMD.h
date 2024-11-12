#ifndef SIMD_H
#define SIMD_H
#include<stdint.h>

#define SIMD_V1

typedef struct _256_uint8_t
{
    uint8_t uint8_data[32];
}_256_uint8_t;

typedef struct _128_uint8_t
{
    uint8_t uint8_data[16];
}_128_uint8_t;

typedef struct _64_uint8_t
{
    uint8_t uint8_data[8];
}_64_uint8_t;


#if defined(SIMD_V1)
//pin map
#define SIMD_RD 16
#define SIMD_WR 17
#define SIMD_AD 18
#define SIMD_CS 21
#define SIMD_EXCUTE 22
#define D0  0
#define D1  1
#define D2  2
#define D3  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7
#define opA 0
#define opB 1
//a+b=0, a*b=2, a*b+c'=1
#define reg_MODE 64
#define ADD 0
#define MUL 2
#define CONV 1

// #define 
inline void cs_select();
inline void cs_deselect();
inline void AD_DATA();
inline void AD_ADDR();
inline void WR_LOW();
inline void WR_HIGH();
inline void RD_LOW();
inline void RD_HIGH();
inline void EXCUTE();
inline void write8(uint8_t data);

void SIMD_init();
void SIMD_write(uint8_t addr,uint8_t data);
void SIMD_read(uint8_t n,uint8_t* arr);


_256_uint8_t _256_load_opA_uint8_t(uint8_t* op_A);
_128_uint8_t _128_load_opA_uint8_t(uint8_t* op_A);
_64_uint8_t _64_load_opA_uint8_t(uint8_t* op_A);
_256_uint8_t _256_load_opB_uint8_t(uint8_t* op_B);
_128_uint8_t _128_load_opB_uint8_t(uint8_t* op_B);
_64_uint8_t _64_load_opB_uint8_t(uint8_t* op_B);

_256_uint8_t _256_load_uint8_t(bool op,uint8_t* ops);
_128_uint8_t _128_load_uint8_t(bool op,uint8_t* ops);
_64_uint8_t _64_load_uint8_t(bool op,uint8_t* ops);

void _256_load_ops_uint8_t(uint8_t* op_A, uint8_t* op_B);
void _128_load_ops_uint8_t(uint8_t* op_A, uint8_t* op_B);
void _64_load_ops_uint8_t(uint8_t* op_A, uint8_t* op_B);

_256_uint8_t _256_ADD_uint8_t(uint8_t* op_A, uint8_t* op_B);
_128_uint8_t _128_ADD_uint8_t(uint8_t* op_A, uint8_t* op_B);
_64_uint8_t _64_ADD_uint8_t(uint8_t* op_A, uint8_t* op_B);

_256_uint8_t _256_ADD_uint8_t(_256_uint8_t op_A, _256_uint8_t op_B);
_128_uint8_t _128_ADD_uint8_t(_128_uint8_t op_A, _128_uint8_t op_B);
_64_uint8_t _64_ADD_uint8_t(_64_uint8_t op_A, _64_uint8_t op_B);

_256_uint8_t _256_ADD_uint8_t();
_128_uint8_t _128_ADD_uint8_t();
_64_uint8_t _64_ADD_uint8_t();

_256_uint8_t _256_mul_uint8_t(uint8_t* op_A, uint8_t* op_B);
_128_uint8_t _128_mul_uint8_t(uint8_t* op_A, uint8_t* op_B);
_64_uint8_t _64_mul_uint8_t(uint8_t* op_A, uint8_t* op_B);

_256_uint8_t _256_mul_uint8_t(_256_uint8_t op_A, _256_uint8_t op_B);
_128_uint8_t _128_mul_uint8_t(_128_uint8_t op_A, _128_uint8_t op_B);
_64_uint8_t _64_mul_uint8_t(_64_uint8_t op_A, _64_uint8_t op_B);

_256_uint8_t _256_mul_uint8_t();
_128_uint8_t _128_mul_uint8_t();
_64_uint8_t _64_mul_uint8_t();

#elif defined(SIMD_V2)
   //empty yet
#else//default setting
    #define SIMD_V1  //default SIMD v1
#endif

#endif
