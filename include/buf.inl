#if !defined(BE_CORE_BUF_HPP_) && !defined(DOXYGEN)
#include "buf.hpp"
#elif !defined(BE_CORE_BUF_INL_)
#define BE_CORE_BUF_INL_

namespace be {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
BufBase<T>::BufBase()
   : buf_(nullptr),
     size_(0),
     deleter_(nullptr)
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
BufBase<T>::BufBase(T* buf, std::size_t size, deleter del)
   : buf_(buf),
     size_(size),
     deleter_(std::move(del))
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
BufBase<T>::~BufBase() {
   if (buf_ && deleter_)
      deleter_(const_cast<value_type*>(buf_));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool BufBase<T>::is_owner() const {
   return static_cast<bool>(deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::size_t BufBase<T>::size() const {
   return size_;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
void BufBase<T>::shrink(std::size_t new_size) {
   if (new_size > size_)
      throw std::invalid_argument("new size must not be larger than old size!");

   size_ = new_size;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
void BufBase<T>::release() {
   deleter_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
BufBase<T>::operator bool() const {
   return buf_ != nullptr && size_ > 0;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
void BufBase<T>::swap_(BufBase<T>& other) {
   using std::swap;
   swap(deleter_, other.deleter_);
   swap(buf_, other.buf_);
   swap(size_, other.size_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
void BufBase<T>::assign_(BufBase<T>& other) {
   using std::swap;

   if (buf_ && deleter_)
      deleter_(const_cast<value_type*>(buf_));

   deleter_ = nullptr;
   buf_ = other.buf_;
   size_ = other.size_;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
inline void delete_array(const void* ptr) {
   delete[] ptr;
}

} // be::detail

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<T, C>::Buf(T* buf, std::size_t size, deleter del)
   : detail::BufBase<T>(buf, size, std::move(del))
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<T, C>::Buf(Buf<T, C>&& other)
   : detail::BufBase<T>(other.get(), other.size(), nullptr)
{
   using std::swap;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<T, C>& Buf<T, C>::operator=(Buf<T, C>&& other) {
   assign_(other);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>::Buf(const T* buf, std::size_t size, deleter del)
   : detail::BufBase<const T>(buf, size, std::move(del))
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>::Buf(Buf<const T, C>&& other)
   : detail::BufBase<const T>(other.get(), other.size(), nullptr)
{
   using std::swap;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>::Buf(Buf<T, C>&& other)
   : detail::BufBase<const T>(other.get(), other.size(), nullptr)
{
   using std::swap;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>& Buf<const T, C>::operator=(Buf<const T, C>&& other) {
   assign_(other);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T, true>::Buf(T* buf, std::size_t size, deleter del)
   : detail::BufBase<T>(buf, size, std::move(del))
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Buf<T, true>::Buf(Buf<U>&& other)
   : detail::BufBase<T>(static_cast<T*>(static_cast<void*>(other.get())),
                        other.size() * sizeof(U), nullptr)
{
   using std::swap;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T, true>& Buf<T, true>::operator=(Buf<T, true>&& other) {
   assign_(other);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T, true>::Buf(const T* buf, std::size_t size, deleter del)
   : detail::BufBase<const T>(buf, size, std::move(del))
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Buf<const T, true>::Buf(Buf<U>&& other)
   : detail::BufBase<const T>(static_cast<const T*>(static_cast<const void*>(other.get())),
                              other.size() * sizeof(U), nullptr)
{
   using std::swap;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T, true>& Buf<const T, true>::operator=(Buf<const T, true>&& other) {
   assign_(other);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_buf(std::size_t size) {
   return Buf<T>(new T[size], size, detail::delete_array);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_buf(T* buf, std::size_t size, std::function<void(void*)> del) {
   return Buf<T>(buf, size, std::move(del));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
Buf<T> copy_buf(const Buf<U>& source) {
   using M = typename std::remove_const<T>::type;
   std::size_t new_size = source.size() * sizeof(U) / sizeof(T);
   Buf<M> buf(new M[new_size], new_size, detail::delete_array);
   memcpy(buf.get(), source.get(), new_size * sizeof(T));
   return Buf<T>(std::move(buf));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
Buf<T> copy_buf(const Buf<U>& source, std::size_t new_size) {
   using M = typename std::remove_const<T>::type;
   Buf<M> buf(new M[new_size], new_size, detail::delete_array);
   memcpy(buf.get(), source.get(), (std::min)(new_size * sizeof(T), source.size() * sizeof(U)));
   return Buf<T>(std::move(buf));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> copy_buf(const S& source) {
   using M = typename std::remove_const<T>::type;
   std::size_t new_size = source.length() / sizeof(T);
   Buf<M> buf(new M[new_size], new_size, detail::delete_array);
   memcpy(buf.get(), source.data(), new_size * sizeof(T));
   return Buf<T>(std::move(buf));
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
Buf<T> tmp_buf(Buf<U>& source) {
   return Buf<U>(source.get(), source.size());
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
Buf<const T> tmp_buf(const Buf<U>& source) {
   return Buf<const U>(source.get(), source.size());
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> tmp_buf(const S& source) {
   return Buf<const char>(source.data(), source.length());
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(S& source) {
   return Buf<char>(&source[0], source.length());
}

///////////////////////////////////////////////////////////////////////////////
template <typename U>
S buf_to_string(const Buf<U>& source) {
   Buf<const char> tmp = tmpBuf(source);
   return S(tmp.get(), tmp.get() + tmp.size());
}

} // be

#endif
