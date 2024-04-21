#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

struct OpeningBracket {};

struct ClosingBracket {};

struct Comma {};


struct Number {
  int value;
};

struct UnknownToken {
  std::string value;
};

struct Plus {};

struct Minus {};

struct Multiply {};

struct Modulo {};

struct Divide {};

struct MaxToken {};

struct SqrToken {};

struct MinToken {};

struct AbsToken {};

using Token = std::variant<OpeningBracket, ClosingBracket, Comma, Number, UnknownToken, MinToken, AbsToken, Plus, Minus,
                           Multiply, Modulo, Divide, MaxToken, SqrToken>;

// 1234

const std::unordered_map<unsigned char, Token> kSymbol2Token {
    {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'/', Divide{}}, {'%', Modulo{}},
    {'(', OpeningBracket{}}, {')', ClosingBracket{}}, {',', Comma{}}};

const std::unordered_map<std::string, Token> kSymbol3Token {
    {"abs", AbsToken{}}, {"sqr", SqrToken{}}, {"min", MinToken{}}, {"max", MaxToken{}}};

int ToDigit(unsigned char symbol) {
  return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
  int value = 0;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while (std::isdigit(symbol)) {
    value = value * 10 + ToDigit(symbol);
    if (pos == input.size() - 1) {
      break;
    }
    symbol = static_cast<unsigned char>(input[++pos]);
  }
  return Number{value};
}

Token ParserName(const std::string &input, size_t &pos) {
  std::string str;
  auto symbol = static_cast<unsigned char>(input[pos]);
  while(std::isalpha(symbol)) {
    str.push_back(tolower(symbol)); // !
    if (pos == input.size() - 1) {
      break;
    }
    symbol = static_cast<unsigned char>(input[++pos]);
  }
  if (auto iter = kSymbol3Token.find(str); iter != kSymbol3Token.end()) {
    return iter->second;
  }
  return UnknownToken {str};
}

/*Возврат nknownToken, если в строке присутсвуют другие символы,
пример ^36 * 2.
*/
Token ParserToken(const std::string &input, size_t &pos) {
  std::string str;
  auto symbol = static_cast<unsigned char>(input[pos++]);
  str.push_back(symbol);

  return UnknownToken {str};
}

std::vector<Token> Tokenize(const std::string& input) {
  std::vector<Token> tokens;
  const size_t size = input.size();
  size_t pos = 0;
  while (pos < size) {
    const auto symbol = static_cast<unsigned char>(input[pos]);
    if (std::isspace(symbol)) {
      ++pos;
    } else if (std::isdigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
    } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
      tokens.emplace_back(it->second);
      ++pos;
    } else if (std::isalpha(symbol)) {
      tokens.emplace_back(ParserName(input, pos));
    } else {
      tokens.emplace_back(ParserToken(input, pos));
    }
  }
  return tokens;
}
