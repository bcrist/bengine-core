#ifdef BE_TEST

#include "ref_ptr.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[core][core:ref_ptr]"

using namespace be;

struct TestA {
   std::size_t ref_count = 0;
};

class TestB {
   friend struct ref::RefCount<TestB>;
public:
   std::size_t refs() const noexcept {
      return ref_count_;
   }
private:
   std::size_t ref_count_ = 0;
};

struct TestC {
   std::size_t ref_count = 0;
};

template <>
struct ref::Destroy<TestC> {
   void operator()(TestC& obj) { }
};

TEST_CASE("be::ref_ptr empty state", BE_CATCH_TAGS) {
   SECTION("TestA") {
      ref_ptr<TestA> p;
      REQUIRE_FALSE(p);
      REQUIRE(( TestA* ) p == nullptr);
      REQUIRE(p.get() == nullptr);
   }

   SECTION("const TestA") {
      ref_ptr<const TestA> p;
      REQUIRE_FALSE(p);
      REQUIRE(( const TestA* ) p == nullptr);
      REQUIRE(p.get() == nullptr);
   }

   SECTION("TestB") {
      ref_ptr<TestB> p;
      REQUIRE_FALSE(p);
      REQUIRE(( TestB* ) p == nullptr);
      REQUIRE(p.get() == nullptr);
   }

   SECTION("const TestB") {
      ref_ptr<const TestB> p;
      REQUIRE_FALSE(p);
      REQUIRE(( const TestB* ) p == nullptr);
      REQUIRE(p.get() == nullptr);
   }
}

TEST_CASE("be::ref_ptr basic usage", BE_CATCH_TAGS) {
   SECTION("TestA") {
      ref_ptr<TestA> p { new TestA };

      REQUIRE(p);
      REQUIRE(p->ref_count == 1);
      
      ref_ptr<TestA> q(p);

      REQUIRE(p);
      REQUIRE(q);
      REQUIRE(p->ref_count == 2);
      REQUIRE(q->ref_count == 2);
      REQUIRE(p == q);
      REQUIRE(( TestA* ) p == ( TestA* ) q);

      ref_ptr<TestA> r(std::move(q));
      REQUIRE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r);
      REQUIRE(p->ref_count == 2);
      REQUIRE(r->ref_count == 2);
      REQUIRE(p == r);
      REQUIRE(q.get() == nullptr);
      REQUIRE(q < p);
      REQUIRE(q <= p);
      REQUIRE(r > q);
      REQUIRE(r >= q);
      REQUIRE(q != p);
      REQUIRE(( TestA* ) r != nullptr);

      ref_ptr<const TestA> s(r);
      REQUIRE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r);
      REQUIRE(s);
      REQUIRE(p->ref_count == 3);
      REQUIRE(r->ref_count == 3);
      REQUIRE(s->ref_count == 3);
      REQUIRE(r == s);
      REQUIRE(( const TestA* ) s == p.get());

      p.reset(new TestA);

      REQUIRE(p->ref_count == 1);
      REQUIRE_FALSE(q);
      REQUIRE(r->ref_count == 2);
      REQUIRE(s->ref_count == 2);

      p.reset();

      REQUIRE_FALSE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r->ref_count == 2);
      REQUIRE(s->ref_count == 2);

      p = r;

      REQUIRE(p->ref_count == 3);
      REQUIRE_FALSE(q);
      REQUIRE(r->ref_count == 3);
      REQUIRE(s->ref_count == 3);

      r = std::move(p);

      REQUIRE_FALSE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r->ref_count == 2);
      REQUIRE(s->ref_count == 2);
   }

   SECTION("TestB") {
      ref_ptr<TestB> p { new TestB };

      REQUIRE(p);
      REQUIRE(p->refs() == 1);
      
      ref_ptr<TestB> q(p);

      REQUIRE(p);
      REQUIRE(q);
      REQUIRE(p->refs() == 2);
      REQUIRE(q->refs() == 2);
      REQUIRE(p == q);
      REQUIRE(( TestB* ) p == ( TestB* ) q);

      ref_ptr<TestB> r(std::move(q));
      REQUIRE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r);
      REQUIRE(p->refs() == 2);
      REQUIRE(r->refs() == 2);
      REQUIRE(p == r);
      REQUIRE(q.get() == nullptr);
      REQUIRE(q < p);
      REQUIRE(q <= p);
      REQUIRE(r > q);
      REQUIRE(r >= q);
      REQUIRE(q != p);
      REQUIRE(( TestB* ) r != nullptr);

      ref_ptr<const TestB> s(r);
      REQUIRE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r);
      REQUIRE(s);
      REQUIRE(p->refs() == 3);
      REQUIRE(r->refs() == 3);
      REQUIRE(s->refs() == 3);
      REQUIRE(r == s);
      REQUIRE(( const TestB* ) s == p.get());

      p.reset(new TestB);

      REQUIRE(p->refs() == 1);
      REQUIRE_FALSE(q);
      REQUIRE(r->refs() == 2);
      REQUIRE(s->refs() == 2);

      p.reset();

      REQUIRE_FALSE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r->refs() == 2);
      REQUIRE(s->refs() == 2);

      p = r;

      REQUIRE(p->refs() == 3);
      REQUIRE_FALSE(q);
      REQUIRE(r->refs() == 3);
      REQUIRE(s->refs() == 3);

      r = std::move(p);

      REQUIRE_FALSE(p);
      REQUIRE_FALSE(q);
      REQUIRE(r->refs() == 2);
      REQUIRE(s->refs() == 2);
   }

   SECTION("TestC") {
      TestC val;

      REQUIRE(val.ref_count == 0);

      ref_ptr<TestC> p { &val };

      REQUIRE(val.ref_count == 1);

      p.reset();

      REQUIRE(val.ref_count == 0);

      p = ref_ptr<TestC> { &val };

      REQUIRE(val.ref_count == 1);
   }
}

TEST_CASE("be::make_ref", BE_CATCH_TAGS) {
   SECTION("TestA") {
      ref_ptr<TestA> ptr1;
      ref_ptr<const TestA> ptr2;

      REQUIRE_NOTHROW(ptr1 = make_ref<TestA>());
      REQUIRE_NOTHROW(ptr2 = make_ref<TestA>());
      REQUIRE(ptr1->ref_count == 1);
      REQUIRE(ptr2->ref_count == 1);
   }

   SECTION("TestB") {
      ref_ptr<TestB> ptr1;
      ref_ptr<const TestB> ptr2;

      REQUIRE_NOTHROW(ptr1 = make_ref<TestB>());
      REQUIRE_NOTHROW(ptr2 = make_ref<TestB>());
      REQUIRE(ptr1->refs() == 1);
      REQUIRE(ptr2->refs() == 1);
   }
}

TEST_CASE("be::ref_ptr std::hash", BE_CATCH_TAGS) {
   ref_ptr<TestA> ptr1 = make_ref<TestA>();
   REQUIRE(std::hash<ref_ptr<TestA>>{}(ptr1) == std::hash<TestA*>{}(ptr1.get()));
}

#endif
