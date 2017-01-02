#pragma once
#ifndef BE_CORE_WORK_QUEUE_TRAITS_HPP_
#define BE_CORE_WORK_QUEUE_TRAITS_HPP_

namespace be {
namespace t {
namespace detail {

template<typename WorkQueue>
struct HasWorkQueueShutdown {
   template<typename U> static constexpr typename std::is_same<void, decltype(std::declval<U>().shutdown())>::type f(U*);
   template<typename U> static constexpr False f(...);
   using type = decltype(f<WorkQueue>(0));
};

template<typename WorkQueue>
struct HasWorkQueueUnShutdown {
   template<typename U> static constexpr typename std::is_same<void, decltype(std::declval<U>().shutdown(false))>::type f(U*);
   template<typename U> static constexpr False f(...);
   using type = decltype(f<WorkQueue>(0));
};

} // be::t::detail
} // be::t

///////////////////////////////////////////////////////////////////////////////
template <typename WorkQueue>
struct WorkQueueTraits {
   using priority_type = typename WorkQueue::priority_type;
   using supports_priorities = std::integral_constant<bool, !std::is_same<priority_type, void>::value>;
   using has_shutdown = typename t::detail::HasWorkQueueShutdown<WorkQueue>::type;
   using has_unshutdown = typename t::detail::HasWorkQueueUnShutdown<WorkQueue>::type;
};

} // be

#endif
