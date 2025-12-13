module;
#include <ncurses.h>
#include <cstring>
#define AUTOSAVE_PT 20
module mde;
import :mde_view;
import :mde_control;

class InvalidFile{};
MarkdownEditor::MarkdownEditor(int argc, char **(&argv), Mode mode)
    : mode{mode},
      // filenames(argv + 1, argv + argc),
      files{},
      activeFileE{0},
      activeFileP{0},
      regexmap {
        {R"(((^|\n))#{1,6}\s{1}([^\n]+))",
          R"($1<h>$3</h>)"}, // # ...
        {R"(\*\*\*\s?([^\n]+)\*\*\*)",
          R"(<bi>$1</bi>)"}, // ***...***
        {R"(\*\*\s?([^\n]+)\*\*)", R"(<b>$1</b>)"}, // **...**
        {R"(\*\_\s?([^\n]+)\_\*)",
          R"(<bi>$1</bi>)"}, // *_..._*
        {R"(\_\*\s?([^\n]+)\*\_)",
          R"(<bi>$1</bi>)"}, // _*...*_
        {R"(\*\s?([^\n]+)\*)",
          R"(<i>$1</i>)"}, // *...*
        {R"((^|\n)> (.+))",
          R"($1<q>$2</q>)"}, // > ...
        {R"((^|\n)\`\`\`([^\n]*)([^`]*\n)\`\`\`)",
          "INTERPRET"}, // ```...```
        {R"(\`([^\n]+)\`)", R"(<c>$1</c>)"}, // `...`
        {R"((^|\n)(\s{4})?(\d+\..+))",
          R"($1<ol>$2$3</ol>)"}, // ol
        {R"((^|\n)----*)",
          R"($1<hl>)"}, // horizontal line
        {R"((^|)(\s{4})?(-|\*)(.+))",
          R"($1<ul>$2•$4</ul>)"}, // ul
        {R"(\\(.))", R"($1)"}, // escaped characters
      } {
  // for (const auto &n:filenames) {
  //   files.emplace_back(std::make_unique<std::fstream>(n));
  // }
  if (argc == 1) throw InvalidFile{};
  int j = 0;
  for (int i = 1; i < argc; ++i) {
    while (argv[i][j]) ++j;
    if (j < 3 || !(argv[i][j - 3] == '.' && argv[i][j - 2] == 'm' &&
      argv[i][j - 1] == 'd')) throw InvalidFile{};
    files.emplace_back(argv[i]);
  }
  addView(std::make_unique<CurseView>(*this));
  addController(std::make_unique<CurseKeyboard>(*this));
  updateViews();
  displayViews();
}

std::string interp(const std::smatch &m) {

}

std::string MarkdownEditor::interpretActiveFile() const {
  std::string s, r;
  size_t tag = 0;
  for (const auto &n:getactivefile().getdata()) {
    s += n; s += '\n';
  }

  for (const auto &n:regexmap) {
    std::regex p{n.first};

    if (std::string{n.second} == std::string{"INTERPRET"}) {

      auto lastpos = s.cbegin();
      auto it = std::sregex_iterator(s.begin(), s.end(), p);
      auto end = std::sregex_iterator();
      for (; it != end; ++it) {
        auto &m = *it;
        r.append(lastpos, m.prefix().second);
        r.append(m[1]);
        r.append("<bc>");
          try {
            r.append(l[m[2].str()].interpret(m[3].str()));
          } catch (...) {
            r.append(m[3].str());
          }
        r.append("</bc>");
        lastpos = m.suffix().first;
      }
      r.append(lastpos, s.cend());
      s = r;
    } else s = std::regex_replace(s, p, n.second);
  }
  return s;
}
void MarkdownEditor::setmode(Mode m) {
  mode = m;
  switch (m) {
    case INSERT:
      cmdstr = "--INSERT--";
      break;
    case NORMAL:
      cmdstr = "";
      if (cmd == APPEND && activefile().getcursorx()) {
        activefile().moveCursorLeft();
        cmd = INVALID;
      }
      ++autosaveCounter;
      break;
    default:
      break;
  }
}

