#define BOOST_TEST_MODULE scope_exit tests
#include <boost/test/included/unit_test.hpp>

#include "scope_exit.hpp"

#include <type_traits>

BOOST_AUTO_TEST_CASE(scope_exit_invariants)
{
    auto se{make_scope_exit([] {})};

    BOOST_TEST(std::is_move_constructible_v<decltype(se)>);

    BOOST_TEST(!std::is_default_constructible_v<decltype(se)>);
    BOOST_TEST(!std::is_copy_constructible_v<decltype(se)>);
    BOOST_TEST(!std::is_copy_assignable_v<decltype(se)>);
    BOOST_TEST(!std::is_move_assignable_v<decltype(se)>);
}

BOOST_AUTO_TEST_CASE(works_with_rvalue_callback)
{
    auto called{false};
    {
        auto se{make_scope_exit([&called] { called = true; })};
    }

    BOOST_TEST(called);
}

BOOST_AUTO_TEST_CASE(works_with_lvalue_callback)
{
    auto called{false};
    auto closure{[&called] { called = true; }};
    {
        auto se{make_scope_exit(closure)};
    }

    BOOST_TEST(called);
}

BOOST_AUTO_TEST_CASE(works_with_bind_with_argument)
{
    auto foo{0};
    auto closure{[&foo](auto val) { foo = val; }};
    {
        auto se{make_scope_exit(std::bind(closure, 10))};
    }

    BOOST_TEST(foo == 10);
}

BOOST_AUTO_TEST_CASE(works_with_traditional_functor)
{
    struct functor {
        bool & called_;
        functor(bool & called)
            : called_{called}
        {
        }
        void operator()() { called_ = true; }
    };

    auto called{false};
    {
        auto se{make_scope_exit(functor{called})};
    }

    BOOST_TEST(called);
}

BOOST_AUTO_TEST_CASE(callback_is_not_called_when_released)
{
    auto called{false};
    {
        auto se{make_scope_exit([&called] { called = true; })};
        se.release();
    }

    BOOST_TEST(!called);
}

BOOST_AUTO_TEST_CASE(callback_only_called_once_when_move_constructed)
{
    auto foo{0};
    auto closure{[&foo] { ++foo; }};
    {
        auto se{make_scope_exit(closure)};
        auto se2{std::move(se)};
    }

    BOOST_TEST(foo == 1);
}

BOOST_AUTO_TEST_CASE(when_released_and_moved_callback_is_not_called)
{
    auto called{false};
    auto closure{[&called] { called = true; }};
    {
        auto se{make_scope_exit(closure)};
        se.release();
        auto se2{std::move(se)};
    }

    BOOST_TEST(!called);
}
