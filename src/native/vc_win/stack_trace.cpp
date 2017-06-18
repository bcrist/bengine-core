#include <be/core/native.hpp>
#ifdef BE_NATIVE_VC_WIN

#include "stack_trace.hpp"
#include "logging.hpp"
#include <mutex>

#include "native/vc_win/vc_win_win32.hpp"
#pragma warning(push)
#pragma warning(disable: 4091) // 'typedef ': ignored on left of '' when no variable is declared
#include <DbgHelp.h>
#pragma warning(pop)

#pragma comment(lib, "Dbghelp.lib")

namespace be {
namespace {

///////////////////////////////////////////////////////////////////////////////
/// \return true if there may be more stack frames available.
BE_NO_INLINE bool get_frames(std::vector<std::uintptr_t>& vec, U32 max_frames, U32 initial_skipped_frames) {
   std::size_t old_size = vec.size();
   vec.resize(old_size + max_frames);

   U16 captured = ::CaptureStackBackTrace(::DWORD(initial_skipped_frames + old_size), max_frames, reinterpret_cast<::PVOID*>(&(vec[old_size])), nullptr);
   vec.resize(old_size + captured);
   return captured == max_frames;
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
BE_NO_INLINE StackTrace get_stack_trace(U32 frames_to_skip) {
   StackTrace trace;

   U32 mandatory_skipped_frames = 2;
   U32 max_frames = 30;

   while (get_frames(trace.frames, max_frames, frames_to_skip + mandatory_skipped_frames)) {
      max_frames += max_frames >> 1; // *= 1.5
   }

   return trace;
}

namespace {

bool& is_symbol_handler_initialized();

///////////////////////////////////////////////////////////////////////////////
std::mutex& monitor() {
   static std::mutex m;
   return m;
}

///////////////////////////////////////////////////////////////////////////////
void cleanup_symbol_handler() {
   bool& initialized = is_symbol_handler_initialized();
   if (initialized) {
      initialized = false;

      if (!::SymCleanup(::GetCurrentProcess())) {
         ::DWORD error = ::GetLastError();

         be_error() << "Error cleaning up DbgHelp symbol handler!"
            & attr("Error") << error
            | default_log();
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
bool init_symbol_handler() {
   ::SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
   if (!::SymInitialize(::GetCurrentProcess(), nullptr, true)) {
      ::DWORD error = ::GetLastError();

      be_error() << "Failed to initialize DbgHelp symbol handler!"
         & attr("Error") << error
         | default_log();

      return false;
   }

   std::atexit(cleanup_symbol_handler);
   return true;
}

///////////////////////////////////////////////////////////////////////////////
bool& is_symbol_handler_initialized() {
   static bool flag = init_symbol_handler();
   return flag;
}

} // be::()

///////////////////////////////////////////////////////////////////////////////
StackFrameSymbolInfo get_stack_frame_symbol_info(std::uintptr_t address) {
   StackFrameSymbolInfo info;
   info.address = address;
   info.line = 0;
   std::lock_guard<std::mutex> guard(monitor());

   if (is_symbol_handler_initialized()) {
      ::HANDLE process = ::GetCurrentProcess();
      ::DWORD64 addr = static_cast<::DWORD64>(address);

      {
         ::DWORD64 displacement;
         char buffer[sizeof(::SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(::WCHAR)];
         ::PSYMBOL_INFOW symbol_info = reinterpret_cast<::PSYMBOL_INFOW>(buffer);
         symbol_info->SizeOfStruct = sizeof(::SYMBOL_INFOW);
         symbol_info->MaxNameLen = MAX_SYM_NAME;

         if (::SymFromAddrW(process, addr, &displacement, symbol_info)) {
            int size = ::WideCharToMultiByte(CP_UTF8, 0, symbol_info->Name, (int)symbol_info->NameLen, NULL, 0, NULL, NULL);
            info.symbol.assign((std::size_t)size, 0);
            ::WideCharToMultiByte(CP_UTF8, 0, symbol_info->Name, (int)symbol_info->NameLen, &info.symbol[0], size, NULL, NULL);
         }
      }
      
      {
         ::IMAGEHLP_MODULEW64 module;
         module.SizeOfStruct = sizeof(module);

         if (::SymGetModuleInfoW64(process, addr, &module)) {
            int size = ::WideCharToMultiByte(CP_UTF8, 0, module.ModuleName, -1, NULL, 0, NULL, NULL);
            info.module.assign((std::size_t)size, 0);
            ::WideCharToMultiByte(CP_UTF8, 0, module.ModuleName, -1, &info.module[0], size, NULL, NULL);
         }
      }

      {
         ::DWORD displacement;
         ::IMAGEHLP_LINEW64 line;
         line.SizeOfStruct = sizeof(line);

         if (::SymGetLineFromAddrW64(process, addr, &displacement, &line)) {
            info.line = static_cast<U32>(line.LineNumber);
            info.line_displacement = static_cast<I32>(displacement);

            int size = ::WideCharToMultiByte(CP_UTF8, 0, line.FileName, -1, NULL, 0, NULL, NULL);
            info.file.assign((std::size_t)size, 0);
            ::WideCharToMultiByte(CP_UTF8, 0, line.FileName, -1, &info.file[0], size, NULL, NULL);
         }
      }
   }
   return info;
}

} // be

#endif
