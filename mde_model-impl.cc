module;
#include <ncurses.h>
module mde;
import :mde_view;
import :mde_control;

MarkdownEditor::MarkdownEditor(int argc, char **(&argv), Mode mode)
    : mode{mode},
      filenames(argv + 1, argv + argc),
      // files{},
      activeFileE{0},
      activeFileP{0} {
  // for (const auto &n:filenames) {
  //   files.emplace_back(std::make_unique<std::fstream>(n));
  // }
  addView(std::make_unique<CurseView>(*this));
  addController(std::make_unique<CurseKeyboard>(*this));
  updateViews();
  displayViews();
}
void MarkdownEditor::run() {
  Action a;
  while (1) {
    a = getAction();
    if (a.isCommand) {
      switch (a.command) {
        case NORMAL_MODE:
          out = 0;
        case INVALID:
          mode = NORMAL; cmdstr = ""; curs_set(1); break;
        case INSERT_MODE:
        case APPEND:
          cmdstr = "--INSERT--"; mode = INSERT; curs_set(1); break;
        case NEWLINE:
        case NEWLINEABV:
          cmdstr = "--INSERT--"; curs_set(1); break;
        case COMMAND_MODE:
          curs_set(0); cmdstr = ":"; mode = COMMAND; break;
        case FINDF:
          mode = COMMAND; 
          curs_set(0);
          cmdstr = "f";
          findstr = "";
          break;
        case FINDB:
          mode = COMMAND;
          curs_set(0);
          cmdstr = "F";
          findstr = "";
          break;
        case WRITE:
          cmdstr = "saved!"; mode = NORMAL; curs_set(1);
          break;
        case WRITE_QUIT:
          return;
        case QUIT:
          return;
        case QUITX:
          return;
        case WRESIZE:
          break;
      }
      cmd = a.command;
    } else {
      if (cmd == FINDF || cmd == FINDB) {
        findstr = a.keystroke;
        mode = NORMAL;
        cmdstr = "";
        curs_set(1);
      }
      else if (a.keystroke == KEY_BACKSPACE) {
        if (mode == COMMAND && !cmdstr.empty()) {
          cmdstr.pop_back();
          if (cmdstr.empty()) {
            mode = NORMAL;
            curs_set(1);
          }
        }
        else out = KEY_BACKSPACE;
      } else {
        if (mode == COMMAND) cmdstr += a.keystroke;
        else out = a.keystroke;
      }
    }
    updateViews();
    displayViews();
    // cmd = INVALID;
    if (cmd == NEWLINE || cmd == NEWLINEABV) mode = INSERT;
  }
}
