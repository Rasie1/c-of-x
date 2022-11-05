#pragma once
#include <tao/pegtl.hpp>

namespace cx::parser {

struct str_and : TAO_PEGTL_STRING("and") {};
struct str_break : TAO_PEGTL_STRING("break") {};
struct str_do : TAO_PEGTL_STRING("do") {};
struct str_else : TAO_PEGTL_STRING("else") {};
struct str_elseif : TAO_PEGTL_STRING("elseif") {};
struct str_end : TAO_PEGTL_STRING("end") {};
struct str_false : TAO_PEGTL_STRING("false") {};
struct str_for : TAO_PEGTL_STRING("for") {};
struct str_function : TAO_PEGTL_STRING("function") {};
struct str_goto : TAO_PEGTL_STRING("goto") {};
struct str_if : TAO_PEGTL_STRING("if") {};
struct str_in : TAO_PEGTL_STRING("in") {};
struct str_local : TAO_PEGTL_STRING("local") {};
struct str_nil : TAO_PEGTL_STRING("nil") {};
struct str_not : TAO_PEGTL_STRING("not") {};
struct str_or : TAO_PEGTL_STRING("or") {};
struct str_repeat : TAO_PEGTL_STRING("repeat") {};
struct str_return : TAO_PEGTL_STRING("return") {};
struct str_then : TAO_PEGTL_STRING("then") {};
struct str_true : TAO_PEGTL_STRING("true") {};
struct str_until : TAO_PEGTL_STRING("until") {};
struct str_while : TAO_PEGTL_STRING("while") {};
struct eq : tao::pegtl::one<'='> {};
struct hs : tao::pegtl::one<'#'> {};
struct ba : tao::pegtl::one<'('> {};
struct bz : tao::pegtl::one<')'> {};

struct sa : tao::pegtl::star<tao::pegtl::any> {};
struct s0 : tao::pegtl::star<tao::pegtl::one<' '>> {};
struct s1 : tao::pegtl::plus<tao::pegtl::one<' '>> {};

template<class Key>
struct key : tao::pegtl::seq<Key, tao::pegtl::not_at<tao::pegtl::identifier_other>> {};

struct sor_keyword : tao::pegtl::sor<str_and, str_break, str_do, str_elseif, str_else, str_end, str_false, str_for, str_function, str_goto, str_if, str_in, str_local, str_nil, str_not, str_repeat, str_return, str_then, str_true, str_until, str_while> {};

struct key_and : key<str_and> {};
struct key_break : key<str_break> {};
struct key_do : key<str_do> {};
struct key_else : key<str_else> {};
struct key_elseif : key<str_elseif> {};
struct key_end : key<str_end> {};
struct key_false : key<str_false> {};
struct key_for : key<str_for> {};
struct key_function : key<str_function> {};
struct key_goto : key<str_goto> {};
struct key_if : key<str_if> {};
struct key_in : key<str_in> {};
struct key_local : key<str_local> {};
struct key_nil : key<str_nil> {};
struct key_not : key<str_not> {};
struct key_or : key<str_or> {};
struct key_repeat : key<str_repeat> {};
struct key_return : key<str_return> {};
struct key_then : key<str_then> {};
struct key_true : key<str_true> {};
struct key_until : key<str_until> {};
struct key_while : key<str_while> {};

struct keyword : key<sor_keyword> {};

// literals

// struct identifier : tao::pegtl::seq<tao::pegtl::not_at<keyword>, tao::pegtl::identifier> {};
struct identifier : tao::pegtl::seq<tao::pegtl::identifier> {};

struct single : tao::pegtl::one<'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '"', '\'', '0', '\n'> {};
struct spaces : tao::pegtl::seq<tao::pegtl::one<'z'>, tao::pegtl::star<tao::pegtl::space>> {};
struct hexbyte : tao::pegtl::if_must<tao::pegtl::one<'x'>, tao::pegtl::xdigit, tao::pegtl::xdigit> {};
struct decbyte : tao::pegtl::if_must<tao::pegtl::digit, tao::pegtl::rep_opt<2, tao::pegtl::digit>> {};
struct unichar : tao::pegtl::if_must<tao::pegtl::one<'u'>, tao::pegtl::one<'{'>, tao::pegtl::plus<tao::pegtl::xdigit>, tao::pegtl::one<'}'>> {};
struct escaped : tao::pegtl::if_must<tao::pegtl::one<'\\'>, tao::pegtl::sor<hexbyte, decbyte, unichar, single, spaces>> {};
struct regular : tao::pegtl::not_one<'\r', '\n'> {};
struct character : tao::pegtl::sor<escaped, regular> {};

template<char Q>
struct short_string : tao::pegtl::if_must<tao::pegtl::one<Q>, tao::pegtl::until<tao::pegtl::one<Q>, character>> {};
// struct long_string : tao::pegtl::raw_string<'[', '=', ']'> {};
// struct literal_string : tao::pegtl::sor<short_string<'"'>, short_string<'\''>, long_string> {};
struct literal_string : tao::pegtl::sor<short_string<'"'>, short_string<'\''>> {};

template<class E>
struct exponent : tao::pegtl::opt_must<E, tao::pegtl::opt<tao::pegtl::one<'+', '-'>>, tao::pegtl::plus<tao::pegtl::digit>> {};

template<class D, class E>
struct numeral_three : tao::pegtl::seq<tao::pegtl::if_must<tao::pegtl::one<'.'>, tao::pegtl::plus<D>>, exponent<E>> {};
template<class D, class E>
struct numeral_two : tao::pegtl::seq<tao::pegtl::plus<D>, tao::pegtl::opt<tao::pegtl::one<'.'>, tao::pegtl::star<D>>, exponent<E>> {};
template<class D, class E>
struct numeral_one : tao::pegtl::sor<numeral_two<D, E>, numeral_three<D, E>> {};

struct decimal : numeral_one<tao::pegtl::digit, tao::pegtl::one<'e', 'E'>> {};
struct hexadecimal : tao::pegtl::if_must<tao::pegtl::istring<'0', 'x'>, numeral_one<tao::pegtl::xdigit, tao::pegtl::one<'p', 'P'>>> {};
struct numeral : tao::pegtl::sor<hexadecimal, decimal> {};
struct digits : tao::pegtl::plus<tao::pegtl::digit> {};

struct indent : tao::pegtl::star<tao::pegtl::one<' '>> {};
struct inc_indent : tao::pegtl::star<tao::pegtl::one<' '>> {};
struct dec_indent;
struct more_indent;
struct less_indent;
struct eq_indent;
struct any;
struct trailer : tao::pegtl::must<s0, tao::pegtl::opt<hs, tao::pegtl::until<tao::pegtl::at<tao::pegtl::eolf>>>, tao::pegtl::eolf> {};
struct peek_trailer : tao::pegtl::seq<s0, tao::pegtl::opt<hs, tao::pegtl::until<tao::pegtl::at<tao::pegtl::eolf>>>, tao::pegtl::eolf> {};


// expressions
struct expression;

// struct short_comment : tao::pegtl::until<tao::pegtl::eolf> {};
// struct comment : tao::pegtl::disable<tao::pegtl::two<'/'>, tao::pegtl::sor<long_string, short_comment>> {};

// struct sep : tao::pegtl::sor<tao::pegtl::ascii::space, comment> {};
struct sep : tao::pegtl::ascii::blank {};
struct seps : tao::pegtl::star<sep> {};

struct bracket_expr : tao::pegtl::if_must<tao::pegtl::one<'('>, seps, expression, seps, tao::pegtl::one<')'>> {};

template<char O, char... N>
struct op_one : tao::pegtl::seq<tao::pegtl::one<O>, tao::pegtl::at<tao::pegtl::not_one<N...>>> {};
template<char O, char P, char... N>
struct op_two : tao::pegtl::seq<tao::pegtl::string<O, P>, tao::pegtl::at<tao::pegtl::not_one<N...>>> {};
template<class S, class O>
struct left_assoc : tao::pegtl::seq<S, seps, tao::pegtl::star_must<O, seps, S, seps>> {};
template<class S, class O>
struct right_assoc : tao::pegtl::seq<S, seps, tao::pegtl::opt_must<O, seps, right_assoc<S, O>>> {};
struct unary_operators : tao::pegtl::sor<tao::pegtl::one<'-'>,
                                            tao::pegtl::one<'#'>,
                                            op_one<'~', '='>> {};
struct expr_10;
struct expr_0;

struct expr_14 : tao::pegtl::sor<bracket_expr, identifier> {};
struct expr_13 : tao::pegtl::sor<
    // key_true, key_false,
    // tao::pegtl::ellipsis,
    // numeral,
    digits,
    literal_string,
    expr_14> {};

struct expr_12 : tao::pegtl::seq<expr_13,
                                    tao::pegtl::star<peek_trailer, inc_indent, more_indent, expression, dec_indent>
                                > {};
struct operation_apply : tao::pegtl::seq<expr_12, tao::pegtl::star<seps, expr_12>> {};
struct expr_11 : tao::pegtl::seq<operation_apply, seps, tao::pegtl::opt<tao::pegtl::one<'^'>, seps, expr_10, seps>> {};
struct unary_apply : tao::pegtl::if_must<unary_operators, seps, expr_10, seps> {};
struct expr_10 : tao::pegtl::sor<unary_apply, expr_11> {};
struct operators_9 : tao::pegtl::sor<tao::pegtl::one<'/'>,
                                     tao::pegtl::one<'*'>,
                                     tao::pegtl::one<'%'>> {};
struct expr_9 : left_assoc<operation_apply, operators_9> {};
struct operators_8 : tao::pegtl::sor<tao::pegtl::one<'+'>,
                                     tao::pegtl::one<'-'>> {};
struct expr_8 : left_assoc<expr_9, operators_8> {};
struct expr_7 : right_assoc<expr_8, op_two<'.', '.', '.'>> {};
struct operators_6 : tao::pegtl::sor<tao::pegtl::two<'<'>,
                                                        tao::pegtl::two<'>'>> {};
struct expr_6 : left_assoc<expr_7, operators_6> {};
struct expr_5 : left_assoc<expr_6, tao::pegtl::one<'&'>> {};
struct expr_4 : left_assoc<expr_5, op_one<'~', '='>> {};
struct expr_3 : left_assoc<expr_4, tao::pegtl::one<'|'>> {};
struct operators_2 : tao::pegtl::sor<tao::pegtl::two<'='>,
                                        tao::pegtl::string<'<', '='>,
                                        tao::pegtl::string<'>', '='>,
                                        op_one<'<', '<'>,
                                        op_one<'>', '>'>,
                                        tao::pegtl::string<'~', '='>> {};
struct expr_2 : left_assoc<expr_3, operators_2> {};
struct operators_1 : tao::pegtl::one<':'> {};
struct expr_1 : left_assoc<expr_8, operators_1> {};
struct operators_0 : tao::pegtl::one<'='> {};
struct expr_0 : left_assoc<expr_1, operators_0> {};
struct expression : expr_0 {};//left_assoc<expr_0, expression> {};


// line structure

struct name : tao::pegtl::plus<tao::pegtl::alpha> {};

struct space_line : tao::pegtl::seq<s1, tao::pegtl::eolf> {};
struct comment_line : tao::pegtl::seq<s0, tao::pegtl::if_must<hs, tao::pegtl::until<tao::pegtl::eolf>>> {};


struct definition : tao::pegtl::if_must<expression, trailer> {};

struct indented : tao::pegtl::must<indent, definition> {};
struct nonempty_line : tao::pegtl::sor<space_line, comment_line, indented> {};

struct empty_line : tao::pegtl::eolf {};

struct line : tao::pegtl::sor<empty_line, nonempty_line> {};

struct grammar : tao::pegtl::until<tao::pegtl::eof, tao::pegtl::must<line>> {};

enum class line_desc
{
    def,
    if_,
    else_,
    let,
    expr
};

struct entry
{
    entry(const std::size_t i, const line_desc t)
        : indent(i),
            type(t)
    {}

