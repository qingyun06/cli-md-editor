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
        case INVALID:
          mode = NORMAL; cmdstr = ""; curs_set(1); break;
        case NORMAL_MODE:
          mode = NORMAL; cmdstr = ""; out = 0; curs_set(1); break;
        case INSERT_MODE:
          cmdstr = "--INSERT--"; mode = INSERT; break;
        case COMMAND_MODE:
          curs_set(0);
          cmdstr = ":";
          mode = COMMAND;
          break;
        case CURSOR_LEFT:
          break;
        case CURSOR_DOWN:
          break;
        case CURSOR_UP:
          break;
        case CURSOR_RIGHT:
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
      if (a.keystroke == KEY_BACKSPACE) {
        if (mode == COMMAND && !cmdstr.empty()) cmdstr.pop_back();
        else out = KEY_BACKSPACE;
      } else {
        if (mode == COMMAND) cmdstr += a.keystroke;
        else out = a.keystroke;
      }
    }
    updateViews();
    displayViews();
    cmd = INVALID;
  }
}
