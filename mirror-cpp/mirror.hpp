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

#include <string>
#include <vector>



#include <iostream>




 // disable warning: class 'ABC' needs to have dll-interface to be used by clients of struct 'XYZ'
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable: 4251)
#endif // defined(_MSC_VER)

namespace mirror {

    struct MIRROR_API name_type_info {
        name_type_info(std::string n, std::type_info const& t);

        std::string const name;
        std::type_info const& type;

        std::string to_string(int indent = 0) const;
    };
    typedef std::shared_ptr<name_type_info> name_type_ptr;

    struct MIRROR_API property_info : name_type_info {
        inline property_info(std::string n, std::type_info const& t) : name_type_info(std::move(n), t) { }

        bool read_only;

        std::string to_string(int indent = 0) const;
    };
    typedef std::shared_ptr<property_info> property_ptr;

    template<typename T>
    inline property_ptr make_property(std::string name) {
        auto res = std::make_shared<property_info>(std::move(name), typeid(T));
        res->read_only = std::is_const<T>::value;
        return res;
    }

    struct MIRROR_API class_info : name_type_info {
        inline class_info(std::string n, std::type_info const& t) : name_type_info(std::move(n), t) { }

        std::shared_ptr<class_info> base_class;

        template<typename T>
        inline void add_property(std::string name) { add_property(make_property<T>(std::move(name))); }
        void add_property(property_ptr p);
        property_ptr find_property_by_name(std::string const& name, bool search_base = false) const;
        std::vector<property_ptr> properties;

        std::string to_string(int indent = 0) const;
    };
    typedef std::shared_ptr<class_info> class_ptr;

    template<typename T>
    inline class_ptr make_class(std::string name, class_ptr base_class = nullptr) {
        auto res = std::make_shared<class_info>(std::move(name), typeid(T));
        res->base_class = base_class;
        return res;
    }

    struct MIRROR_API class_registry {
        void add_class(class_ptr c);

        class_ptr find_class_by_name(std::string const& name) const;

        template<typename T>
        inline class_ptr find_class_by_type() const { return find_class_by_type(typeid(T)); }
        class_ptr find_class_by_type(std::type_info const& type) const;

        std::vector<class_ptr> classes;
    };

} // namespace mirror

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif // defined(_MSC_VER)
