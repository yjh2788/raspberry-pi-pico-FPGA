#include "pico/stdlib.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Array.h"

void pinMode2(int led_pin, int mode)
{
    if (mode)
    {
        gpio_set_dir(led_pin, GPIO_IN);
    }
    else
    {
        gpio_set_dir(led_pin, GPIO_OUT);
    }
}
void blink2(int num)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);
    pinMode2(LED_PIN,0);
    gpio_put(LED_PIN, 0);
    sleep_ms(1000);

    for (int i =0; i<num;i++)
    {
        gpio_put(LED_PIN, 1);
        sleep_ms(250);
        gpio_put(LED_PIN, 0);
        sleep_ms(250);
    }
}


template<typename T>
Array<T>::Array(size_t size) {
    _size = size;
    data = (T*)malloc(sizeof(T)*size);//new T[size];  // memory allocation
    if (data == nullptr) {
        //std::cerr << "Memory allocation failed" << std::endl;
        _size = 0;
    }
}

template<typename T>
Array<T>::Array() {
    _size = 0;
    data = nullptr;
}

template<typename T>
Array<T>::~Array() {
    delete[] this->data;  // free memory
    this->data = NULL;
    this->_size = 0;
}

template<typename T>
T& Array<T>::operator[](size_t index) {
    return data[index];
}

template<typename T>
size_t Array<T>::size() const {
    return _size;
}

template<typename T>
 Array<T> Array<T>::createArray(size_t size)
{
    return Array<T>(size);
}

template<typename T>
void Array<T>::freeArray(Array<T>* arr) {
    delete []arr->data;
    arr->data = NULL;
    arr->_size = 0;
}

template<typename T>
void Array<T>::freeArray() {
    delete []this->data;
    this->data = NULL;
    this->_size = 0;
}

template class Array<uint8_t>;
template class Array<uint16_t>;
template class Array<uint32_t>;
template class Array<uint64_t>;
template class Array<int8_t>;
template class Array<int16_t>;
template class Array<int32_t>;
template class Array<int64_t>;
template class Array<double>;
template class Array<float>;