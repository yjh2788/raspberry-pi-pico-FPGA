
#ifndef ARRAY_H
#define ARRAY_H
#include <stdio.h>
#include <iostream>

template<typename T>
class Array {
public:
    Array(size_t size);
    Array();
    ~Array();

    T& operator[](size_t index);
    size_t size() const;

    static Array<T> createArray(size_t size);
    void freeArray(Array<T>* arr);
    void freeArray();


private:
    T* data;
    size_t _size;
};

template<typename T>
Array<T>::Array(size_t size) {
    _size = size;
    data = new T[size];  // memory allocation
    if (data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
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
#endif