#include "headers.hpp"

////////////////////////   lexer

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token
{
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,
};

static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int
gettok()
{
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def")
      return tok_def;

    if (IdentifierStr == "extern")
      return tok_extern;

    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), 0);
    std::cout << "NumStr: " << NumStr << '\n';
    std::cout << "number: " << NumVal << '\n';
    return tok_number;
  }

  // Comment until end of line.
  if (LastChar == '#') {
    do
      LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return gettok();
  }

  // Check for end of file.  Don't eat the EOF.
  if (LastChar == EOF)
    return tok_eof;

  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

////////////////////////

/// ExprAST - Base class for all expression nodes.
class ExprAST
{
public:
  virtual ~ExprAST() = default;
};

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST
{
  double Val;

public:
  NumberExprAST(double Val) : Val(Val)
  { }
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST
{
  std::string Name;

public:
  VariableExprAST(const std::string &Name) : Name(Name)
  { }
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST
{
  char Op;
  std::unique_ptr<ExprAST> LHS, RHS;

public:
  BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                std::unique_ptr<ExprAST> RHS)
      : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS))
  { }
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST
{
  std::string Callee;
  std::vector<std::unique_ptr<ExprAST>> Args;

public:
  CallExprAST(const std::string &Callee,
              std::vector<std::unique_ptr<ExprAST>> Args)
      : Callee(Callee), Args(std::move(Args))
  { }
};

// 这里代表函数签名
/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST
{
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &Name, std::vector<std::string> Args)
      : Name(Name), Args(std::move(Args))
  { }

  const std::string &
  getName() const
  {
    return Name;
  }
};

// 函数签名和函数体 两者
/// FunctionAST - This class represents a function definition itself.
class FunctionAST
{
  std::unique_ptr<PrototypeAST> Proto; // 函数签名
  std::unique_ptr<ExprAST> Body;       // 函数体

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
      : Proto(std::move(Proto)), Body(std::move(Body))
  { }
};

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;

static int
getNextToken()
{
  return CurTok = gettok();
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST>
LogError(const char *Str)
{
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

std::unique_ptr<PrototypeAST>
LogErrorP(const char *Str)
{
  LogError(Str);
  return nullptr;
}

/// numberexpr ::= number
static std::unique_ptr<ExprAST>
ParseNumberExpr()
{
  auto Result = std::make_unique<NumberExprAST>(NumVal);
  getNextToken(); // consume the number
  return std::move(Result);
}

int
main()
{
  std::cout << "<------------------------------------>" << std::endl;
  std::cout << __FILE__ << std::endl;
}