    std::size_t indent;
    line_desc type;
};

struct state
{
    std::size_t current_indent = 0;  // Temporary value, the indentation of the current line.
    std::size_t minimum_indent = 0;  // Set to non-zero when the next line needs a greater indent.

    std::vector<entry> stack = {entry(0, line_desc::expr)};  // Follows the nesting of the indented blocks.
};

template<class Rule>
struct action
{};

template<>
struct action<definition>
{
    static void apply0(state& s)
    {
        s.stack.emplace_back(s.current_indent, line_desc::expr);
    }
};

template<>
struct action<indent>
{
    template<class ActionInput>
    static void apply(const ActionInput& in, state& s)
    {
        s.current_indent = in.size();
    }
};
template<>
struct action<inc_indent>
{
    template<class ActionInput>
    static void apply(const ActionInput& in, state& s)
    {
        // auto prev = s.stack.back().indent;
        s.stack.emplace_back(s.current_indent, line_desc::let);
        s.current_indent = in.size();
        // std::cout << "-i------------------ indent " << prev << " <- " << s.current_indent << " : " << in.position().line << ", " << in.position().column << std::endl;
    }
};
template<>
struct action<dec_indent>
{

    template<class ActionInput>
    static void apply(const ActionInput&, state& s)
    {
        // auto prev = s.current_indent;
        s.current_indent = s.stack.back().indent;
        // std::cout << "-d------------------ indent " << prev << " <- " << s.current_indent <<  " : " << in.position().line << ", " << in.position().column << std::endl;
        s.stack.pop_back();
    }
};
struct dec_indent
{
    using rule_t = tao::pegtl::ascii::any::rule_t;
    using subs_t = tao::pegtl::empty_list;
    template<tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template<typename...>
                class Action,
                template<typename...>
                class Control,
                typename ParseInput,
                typename... States>
    static bool match(ParseInput&, state&, States&&...)
    {
        return true;
    }
};
struct any
{
    using rule_t = tao::pegtl::ascii::any::rule_t;
    using subs_t = tao::pegtl::empty_list;
    template<tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template<typename...>
                class Action,
                template<typename...>
                class Control,
                typename ParseInput,
                typename... States>
    static bool match(ParseInput&, state&, States&&...)
    {
        return true;
    }
};

struct more_indent
{
    using rule_t = tao::pegtl::ascii::any::rule_t;
    using subs_t = tao::pegtl::empty_list;

