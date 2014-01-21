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
};

struct B : A {
    double b;
};

struct C : B {
    std::string c;
};

CATCH_TEST_CASE(
    "Test registering a class",
    "[mirror][register_class]"
) {
    auto reg = mirror::class_registry();

    CATCH_CHECK_FALSE(reg.find_by_name("A").get());
    CATCH_CHECK_FALSE(reg.find_by_type<A>().get());

    auto a = mirror::make_class<A>("A");
    CATCH_REQUIRE(a.get());

    reg.register_class(a);
    CATCH_CHECK(reg.find_by_name("A").get() == a.get());
    CATCH_CHECK(reg.find_by_type<A>().get() == a.get());
}
