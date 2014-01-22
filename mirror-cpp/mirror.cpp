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

#include "mirror.hpp"

#include <algorithm>
#include <cassert>

template<typename CONT>
static inline auto find_by_name(
    CONT&& cont,
    std::string const& name
) -> decltype(cont.begin()) {
    assert(!name.empty());

    // use linear search based on the type
    typedef decltype(*cont.begin()) T;
    return std::find_if(
        cont.begin(), cont.end(),
        [&](T const& c) { return (c->name == name); }
    );
}

template<typename CONT>
static inline auto find_by_type(
    CONT&& cont,
    std::type_info const& type
) -> decltype(cont.begin()) {

    // use linear search based on the type
    typedef decltype(*cont.begin()) T;
    return std::find_if(
        cont.begin(), cont.end(),
        [&](T const& c) { return (c->type == type); }
    );
}

mirror::name_type_info::name_type_info(
    std::string n,
    std::type_info const& t
) :
    name(std::move(n)),
    type(t)
{
    assert(!name.empty());
}

std::string mirror::name_type_info::to_string(
    int indent
) const {
    assert(indent >= 0);

    std::string result;
    result += std::string(indent,   ' ') + "name: " + name + "\n";
    result += std::string(indent+1, ' ') + "type: " + type.name() + "\n";
    return result;
}

std::string mirror::property_info::to_string(
    int indent
) const {
    assert(indent >= 0);

    std::string result;
    result += name_type_info::to_string(indent);
    result += std::string(indent+1, ' ') + "read_only: " + (read_only ? "true" : "false") + "\n";
    return result;
}

std::string mirror::method_info::to_string(
    int indent
) const {
    assert(indent >= 0);

    std::string result;
    result += name_type_info::to_string(indent);
/*
    if(!parameters.empty()) {
        result += std::string(indent+1, ' ') + "parameters:\n";
        for(auto&& i : parameters) { result += i->to_string(indent+2); }
    }
*/
    return result;
}

void mirror::class_info_base::add_property_impl(
    property_ptr p
) {
    assert(p);
    assert(!find_property_by_name(p->name, true));

    auto it = find_by_name(properties, name);
    properties.emplace(it, std::move(p));
}

mirror::property_ptr mirror::class_info_base::find_property_by_name(
    std::string const& name,
    bool search_base
) const {
    auto it = find_by_name(properties, name);
    if((it != properties.end()) && ((*it)->name == name)) { return *it; }

    if(search_base && base_class) {
        return base_class->find_property_by_name(name, search_base);
    }

    return nullptr;
}

std::string mirror::class_info_base::to_string(
    int indent
) const {
    assert(indent >= 0);

    std::string result;
    result += name_type_info::to_string(indent);

    if(base_class) {
        result += std::string(indent+1, ' ') + "base: " + base_class->name + "\n";
        result += base_class->to_string(indent+2) + "\n";
    }

    if(!properties.empty()) {
        result += std::string(indent+1, ' ') + "properties:\n";
        for(auto&& i : properties) { result += i->to_string(indent+2); }
    }

    if(!methods.empty()) {
        result += std::string(indent+1, ' ') + "methods:\n";
        for(auto&& i : methods) { result += i->to_string(indent+2); }
    }

    return result;
}

void mirror::class_registry::add_class(
    class_base_ptr c
) {
    assert(c);
    assert(!find_class_by_name(c->name));

    auto it = find_by_name(classes, c->name);
    classes.emplace(it, std::move(c));
}

mirror::class_base_ptr mirror::class_registry::find_class_by_name(
    std::string const& name
) const {
    auto it = find_by_name(classes, name);
    if(it == classes.end()) { return nullptr; }
    if((*it)->name != name) { return nullptr; }
    return *it;
}

mirror::class_base_ptr mirror::class_registry::find_class_by_type(
    std::type_info const& type
) const {
    auto it = find_by_type(classes, type);
    if(it == classes.end()) { return nullptr; }
    return *it;
}
