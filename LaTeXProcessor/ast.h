#include <string>

/*	Grammar
	eq ::=
		| Implicit {e e} // Implicit Equation i.e. f(x,y,z) = 0
		| Parametric {e e e} // Parametric Curve/Surface
	e ::=
		| Number {n}
		| Variable {v} 
		| Operator {o e e} 
		| Fraction {e e}
		| Parentheses {e}
		| Trig {t e}
		| Root {e e}
		| Log {e e}
		| Abs {e}
	n ::= number
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
		| sinh
		| cosh
		| tanh
*/
typedef union Equation {
	Implicit implicit;
	Parametric parametric;
} Equation;

typedef struct Implicit {
	Expr* e1;
	Expr* e2;
} Implicit;

typedef struct Parametric {
	Expr* x;
	Expr* y;
	Expr* z;
} Parametric;

typedef union Expr {
	Num num;
	Variable var;
	Op op;
	Frac frac;
	Paren paren;
	Trig trig;
	Root root;
	Log log;
	Abs abs;
} Expr;

typedef struct Num {
	double value;
} Num;

typedef struct Var {
	std::string value;
} Variable;

typedef struct Op {
	char op;
	Expr* e1;
	Expr* e2;
} Op;

typedef struct Frac {
	Expr* numerator;
	Expr* denominator;
} Frac;

typedef struct Paren {
	Expr* e;
} Paren;

typedef struct Trig {
	std::string func;
	Expr* e;
} Trig;

typedef struct Root {
	Expr* root;
	Expr* e;
} Root;

typedef struct Log {
	Expr* base;
	Expr* e;
} Log;

typedef struct Abs {
	Expr* e;
};

