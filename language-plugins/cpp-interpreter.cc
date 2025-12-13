#include "language-plugin-api.h"
import <iostream>;
import <string>;
import <regex>;
import <map>;
import <iterator>;
import <unordered_set>;

enum Tag {
  unknown,
  keyword,
  control,
  number,
  string,
  identifier,
  comment,
  directive,
  oper,
  orparen,
  crparen,
  ocparen,
  ccparen,
  osparen,
  csparen,
};

std::map<Tag, std::string> map {
  {keyword,"keyword" },
  {number,  "number"},
  {control, "control"},
  {string,  "string"},
  {identifier,  "identifier"},
  {comment,  "comment"},
  {directive,  "directive"},
  {oper,  "oper"},
  {orparen,  "orparen"},
  {crparen,  "crparen"},
  {ocparen,  "ocparen"},
  {ccparen,  "ccparen"},
  {osparen,  "osparen"},
  {csparen,  "cspare"},
  {comment, "comment"},
};

struct Token {
  Tag tag;
  std::string data;
};

class Lexer {
  const std::unordered_set<std::string> controls = {
    "break",
    "case",
    "catch",
    "continue",
    "default",
    "delete",
    "do",
    "else",
    "for",
    "goto",
    "if",
    "new",
    "return",
    "switch",
    "throw",
    "try",
    "while",
  };
  const std::unordered_set<std::string> keywords = {
    "alignas",
    "alignof",
    "and",
    "and_eq",
    "asm",
    "atomic_cancel",
    "atomic_commit",
    "atomic_noexcept",
    "auto",
    "bitand",
    "bitor",
    "bool",
    "char",
    "char8_t",
    "char16_t",
    "char32_t",
    "class (1)",
    "compl",
    "concept",
    "const",
    "consteval",
    "constexpr",
    "constinit",
    "const_cast",
    "contract_assert",
    "co_await",
    "co_return",
    "co_yield",
    "decltype",
    "double",
    "dynamic_cast",
    "enum",
    "explicit",
    "export",
    "extern",
    "false",
    "float",
    "friend",
    "inline",
    "int",
    "long",
    "mutable",
    "namespace",
    "noexcept",
    "not",
    "not_eq",
    "nullptr",
    "operator",
    "or",
    "or_eq",
    "private",
    "protected",
    "public",
    "reflexpr",
    "register",
    "reinterpret_cast",
    "requires",
    "short",
    "signed",
    "sizeof",
    "static",
    "static_assert",
    "static_cast",
    "struct",
    "synchronized",
    "template",
    "this",
    "thread_local",
    "true",
    "typedef",
    "typeid",
    "typename",
    "union",
    "unsigned",
    "using",
    "virtual",
    "void",
    "volatile",
    "wchar_t",
    "xor",
    "xor_eq",
  };
  bool isop(const char &c) {
    switch (c) {
      case '+':
      case '-':
      case '/':
      case '*':
      case '&':
      case '%':
      case '|':
      case '=':
      case '<':
      case '>':
      case '^':
      case '!':
      case '~':
      case '?':
      case ',':
      case '.':
      case ':':
      case ';':
        return true;
      default:
        return false;
    }
  }
  using str_it = decltype(std::string().cbegin());
  void skipignores(str_it &it, str_it end) {
    while (it != end && std::isspace(*it)) ++it;
  }
  void advance_num(str_it &it, str_it end) {
    while (it != end && std::isdigit(*it)) ++it;
  }
  void advance_str(str_it &it, str_it end) {
    while (it != end && *it != '\"') ++it;
    if (it != end) ++it;
  }
  void advance_op(str_it &it, str_it end) {
    while (it != end && isop(*it)) ++it;
  }
  void advance_id(str_it &it, str_it end) {
    while (it != end && std::isalnum(*it)) ++it;
  }
  void advance_comment(str_it &it, str_it end) {
    while (it != end && *it != '\n') ++it;
  }

 public:
  std::vector<Token> tokens;

  void print() {
    for (const auto &n:tokens) std::cout << n.data << ", " << map[n.tag] << '\n';
  }
  void lex(const std::string &s) {
    const auto end = s.end();
    str_it start_it;
    std::string tmpstr;
    char c;
    for (auto it = s.begin(); it != end;) {
      c = *it;
      start_it = it;
      Tag tag;
      switch (c) {
        case '(': tag = orparen; ++it; break;
        case ')': tag = crparen; ++it; break;
        case '[': tag = osparen; ++it; break;
        case ']': tag = csparen; ++it; break;
        case '{': tag = ocparen; ++it; break;
        case '}': tag = ccparen; ++it; break;
        case '\"': tag = string; advance_str(++it, end); break;
        case '#': tag = directive; advance_id(++it, end); break;
        default:
          if (std::isdigit(c)) {
            tag = number;
            advance_num(it, end);
          }
          else if (isop(c)) {
            if (c == '/') {
              auto next = it; ++next;
              if (next != end && *next == '/') {
                tag = comment;
                advance_comment(it, end);
              } else {
                tag = oper;
                advance_op(it, end);
              }
            } else {
              tag = oper;
              advance_op(it, end);
            }
          } else if (std::isdigit(c)) {
            tag = number;
            advance_num(it, end);
          } else if (std::isalpha(c)) {
            advance_id(it, end);
            tmpstr = std::string(start_it, it);
            if (keywords.contains(tmpstr)) tag = keyword;
            else if (controls.contains(tmpstr)) tag = control;
            else tag = identifier;
          } else ++it;
      }
      skipignores(it, end);
      tokens.emplace_back(tag, std::string(start_it, it));
    }
  }
};

class cppInterpreter : public LanguagePlugin {
  public:
  cppInterpreter(): LanguagePlugin{std::string("cpp")} {}
  std::string interpret(const std::string &s) const override {
    std::vector<char> parenstack;
    Lexer l;
    l.lex(s);
    const auto &tokens = l.tokens;
    std::string res;
    std::string data;
    Tag tag;
    for (const auto &token:tokens) {
      tag = token.tag;
      data = token.data;
      if (data == "(" || data == "[" || data == "{") {
        parenstack.push_back(data[0]);
        res += data;
      }
      else if (data == ")") {
        if (parenstack.empty() || parenstack.back() != '(') {
          res += "<RED>"; res += ')'; res += "</RED>";
        } else {
          parenstack.pop_back(); res += ')';
        }
      } else if (data == "]") {
        if (parenstack.empty() || parenstack.back() != '[') {
          res += "<RED>"; res += ']'; res += "</RED>";
        } else {
          parenstack.pop_back(); res += ']';
        }
      } else if (data == "}") {
        if (parenstack.empty() || parenstack.back() != '{') {
          res += "<RED>"; res += '}'; res += "</RED>";
        } else {
          parenstack.pop_back(); res += '}';
        }
      } else {
        switch (tag) {
          case keyword:
            res += "<BLUE>"; res += data; res += "</BLUE>";
            break;
          case string:
            res += "<YELLOW>"; res += data; res += "</YELLOW>";
            break;
          case comment:
            res += "<GREEN>"; res += data; res += "</GREEN>";
            break;
          case number:
            res += "<CYAN>"; res += data; res += "</CYAN>";
            break;
          case directive:
          case control:
            res += "<MAGENTA>"; res += data; res += "</MAGENTA>";
            break;
          default:
            res += data;            
        }
      }
    }
    return res;
  }
};

extern "C" LanguagePlugin *create_interp() { return new cppInterpreter; }
extern "C" void destroy_interp(LanguagePlugin *i) { delete i; }
