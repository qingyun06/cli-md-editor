module;
#define ESC 27
#include <ncurses.h>
module mde;
import <string>;

CurseKeyboard::CurseKeyboard(const MarkdownEditor &m)
    : m{m},
      mapping{{
        {ESC, NORMAL_MODE},
        {'\n', ENTER},
        {'\r', ENTER},
        {'^', CARET},
        {'h', CURSOR_LEFT},
        {KEY_LEFT, CURSOR_LEFT},
        {'j', CURSOR_DOWN},
        {KEY_DOWN, CURSOR_DOWN},
        {'k', CURSOR_UP},
        {KEY_UP, CURSOR_UP},
        {'l', CURSOR_RIGHT},
        {KEY_RIGHT, CURSOR_RIGHT},
        {'w', CURSOR_BEG_NEXT_W},
        {'b', CURSOR_BEG_PREV_W},
        {'0', CURSOR_BEG_LINE},
        {'$', CURSOR_END_LINE},
        {'r', CARET},
        {'i', INSERT_MODE},
        {'a', APPEND},
        {':', COMMAND_MODE},
        {'o', NEWLINE}, 

        {KEY_RESIZE, WRESIZE}
      }} {}
Action CurseKeyboard::action() {
  int n;
  Action a;
  while ((n = getch()) == ERR) continue;
  if (m.getMode() == NORMAL || n == ESC) {
    a.isCommand = 1;
    if (mapping.find(n) != mapping.end()) a.command = mapping[n];
    else a.command = INVALID;
  } else {
    if (m.getMode() == COMMAND && mapping[n] == ENTER) {
      a.isCommand = 1;
      const std::string &s = m.getcmdstr();
      if (s == ":w") a.command = WRITE;
      else if (s == ":wq") a.command = WRITE_QUIT;
      else if (s == ":q") a.command = QUIT;
      else if (s == ":q!") a.command = QUITX;
      else a.command = INVALID;
    } else {
      a.isCommand = 0;
      a.keystroke = n;
    }
  }
  return a;
}
