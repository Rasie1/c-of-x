#pragma once

#include <string>
#include <memory>
#include <cstring>

namespace cx {

class rc {
        mutable std::size_t count{0};
    public:
        virtual ~rc() {}
        void add_ref() { ++count; }
        void remove_ref() { if (--count == 0) delete this; }
};

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

template <typename Value, typename ...Patterns>
auto match(Value&& value, Patterns&&... patterns) {
    return std::visit(overload{
        std::forward<Patterns>(patterns)...
    }, std::forward<Value>(value));
}

inline std::string unescape(const std::string& s) {
    std::string res;
    auto it = s.begin();
    while (it != s.end()) {
        char c = *it++;
        if (c == '\\' && it != s.end()) {
            switch (*it++) {
            case '\\': c = '\\'; break;
            case 'n': c = '\n'; break;
            case 't': c = '\t'; break;
            case 'a': c = '\a'; break;
            case 'b': c = '\b'; break;
            case 'f': c = '\f'; break;
            case 'r': c = '\r'; break;
            case 'v': c = '\v'; break;
            case '\'': c = '\''; break;
            case '\"': c = '\"'; break;
            case '?': c = '\?'; break;
            default: 
                continue;
            }
        }
        res += c;
    }

    return res;
}

inline void unescape_char(unsigned char u, char *buffer, size_t buflen)
{
    if (buflen < 2)
        *buffer = '\0';
    else if (isprint(u) && u != '\'' && u != '\"' && u != '\\' && u != '\?')
        sprintf(buffer, "%c", u);
    else if (buflen < 3)
        *buffer = '\0';
    else {
        switch (u) {
        case '\a':  strcpy(buffer, "\\a"); break;
        case '\b':  strcpy(buffer, "\\b"); break;
        case '\f':  strcpy(buffer, "\\f"); break;
        case '\n':  strcpy(buffer, "\\n"); break;
        case '\r':  strcpy(buffer, "\\r"); break;
        case '\t':  strcpy(buffer, "\\t"); break;
        case '\v':  strcpy(buffer, "\\v"); break;
        case '\\':  strcpy(buffer, "\\\\"); break;
        case '\'':  strcpy(buffer, "\\'"); break;
        case '\"':  strcpy(buffer, "\\\""); break;
        case '\?':  strcpy(buffer, "\\\?"); break;
        default:
            if (buflen < 5)
                *buffer = '\0';
            else
                sprintf(buffer, "\\%03o", u);
            break;
        }
    }
}

inline void unescape(const char *str, char *buffer, size_t buflen)
{
    unsigned char u;
    size_t len;

    while ((u = (unsigned char)*str++) != '\0') {
        unescape_char(u, buffer, buflen);
        if ((len = strlen(buffer)) == 0)
            return;
        buffer += len;
        buflen -= len;
    }
    *buffer = '\0';
}

struct escaped {
    const char* str;
    friend inline std::ostream& operator<<(std::ostream& os, const escaped& e) {
        for (const char* char_p = e.str; *char_p != '\0'; char_p++) {
            switch (*char_p) {
                case '\a':  os << "\\a"; break;
                case '\b':  os << "\\b"; break;
                case '\f':  os << "\\f"; break;
                case '\n':  os << "\\n"; break;
                case '\r':  os << "\\r"; break;
                case '\t':  os << "\\t"; break;
                case '\v':  os << "\\v"; break;
                case '\\':  os << "\\\\"; break;
                case '\'':  os << "\\'"; break;
                case '\"':  os << "\\\""; break;
                case '\?':  os << "\\\?"; break;
                default: os << *char_p;
            }
        }
        return os;
    }
};

#ifndef defer
struct defer_dummy {};
template <class F> struct deferrer { F f; ~deferrer() { f(); } };
template <class F> deferrer<F> operator*(defer_dummy, F f) { return {f}; }
#define DEFER_HELPER1(LINE) zz_defer##LINE
#define DEFER_HELPER2(LINE) DEFER_HELPER1(LINE)
#define defer auto DEFER_HELPER2(__LINE__) = defer_dummy{} *[&]()
#endif // defer

auto copy(const auto& x) {
    auto xCopy = x;
    return xCopy;
}

template<typename T>
class stash {
    T  oldValue;
    T& storedVariable;
public:
    stash(T& variable, T&& newValue): storedVariable(variable) {
        this->oldValue = variable;
        variable = newValue;
    }

    ~stash() {
        this->storedVariable = oldValue;
    }
};

}