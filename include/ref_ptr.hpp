#pragma once
#ifndef BE_CORE_REF_HPP_
#define BE_CORE_REF_HPP_

#include <be/core/be.hpp>

namespace be {
namespace ref {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct HasPublicRefCountField {
   template <typename U> static constexpr True f(decltype(std::declval<U>().ref_count)*);
   template <typename U> static constexpr False f(...);
   using type = decltype(f<T>(nullptr));
   static constexpr bool value = type::value;
};

} // be::ref::detail

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct RefCount {
   std::size_t& operator()(T& obj) {
      if constexpr (detail::HasPublicRefCountField<T>::value) {
         return obj.ref_count;
      } else {
         return obj.ref_count_;
      }
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Destroy {
   void operator()(T& obj) {
      delete &obj;
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Acquire {
   void operator()(T& obj) {
      ++(RefCount<T>{}(obj));
   }
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Release {
   void operator()(T& obj) {
      if (--(RefCount<T>{}(obj)) <= 0) {
         Destroy<T>{}(obj);
      }
   }
};
} // be::ref

///////////////////////////////////////////////////////////////////////////////
/// \brief  An unsynchronized reference-counting smart pointer supporting
///         intrusive storage of reference count.
///
/// \details Since the reference count is intended to be stored inside the
///         object itself, ref_ptr has some properties that might be
///         unexpected:
///            - constructing or resetting a `ref_ptr<const T>` to a pointer
///              requires that it be a pointer to non-const `T`.  Otherwise
///              the reference count would have to be mutable, which isn't
///              desirable.  However `ref_ptr<const T>` won't allow mutation
///              of the object other than the reference count.
///            - There is no weak version of ref_ptr.  In some cases it may be
///              possible to use raw pointers as a sort of weak ref_ptr as
///              long as all such pointers can be nulled out when Destroy is
///              called.
template <typename T>
class ref_ptr final {
   static_assert(!std::is_const_v<T>, "value_type must be non-const!");

   friend class ref_ptr<const T>;

   friend void swap(ref_ptr<T>& a, ref_ptr<T>& b) noexcept {
      using std::swap;
      swap(a.ptr_, b.ptr_);
   }

public:
   using value_type = T;

   ref_ptr() : ptr_(nullptr) { }

   ref_ptr(value_type* ptr) : ptr_(nullptr) {
      if (ptr) {
         ref::Acquire<value_type>{}(*ptr);
         ptr_ = ptr;
      }
   }

   ref_ptr(const ref_ptr<value_type>& other) : ptr_(nullptr) {
      if (other.ptr_) {
         ref::Acquire<value_type>{}(*other.ptr_);
         ptr_ = other.ptr_;
      }
   }

   ref_ptr(ref_ptr<value_type>&& other) : ptr_(nullptr) {
      using std::swap;
      swap(ptr_, other.ptr_);
   }

   ref_ptr<value_type>& operator=(const ref_ptr<value_type>& other) {
      using std::swap;
      ref_ptr<value_type> copy(other);
      swap(ptr_, copy.ptr_);
      return *this;
   }

   ref_ptr<value_type>& operator=(ref_ptr<value_type>&& other) {
      using std::swap;
      swap(ptr_, other.ptr_);
      other.reset();
      return *this;
   }

   ~ref_ptr() {
      if (ptr_) {
         ref::Release<value_type>{}(*ptr_);
         assert(nullptr == (ptr_ = nullptr));
      }
   }

   void reset() {
      if (ptr_) {
         ref::Release<value_type>{}(*ptr_);
         ptr_ = nullptr;
      }
   }

   void reset(value_type* ptr) {
      using std::swap;
      ref_ptr<value_type> copy(ptr);
      swap(ptr_, copy.ptr_);
   }

   explicit operator bool() const noexcept {
      return !!ptr_;
   }

   explicit operator value_type*() const noexcept {
      return ptr_;
   };

   value_type* get() const noexcept {
      return ptr_;
   }

   value_type* operator*() const noexcept {
      return ptr_;
   }

   value_type* operator->() const noexcept {
      return ptr_;
   }

   bool operator<(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ < other.ptr_;
   }

   bool operator<(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ < other.get();
   }

   bool operator<=(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ <= other.ptr_;
   }

   bool operator<=(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ <= other.get();
   }

   bool operator==(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ == other.ptr_;
   }

   bool operator==(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ == other.get();
   }

   bool operator!=(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ != other.ptr_;
   }

   bool operator!=(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ != other.get();
   }

   bool operator>=(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ >= other.ptr_;
   }

   bool operator>=(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ >= other.get();
   }

   bool operator>(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ > other.ptr_;
   }

   bool operator>(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ > other.get();
   }

private:
   value_type* ptr_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename T>
class ref_ptr<const T> final {
   friend void swap(ref_ptr<const T>& a, ref_ptr<const T>& b) noexcept {
      using std::swap;
      swap(a.ptr_, b.ptr_);
   }

public:
   using value_type = std::remove_const_t<T>;

   ref_ptr() : ptr_(nullptr) { }

   ref_ptr(value_type* ptr) : ptr_(nullptr) {
      if (ptr) {
         ref::Acquire<value_type>{}(*ptr);
         ptr_ = ptr;
      }
   }

   ref_ptr(const ref_ptr<value_type>& other) : ptr_(nullptr) {
      if (other.ptr_) {
         ref::Acquire<value_type>{}(*other.ptr_);
         ptr_ = other.ptr_;
      }
   }

   ref_ptr(const ref_ptr<const value_type>& other) : ptr_(nullptr) {
      if (other.ptr_) {
         ref::Acquire<value_type>{}(*other.ptr_);
         ptr_ = other.ptr_;
      }
   }

   ref_ptr(ref_ptr<value_type>&& other) : ptr_(nullptr) {
      using std::swap;
      swap(ptr_, other.ptr_);
   }

   ref_ptr(ref_ptr<const value_type>&& other) : ptr_(nullptr) {
      using std::swap;
      swap(ptr_, other.ptr_);
   }

   ref_ptr<const value_type>& operator=(const ref_ptr<value_type>& other) {
      using std::swap;
      ref_ptr<const value_type> copy(other);
      swap(ptr_, copy.ptr_);
      return *this;
   }

   ref_ptr<const value_type>& operator=(const ref_ptr<const value_type>& other) {
      using std::swap;
      ref_ptr<const value_type> copy(other);
      swap(ptr_, copy.ptr_);
      return *this;
   }

   ref_ptr<const value_type>& operator=(ref_ptr<value_type>&& other) {
      using std::swap;
      swap(ptr_, other.ptr_);
      other.reset();
      return *this;
   }

   ref_ptr<const value_type>& operator=(ref_ptr<const value_type>&& other) {
      using std::swap;
      swap(ptr_, other.ptr_);
      other.reset();
      return *this;
   }

   ~ref_ptr() {
      if (ptr_) {
         ref::Release<value_type>{}(*ptr_);
      }
   }

   void reset() {
      if (ptr_) {
         ref::Release<value_type>{}(*ptr_);
      }
   }

   void reset(value_type* ptr) {
      using std::swap;
      ref_ptr<const value_type> copy(ptr);
      swap(ptr_, copy.ptr_);
   }

   explicit operator bool() const noexcept {
      return !!ptr_;
   }

   explicit operator const value_type*() const noexcept {
      return ptr_;
   };

   const value_type* get() const noexcept {
      return ptr_;
   }

   const value_type* operator*() const noexcept {
      return ptr_;
   }

   const value_type* operator->() const noexcept {
      return ptr_;
   }

   bool operator<(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ < other.get();
   }

   bool operator<(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ < other.ptr_;
   }

   bool operator<=(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ <= other.get();
   }

   bool operator<=(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ <= other.ptr_;
   }

   bool operator==(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ == other.get();
   }

   bool operator==(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ == other.ptr_;
   }

   bool operator!=(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ != other.get();
   }

   bool operator!=(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ != other.ptr_;
   }

   bool operator>=(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ >= other.get();
   }

   bool operator>=(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ >= other.ptr_;
   }

   bool operator>(const ref_ptr<value_type>& other) const noexcept {
      return ptr_ > other.get();
   }

   bool operator>(const ref_ptr<const value_type>& other) const noexcept {
      return ptr_ > other.ptr_;
   }

private:
   value_type* ptr_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename T, typename... Args>
ref_ptr<T> make_ref(Args&&... args) {
   return ref_ptr<T>(new T(std::forward<Args>(args)...));
}

} // be

///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct std::hash<be::ref_ptr<T>> {
   std::size_t operator()(const be::ref_ptr<T>& o) const {
      return std::hash<T*>{}(o.get());
   }
};

#endif
