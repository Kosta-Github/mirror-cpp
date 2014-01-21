//
// The MIT License (MIT)
//
// Copyright (c) 2014 by Konstantin (Kosta) Baumann & Autodesk Inc.
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <catch/catch.hpp>

#include <mirror-cpp/mirror.hpp>

struct A {
    int a;
    int const a_const;
};

struct B : A {
    double b;
    double const b_const;

    A a2;
    A const a2_const;
};

struct C : B {
    std::string c;
    std::string const c_const;
};

CATCH_TEST_CASE(
    "Test registering a class",
    "[mirror][register_class]"
) {
    auto reg = mirror::class_registry();

    CATCH_CHECK_FALSE(reg.find_class_by_name("A").get());
    CATCH_CHECK_FALSE(reg.find_class_by_type<A>().get());

    auto a = mirror::make_class<A>("A");
    CATCH_REQUIRE(a.get());

    reg.add_class(a);
    CATCH_CHECK(reg.find_class_by_name("A").get() == a.get());
    CATCH_CHECK(reg.find_class_by_type<A>().get() == a.get());

    std::cout << a->to_string() << std::endl;
}

template<typename T1, typename T2>
static void check_prop(bool const read_only) {
    auto p = mirror::make_property<T1>("prop");
    CATCH_REQUIRE(p.get());
    CATCH_CHECK(p->name == "prop");
    CATCH_CHECK(p->type == typeid(T2));
    CATCH_CHECK((int)p->read_only == read_only);
}

CATCH_TEST_CASE(
    "Test creating a property",
    "[mirror][make_property]"
) {
    check_prop<decltype(A::a),          int>(               false);
    check_prop<decltype(A::a_const),    int const>(         true);
    check_prop<decltype(B::b),          double>(            false);
    check_prop<decltype(B::b_const),    double const>(      true);
    check_prop<decltype(B::a2),         A>(                 false);
    check_prop<decltype(B::a2_const),   A const>(           true);
    check_prop<decltype(C::a),          int>(               false);
    check_prop<decltype(C::a_const),    int const>(         true);
    check_prop<decltype(C::c),          std::string>(       false);
    check_prop<decltype(C::c_const),    std::string const>( true);
}

CATCH_TEST_CASE(
    "Test registering a property",
    "[mirror][register_property]"
) {
    auto a = mirror::make_class<A>("A");
    a->add_property<decltype(A::a)>("a");
    CATCH_CHECK(a->find_property_by_name("a").get());
}

CATCH_TEST_CASE(
    "Test registering two properties",
    "[mirror][register_property]"
) {
    auto a = mirror::make_class<A>("A");
    a->add_property<decltype(A::a)>("a");
    CATCH_CHECK(a->find_property_by_name("a").get());
    a->add_property<decltype(A::a_const)>("a_const");
    CATCH_CHECK(a->find_property_by_name("a").get());
    CATCH_CHECK(a->find_property_by_name("a_const").get());
}

CATCH_TEST_CASE(
    "Test registering properties",
    "[mirror][register_property]"
) {
    auto a = mirror::make_class<A>("A");
    a->add_property<decltype(A::a)>("a");
    a->add_property<decltype(A::a_const)>("a_const");

    CATCH_CHECK(a->find_property_by_name("a").get());
    CATCH_CHECK(a->find_property_by_name("a_const").get());
    CATCH_CHECK_FALSE(a->find_property_by_name("b").get());
    CATCH_CHECK_FALSE(a->find_property_by_name("b_const").get());

    auto b = mirror::make_class<B>("B", a);
    b->add_property<decltype(B::b)>("b");
    b->add_property<decltype(B::b_const)>("b_const");
    b->add_property<decltype(B::a2)>("a2");
    b->add_property<decltype(B::a2_const)>("a2_const");

    std::cout << b->to_string() << std::endl;

    CATCH_CHECK(b->find_property_by_name("b").get());
    CATCH_CHECK(b->find_property_by_name("b_const").get());
    CATCH_CHECK(b->find_property_by_name("a2").get());
    CATCH_CHECK(b->find_property_by_name("a2_const").get());

    CATCH_CHECK_FALSE(b->find_property_by_name("a").get());         // "a" is in the base class
    CATCH_CHECK_FALSE(b->find_property_by_name("a_const").get());   // "a" is in the base class
    CATCH_CHECK(b->find_property_by_name("a", true).get());         // "a" is in the base class
    CATCH_CHECK(b->find_property_by_name("a_const", true).get());   // "a" is in the base class

    std::cout << b->to_string() << std::endl;
}
