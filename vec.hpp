#ifndef VEC_HPP
#define VEC_HPP

#include <stdexcept>

namespace topit
{
  template<class T>
  struct vec
  {
    vec();
    ~vec();
    
    vec(const vec&);
    vec(vec&&) noexcept;
    vec& operator=(const vec&);
    vec& operator=(vec&&) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void pushBack(const T& v);
    void popBack();
    void insert(size_t i, const T& v);
    void erase(size_t i);

  private:
    T* data_;
    size_t size_, cap_;
  };

  template<class T>
  bool operator==(const vec<T>& lhs, const vec<T>& rhs);
}



template<class T>
topit::vec<T>::vec():
  data_(nullptr),
  size_(0),
  cap_(0)
{}

template<class T>
topit::vec<T>::~vec()
{
  delete[] data_;
}

template<class T>
topit::vec<T>::vec(const vec<T>& rhs):
  data_(rhs.getSize() ? new T[rhs.getSize()] : nullptr),
  size_(rhs.getSize()),
  cap_(rhs.getSize())
{
  for (size_t i = 0; i < rhs.getSize(); ++i) {
    try {
      data_[i] = rhs[i];
    } catch (...) {
      delete[] data_;
      throw;
    }
  }
}

template<class T>
topit::vec<T>::vec(vec<T>&& rhs) noexcept :
  data_(rhs.data_),
  size_(rhs.size_),
  cap_(rhs.cap_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.cap_ = 0;
}



template<class T>
topit::vec<T>& topit::vec<T>::operator=(const vec<T>& rhs)
{
  if (this != &rhs) {
    T* new_data = rhs.getSize() ? new T[rhs.getSize()] : nullptr;
    for (size_t i = 0; i < rhs.getSize(); ++i) {
      try {
        new_data[i] = rhs[i];
      } catch (...) {
        delete[] new_data;
        throw;
      }
    }
    delete[] data_;
    data_ = new_data;
    size_ = rhs.getSize();
    cap_ = rhs.getSize();
  }
  return *this;
}

template<class T>
topit::vec<T>& topit::vec<T>::operator=(vec<T>&& rhs) noexcept
{
  if (this != &rhs) {
    delete[] data_;
    data_ = rhs.data_;
    size_ = rhs.size_;
    cap_ = rhs.cap_;

    rhs.data_ = nullptr;
    rhs.size_ = 0;
    rhs.cap_ = 0;
  }
  return *this;
}



template<class T>
bool topit::operator==(const vec<T>& lhs, const vec<T>& rhs)
{
  bool isEqual = lhs.getSize() == rhs.getSize();
  for (size_t i = 0; i < lhs.getSize() && (isEqual = lhs[i] == rhs[i]); ++i);
  return isEqual;
}

template<class T>
bool topit::vec<T>::isEmpty() const noexcept
{
  return !size_;
}

template<class T>
size_t topit::vec<T>::getSize() const noexcept
{
  return size_;
}

template<class T>
size_t topit::vec<T>::getCapacity() const noexcept
{
  return cap_;
}

template<class T>
const T& topit::vec<T>::at(size_t id) const
{
  if (id < getSize()) {
    return data_[id];
  }
  throw std::out_of_range("bad id");
}

template<class T>
T& topit::vec<T>::at(size_t id)
{
  const vec<T>* cthis = this;
  return const_cast<T&>(cthis->at(id));
}

template<class T>
T& topit::vec<T>::operator[](size_t id) noexcept
{
  return data_[id];
}

template<class T>
const T& topit::vec<T>::operator[](size_t id) const noexcept
{
  return data_[id];
}


template<class T>
void topit::vec<T>::pushBack(const T& v)
{
  if (size_ == cap_) {
    size_t new_cap = (cap_ == 0) ? 1 : cap_ * 2;
    T* new_data = new T[new_cap];
    for (size_t i = 0; i < size_; ++i) {
      try {
        new_data[i] = data_[i];
      } catch (...) {
        delete[] new_data;
        throw;
      }
    }
    delete[] data_;
    data_ = new_data;
    cap_ = new_cap;
  }
  data_[size_++] = v;
}

template<class T>
void topit::vec<T>::popBack()
{
  if (size_ > 0) {
    --size_;
  }
}

template<class T>
void topit::vec<T>::insert(size_t id, const T& v)
{
  if (id > size_) {
    throw std::out_of_range("bad id");
  }
  if (size_ == cap_) {
    size_t new_cap = (cap_ == 0) ? 1 : cap_ * 2;
    T* new_data = new T[new_cap];
    for (size_t i = 0; i < size_; ++i) {
      try {
        new_data[i] = data_[i];
      } catch (...) {
        delete[] new_data;
        throw;
      }
    }
    delete[] data_;
    data_ = new_data;
    cap_ = new_cap;
  }
  for (size_t i = size_; i > id; --i) {
    data_[i] = data_[i - 1];
  }
  data_[id] = v;
  ++size_;
}

template<class T>
void topit::vec<T>::erase(size_t id)
{
  if (id >= size_) {
    throw std::out_of_range("bad id");
  }
  for (size_t i = id; i < size_ - 1; ++i) {
    data_[i] = data_[i + 1];
  }
  --size_;
}

#endif
