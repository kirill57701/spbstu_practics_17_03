#define VEC_HPP
#ifdef VEC_HPP

namespace topit {
  template<class T>
  struct vec {
    vec();
    ~vec();
  private:
    T* data_;
    size_t size_, cap_;
  };
}
template<class T>
topit::vec<T>::~vec()
{}
template<class T>
topit::vec<T>::vec():
  data_(),
  size_(),
  cap_()
{}

#endif