void MarkdownEditor::run() {
  Action a;
  int n;
  std::string c;
  while (1) {
    a = getAction();
    if (a.isCommand) {
      switch (a.command) {
        // MODE SETTING -----------------------------------------------------
        case INVALID:
        case NORMAL_MODE:
          setmode(NORMAL);
          break;
        case INSERT_MODE:
          setmode(INSERT);
          break;
        case APPEND:
          setmode(INSERT);
          activefile().moveAppend();
          break;
        case COMMAND_MODE:
          setmode(COMMAND);
          cmdstr = ":";
          break;

        // NAVIGATION -------------------------------------------------------
        case CURSOR_LEFT:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorLeft();
          break;
        case CURSOR_RIGHT:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorRight();
          break;
        case CURSOR_UP:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorUp();
          break;
        case CURSOR_DOWN:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorDown();
          break;
        case CURSOR_BEG_LINE:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorbegl();
          break;
        case CURSOR_END_LINE:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorendl();
          break;
        case CURSOR_BEG_NEXT_W:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorw();
          break;
        case CURSOR_BEG_PREV_W:
          for (int i = 0; i < a.multiplier; ++i) activefile().moveCursorb();
          break;

        case FINDF:
          setmode(COMMAND);
          cmdstr = "f";
          // searchstr = "";
          break;
        case FINDB:
          setmode(COMMAND);
          cmdstr = "F";
          // searchstr = "";
          break;
        case FINDR:
          for (int i = 0; i < a.multiplier; ++i) activefile().findrepeat();
          break;
        case FINDRO:
          for (int i = 0; i < a.multiplier; ++i) activefile().findopposite();
          break;
        case CURSOR_BEG_FILE:
          activefile().moveCursorbegf();
          setmode(NORMAL);
          break;
        case CURSOR_END_FILE:
          activefile().moveCursorendf();
          setmode(NORMAL);
          break;

        // INSERTION --------------------------------------------------------
        case NEWLINE:
          setmode(INSERT);
          activefile().newlineblw();
          break;
        case NEWLINEABV:
          setmode(INSERT);
          activefile().newlineabv();
          break;

        // SAVE/QUIT --------------------------------------------------------
        case WRITE:
          activefile().write();
          setmode(NORMAL);
          cmdstr = "saved!";
          break;
        case WRITE_QUIT:
          for (auto &n:files) n.write();
          return;
        case QUIT:
          return;
        case QUITX:
          return;

        // MISC -------------------------------------------------------------
        
        case UNDO:
          for (int i = 0; i < a.multiplier; ++i) activefile().undo();
          break;
        case REDO:
          for (int i = 0; i < a.multiplier; ++i) activefile().redo();
          break;


        case YANKY:
          setmode(NORMAL);
          yankstr = "\n" + activefile().yanky();
          break;
        case YANKW:
          setmode(NORMAL);
          yankstr = activefile().yankw();
          break;
        case YANKIW:
          setmode(NORMAL);
          yankstr = activefile().yankiw();
          break;
        case YANKAW:
          setmode(NORMAL);
          yankstr = activefile().yankaw();
          break;
        case YANKD:
          setmode(NORMAL);
          yankstr = activefile().yankend();
          break;
        case YANKC:
          setmode(NORMAL);
          yankstr = activefile().yankbeg();
          break;
        case YANKTX:
          cmdstr = "yt";
          updateViews();
          displayViews();
          n = getch();
          if (std::iscntrl(n)) {}
          else {
            activefile().setsearchc(n);
            yankstr = activefile().yanktx(n);
          }
          setmode(NORMAL);
          break;
        case YANKFX:
          cmdstr = "yf";
          updateViews();
          displayViews();
          n = getch();
          if (std::iscntrl(n)) {}
          else {
            activefile().setsearchc(n);
            yankstr = activefile().yankfx(n);
          }
          setmode(NORMAL);
          break;

        case PUT:
          if (yankstr[0] == '\n')
            for (int i = 0; i < a.multiplier; ++i)
              activefile().insertnl(yankstr.substr(1));
          else for (int i = 0; i < a.multiplier; ++i) 
            activefile().insert(yankstr);
          break;

        case CHANGELINE:
          for (int i = 0; i < a.multiplier; ++i) activefile().changeline();
          setmode(INSERT);
          break;
        case CHANGEWORD:
          for (int i = 0; i < a.multiplier; ++i) activefile().changew();
          setmode(INSERT);
          break;
        case CHANGE_ENDWORD:
          for (int i = 0; i < a.multiplier; ++i) activefile().changeendw();
          setmode(INSERT);
          break;
        case CHANGE_ENDLINE:
          for (int i = 0; i < a.multiplier; ++i) activefile().changeendl();
          setmode(INSERT);
          break;
        case DELETELINE:
          for (int i = 0; i < a.multiplier; ++i) activefile().deleteline();
          setmode(NORMAL);
          break;
        case DELETEWORD:
          for (int i = 0; i < a.multiplier; ++i) activefile().deleteword();
          setmode(NORMAL);
          break;
        case DELETE_ENDWORD:
          for (int i = 0; i < a.multiplier; ++i) activefile().deleteendword();
          setmode(NORMAL);
          break;
        case DELETE_ENDLINE:
          for (int i = 0; i < a.multiplier; ++i) activefile().deleteendline();
          setmode(NORMAL);
          break;

        case OPENFILE:
          setmode(COMMAND);
          break;
        case OPENFILEX:
          c = a.data;
          setmode(NORMAL);
          n = 0;
          // printw("%s, %s", c, filenames[n]);
          while (n < files.size()) {
            if (strcmp(c.c_str(), files[n].getname()) == 0) {
              activeFileE = n; break;
            }
            ++n;
          }
          if (n == files.size()) {
            files.push_back(File{c.c_str()});
            activeFileE = files.size() - 1;
          }
          break;
        case CLOSEFILE:
          files.erase(files.begin() + activeFileE);
          if (!files.size()) return;
          if (activeFileE) --activeFileE;
        case CYCLEFPREV:
          if (activeFileE) --activeFileE;
          else activeFileE = files.size() - 1;
          break;
        case CYCLEFNEXT:
          if (activeFileE == files.size() - 1) activeFileE = 0;
          else ++activeFileE;
          break;
        default:
          break;
      }
      cmd = a.command;
      if (a.multiplier != 1 && cmdstr.length() && cmdstr[0] != ':') cmdstr = "";
    } else {
      int n = a.keystroke;

      switch (mode) {
        case NORMAL:
          // if ((n >= '1' && n <= '9') || n == 'c' || n == 'd' || n == 'y') {
            // setmode(COMMAND);
            cmdstr += n;
          // }
          break;
        case COMMAND:
          if (n == KEY_BACKSPACE) {
            cmdstr.pop_back();
            if (cmdstr.empty()) setmode(NORMAL);
          } else {
            switch (cmd) {
              case FINDF:
                activefile().setsearchc(n);
                activefile().setlastsearchdir(1);
                activefile().findforward();
                setmode(NORMAL);
                break;
              case FINDB:
                activefile().setsearchc(n);
                activefile().setlastsearchdir(0);
                activefile().findbackward();
                setmode(NORMAL);
                break;
              // case FINDR:
              //   activefile().findrepeat();
              //   break;
              // case FINDRO:
              //   activefile().findopposite();
              //   break;
              default:
                cmdstr += n;
            }
          }
          break;
        case INSERT:
          activefile().insert(n);
          break;
        default:
          break;
      }

      // if (mode == NORMAL) {
      //   if ((n >= '1' && n <= '9') || n == 'c' || n == 'd' || n == 'y') {
      //     setmode(COMMAND);
      //     cmdstr += n;
      //   }
      // } else if (mode == COMMAND) {

      // } else if ((mode == INSERT)) {

      // }


      // if (mode == NORMAL &&
      //     (n >= '1' && n <= '9') || n == 'c' || n == 'd' || n == 'y') {
      //   setmode(COMMAND);
      //   cmdstr += n;
      // } else if (cmd == FINDF || cmd == FINDB) {
      //   setmode(NORMAL);
      //   searchstr = n;
      // } else if (n == KEY_BACKSPACE) {
      //   if (mode == COMMAND && !cmdstr.empty()) {
      //     cmdstr.pop_back();
      //     if (cmdstr.empty()) {
      //       setmode(NORMAL);
      //     }
      //   }
      //   else out = KEY_BACKSPACE;
      // } else {
      //   if (mode == COMMAND) cmdstr += n;
      //   else out = n;
      // }
    }

    if (autosaveCounter == AUTOSAVE_PT) {
      for (auto &n:files) n.write();
      cmdstr = "autosave successful!";
      autosaveCounter = 0;
    }
    updateViews();
    displayViews();
    // cmd = INVALID;
    // if (cmd == NEWLINE || cmd == NEWLINEABV) mode = INSERT;
  }
}
