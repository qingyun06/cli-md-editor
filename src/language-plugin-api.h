#include <string>
class LanguagePlugin {
  const std::string name;
 public:
  LanguagePlugin(std::string name): name{name} {}
  const std::string &getname() const { return name; }
  virtual std::string interpret(const std::string &) const = 0;
};

extern "C" LanguagePlugin* create_interp();
extern "C" void destroy_interp(LanguagePlugin*);
