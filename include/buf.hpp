#pragma once
#ifndef BE_CORE_BUF_HPP_
#define BE_CORE_BUF_HPP_

#include "be.hpp"
#include <functional>

namespace be {

template <typename T, bool C> class Buf;

namespace detail {

template <typename T> struct IsChar : False { };
template <> struct IsChar<char> : True { };
template <> struct IsChar<unsigned char> : True { };

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class BufBase : Immovable {
   template <typename T, bool C> friend class Buf;
public:
   using value_type = typename std::remove_const<T>::type;
   using deleter = std::function<void(void*)>;

   BufBase();
   BufBase(T* buf, std::size_t size, deleter del);
   ~BufBase();

   bool is_owner() const;
   std::size_t size() const;

   void shrink(std::size_t new_size);
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
template <typename T = char, bool C = ::be::detail::IsChar<typename std::remove_const<T>::type>::value>
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
Buf<T> make_buf(T* buf, std::size_t size, std::function<void(void*)> deleter = nullptr);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U = T>
Buf<T> copy_buf(const Buf<U>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U = T>
Buf<T> copy_buf(const Buf<U>& source, std::size_t new_size);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> copy_buf(const S& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U = T>
Buf<T> tmp_buf(Buf<U>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename U = T>
Buf<const T> tmp_buf(const Buf<U>& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<const T> tmp_buf(const S& source);

///////////////////////////////////////////////////////////////////////////////
template <typename T>
Buf<T> tmp_buf(S& source);

///////////////////////////////////////////////////////////////////////////////
template <typename U>
S buf_to_string(const Buf<U>& source);

} // be

#include "buf.inl"

#endif
