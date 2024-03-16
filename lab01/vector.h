#pragma once

const int VEC_MIN_CAPACITY = 1;

template <class T>
class TVector {
private:
    int capacity;
    int size;
    T* data;
public:
    TVector(int size = 0) : capacity{VEC_MIN_CAPACITY}, size{size} {
        capacity = std::max(size, VEC_MIN_CAPACITY);
        data = new T[capacity];
    }

    TVector(int size, T element) {
        this->size = size;
        capacity = size * 2;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = element;
        }
    }

    TVector(const TVector<T>& other) {
        size = other.size;
        capacity = other.capacity;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    TVector(TVector<T>&& other) : capacity{other.capacity}, size{other.size}, data{other.data} {
        other.data = nullptr;
    }

    ~TVector() {
        delete[] data;
    }

    void PushBack(T& element) {
        if (size + 1 > capacity) {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i < size; ++i) {
                newData[i] = data[i];
            }
            delete[] data;
            data = newData;
        }
        data[size] = element;
        ++size;
    }

    void PushBack(T&& element) {
        if (size + 1 > capacity) {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i < size; ++i) {
                newData[i] = std::move(data[i]);
            }
            delete[] data;
            data = newData;
        }
        data[size] = std::move(element);
        ++size;
    }

    T& operator[](int idx) {
        if (idx >= size) {
            throw std::logic_error("Out of range");
        }
        return data[idx];
    }

    const T& operator[](int idx) const {
        if (idx >= size) {
            throw std::logic_error("Out of range");
        }
        return data[idx];
    }

    TVector<T>& operator=(const TVector<T>& other) {
        size = other.size;
        capacity = other.capacity;
        delete[] data;
        data = new T[capacity];
        for (int i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    TVector<T>& operator=(TVector<T>&& other) noexcept {
        capacity = other.capacity;
        size = other.size;
        delete[] data;
        data = other.data;
        other.data = nullptr;
        return *this;
    }

    int Size() const {
        return size;
    }
};
