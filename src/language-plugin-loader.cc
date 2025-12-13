module;
#include <dlfcn.h>
#include "language-plugin-api.h"
export module langpluginloader;
import <filesystem>;
import <iostream>;
import <string>;
import <map>;
import <memory>;
namespace fs = std::filesystem;

export class LanguagePluginLoader {
 public:
  LanguagePluginLoader() {
    for (auto& entry : fs::directory_iterator("../language-plugins")) {
      if (entry.path().extension() != ".so") continue;
      void* handle = dlopen(entry.path().c_str(), RTLD_NOW);
      if (!handle) continue;  // skip invalid .so

      auto create = (LanguagePlugin*(*)()) dlsym(handle, "create_interp");
      auto destroy = (void(*)(LanguagePlugin*)) dlsym(handle, "destroy_interp");

      LanguagePlugin* p = create();
      map[p->getname()] = std::unique_ptr<LanguagePlugin>(p);
    }
  }
  const LanguagePlugin &operator[](const std::string &s) const
    { return *(map.at(s)); }
 private:
  std::map<std::string, std::unique_ptr<LanguagePlugin>> map;
};
