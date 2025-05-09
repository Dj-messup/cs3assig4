#pragma once

#include <cstddef>
#include <cassert>

// A simple resizable array class template (like std::vector)
// Stores items of type T and grows its buffer automatically.
template <typename T>
class DynamicArray
{
private:
    T *data;          // pointer to the actual array buffer
    size_t size;      // how many elements are in use
    size_t capacity;  // how big the buffer currently is

    // Private helper to resize the buffer to newCapacity
    void resize(size_t newCapacity);

public:
    DynamicArray();                              // default ctor
    DynamicArray(const DynamicArray &other);     // copy ctor
    DynamicArray &operator=(const DynamicArray &other);  // copy assignment

    // element access (no bounds checking)
    T &operator[](size_t index);
    const T &operator[](size_t index) const;

    // element access (with bounds check via assert)
    T &at(size_t index);
    const T &at(size_t index) const;

    // report how many items are stored
    size_t getSize();
    size_t getSize() const;

    // report how big the buffer is
    size_t getCapacity();
    size_t getCapacity() const;

    // is the array empty?
    bool isEmpty();
    bool isEmpty() const;

    // add/remove at the end
    void push_back(const T &value);
    void pop_back();

    // clear everything
    void clear();

    // shrink buffer to fit exactly `size`
    void shrink_to_fit();

    ~DynamicArray();
};

// grow or shrink the buffer
template <typename T>
void DynamicArray<T>::resize(size_t newCapacity)
{
    T *newData = new T[newCapacity];             // allocate new buffer
    for (size_t i = 0; i < size; ++i)             // copy existing items
        newData[i] = data[i];

    delete[] data;                                // free old buffer

    data = newData;                               // point to new memory
    capacity = newCapacity;                       // update capacity
}

template <typename T>
DynamicArray<T>::DynamicArray()
  : data(nullptr), size(0), capacity(0)
{
    // start with no buffer; will allocate on first push_back
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray &other)
  : size(other.size), capacity(other.capacity)
{
    data = new T[capacity];                       // allocate new buffer
    for (size_t i = 0; i < size; ++i)
        data[i] = other.data[i];                  // copy elements
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &other)
{
    if (this != &other)
    {
        delete[] data;                            // free old memory
        data = new T[other.capacity];             // allocate new
        for (size_t i = 0; i < other.size; ++i)
            data[i] = other.data[i];              // copy elements

        size     = other.size;
        capacity = other.capacity;
    }
    return *this;
}

template <typename T>
T &DynamicArray<T>::operator[](size_t index)
{
    return data[index];                           // no safety check
}

template <typename T>
const T &DynamicArray<T>::operator[](size_t index) const
{
    return data[index];
}

template <typename T>
T &DynamicArray<T>::at(size_t index)
{
    assert(index < size);                         // crash if out-of-bounds
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
size_t DynamicArray<T>::getSize() const
{
    return size;
}

template <typename T>
size_t DynamicArray<T>::getCapacity()
{
    return capacity;
}

template <typename T>
size_t DynamicArray<T>::getCapacity() const
{
    return capacity;
}

template <typename T>
bool DynamicArray<T>::isEmpty()
{
    return size == 0;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const
{
    return size == 0;
}

template <typename T>
void DynamicArray<T>::push_back(const T &value)
{
    // grow if buffer is full
    if (size >= capacity)
    {
        size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
        resize(newCapacity);
    }
    data[size++] = value;                         // place new item
}

template <typename T>
void DynamicArray<T>::pop_back()
{
    if (size > 0)
        --size;                                   // just forget last element
}

template <typename T>
void DynamicArray<T>::clear()
{
    size = 0;                                     // effectively empty
}

template <typename T>
void DynamicArray<T>::shrink_to_fit()
{
    if (size < capacity)
        resize(size);                             // cut buffer down
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] data;                                // cleanup
}
