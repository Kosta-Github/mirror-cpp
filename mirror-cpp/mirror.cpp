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

void mirror::class_registry::register_class(
    class_ptr c
) {
    assert(c);
    assert(!find_by_name(c->name));

    // use binary/logarithmic search based on the name
    auto it = std::lower_bound(
        m_classes.begin(), m_classes.end(), c->name,
        [](class_ptr const& c, std::string const& n) {
            return (c->name < n);
        }
    );

    m_classes.emplace(it, std::move(c));
}

mirror::class_ptr mirror::class_registry::find_by_name(
    std::string const& name
) const {
    // use binary/logarithmic search based on the name
    auto it = std::lower_bound(
        m_classes.begin(), m_classes.end(), name,
        [](class_ptr const& c, std::string const& n) {
            return (c->name < n);
        }
    );

    if(it == m_classes.end()) { return nullptr; }
    if((*it)->name != name) { return nullptr; }
    return *it;
}

mirror::class_ptr mirror::class_registry::find_by_type(
    std::type_info const& type
) const {
    // use linear search based on the type
    auto it = std::find_if(
        m_classes.begin(), m_classes.end(),
        [&](class_ptr const& c) {
            return (c->type == type);
        }
    );
    
    if(it == m_classes.end()) { return nullptr; }
    return *it;
}
