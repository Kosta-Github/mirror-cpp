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

#include <memory>
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
        virtual ~name_type_info() { }

        name_type_info(std::string n, std::type_info const& t);

        std::string const name;
        std::type_info const& type;

        virtual std::string to_string(int indent = 0) const;
    };
    typedef std::shared_ptr<name_type_info> name_type_ptr;

    struct MIRROR_API property_info : name_type_info {
        inline property_info(std::string n, std::type_info const& t) : name_type_info(std::move(n), t) { }

        bool read_only;

        virtual std::string to_string(int indent = 0) const override;
    };
    typedef std::shared_ptr<property_info> property_ptr;

    template<typename T>
    inline property_ptr make_property(std::string name) {
        auto res = std::make_shared<property_info>(std::move(name), typeid(T));
        res->read_only = std::is_const<T>::value;
        return res;
    }

    struct MIRROR_API method_info : name_type_info {
        inline method_info(std::string n, std::type_info const& t) : name_type_info(std::move(n), t) { }

        virtual std::string to_string(int indent = 0) const override;
    };
    typedef std::shared_ptr<method_info> method_ptr;

    template<typename T>
    inline method_ptr make_method(std::string name) {
        auto res = std::make_shared<method_info>(std::move(name), typeid(T));
        return res;
    }
    
    struct MIRROR_API class_info_base : name_type_info {
        inline class_info_base(std::string n, std::type_info const& t) : name_type_info(std::move(n), t) { }

        std::shared_ptr<class_info_base> base_class;

        template<typename T>
        inline void add_property_impl(std::string name) { add_property_impl(make_property<T>(std::move(name))); }
        void add_property_impl(property_ptr p);
        property_ptr find_property_by_name(std::string const& name, bool search_base = false) const;
        std::vector<property_ptr> properties;

        template<typename T>
        inline void add_method_impl(std::string name) { methods.emplace_back(make_method<T>(std::move(name))); }
        std::vector<method_ptr> methods;

        virtual std::string to_string(int indent = 0) const override;
    };
    typedef std::shared_ptr<class_info_base> class_base_ptr;

    template<typename T>
    struct class_info : class_info_base {
        inline class_info(std::string n) : class_info_base(std::move(n), typeid(T)) { }

        template<typename MEMBER>
        inline void add_property(std::string name, MEMBER (T::*member)) {
            add_property_impl<MEMBER>(std::move(name));
        }

        template<typename RESULT>
        void add_method(std::string name, RESULT (T::*method)()) {
            add_method_impl<RESULT (T::*)()>(std::move(name));
        }
        template<typename RESULT>
        void add_method(std::string name, RESULT (T::*method)() const) {
            add_method_impl<RESULT (T::*)() const>(std::move(name));
        }

        template<typename RESULT, typename ARG1>
        void add_method(std::string name, RESULT (T::*method)(ARG1)) {
            add_method_impl<RESULT (T::*)(ARG1)>(std::move(name));
        }
        template<typename RESULT, typename ARG1>
        void add_method(std::string name, RESULT (T::*method)(ARG1) const) {
            add_method_impl<RESULT (T::*)(ARG1) const>(std::move(name));
        }

        template<typename RESULT, typename ARG1, typename ARG2>
        void add_method(std::string name, RESULT (T::*method)(ARG1, ARG2)) {
            add_method_impl<RESULT (T::*)(ARG1, ARG2)>(std::move(name));
        }
        template<typename RESULT, typename ARG1, typename ARG2>
        void add_method(std::string name, RESULT (T::*method)(ARG1, ARG2) const) {
            add_method_impl<RESULT (T::*)(ARG1, ARG2) const>(std::move(name));
        }

    };

    template<typename T>
    inline std::shared_ptr<class_info<T>> make_class(std::string name, class_base_ptr base_class = nullptr) {
        auto res = std::make_shared<class_info<T>>(std::move(name));
        res->base_class = std::move(base_class);
        return res;
    }

    struct MIRROR_API class_registry {
        void add_class(class_base_ptr c);

        class_base_ptr find_class_by_name(std::string const& name) const;

        template<typename T>
        inline class_base_ptr find_class_by_type() const { return find_class_by_type(typeid(T)); }
        class_base_ptr find_class_by_type(std::type_info const& type) const;

        std::vector<class_base_ptr> classes;
    };

} // namespace mirror

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif // defined(_MSC_VER)
