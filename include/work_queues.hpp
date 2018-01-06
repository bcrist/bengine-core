#pragma once
#ifndef BE_CORE_WORK_QUEUES_HPP_
#define BE_CORE_WORK_QUEUES_HPP_

#include "be.hpp"
#include "work_queue_traits.hpp"
#include <queue>
#include <vector>
#include <functional>

namespace be {

///////////////////////////////////////////////////////////////////////////////
template <typename PriorityType = I32, typename Comparator = std::less<PriorityType>>
class PriorityWorkQueue : Movable {
   using value_type = std::pair<PriorityType, std::function<void()>>;
   struct value_comparator : private Comparator {
      bool operator()(value_type& a, value_type& b) const {
         return static_cast<const Comparator*>(this)->operator()(a.first, b.first);
      }
   };

public:
   using priority_type = PriorityType;

   PriorityType default_priority() const {
      return default_priority_;
   }

   void default_priority(PriorityType priority) {
      default_priority_ = priority;
   }

   void post(std::function<void()> func) {
      q_.emplace(default_priority_, std::move(func));
   }

   void post(std::function<void()> func, PriorityType priority) {
      q_.emplace(priority, std::move(func));
   }

   explicit operator bool() const {
      return true;
   }

   std::size_t size() const {
      return q_.size();
   }

   bool empty() const {
      return q_.empty();
   }

   std::function<void()> poll() {
      std::function<void()> func { };
      if (!q_.empty()) {
         func = std::move(q_.top().second);
         q_.pop();
      }
      return func;
   }

   std::function<void()> pull() {
      return poll();
   }

private:
   PriorityType default_priority_ = PriorityType();
   std::priority_queue<value_type, std::vector<value_type>, value_comparator> q_;
};

///////////////////////////////////////////////////////////////////////////////
template <typename Comparator>
class PriorityWorkQueue<void, Comparator> : Movable {
public:
   using priority_type = void;

   void post(std::function<void()> func) {
      q_.push(std::move(func));
   }

   explicit operator bool() const {
      return true;
   }

   std::size_t size() const {
      return q_.size();
   }

   bool empty() const {
      return q_.empty();
   }

   std::function<void()> poll() {
      std::function<void()> func { };
      if (!q_.empty()) {
         func = std::move(q_.front());
         q_.pop();
      }
      return func;
   }

   std::function<void()> pull() {
      return poll();
   }

private:
   std::queue<std::function<void()>> q_;
};

using SimpleWorkQueue = PriorityWorkQueue<void>;

} // be

#endif
