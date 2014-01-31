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
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace mirror {

    struct value {
        typedef std::vector<value>              array_t;
        typedef std::map<std::string, value>    dict_t;

        inline explicit value()                     : m_obj(nullptr),                                           m_type(&typeid(nullptr))                            { }
        inline explicit value(nullptr_t v)          : m_obj(nullptr),                                           m_type(&typeid(nullptr))                            { }
        inline explicit value(bool v)               : m_obj(std::make_shared<bool>(v)),                         m_type(&typeid(bool))                               { }
        inline explicit value(int64_t v)            : m_obj(std::make_shared<int64_t>(v)),                      m_type(&typeid(int64_t))                            { }
        inline explicit value(double v)             : m_obj(std::make_shared<double>(v)),                       m_type(&typeid(double))                             { }
        inline explicit value(char const* v)        : m_obj(v ? std::make_shared<std::string>(v) : nullptr),    m_type(v ? &typeid(std::string) : &typeid(nullptr)) { }
        inline explicit value(std::string v)        : m_obj(std::make_shared<std::string>(std::move(v))),       m_type(&typeid(std::string))                        { }

        template<typename T>
        inline explicit value(std::shared_ptr<T> v) : m_obj(std::move(v)) { m_type = (m_obj ? &typeid(T*) : nullptr); }

        inline static value array() { value res; res.m_obj = std::make_shared<array_t>();   res.m_type = &typeid(array_t);  return res; }
        inline static value dict()  { value res; res.m_obj = std::make_shared<dict_t>();    res.m_type = &typeid(dict_t);   return res; }

        inline bool is_null()       const { return (m_obj  == nullptr);                                                                                     }
        inline bool is_bool()       const { return (m_type == &typeid(bool));                                                                               }
        inline bool is_int()        const { return (m_type == &typeid(int64_t));                                                                            }
        inline bool is_double()     const { return (m_type == &typeid(double));                                                                             }
        inline bool is_string()     const { return (m_type == &typeid(std::string));                                                                        }
        inline bool is_array()      const { return (m_type == &typeid(array_t));                                                                            }
        inline bool is_dict()       const { return (m_type == &typeid(dict_t));                                                                             }
        inline bool is_ptr()        const { return (!is_null() && !is_bool() && !is_int() && !is_double() && !is_string() && !is_array() && !is_dict());    }
        template<typename T>
        inline bool is_ptr_type()   const { return (as_ptr<T>() != nullptr);                                                                                }

        inline array_t& as_array() { assert(is_null() || is_array()); if(is_null()) { m_obj = std::make_shared<array_t>(); m_type = &typeid(array_t); } return *std::static_pointer_cast<array_t>(m_obj); }
        inline dict_t&  as_dict()  { assert(is_null() || is_dict());  if(is_null()) { m_obj = std::make_shared<dict_t>();  m_type = &typeid(dict_t);  } return *std::static_pointer_cast<dict_t>( m_obj); }

        inline array_t const& as_array() const { if(!is_null() && is_array()) { return *std::static_pointer_cast<array_t>(m_obj); } static const array_t a; return a; }
        inline dict_t  const& as_dict()  const { if(!is_null() && is_dict())  { return *std::static_pointer_cast<dict_t>( m_obj); } static const dict_t  d; return d; }

        template<typename T>
        inline std::shared_ptr<T> as_ptr() const { return ((m_type == &typeid(T*)) ? std::static_pointer_cast<T>(m_obj) : nullptr); }

//    private:
        std::shared_ptr<void> m_obj;
        std::type_info const* m_type; // no owning pointer
    };

    typedef std::vector<value> values;

} // namespace mirror