    template<tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template<typename...>
                class Action,
                template<typename...>
                class Control,
                typename ParseInput,
                typename... States>
    static bool match(ParseInput& in, state& s, States&&...)
    {
        // std::cout << "->------------------ " << in.position().column << " " << s.stack.back().indent <<  " : " << in.position().line << ", " << in.position().column << std::endl;

        if (in.position().column > s.stack.back().indent + 1) {
            return true; 
        } else {
            // auto prev = s.current_indent;
            s.current_indent = s.stack.back().indent;
            // std::cout << "-d>----------------- indent " << prev << " <- " << s.current_indent <<  " : " << in.position().line << ", " << in.position().column << std::endl;
            s.stack.pop_back();
            return false;
        }
    }
};    
struct eq_indent
{
    using rule_t = tao::pegtl::ascii::any::rule_t;
    using subs_t = tao::pegtl::empty_list;

    template<tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template<typename...>
                class Action,
                template<typename...>
                class Control,
                typename ParseInput,
                typename... States>
    static bool match(ParseInput& in, state& s, States&&...)
    {
        // std::cout << "->------------------ " << in.position().column << " " << s.stack.back().indent <<  " : " << in.position().line << ", " << in.position().column << std::endl;
        return in.position().column == s.stack.back().indent + 1;
    }
};

struct less_indent
{
    using rule_t = tao::pegtl::ascii::any::rule_t;
    using subs_t = tao::pegtl::empty_list;

