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
     size_(buf ? size : 0),
     deleter_(del)
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
BufBase<T>::~BufBase() {
   if (buf_ && deleter_) {
      deleter_(const_cast<void*>(static_cast<const void*>(buf_)), size_ * sizeof(value_type));
   }
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool BufBase<T>::is_owner() const {
   return !!deleter_;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
std::size_t BufBase<T>::size() const {
   return size_;
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

   if (buf_ && deleter_) {
      deleter_(const_cast<void*>(static_cast<const void*>(buf_)), size_ * sizeof(value_type));
   }

   deleter_ = nullptr;
   buf_ = other.buf_;
   size_ = other.size_;
   swap(deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
inline void delete_array(void* ptr, std::size_t size) {
   delete[] ptr;
}

///////////////////////////////////////////////////////////////////////////////
inline void aligned_free(void* ptr, std::size_t size) {
   BE_ALIGNED_FREE(ptr);
}

} // be::detail

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<T, C>::Buf(T* buf, std::size_t size, deleter del)
   : detail::BufBase<T>(buf, size, del)
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<T, C>::Buf(Buf<T, C>&& other)
   : detail::BufBase<T>(other.get(), other.size(), nullptr)
{
   using std::swap;
   swap(this->deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<T, C>& Buf<T, C>::operator=(Buf<T, C>&& other) {
   this->assign_(other);
   return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>::Buf(const T* buf, std::size_t size, deleter del)
   : detail::BufBase<const T>(buf, size, del)
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>::Buf(Buf<const T, C>&& other)
   : detail::BufBase<const T>(other.get(), other.size(), nullptr)
{
   using std::swap;
   swap(this->deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, bool C>
Buf<const T, C>::Buf(Buf<T, C>&& other)
   : detail::BufBase<const T>(other.get(), other.size(), nullptr)
{
   using std::swap;
   swap(this->deleter_, other.deleter_);
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
   : detail::BufBase<T>(buf, size, del)
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Buf<T, true>::Buf(Buf<U>&& other)
   : detail::BufBase<T>(static_cast<T*>(static_cast<void*>(other.get())),
                        other.size() * sizeof(U), nullptr)
{
   using std::swap;
   swap(this->deleter_, other.deleter_);
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
   : detail::BufBase<const T>(buf, size, del)
{ }

///////////////////////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
Buf<const T, true>::Buf(Buf<U>&& other)
   : detail::BufBase<const T>(static_cast<const T*>(static_cast<const void*>(other.get())),
                              other.size() * sizeof(U), nullptr)
{
   using std::swap;
   swap(this->deleter_, other.deleter_);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T, true>& Buf<const T, true>::operator=(Buf<const T, true>&& other) {
   this->assign_(other);
   return *this;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_buf(std::size_t size) {
   if (size == 0) {
      return Buf<T>();
   } else {
      return Buf<T>(new T[size], size, detail::delete_array);
   }
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_buf(T* buf, std::size_t size, typename Buf<T>::deleter del) {
   return Buf<T>(buf, size, del);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_aligned_buf(std::size_t size, std::size_t alignment, bool uninitialized) {
   static_assert(std::is_trivially_destructible<T>::value, "make_aligned_buf() requires a trivially-destructible type!");
   if (size == 0) {
      return Buf<T>();
   } else if (uninitialized) {
      void* ptr = BE_ALIGNED_ALLOC(alignment, size * sizeof(T));
      if (!ptr) {
         throw std::bad_alloc();
      }
      return Buf<T>(static_cast<T*>(ptr), size, detail::aligned_free);
   } else {
      if (alignment <= 8) {
         return make_buf<T>(size);
      } else {
         void* ptr = BE_ALIGNED_ALLOC(alignment, size * sizeof(T));
         if (!ptr) {
            throw std::bad_alloc();
         }
         T* tptr = static_cast<T*>(ptr);
         for (std::size_t i = 0; i < size; ++i) {
            new (static_cast<void*>(tptr + i)) T;
         }
         return Buf<T>(tptr, size, detail::aligned_free);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(T* buf, std::size_t size) {
   return Buf<T>(buf, size);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N>
Buf<T> tmp_buf(T (&arr)[N]) {
   return Buf<T>(arr, N);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(Buf<T>& source) {
   return Buf<T>(source.get(), source.size());
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> tmp_buf(const Buf<const T>& source) {
   return Buf<const T>(source.get(), source.size());
}


///////////////////////////////////////////////////////////////////////////////
inline Buf<char> tmp_buf(S& source) {
   return Buf<char>(&source[0], source.length());
}

///////////////////////////////////////////////////////////////////////////////
inline Buf<const char> tmp_buf(SV source) {
   return Buf<const char>(source.data(), source.length());
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(std::vector<T>& source) {
   return Buf<T>(source.data(), source.size());
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> tmp_buf(const std::vector<T>& source) {
   return Buf<const T>(source.data(), source.size());
}


///////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N>
Buf<T> tmp_buf(std::array<T, N>& source) {
   return Buf<T>(source.data(), N);
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N>
Buf<const T> tmp_buf(const std::array<T, N>& source) {
   return Buf<const T>(source.data(), N);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> sub_buf(Buf<T>& source, std::size_t offset) {
   if (offset < source.size()) {
      return Buf<T>(source.get() + offset, source.size() - offset);
   } else {
      return Buf<T>();
   }
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> sub_buf(const Buf<const T>& source, std::size_t offset) {
   if (offset < source.size()) {
      return Buf<const T>(source.get() + offset, source.size() - offset);
   } else {
      return Buf<const T>();
   }
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> sub_buf(Buf<T>& source, std::size_t offset, std::size_t length) {
   if (offset < source.size()) {
      if (offset + length <= source.size()) {
         return Buf<T>(source.get() + offset, length);
      } else {
         return Buf<T>(source.get() + offset, source.size() - offset);
      }
   } else {
      return Buf<T>();
   }
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> sub_buf(const Buf<const T>& source, std::size_t offset, std::size_t length) {
   if (offset < source.size()) {
      if (offset + length <= source.size()) {
         return Buf<const T>(source.get() + offset, length);
      } else {
         return Buf<const T>(source.get() + offset, source.size() - offset);
      }
   } else {
      return Buf<const T>();
   }
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> copy_buf(const Buf<T>& source) {
   Buf<T> buf;
   if (source) {
      buf = Buf<T>(new T[source.size()], source.size(), detail::delete_array);
      memcpy(buf.get(), source.get(), buf.size() * sizeof(T));
   }
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> copy_buf(const Buf<const T>& source) {
   Buf<T> buf;
   if (source) {
      buf = Buf<T>(new T[source.size()], source.size(), detail::delete_array);
      memcpy(buf.get(), source.get(), buf.size() * sizeof(T));
   }
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
Buf<T> copy_buf(const Buf<U>& source) {
   Buf<T> buf;
   if (source) {
      using M = typename Buf<T>::value_type;
      std::size_t new_size = source.size() * sizeof(U) / sizeof(T);
      Buf<M> mbuf(new M[new_size], new_size, detail::delete_array);
      memcpy(mbuf.get(), source.get(), new_size * sizeof(T));
      buf = Buf<T>(std::move(mbuf));
   }
   return buf;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> concat_buf(const Buf<T>& first, const Buf<T>& second) {
   Buf<T> buf;
   if (first || second) {
      std::size_t new_size = first.size() + second.size();
      buf = Buf<T>(new T[new_size], new_size, detail::delete_array);
      if (first) {
         memcpy(buf.get(), first.get(), first.size() * sizeof(T));
      }
      if (second) {
         memcpy(buf.get() + first.size(), second.get(), second.size() * sizeof(T));
      }
   }
   return buf;
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U, typename V>
Buf<T> concat_buf(const Buf<U>& first, const Buf<V>& second) {
   Buf<T> buf;
   if (first || second) {
      using M = typename Buf<T>::value_type;
      std::size_t new_size = (first.size() * sizeof(U) + second.size() * sizeof(V) + sizeof(T) - 1) / sizeof(T);
      Buf<M> mbuf(new M[new_size], new_size, detail::delete_array);
      if (first) {
         memcpy(mbuf.get(), first.get(), first.size() * sizeof(U));
      }
      if (second) {
         memcpy(static_cast<char*>(static_cast<void*>(mbuf.get())) + first.size() * sizeof(U), second.get(), second.size() * sizeof(V));
      }
      buf = Buf<T>(std::move(mbuf));
   }
   return buf;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
S buf_to_string(const Buf<T>& source) {
   Buf<const char> tmp = tmp_buf(source);
   return S(tmp.get(), tmp.get() + tmp.size());
}

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
std::vector<T> buf_to_vector(const Buf<U>& source) {
   std::vector<T> vec;
   std::size_t new_size = source.size() * sizeof(U) / sizeof(T);
   vec.resize(new_size);
   memcpy(vec.data(), source.get(), new_size * sizeof(T));
   return vec;
}

} // be

#endif
