export module interpreter;
import <map>;
import <string>;
import latex_unicode;
export class LanguagePlugin {
 public:
  Interpreter();
  virtual std::string interpret(const std::string &) = 0;
};
