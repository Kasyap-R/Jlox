/*#include "ExprVisitor.h"*/
/**/
/*auto AstPrinter::print(Expr &expr) -> std::string {*/
/*    return std::visit(*this, expr);*/
/*}*/
/**/
/*auto AstPrinter::operator()(unique_ptr<Binary> &expr) -> std::string {*/
/*    return parenthesize(expr->op.lexeme, expr->left, expr->right);*/
/*}*/
/**/
/*auto AstPrinter::operator()(unique_ptr<Grouping> &expr) -> std::string {*/
/*    return parenthesize("group", expr->expr);*/
/*}*/
/**/
/*auto AstPrinter::operator()(unique_ptr<Literal> &expr) -> std::string {*/
/*    return expr->val.lexeme;*/
/*}*/
/**/
/*auto AstPrinter::operator()(unique_ptr<Unary> &expr) -> std::string {*/
/*    return parenthesize(expr->op.lexeme, expr->expr);*/
/*}*/
