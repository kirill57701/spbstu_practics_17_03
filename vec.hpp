#ifdef VEC_HPP
#define VEC_HPP

template<class T>
struct vec {
  T* data;
  size_t size, cap;
};

#endif