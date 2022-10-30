#pragma once

#include <string>
#include <memory>

namespace cx {

class rc {
        mutable std::size_t count{0};
    public:
        virtual ~rc() {}
        void add_ref() { ++count; }
        void remove_ref() { if (--count == 0) delete this; }
};

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

using std::move;

}