#pragma once
#include "Token.h"
#include <memory>
#include <utility>
#include <variant>

struct Equality;
struct Comparison;
struct Term;
struct Factor;
struct Unary;
struct Primary;
struct Binary;

using std::unique_ptr;
typedef std::variant<unique_ptr<Equality>, unique_ptr<Comparison>,
                     unique_ptr<Term>, unique_ptr<Factor>, unique_ptr<Unary>,
                     unique_ptr<Primary>, unique_ptr<Binary>>
    Expr;

struct Primary {
    std::variant<Token, Expr> val;
};

struct Unary {
    std::variant<std::pair<Token, unique_ptr<Unary>>, unique_ptr<Primary>> val;
};

struct Factor {
    Unary left_unary;
    std::vector<std::pair<Token, Unary>> rem_unaries;
};

struct Term {
    Factor left_factor;
    std::vector<std::pair<Token, Factor>> rem_factors;
};

struct Comparison {
    Term left_term;
    std::vector<std::pair<Token, Term>> rem_terms;
};

struct Equality {
    Comparison left_compare;
    std::vector<std::pair<Token, Comparison>> rem_compares;
};

struct Binary {
    Expr left_expr;
    Token op;
    Expr right_expr;
};
