#pragma once

#include "Expr.h"
#include <memory>
#include <sstream>
using std::unique_ptr;

// ExprVisitor is templated as all visitors don't neccesarily return the same
// type Each concrete visitor will provide a type that they return
template <typename T> class ExprVisitor {
    virtual auto operator()(unique_ptr<Binary> &) -> T = 0;
    virtual auto operator()(unique_ptr<Grouping> &) -> T = 0;
    virtual auto operator()(unique_ptr<Literal> &) -> T = 0;
    virtual auto operator()(unique_ptr<Unary> &) -> T = 0;
};

class AstPrinter : ExprVisitor<std::string> {
  public:
    auto print(Expr &expr) -> std::string;
    auto operator()(unique_ptr<Binary> &) -> std::string override;
    auto operator()(unique_ptr<Grouping> &) -> std::string override;
    auto operator()(unique_ptr<Literal> &) -> std::string override;
    auto operator()(unique_ptr<Unary> &) -> std::string override;

  private:
    template <typename... Args>
    auto parenthesize(const std::string &name, Args &...expressions)
        -> std::string {
        std::stringstream builder;
        builder << "(" << name;
        auto loop = [this, &builder](auto &arg) {
            builder << " " << this->print(arg);
        };
        (loop(expressions), ...);
        builder << ")";
        return builder.str();
    }
};
