export module mde:mde_model;
import <fstream>;
import <iostream>;
import <vector>;
import <memory>;
import <string>;
import action;
import model;
import mode;

export class MarkdownEditor : public Model {
 public:
  MarkdownEditor(int argc, char **(&argv), Mode mode = NORMAL);
  Mode getMode() const { return mode; }
  const int &getOut() const { return out; }
  const std::string &getcmdstr() const { return cmdstr; }
  const char *getActiveFileE() const { return filenames[activeFileE]; }
  const char *getActiveFileP() const { return filenames[activeFileP]; }
  const Command &getcmd() const { return cmd; }
  bool write() const { return cmd == WRITE || cmd == WRITE_QUIT; }
  void run();

 private:
  Command cmd;
  std::string cmdstr;
  int out = 0;
  Mode mode;
  std::vector<const char*> filenames;
  // std::vector<std::unique_ptr<std::fstream>> files;
  int activeFileE, activeFileP;
};
