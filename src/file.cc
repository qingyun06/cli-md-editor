module;
#include <cctype>
export module file;
import <fstream>;
import <iostream>;
import <sstream>;
import <vector>;
import <utility>;
import <string>;

export class File {
  struct Operation {
    bool isInsertion;
    size_t oldx, oldy, newx, newy;
    std::string s;
    bool isNewline;
  };
  std::string filename;
  size_t cursorx = 0, cursory = 0, cursormaxx = 0, 
         anchor = 0, viewstart, viewend, cc = 0, wc = 0;
  char searchc;
  bool lastsearchdir; // 0 for left, 1 for right
  std::string rawstr;
  std::vector<std::string> lines;
  std::vector<Operation> undoStack;
  std::vector<Operation> redoStack;
  void shiftviewup() { --viewstart; --viewend; }
  void shiftviewdown() { ++viewstart; ++viewend; }
  void skipwsl();
  void skipwsr();
  unsigned int peekf();
  unsigned int peek2b();
  unsigned int peekb();

 public:
  File(const char *s): filename(s) {
    std::fstream f{s};
    if (f.good()) {
      std::string str;
      while (std::getline(f, str)) {
        rawstr += str; rawstr += '\n';
        lines.push_back(str);
        ++cc;
      }
    } else {
      lines.push_back("");
    }
    if (!isspace(*rawstr.begin())) ++wc;
    for (auto it = rawstr.begin(); it != rawstr.end(); ++it) {
      auto next = it;
      if (isspace(*it)&& ++next != rawstr.end() && !isspace(*next)) ++wc;
      ++cc;
    }
  }

  // GET SET
  const char *getname() const { return filename.c_str(); }
  std::string getdisplayline(int i) const
    { return viewstart + i >= lines.size() ? 
             std::string("~") : lines[viewstart + i]; } 

  std::string getline(size_t i) const { return lines[i]; }

  void setsearchc(char c) { searchc = c; }
  void setlastsearchdir(bool d) { lastsearchdir = d; }

  size_t getcursorx() const { return cursorx; }
  size_t getcursory() const { return cursory; }
  size_t getviewstart() const { return viewstart; }
  void setviewstart(size_t n) { viewstart = n; }
  size_t getviewend() const { return viewend; }
  void setviewend(size_t n) { viewend = n; }

  const std::string &getrawstr() const { return rawstr; }
  const std::vector<std::string> &getdata() const { return lines; }

  // void updatecounters() const {
  //   size_t twc = 0, tcc = 0;
  //   if (!isspace(*rawstr.begin())) ++twc;
  //   for (auto it = rawstr.begin(); it != rawstr.end(); ++it) {
  //     auto next = it;
  //     if (isspace(*it) && ++next != rawstr.end() && !isspace(*next)) ++twc;
  //     ++tcc;
  //   }
  //   wc = twc; cc = tcc;
  // }
  const size_t &getwc() const { return wc; }
  const size_t &getcc() const { return cc; }
  size_t getlc() const { return lines.size() + 1; }


  // NAVIGATION
  void moveCursorUp();
  void moveCursorDown();
  void moveCursorLeft();
  void moveCursorRight();
  void moveCursorw();
  void moveCursorb();
  void findforward();
  void findbackward();
  void findrepeat();
  void findopposite();
  void moveCursorbegl();
  void moveCursorendl();
  void moveCursorbegf();
  void moveCursorendf();
  void moveAppend();
  
  // INSERTION
  void insert(int c);
  void insert(std::string s);
  void insertnl(std::string s);
  void newlineblw();
  void newlineabv();

  void undo();
  void redo();


  // MISC
  std::string yanky();
  std::string yankw();
  std::string yankiw();
  std::string yankaw();
  std::string yankend();
  std::string yankbeg();
  std::string yanktx(char x);
  std::string yankfx(char x);

  void changeline();
  void changew();
  void changeendw();
  void changeendl();

  void deleteline();
  void deleteword();
  void deleteendword();
  void deleteendline();

  void write();
};