    template<tao::pegtl::apply_mode A,
                tao::pegtl::rewind_mode M,
                template<typename...>
                class Action,
                template<typename...>
                class Control,
                typename ParseInput,
                typename... States>
    static bool match(ParseInput& in, state& s, States&&...)
    {
        // std::cout << "-<------------------ " << in.position().column << " " << s.stack.back().indent <<  " : " << in.position().line << ", " << in.position().column << std::endl;
        return in.position().column < s.stack.back().indent + 1;
    }
};

template<>
struct action<grammar>
{
    template<class ActionInput>
    static void apply(const ActionInput& in, state& s)
    {
        if (s.minimum_indent > 0) {
            throw tao::pegtl::parse_error("expected indented block instead of eof", in);
        }
    }
};

struct rearrange : tao::pegtl::parse_tree::apply<rearrange> 
{
    template<class Node, class... States>
    static void transform(std::unique_ptr<Node>& n, States&&... st)
    {
        // if( n->children.size() == 1 ) {
        //     n = std::move( n->children.back() );
        // }
        // else {
        //     n->remove_content();
        //     auto& c = n->children;
        //     auto r = std::move( c.back() );
        //     c.pop_back();
        //     auto o = std::move( c.back() );
        //     c.pop_back();
        //     o->children.emplace_back( std::move( n ) );
        //     o->children.emplace_back( std::move( r ) );
        //     n = std::move( o );
        //     transform( n->children.front(), st... );
        // }
    }
};

}  // namespace cx::parser
