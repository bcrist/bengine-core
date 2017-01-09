#ifdef BE_TEST_PERF

#include <catch/catch.hpp>
#include "source_handle.hpp"
#include "const_handle.hpp"
#include "handle.hpp"
#include <iostream>

#define BE_CATCH_TAGS "[core][core:handle]"

using namespace be;

template <class T>
class DebugHandle : public detail::HandleBase<T> {
public:
   DebugHandle() {}
   DebugHandle(const detail::HandleBase<T>& other) : detail::HandleBase<T>(other) {}
   DebugHandle<T>& operator=(const detail::HandleBase<T>& other) { index_ = other.index_; check_ = other.check_; }

   uint32_t getIndex() const { return index_; }
   uint32_t getCheck() const { return check_; }
};


class TestAsset {
public:
   TestAsset() { handle_.associate(this); }

   Handle<TestAsset> getHandle() { return handle_; }
   ConstHandle<TestAsset> getHandle() const { return handle_; }

private:
   SourceHandle<TestAsset> handle_;
};


namespace Catch {

S toString(const ConstHandle<TestAsset>& handle) {
   DebugHandle<TestAsset> dh(handle);

   std::ostringstream oss;
   oss << std::hex << std::setfill('0');
   oss << std::setw(8) << dh.getIndex() << 'H' << std::setw(8) << dh.getCheck() << ':' << std::setw(8) << dh.getIndex();
   return oss.str();
}

} // Catch


TEST_CASE("Handle Index Retirement", BE_CATCH_TAGS) {
   std::cout << "Checking handle index retirement..." << std::endl;
   std::cout << "Note: this test may take a while" << std::endl;

   std::vector<std::unique_ptr<TestAsset> > tempvec;

   for (;;) {
      TestAsset* ptr = new TestAsset();
      tempvec.push_back(std::unique_ptr<TestAsset>(ptr));

      DebugHandle<TestAsset> dh(ptr->getHandle());
      if ((dh.getCheck() & 0x7FFFFFFF) == 0)
         break;

      std:: cout << (dh.getCheck() & 0x7FFFFFFF) << std::endl;
   }

   uint32_t index = DebugHandle<TestAsset>(tempvec.back()->getHandle()).getIndex() + 1;


   for (U64 i = 0ll; i <= 65536ll*32768ll; ++i) {
      TestAsset ta0;

      if (i % (65536ll) == 0) {
         float percent = int((i >> 16) / 32.768) / 10.0f;

         std::cout << "  " << percent << " % Complete        \r";

         if (i == 65536ll*32768ll) {
            DebugHandle<TestAsset> debug_handle(ta0.getHandle());
            REQUIRE(debug_handle.getIndex() != index);
         }
      }
   }

   std::cout << std::endl;
}

#endif
