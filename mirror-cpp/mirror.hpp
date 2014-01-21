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

#pragma once

#include "mirror-cpp.hpp"

#include <cassert>
#include <string>
#include <vector>

 // disable warning: class 'ABC' needs to have dll-interface to be used by clients of struct 'XYZ'
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable: 4251)
#endif // defined(_MSC_VER)

namespace mirror {

    struct class_info {
        inline class_info(std::string n, std::type_info const& t) : name(std::move(n)), type(t) { assert(!name.empty()); }

        std::string name;
        std::type_info const& type;

        std::shared_ptr<class_info> base_class;

        std::string to_string(int indent = 0) const;
    };
    typedef std::shared_ptr<class_info> class_ptr;

    struct MIRROR_API class_registry {
        void register_class(class_ptr c);

        class_ptr find_by_name(std::string const& name) const;
        class_ptr find_by_type(std::type_info const& type) const;

        template<typename T>
        inline class_ptr find_by_type() const { return find_by_type(typeid(T)); }

    private:
        std::vector<class_ptr> m_classes;
    };


    template<typename T>
    inline class_ptr make_class(std::string name, class_ptr base_class = nullptr) {
        auto res = std::make_shared<class_info>(std::move(name), typeid(T));
        res->base_class = base_class;
        return res;
    }

} // namespace mirror

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif // defined(_MSC_VER)
