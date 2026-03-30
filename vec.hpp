#ifndef VEC_HPP
#define VEC_HPP

#include <initializer_list>
#include <cassert>
#include <stdexcept>
#include <utility>

namespace topit
{
  template<class T>
  struct vec
  {
    using iter = T*;
    using const_iter = const T*;

    ~vec();
    vec();
    vec(const vec<T>&);
    vec(vec<T>&&) noexcept;
    vec(size_t size, const T& init);
    explicit vec(std::initializer_list<T> il);

    vec<T>& operator=(const vec<T>&);
    vec<T>& operator=(vec<T>&&) noexcept;

    void swap(vec<T>&) noexcept;

    bool isEmpty() const noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;

    iter begin() noexcept { return data_; }
    iter end() noexcept { return data_ + size_; }
    const_iter begin() const noexcept { return data_; }
    const_iter end() const noexcept { return data_ + size_; }

    void reserve(size_t required);
    void shrinkToFit();
    
    iter insert(iter pos, const T& v);
    iter insert(iter pos, size_t c, const T& v);
    template< class Fwditer >
    void insert(iter pos, Fwditer b, Fwditer e);

    iter erase(iter pos);
    iter erase(iter first, iter last);

    void pushBack(const T& v);
    void popBack();
    void erase(size_t i) { erase(begin() + i); }

    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    T& at(size_t id);
    const T& at(size_t id) const;

  private:
    T* data_;
    size_t size_, cap_;

    static T* alloc(size_t n);
    void destroy() noexcept;
    explicit vec(size_t size);
    void unsafePushBack(const T& v);
  };

  template<class T>
  T* vec<T>::alloc(size_t n) {
    return n ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
  }

  template<class T>
  void vec<T>::destroy() noexcept {
    if (data_) {
      for (size_t i = 0; i < size_; ++i) data_[i].~T();
      operator delete(data_);
    }
  }

  template< class T >
  topit::vec<T>::vec(size_t size):
  data_(alloc(size)),
  size_(0),
  cap_(size)
{}

  template<class T>
  vec<T>::vec() : data_(nullptr), size_(0), cap_(0) {}

  template<class T>
  vec<T>::~vec() { destroy(); }

  template<class T>
  vec<T>::vec(const vec<T>& rhs) : data_(alloc(rhs.size_)), size_(0), cap_(rhs.size_) {
    try {
      for (size_t i = 0; i < rhs.size_; ++i) unsafePushBack(rhs.data_[i]);
    } catch (...) { destroy(); throw; }
  }

  template<class T>
  vec<T>::vec(vec<T>&& rhs) noexcept : data_(rhs.data_), size_(rhs.size_), cap_(rhs.cap_) {
    rhs.data_ = nullptr; rhs.size_ = 0; rhs.cap_ = 0;
  }

  template<class T>
  vec<T>::vec(size_t size, const T& init) : data_(alloc(size)), size_(0), cap_(size) {
    try {
      for (size_t i = 0; i < size; ++i) unsafePushBack(init);
    } catch (...) { destroy(); throw; }
  }

  template<class T>
  vec<T>::vec(std::initializer_list<T> il) : data_(alloc(il.size())), size_(0), cap_(il.size()) {
    try {
      for (const auto& x : il) unsafePushBack(x);
    } catch (...) { destroy(); throw; }
  }

  template<class T>
  void vec<T>::reserve(size_t required) {
    if (required <= cap_) return;
    T* new_ptr = alloc(required);
    size_t i = 0;
    try {
      for (; i < size_; ++i) new (new_ptr + i) T(std::move(data_[i]));
    } catch (...) {
      for (size_t j = 0; j < i; ++j) new_ptr[j].~T();
      operator delete(new_ptr);
      throw;
    }
    size_t old_sz = size_;
    destroy();
    data_ = new_ptr;
    size_ = old_sz;
    cap_ = required;
  }

  template<class T>
  typename vec<T>::iter vec<T>::insert(iter pos, const T& v) {
    size_t offset = pos - begin();
    if (size_ == cap_) reserve(cap_ == 0 ? 1 : cap_ * 2);
    
    iter target = begin() + offset;
    if (target != end()) {
      new (data_ + size_) T(std::move(*(end() - 1)));
      for (iter it = end() - 1; it > target; --it) *it = std::move(*(it - 1));
      *target = v;
    } else {
      new (target) T(v);
    }
    size_++;
    return begin() + offset;
  }

  template<class T>
  typename vec<T>::iter vec<T>::erase(iter pos) {
    if (pos == end()) return pos;
    size_t offset = pos - begin();
    for (iter it = pos; it < end() - 1; ++it) *it = std::move(*(it + 1));
    (end() - 1)->~T();
    size_--;
    return begin() + offset;
  }

  template<class T>
  void vec<T>::shrinkToFit() {
    if (size_ < cap_) {
      vec<T> temp(*this);
      swap(temp);
    }
  }

  template<class T>
  void vec<T>::unsafePushBack(const T& v) {
    new (data_ + size_) T(v);
    size_++;
  }

  template<class T>
  void vec<T>::swap(vec<T>& rhs) noexcept {
    std::swap(data_, rhs.data_);
    std::swap(size_, rhs.size_);
    std::swap(cap_, rhs.cap_);
  }

  template<class T>
  vec<T>& vec<T>::operator=(const vec<T>& rhs) {
    if (this != std::addressof(rhs)) { vec<T> c(rhs); swap(c); }
    return *this;
  }

  template<class T>
 vec<T>& vec<T>::operator=(vec<T>&& rhs) noexcept {
    if (this != std::addressof(rhs)) {
      swap(rhs);
    }
    return *this;
  }

  template<class T>
  T& vec<T>::at(size_t id) {
    if (id >= size_) throw std::out_of_range("vec::at");
    return data_[id];
  }

  template<class T>
  const T& vec<T>::at(size_t id) const {
    if (id >= size_) throw std::out_of_range("vec::at");
    return data_[id];
  }

  template<class T>
  T& vec<T>::operator[](size_t id) noexcept { return data_[id]; }

  template<class T>
  const T& vec<T>::operator[](size_t id) const noexcept { return data_[id]; }

  template<class T>
  size_t vec<T>::getSize() const noexcept { return size_; }

  template<class T>
  size_t vec<T>::getCapacity() const noexcept { return cap_; }

  template<class T>
  bool vec<T>::isEmpty() const noexcept { return !size_; }

  template<class T>
  void vec<T>::pushBack(const T& v) {
    if (size_ == cap_) reserve(cap_ == 0 ? 1 : cap_ * 2);
    unsafePushBack(v);
  }

  template<class T>
  void vec<T>::popBack() { if (size_) { (data_ + size_ - 1)->~T(); size_--; } }

  template<class T>
  bool operator==(const vec<T>& lhs, const vec<T>& rhs) {
    if (lhs.getSize() != rhs.getSize()) return false;
    for (size_t i = 0; i < lhs.getSize(); ++i) if (!(lhs[i] == rhs[i])) return false;
    return true;
  }
}

#endif