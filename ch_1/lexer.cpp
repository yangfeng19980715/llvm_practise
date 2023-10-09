#include "headers.hpp"

// The lexer returns tokens [0-255] if it is an unknown character, otherwise one
// of these for known things.
enum Token {
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
static int gettok() {
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

int main() {

  while (true) {

    switch (gettok()) {
    case Token::tok_def:
      std::cout << "tok_def" << std::endl;
      break;
    case Token::tok_eof:
      std::cout << "tok_eof" << std::endl;
      std::cout << "finished parse\n";
      exit(0);
    case Token::tok_extern:
      std::cout << "tok_extern" << std::endl;
      break;
    case Token::tok_identifier:
      std::cout << "tok_identifier" << std::endl;
      break;
    case Token::tok_number:
      std::cout << "tok_number" << std::endl;
      break;
    default:
      std::cout << "other" << std::endl;
      break;
    }
  }
}
