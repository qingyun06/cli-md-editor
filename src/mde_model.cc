export module mde:mde_model;
import <fstream>;
import <iostream>;
import <vector>;
import <regex>;
import <memory>;
import <map>;
import <algorithm>;
import <string>;
import action;
import model;
import mode;
import file;
import langpluginloader;

export class MarkdownEditor : public Model {
 public:
  MarkdownEditor(int argc, char **(&argv), Mode mode = NORMAL);
  const Mode &getMode() const { return mode; }
  const int &getOut() const { return out; }
  const std::string &getcmdstr() const { return cmdstr; }
  // const char *getActiveFileE() const { return filenames[activeFileE]; }
  // const char *getActiveFileP() const { return filenames[activeFileP]; }
  const Command &getcmd() const { return cmd; }
  const std::string &getsearchstr() const { return searchstr; }
  const std::string &getyankstr() const { return yankstr; }
  const File &getactivefile() const { return files[activeFileE]; }
  const std::vector<File> &getfiles() const {return files; }
  std::string interpretActiveFile() const;
  void run();

 private:
  int autosaveCounter = 0;
  Mode mode;
  Command cmd;
  std::string searchstr;
  std::string cmdstr;
  std::string yankstr;
  int out = 0;
  // std::vector<const char*> filenames;
  std::vector<File> files;
  unsigned int activeFileE, activeFileP;
  
  void setmode(Mode mode);
  File &activefile() { return files[activeFileE]; }
  // std::map<std::string, std::unique_ptr<Interpreter>> interpreters;
  LanguagePluginLoader l;
  std::map<const char *, const char *> regexmap;
};
