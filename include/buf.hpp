#pragma once
#ifndef BE_CORE_BUF_HPP_
#define BE_CORE_BUF_HPP_

#include "t_is_char.hpp"
#include <vector>
#include <array>

namespace be {

template <typename T = char, bool C = t::IsChar<std::remove_cv_t<T>>::value> class Buf;

namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class BufBase : Immovable {
   template <typename T, bool C> friend class Buf;
public:
   using value_type = std::remove_cv_t<T>;
   using deleter = void (*)(void*, std::size_t);

   BufBase();
   BufBase(T* buf, std::size_t size, deleter del);
   ~BufBase();

   bool is_owner() const;
   std::size_t size() const;

   void release();

   explicit operator bool() const;
   explicit operator T*() { return buf_; };
   explicit operator const T*() const { return buf_; };

   T* get() { return buf_; }
   const T* get() const { return buf_; }
   T* operator->() { return buf_; }
   const T* operator->() const { return buf_; }
   T& operator*() { return *buf_; }
   const T& operator*() const { return *buf_; }
   T& operator[](std::size_t index) { return buf_[index]; }
   const T& operator[](std::size_t index) const { return buf_[index]; }

protected:
   void swap_(BufBase<T>& other);
   void assign_(BufBase<T>& other);

   T* buf_;
   std::size_t size_;
   deleter deleter_;
};

} // be::detail

///////////////////////////////////////////////////////////////////////////////
/// \brief  buffer of non-const non-char
template <typename T, bool C>
class Buf final : public detail::BufBase<T> {
   friend void swap(Buf<T, C>& a, Buf<T, C>& b) { a.swap_(b); }
public:
   Buf() { }
   Buf(T* buf, std::size_t size, deleter del = nullptr);
   Buf(Buf<T, C>&& other);
   Buf<T, C>& operator=(Buf<T, C>&& other);
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  buffer of const non-char
template <typename T, bool C>
class Buf<const T, C> final : public ::be::detail::BufBase<const T> {
   friend void swap(Buf<const T, C>& a, Buf<const T, C>& b) { a.swap_(b); }
public:
   Buf() { }
   Buf(const T* buf, std::size_t size, deleter deleter = nullptr);
   Buf(Buf<const T, C>&& other);
   Buf(Buf<T, C>&& other);
   Buf<const T, C>& operator=(Buf<const T, C>&& other);
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  buffer of non-const char
template <typename T>
class Buf<T, true> final : public ::be::detail::BufBase<T> {
   friend void swap(Buf<T, true>& a, Buf<T, true>& b) { a.swap_(b); }
public:
   Buf() { }
   Buf(T* buf, std::size_t size, deleter deleter = nullptr);
   template <typename U> Buf(Buf<U>&& other);
   Buf<T, true>& operator=(Buf<T, true>&& other);
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  buffer of const char
template <typename T>
class Buf<const T, true> final : public ::be::detail::BufBase<const T> {
   friend void swap(Buf<const T, true>& a, Buf<const T, true>& b) { a.swap_(b); }
public:
   Buf() { }
   Buf(const T* buf, std::size_t size, deleter deleter = nullptr);
   template <typename U> Buf(Buf<U>&& other);
   Buf<const T, true>& operator=(Buf<const T, true>&& other);
};


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_buf(std::size_t size);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_buf(T* buf, std::size_t size, typename Buf<T>::deleter del = nullptr);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> make_aligned_buf(std::size_t size, std::size_t alignment, bool uninitialized = std::is_trivially_constructible<T>::value);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(T* buf, std::size_t size);

///////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N>
Buf<T> tmp_buf(T (&arr)[N]);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(Buf<T>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> tmp_buf(const Buf<const T>& source);


///////////////////////////////////////////////////////////////////////////////
Buf<char> tmp_buf(S& source);

///////////////////////////////////////////////////////////////////////////////
Buf<const char> tmp_buf(const S& source);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(std::vector<T>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> tmp_buf(const std::vector<T>& source);


///////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N>
Buf<T> tmp_buf(std::array<T, N>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t N>
Buf<const T> tmp_buf(const std::array<T, N>& source);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> sub_buf(Buf<T>& source, std::size_t offset);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> sub_buf(const Buf<const T>& source, std::size_t offset);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> sub_buf(Buf<T>& source, std::size_t offset, std::size_t length);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> sub_buf(const Buf<const T>& source, std::size_t offset, std::size_t length);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> copy_buf(const Buf<T>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> copy_buf(const Buf<const T>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
Buf<T> copy_buf(const Buf<U>& source);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> concat_buf(const Buf<T>& first, const Buf<T>& second);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U, typename V>
Buf<T> concat_buf(const Buf<U>& first, const Buf<V>& second);


///////////////////////////////////////////////////////////////////////////////
template <typename T>
S buf_to_string(const Buf<T>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U>
std::vector<T> buf_to_vector(const Buf<U>& source);

} // be

#include "buf.inl"

#endif
