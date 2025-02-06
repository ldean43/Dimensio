#pragma once
#include <string>
#include <vector>
#include <stack>

/*	Grammar
	eq ::=
		| Implicit {e e} // Implicit Equation i.e. f(x,y,z) = 0
		| Parametric {e e e} // Parametric Curve/Surface
	e ::=
		| Number {n}
		| Variable {v} 
		| Operator {o e e} 
		| Fraction {e e}
        | Sqrt {e e}
        | Log {e e} // Log with arbitrary base
        | Ln {e} // Natural Log
        | Lg {e} // Log base 2
        | Abs {e}
		| Trig {t e}
	n ::= double
	v ::= variable
	o ::= + | - | * | / | ^
	t ::= sin
		| cos
		| tan
		| sec
		| csc
		| cot
		| arcsin
		| arccos
		| arctan
        | arcsec
        | arccsc
        | arccot
		| sinh
		| cosh
		| tanh
*/

struct Expr {
    // base expr struct for inheritance
};

struct Equation {
	// base equation struct for inheritance
}  ;

struct Num : public Expr {
	double value;

    Num(double value) : value(value) {}
};

struct Var : public Expr {
	std::string value;

    Var(std::string value) : value(value) {}
};

struct Op : public Expr {
	char op;
	Expr* e1;
	Expr* e2;

    Op(char op, Expr* e1, Expr* e2) : op(op), e1(e1), e2(e2) {}
};

struct Frac : public Expr {
	Expr* numerator;
	Expr* denominator;

    Frac(Expr* numerator, Expr* denominator) : 
    numerator(numerator), denominator(denominator) {}
};

struct Sqrt : public Expr {
    Expr* root;
    Expr* e;

    Sqrt(Expr* root, Expr* e) : root(root), e(e) {}
};

struct Log : public Expr {
	Expr* base;
	Expr* e;

    Log(Expr* base, Expr* e) : base(base), e(e) {}
};

struct Ln : public Expr {
	Expr* e;

    Ln(Expr* e) : e(e) {}
};

struct Lg : public Expr {
	Expr* e;

    Lg(Expr* e) : e(e) {}
};

struct Abs : public Expr {
	Expr* e;

    Abs(Expr* e) : e(e) {}
};

struct Trig : public Expr {
	std::string func;
	Expr* e;

    Trig(std::string func, Expr* e) : func(func), e(e) {}
};

struct Implicit {
	Expr* e1;
	Expr* e2;

    Implicit(Expr* e1, Expr* e2) : e1(e1), e2(e2) {}
};

struct Parametric {
	Expr* x;
	Expr* y;
	Expr* z;

    Parametric(Expr* x, Expr* y, Expr* z) {}
};

std::vector<std::string> lex(std::string input);