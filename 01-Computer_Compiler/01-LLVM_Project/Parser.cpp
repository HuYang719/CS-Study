#include <iostream>  
#include <cstdio>  
#include <fstream>  
#include <algorithm>  
#include <cmath>  
#include <deque>  
#include <vector>  
#include <queue>  
#include <string>  
#include <cstring>  
#include <map>  
#include <stack>  
#include <set>

// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
	virtual ~ExprAST() {}
};

/// NumberExprAst - Expression class for numberic literals like "1.0".
class NumberExprAst : public ExprAST{
	double Val;

public:
	NumberExprAst(double Val) : Val(Val) {}
};

// VariableExprAST - Expression class for referencing a variable, like "a"
class VariableExprAST : public ExprAST {
	std::string Name;

public:
	VariableExprAST(const std::string &Name) : Name(Name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
	char Op;
	std::unique_ptr<ExprAST> LHS, RHS;

public:
	BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, std::unique_ptr<ExprAST> RHS) : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {} 
};

// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
	std::string Callee;
	std::vector<std::unique_ptr<ExprAST>> Args;

public:
	CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args): Callee(Callee), Args(std::move(Args)) {}
};


// PrototypeAST - This class represents the "prototype" for a function, which captures its name, and its argument names
// (thus implicitly the number of arguments the function takes).
class PrototypeAST {
	std::string Name;
	std::vector<std::string> Args;

public:
	PrototypeAST(const std::string &name, std::vector<std::string> Args) : Name(name), Args(std::move(Args)) {}

	const std::string &getName() const {return Name;}
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
	std::unique_ptr<PrototypeAST> Proto;
	std::unique_ptr<ExprAST> Body;

public:
	FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body) : Proto(std::move(Proto)), Body(std::move(Body)){}
};

// CurTok/getNextToken - Provide a simple token buffer. CurTok is the current token the parser is lokking at.
// getNextToken reads another token from the lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() {
	return CurTok = gettok();
}

// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
	fprintf(stderr, "LogError: %s\n", Str);
	return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
	LogError(Str);
	return nullptr;
}

// numberexpr :: = number
// 返回的是unique_ptr<ExprAST>结构
static std::unique_ptr<ExprAST> ParseNumberExpr() {
	auto Result = std::make_unique<NumberExprAST>(NumVal);
	getNextToken();
	return std::move(Result);
}

// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
	getNextToken(); //eat (.
	auto V = ParseExpression();
	if(!V)
		return nullptr;
	// the last eat
	if (CurTok != ')')
		return LogError("expected ')");
	getNextToken(); // eat ).
	return V;
}

/// identifierexpr
/// ::=identifier
/// ::=identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
	std::string IdName = IdentifierStr;

	getNextToken();

	std::vector<std::unique_ptr<ExprAST>> Args;
	if (CurTok != ')') {
		while (1) {
			if (auto Arg = ParseExpression())
				Args.push_back(std::move(Arg));
			else
				return nullptr;

			if (CurTok == ')')
				break;

			if (CurTok != ',')
				return LogError("Expected ')' or ',' in argument list");
			getNextToken();
		}
	}

	getNextToken();

	return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// Determine what sort of expression
/// primary
/// :: = identifierexpr
/// :: = numberexpr
/// :: = parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
	switch (CurTok) {
		default:
			return LogError("unknown token when expecting an expression");
		case tok_identifier:
			return ParseIdentifierExpr();
		case tok_number:
			return ParseNumberExpr();
		case '(':
			return ParseParenExpr();
	}
}

/// Binary Expression Parsing
/// BinopPrecedence - This holds the precedence for 
/// each binary operator that is defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
	if (!isascii(CurTok))
		return -1;

	// Make sure it's a declared binop.
	int TokPrec = BinopPrecedence[CurTok];
	if (TokPrec <= 0) return -1;
	return TokPrec;
}

/// expression
/// ::= primary binoprhs
/// binoprhs allow empty
static std::unique_ptr<ExprAST> ParseExpression() {
	auto LHS = ParsePrimary();
	if (!LHS)
		return nullptr

	return ParseBinOpRHS(0, std::move(LHS));
}

/// binoprhs
/// ::= ('+' primary )*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
	// if this is a binop. find its precedence.
	while (1) {
		int TokPrec = GetTokPrecedence();

		// If this is a binop that binds at least as tightly as the current binop,
		// consume it, otherwise we are done.
		if (TokPrec < ExprPrec)
			return LHS;

		// OK, we know this is a binop.
		int BinOp = CurTok;
		getNextToken(); // eat binop

		// Parse the primary expression after the binary operator.
		auto RHS = ParsePrimary();
		if (!RHS)
			return nullptr;

		// If BinOp binds less tightly with RHS than the operator after RHS,
		// let the pending operator take RHS as its LHS.
		int NextPrec = GetTokPrecedence();
    	if (TokPrec < NextPrec) {
      		RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
     		if (!RHS) {
        		return nullptr;
     		}
    	}

    	// Merge LHS/RHS.
    	LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
	} // loop around to the top of the while loop
}

int main() {
	// Install standard binary operators.
	// 1 is lowest precedence.
	BinopPrecedence['<'] = 10;
	BinopPrecedence['+'] = 20;
	BinopPrecedence['-'] = 20;
	BinopPrecedence['*'] = 40; // highest
}




