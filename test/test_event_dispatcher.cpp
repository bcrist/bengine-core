#ifdef BE_TEST

#include "service_event_dispatcher.hpp"
#include "event_traits.hpp"
#include "ensure_init.hpp"
#include "service_helpers.hpp"
#include <catch/catch.hpp>

#define BE_CATCH_TAGS "[core][core:events]"

using namespace be;

struct EmptyEvent { };
struct EmptyEvent2 { };

TEST_CASE("be::EventDispatcher empty events", BE_CATCH_TAGS) {
   ensure_init();
   auto& dispatcher = service<EventDispatcher>();
   dispatcher.reset();

   int a = 0;
   int b = 0;

   Id first_cb = dispatcher.subscribe(EmptyEvent(), [&](const EmptyEvent&) { ++a; });
   dispatcher.subscribe(EmptyEvent2(), [&](const EmptyEvent2&) { ++b; });

   REQUIRE(a == 0);
   REQUIRE(b == 0);
   dispatcher.fire(EmptyEvent());
   REQUIRE(a == 1);
   REQUIRE(b == 0);
   dispatcher.defer(EmptyEvent2());
   REQUIRE(a == 1);
   REQUIRE(b == 0);
   dispatcher.dispatch_deferred();
   REQUIRE(a == 1);
   REQUIRE(b == 1);
   dispatcher.defer(EmptyEvent());
   REQUIRE(a == 1);
   REQUIRE(b == 1);
   dispatcher.subscribe(EmptyEvent(), [&](const EmptyEvent&) { ++b; });
   dispatcher.subscribe(first_cb, EmptyEvent2(), [&](const EmptyEvent2&) { a *= 2; });
   REQUIRE(a == 1);
   REQUIRE(b == 1);
   dispatcher.dispatch_deferred();
   REQUIRE(a == 2);
   REQUIRE(b == 2);
   dispatcher.fire(EmptyEvent2());
   REQUIRE(a == 4);
   REQUIRE(b == 3);
   dispatcher.defer(EmptyEvent());
   REQUIRE(a == 4);
   REQUIRE(b == 3);
   dispatcher.unsubscribe(first_cb, EmptyEvent());
   REQUIRE(a == 4);
   REQUIRE(b == 3);
   dispatcher.dispatch_deferred();
   REQUIRE(a == 4);
   REQUIRE(b == 4);
   dispatcher.fire(EmptyEvent2());
   REQUIRE(a == 8);
   REQUIRE(b == 5);
   dispatcher.unsubscribe(first_cb, EmptyEvent2());
   REQUIRE(a == 8);
   REQUIRE(b == 5);
   dispatcher.fire(EmptyEvent2());
   REQUIRE(a == 8);
   REQUIRE(b == 6);
   dispatcher.unsubscribe_all_callbacks(EmptyEvent());
   dispatcher.unsubscribe_all_callbacks(EmptyEvent2());
   REQUIRE(a == 8);
   REQUIRE(b == 6);
   dispatcher.fire(EmptyEvent());
   REQUIRE(a == 8);
   REQUIRE(b == 6);
   dispatcher.fire(EmptyEvent2());
   REQUIRE(a == 8);
   REQUIRE(b == 6);
}

struct TargetedEvent {
   Id event_source;
};

TEST_CASE("be::EventDispatcher targetted single-type event", BE_CATCH_TAGS) {
   ensure_init();
   auto& dispatcher = service<EventDispatcher>();
   dispatcher.reset();

   int a = 0;
   int b = 0;

   dispatcher.subscribe(TargetedEvent { Id(1) }, [&](const TargetedEvent&) { ++a; });
   dispatcher.subscribe(TargetedEvent { Id(2) }, [&](const TargetedEvent&) { ++b; });

   REQUIRE(a == 0);
   REQUIRE(b == 0);
   dispatcher.fire(TargetedEvent { Id(1) });
   REQUIRE(a == 1);
   REQUIRE(b == 0);
   dispatcher.defer(TargetedEvent { Id(2) });
   REQUIRE(a == 1);
   REQUIRE(b == 0);
   dispatcher.dispatch_deferred();
   REQUIRE(a == 1);
   REQUIRE(b == 1);
}

struct MultiEvent {
   enum class type_enum {
      type_a,
      type_b
   };

   const EventType<MultiEvent>* event_type() const {
      switch (type) {
         case type_enum::type_a: return &(type_ref<type_enum::type_a>());
         default: return &(type_ref<type_enum::type_b>());
      }
   }

   int* event_source() const {
      return source;
   }

   type_enum type;
   int* source;

private:
   template <type_enum T>
   static const EventType<MultiEvent>& type_ref() {
      static EventType<MultiEvent> type(name(T));
      return type;
   }

   static const char* name(type_enum t) {
      switch (t) {
         case type_enum::type_a: return "MultiEvent::type_a";
         case type_enum::type_b: return "MultiEvent::type_a";
         default: return "?";
      }
   }
};


TEST_CASE("be::EventDispatcher targeted multi-type event", BE_CATCH_TAGS) {
   ensure_init();
   auto& dispatcher = service<EventDispatcher>();
   dispatcher.reset();

   int a = 0;
   int b = 0;

   dispatcher.subscribe(MultiEvent { MultiEvent::type_enum::type_a, &a }, [&](const MultiEvent& e) { ++*(e.event_source()); });
   dispatcher.subscribe(MultiEvent { MultiEvent::type_enum::type_a, &b }, [&](const MultiEvent& e) { ++*(e.event_source()); });
   dispatcher.subscribe(MultiEvent { MultiEvent::type_enum::type_b, &b }, [&](const MultiEvent& e) { *(e.event_source()) *= 4; });

   REQUIRE(a == 0);
   REQUIRE(b == 0);
   dispatcher.fire(MultiEvent { MultiEvent::type_enum::type_a, &a });
   REQUIRE(a == 1);
   REQUIRE(b == 0);
   dispatcher.fire(MultiEvent { MultiEvent::type_enum::type_a, &b });
   REQUIRE(a == 1);
   REQUIRE(b == 1);
   dispatcher.fire(MultiEvent { MultiEvent::type_enum::type_b, &a });
   REQUIRE(a == 1);
   REQUIRE(b == 1);
   dispatcher.fire(MultiEvent { MultiEvent::type_enum::type_b, &b });
   REQUIRE(a == 1);
   REQUIRE(b == 4);
}

#endif
