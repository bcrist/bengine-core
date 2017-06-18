#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN

#include <be/core/time.hpp>

#include "native/vc_win/vc_win_win32.hpp"
#include <VersionHelpers.h>

namespace be {
namespace {

///////////////////////////////////////////////////////////////////////////////
U64 query_performance_frequency() {
   ::LARGE_INTEGER qpf;
   ::QueryPerformanceFrequency(&qpf);
   return qpf.QuadPart;
}

///////////////////////////////////////////////////////////////////////////////
U64 get_qpc_frequency() {
   static U64 freq = query_performance_frequency();
   return freq;
}

///////////////////////////////////////////////////////////////////////////////
F64 get_qpc_tick_period() {
   static F64 mult = 1.0 / (F64)get_qpc_frequency();
   return mult;
}

///////////////////////////////////////////////////////////////////////////////
typedef VOID (WINAPI *PFGetSystemTimeAsFileTime)(::LPFILETIME);

///////////////////////////////////////////////////////////////////////////////
PFGetSystemTimeAsFileTime obtain_system_time_func() {
   if (::IsWindows8OrGreater()) {
      ::FARPROC fp;
      ::HMODULE k32 = ::LoadLibraryW(L"kernel32.dll");
      if (k32 != NULL) {
         fp = ::GetProcAddress(k32, "GetSystemTimePreciseAsFileTime");
         if (fp != NULL) {
            return (PFGetSystemTimeAsFileTime)fp;
         }
      }
   }
   return GetSystemTimeAsFileTime;
}

///////////////////////////////////////////////////////////////////////////////
PFGetSystemTimeAsFileTime get_system_time_func() {
   static PFGetSystemTimeAsFileTime func = obtain_system_time_func();
   return func;
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
TU ts_now() noexcept {
   //return std::chrono::duration_cast<TU>(std::chrono::system_clock::now().time_since_epoch());

   ::FILETIME file_time;
   get_system_time_func()(&file_time);

   ::ULARGE_INTEGER ull;
   ull.LowPart = file_time.dwLowDateTime;
   ull.HighPart = file_time.dwHighDateTime;

   return TU((ull.QuadPart - 116444736000000000ull) / 10ull);
}

///////////////////////////////////////////////////////////////////////////////
U64 perf_now() noexcept {
   ::LARGE_INTEGER ll;
   ::QueryPerformanceCounter(&ll);
   return (U64)ll.QuadPart;
}

///////////////////////////////////////////////////////////////////////////////
TU perf_delta_to_tu(U64 delta) noexcept {
   return TU(delta * TU::period::den / get_qpc_frequency());
}

///////////////////////////////////////////////////////////////////////////////
F64 perf_delta_to_seconds(U64 delta) noexcept {
   return (F64)delta * get_qpc_tick_period();
}

///////////////////////////////////////////////////////////////////////////////
tm* time_t_to_local_tm(time_t t, tm& in) noexcept {
   localtime_s(&in, &t);
   return &in;
}

} // be

#endif
