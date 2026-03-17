#define VEC_HPP
#ifdef VEC_HPP

namespace topit {
  template<class T>
  struct vec {
    vec();
    ~vec();
    
    bool is_empty() const noexcept;
    void push_back(const T& v);

  private:
    T* data_;
    size_t size_, cap_;
  };
}

template<class T>
void topit::vec<T>::push_back(const T& v) {
  
}

template<class T>
bool topit::vec<T>::is_empty() const noexcept{
  return !size_;
}

template<class T>
topit::vec<T>::~vec() {
  delete[] data_;
}

template<class T>
topit::vec<T>::vec():
  data_(),
  size_(),
  cap_()
{}

#endif