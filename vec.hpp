#ifndef VEC_HPP
#define VEC_HPP

#include <initializer_list>
#include <cassert>

namespace topit
{
  template< class T >
  struct vec
  {
    ~vec();
    vec();
    vec(const vec< T >&);
    vec(vec< T >&&) noexcept;
    vec(size_t size, const T& init);
    explicit vec(std::initializer_list< T > il);

    vec< T >& operator=(const vec< T >&);
    vec< T >& operator=(vec< T >&&) noexcept;

    void swap(vec< T >&) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    // Методы с классной работы
    void reserve(size_t required);
    void shrinkToFit();
    void pushBackCount(size_t k, const T& v);

    template< class IT >
    void pushBackRange(IT b, size_t c);

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

    void pushBack(const T& v);
    void popBack();

    void insert(size_t i, const T& v);
    void erase(size_t i);

    void insert(size_t i, const vec< T >& rhs, size_t start, size_t end);
    void erase(size_t start, size_t end);

    template< class VectorIterator, class FwdIterator >
    void insert(VectorIterator pos, FwdIterator begin, FwdIterator end);

   private:
    T* data_;
    size_t size_, cap_;

    explicit vec(size_t size);
    void unsafePushBack(const T& v);
  };

  template< class T >
  bool operator==(const vec< T >& lhs, const vec< T >& rhs);
}

template< class T >
topit::vec<T>::vec(size_t size):
  data_(size ? new T[size] : nullptr),
  size_(size),
  cap_(size)
{}

template< class T >
topit::vec<T>::vec():
  data_(nullptr),
  size_(0),
  cap_(0)
{}

template< class T >
topit::vec<T>::~vec()
{
  delete[] data_;
}

template< class T >
topit::vec<T>::vec(const vec<T>& rhs):
  vec(rhs.getSize()) // Делегирование аллокации
{
  for (size_t i = 0; i < rhs.getSize(); ++i) {
    data_[i] = rhs[i];
  }
}

template< class T >
topit::vec<T>::vec(vec<T>&& rhs) noexcept:
  data_(rhs.data_),
  size_(rhs.size_),
  cap_(rhs.cap_)
{
  rhs.data_ = nullptr;
  rhs.size_ = 0;
  rhs.cap_ = 0;
}

template< class T >
topit::vec<T>::vec(size_t size, const T& init):
  vec(size)
{
  for (size_t i = 0; i < size; ++i) {
    data_[i] = init;
  }
}

template< class T >
topit::vec<T>::vec(std::initializer_list<T> il):
  vec(il.size())
{
  size_t i = 0;
  for (auto it = il.begin(); it != il.end(); ++it) {
    data_[i++] = *it;
  }
}

template< class T >
void topit::vec<T>::swap(vec<T>& rhs) noexcept
{
  std::swap(data_, rhs.data_);
  std::swap(size_, rhs.size_);
  std::swap(cap_, rhs.cap_);
}

template< class T >
topit::vec<T>& topit::vec<T>::operator=(const vec<T>& rhs)
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  vec<T> cpy = rhs;
  swap(cpy);
  return *this;
}

template< class T >
topit::vec<T>& topit::vec<T>::operator=(vec<T>&& rhs) noexcept
{
  if (this == std::addressof(rhs)) {
    return *this;
  }
  vec<T> cpy(std::move(rhs));
  swap(cpy);
  return *this;
}

template< class T >
T& topit::vec<T>::operator[](size_t id) noexcept
{
  return data_[id];
}

template< class T >
const T& topit::vec<T>::operator[](size_t id) const noexcept
{
  return data_[id];
}

template< class T >
const T& topit::vec<T>::at(size_t id) const
{
  if (id < getSize()) {
    return (*this)[id];
  }
  throw std::out_of_range("bad id");
}

template< class T >
T& topit::vec<T>::at(size_t id)
{
  const vec<T>* cthis = this;
  return const_cast<T&>(cthis->at(id));
}

template< class T >
size_t topit::vec<T>::getSize() const noexcept { return size_; }

template< class T >
size_t topit::vec<T>::getCapacity() const noexcept { return cap_; }

template< class T >
bool topit::vec<T>::isEmpty() const noexcept { return !size_; }

template< class T >
void topit::vec<T>::pushBack(const T& v)
{
  if (size_ == cap_) {
    reserve(cap_ == 0 ? 1 : cap_ * 2);
  }
  unsafePushBack(v);
}

template< class T >
void topit::vec<T>::unsafePushBack(const T& v)
{
  assert(size_ < cap_);
  data_[size_++] = v;
}

template< class T >
void topit::vec<T>::reserve(size_t required)
{
  if (required > cap_) {
    T* new_data = new T[required];
    for (size_t i = 0; i < size_; ++i) {
      new_data[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = new_data;
    cap_ = required;
  }
}

template< class T >
void topit::vec<T>::popBack()
{
  if (size_ > 0) {
    --size_;
  }
}

template< class T >
bool topit::operator==(const vec<T>& lhs, const vec<T>& rhs)
{
  if (lhs.getSize() != rhs.getSize()) return false;
  for (size_t i = 0; i < lhs.getSize(); ++i) {
    if (!(lhs[i] == rhs[i])) return 0;
  }
  return 1;
}

#endif