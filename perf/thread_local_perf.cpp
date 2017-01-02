// This test is disabled because it doesn't effectively measure what it should - the time taken to get a block-scope static or thread_local object.
//
//#if defined(BE_TEST_PERF)
//
//#include <catch.hpp>
//#include <Windows.h>
//#include <random>
//
//#pragma warning(disable: 4459) // declaration hides global
//#include <boost/accumulators/accumulators.hpp>
//#include <boost/accumulators/statistics.hpp>
//
//#define BE_CATCH_TAGS "[platform][platform:tls]"
//
//using namespace be;
//using namespace boost::accumulators;
//
//template <int N>
//int& tls_int() {
//   thread_local int var = 0;
//   return var;
//}
//
//template <int N>
//int& static_int() {
//   static int var = 0;
//   return var;
//}
//
//TEST_CASE("block thread_local vs block static vs local", BE_CATCH_TAGS) {
//
//   using data_vec = std::vector<std::pair<U64, int>>;
//
//   data_vec tls_ticks;
//   data_vec static_ticks;
//   data_vec local_ticks;
//
//   LARGE_INTEGER f;
//   QueryPerformanceFrequency(&f);
//
//   U64 freq = (U64)f.QuadPart;
//
//   std::mt19937 prng;
//   std::uniform_int_distribution<int> dist;
//
//   const int runs = 60;
//   const int warmup = 20;
//   const int n = 100000;
//
//   LARGE_INTEGER q1, q2;
//
//   std::vector<int> data;
//   data.reserve(n);
//
//   for (int i = 0; i < runs + warmup; ++i) {
//
//      data.clear();
//      for (int j = 0; j < n; ++j) {
//         data.push_back(dist(prng));
//      }
//
//      int local = 0;
//      QueryPerformanceCounter(&q1);
//      for (int j = 0; j < n; ++j) {
//         local += data[j];
//      }
//      QueryPerformanceCounter(&q2);
//      if (i >= warmup) {
//         local_ticks.emplace_back(q2.QuadPart - q1.QuadPart, local);
//      }
//
//      int l = n / 10;
//      QueryPerformanceCounter(&q1);
//      for (int j = 0; j < l; ++j) {
//         tls_int<0>() += data[j];
//         tls_int<1>() += data[j+1];
//         tls_int<2>() += data[j+2];
//         tls_int<3>() += data[j+3];
//         tls_int<4>() += data[j+4];
//         tls_int<5>() += data[j+5];
//         tls_int<6>() += data[j+6];
//         tls_int<7>() += data[j+7];
//         tls_int<8>() += data[j+8];
//         tls_int<9>() += data[j+9];
//      }
//      QueryPerformanceCounter(&q2);
//      if (i >= warmup) {
//         tls_ticks.emplace_back(q2.QuadPart - q1.QuadPart, tls_int<9>());
//      }
//
//      QueryPerformanceCounter(&q1);
//      for (int j = 0; j < l; ++j) {
//         static_int<0>() += data[j];
//         static_int<1>() += data[j+1];
//         static_int<2>() += data[j+2];
//         static_int<3>() += data[j+3];
//         static_int<4>() += data[j+4];
//         static_int<5>() += data[j+5];
//         static_int<6>() += data[j+6];
//         static_int<7>() += data[j+7];
//         static_int<8>() += data[j+8];
//         static_int<9>() += data[j+9];
//      }
//      QueryPerformanceCounter(&q2);
//      if (i >= warmup) {
//         static_ticks.emplace_back(q2.QuadPart - q1.QuadPart, static_int<9>());
//      }
//   }
//
//   accumulator_set<F64, stats<tag::variance>> local_acc;
//   accumulator_set<F64, stats<tag::variance>> tls_acc;
//   accumulator_set<F64, stats<tag::variance>> static_acc;
//
//   F64 div = (F64)freq;
//   div /= 1000;
//
//   std::for_each(local_ticks.begin(), local_ticks.end(),   [&](const std::pair<U64, int>& val) { local_acc((F64)val.first / div); });
//   std::for_each(tls_ticks.begin(), tls_ticks.end(),       [&](const std::pair<U64, int>& val) { tls_acc((F64)val.first / div); });
//   std::for_each(static_ticks.begin(), static_ticks.end(), [&](const std::pair<U64, int>& val) { static_acc((F64)val.first / div); });
//
//   std::cout << "Runs: " << runs << "\tWarmup: " << warmup << "\tN: " << n << std::endl << std::endl;
//
//   std::cout << "Local:\tu: " << mean(local_acc) << " ms\ts: " << sqrt(variance(local_acc)) << " ms" << std::endl;
//   std::cout << "TLS:\tu: " << mean(tls_acc) << " ms\ts: " << sqrt(variance(tls_acc)) << " ms" << std::endl;
//   std::cout << "Static:\tu: " << mean(static_acc) << " ms\ts: " << sqrt(variance(static_acc)) << " ms" << std::endl;
//}
//
//#endif
