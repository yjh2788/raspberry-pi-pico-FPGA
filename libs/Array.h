
#ifndef ARRAY_H
#define ARRAY_H
#include <stdio.h>
#include <iostream>

template<typename T>
class Array {
public:
    Array(size_t size);
    Array(size_t size, int num);
    Array();
    ~Array();

    T& operator[](size_t index);
    size_t size() const;

    static Array<T> createArray(size_t size);
    Array<T> zeros(size_t size);
    void freeArray(Array<T>* arr);
    void freeArray();
    T* getbuf();


private:
    T* data;
    size_t _size;
};

#endif