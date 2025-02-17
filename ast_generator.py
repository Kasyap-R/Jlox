includes = ['"Token.h"', "<memory>", "<variant>"]

structs = {
    "Binary": ["Expr left", "Token op", "Expr right"],
    "Unary": ["Token op", "Expr expr"],
    "Literal": ["Token val"],
    "Grouping": ["Expr expr"],
}


def write_ln(file, text=""):
    file.write(text + "\n")


def write_includes(f):
    write_ln(f, "#pragma once")
    for include in includes:
        write_ln(f, "#include " + include)


def write_expr_variant(f):
    variant_type = "std::variant<"
    for i, (expr_type, _) in enumerate(structs.items()):
        variant_type += f"unique_ptr<{expr_type}>"
        if i < len(structs) - 1:
            variant_type += ","
        else:
            variant_type += ">"
    write_ln(f, "using std::unique_ptr;")
    write_ln(f, f"typedef {variant_type} ExprVariant;")


def write_forward_decls(f):
    for exprtype in structs:
        write_ln(f, f"struct {exprtype};")


def write_expr_struct(f):
    write_ln(f, "struct Expr {ExprVariant expr;};")


def write_structs(f):
    for exprtype, fields in structs.items():
        fields_string = ";".join(fields)
        fields_string += ";"
        write_ln(f, f"struct {exprtype}" + "{" + fields_string + "};")


def write_file(filename: str):
    with open(filename, "w") as f:
        write_includes(f)
        write_ln(f)
        write_forward_decls(f)
        write_ln(f)
        write_expr_variant(f)
        write_ln(f)
        write_expr_struct(f)
        write_ln(f)
        write_structs(f)
        write_ln(f)


if __name__ == "__main__":
    write_file("include/Expr.h")
