#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <cstddef>

template <typename T>
class DynamicArray
{
private:
    T *data;
    size_t size;
    size_t capacity;
    void resize(size_t newCapacity);

public:
    DynamicArray();
    DynamicArray(const DynamicArray &other);
    DynamicArray &operator=(const DynamicArray &other);
    T &operator[](size_t index);
    const T &operator[](size_t index) const;
    T &at(size_t index);
    const T &at(size_t index) const;
    size_t getSize();
    size_t getCapacity();
    bool isEmpty();
    void push_back(const T &value);
    void pop_back();
    void clear();
    void shrink_to_fit();
    ~DynamicArray();
};

template <typename T>
void DynamicArray<T>::resize(size_t newCapacity)
{
    T *newData = new T[newCapacity];
    for (size_t i = 0; i < size; ++i)
    {
        newData[i] = data[i];
    }

    delete[] data;

    data = newData;
    capacity = newCapacity;
}

template <typename T>
DynamicArray<T>::DynamicArray() : data(nullptr), size(0), capacity(0) {}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray &other) : size(other.size), capacity(other.capacity)
{
    data = new T[capacity];
    for (size_t i = 0; i < size; ++i)
    {
        data[i] = other.data[i];
    }
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &other)
{
    if (this != &other)
    {
        delete[] data;

        data = new T[other.capacity];
        for (size_t i = 0; i < other.size; ++i)
        {
            data[i] = other.data[i];
        }

        size = other.size;
        capacity = other.capacity;
    }
    return *this;
}

template <typename T>
T &DynamicArray<T>::operator[](size_t index)
{
    return data[index];
}

template <typename T>
const T &DynamicArray<T>::operator[](size_t index) const
{
    return data[index];
}

template <typename T>
T &DynamicArray<T>::at(size_t index)
{
    assert(index < size);
    return data[index];
}

template <typename T>
const T &DynamicArray<T>::at(size_t index) const
{
    assert(index < size);
    return data[index];
}

template <typename T>
size_t DynamicArray<T>::getSize()
{
    return size;
}

template <typename T>
size_t DynamicArray<T>::getCapacity()
{
    return capacity;
}

template <typename T>
bool DynamicArray<T>::isEmpty()
{
    return size == 0;
}

template <typename T>
void DynamicArray<T>::push_back(const T &value)
{
    if (size >= capacity)
    {
        size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
        resize(newCapacity);
    }

    data[size++] = value;
}

template <typename T>
void DynamicArray<T>::pop_back()
{
    if (size > 0)
    {
        --size;
    }
}

template <typename T>
void DynamicArray<T>::clear()
{
    size = 0;
}

template <typename T>
void DynamicArray<T>::shrink_to_fit()
{
    if (size < capacity)
    {
        resize(size);
    }
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] data;
}

#endif