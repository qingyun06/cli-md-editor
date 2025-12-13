module;
#define ESC 27
#define CTRL_R 18
#define CTRL_T 20
#define CTRL_W 23
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
        {'O', NEWLINEABV},
        {'f', FINDF},
        {'F', FINDB},
        {';', FINDR},
        {',', FINDRO},
        {KEY_RESIZE, WRESIZE},
        {'[', WRESIZEL},
        {']', WRESIZER},
        {'-', CYCLEFPREV},
        {'=', CYCLEFNEXT},
        {'p', PUT},
        {'u', UNDO},
        {CTRL_R, REDO},
        {CTRL_T, OPENFILE},
        {CTRL_W, CLOSEFILE},
      }} {}
bool isPosInt(const std::string & s) {
  if (!s.length()) return false;
  if (s[0] == '0') return false;
  for (const auto &n:s) if (!(n >= '0' && n <= '9')) return false;
  return true;
}
int getmultiplier(const std::string &s) {
  std::string r;
  for (const auto &n:s) if ((n >= '0' && n <= '9')) r += n;
  return r.empty() ? 1 : std::stoi(r);
}
std::string skipmult(const std::string &s) {
  int i = 0;
  while (i < s.length()) {
    if (s[i] >= '0' && s[i] <= '9') ++i;
    else break;
  }
  return s.substr(i);
}
Action CurseKeyboard::action() {
  int n;
  Action a;
  const std::string &s = skipmult(m.getcmdstr());
  while ((n = getch()) == ERR) continue;
  if (n == ESC) {
    a.isCommand = 1;
    a.command = NORMAL_MODE;
    return a;
  }
  switch (m.getMode()) {
    case NORMAL:
      if (n >= '1' && n <= '9' ||
        (m.getcmdstr() == "" && (n == 'c' || n == 'd' || n == 'y')) ||
        (m.getcmdstr() == "y" && (n == 'a' || n == 'i'))) {
        a.isCommand = 0;
        a.keystroke = n;
      } else {
        if (m.getcmd() != OPENFILE && s == "y") {
            a.isCommand = 1;
            switch (n) {
              case 'y': a.command = YANKY; break;
              case 'w': a.command = YANKW; break;
              case '$': a.command = YANKD; break;
              case '^': a.command = YANKC; break;
              case 'i':
              case 'a': a.isCommand = 0; a.keystroke = n; break;
              case 't': a.command = YANKTX; break;
              case 'f': a.command = YANKFX; break;
              default: a.command = INVALID;
            }
        } else if (m.getcmd() != OPENFILE && s == "yi") {
          if (n == 'w') a.command = YANKIW;
          else a.command = INVALID;
          a.isCommand = 1;
        } else if (m.getcmd() != OPENFILE && s == "ya") {
          if (n == 'w') a.command = YANKAW;
          else a.command = INVALID;
          a.isCommand = 1;
        } else if (m.getcmd() != OPENFILE && s == "c") {
          a.isCommand = 1;
          switch (n) {
            case 'c': a.command = CHANGELINE; break;
            case 'w': a.command = CHANGEWORD; break;
            case 'e': a.command = CHANGE_ENDWORD; break;
            case '$': a.command = CHANGE_ENDLINE; break;
            default: a.command = INVALID;
          }
        } else if (m.getcmd() != OPENFILE && s == "d") {
          a.isCommand = 1;
          switch (n) {
            case 'd': a.command = DELETELINE; break;
            case 'w': a.command = DELETEWORD; break;
            case 'e': a.command = DELETE_ENDWORD; break;
            case '$': a.command = DELETE_ENDLINE; break;
            default: a.command = INVALID;
          }
        } else {
          a.isCommand = 1;
          if (mapping.find(n) != mapping.end()) a.command = mapping[n];
          else a.command = INVALID;
          a.multiplier = getmultiplier(m.getcmdstr());
        }
      }
      break;
    case INSERT:
      a.isCommand = 0;
      a.keystroke = n;
      break;
    case COMMAND:
      if (mapping[n] == ENTER) {
        a.isCommand = 1;
        if (s == ":w") a.command = WRITE;
        else if (s == ":wq") a.command = WRITE_QUIT;
        else if (s == ":q") a.command = QUIT;
        else if (s == ":q!") a.command = QUITX;
        else if (s == ":0") a.command = CURSOR_BEG_FILE;
        else if (s == ":$") a.command = CURSOR_END_FILE;
        else if (m.getcmd() == OPENFILE) {
          a.command = OPENFILEX; a.data = s;
        }
        else a.command = INVALID;
      }
      
      // else if (m.getcmd() != OPENFILE && s == "y") {
      //     a.isCommand = 1;
      //     switch (n) {
      //       case 'y': a.command = YANKY; break;
      //       case 'w': a.command = YANKW; break;
      //       case '$': a.command = YANKD; break;
      //       case '^': a.command = YANKC; break;
      //       case 'i':
      //       case 'a': a.isCommand = 0; a.keystroke = n; break;
      //       case 't': a.command = YANKTX; break;
      //       case 'f': a.command = YANKFX; break;
      //       default: a.command = INVALID;
      //     }
      // } else if (m.getcmd() != OPENFILE && s == "yi") {
      //   if (n == 'w') a.command = YANKIW;
      //   else a.command = INVALID;
      //   a.isCommand = 1;
      // } else if (m.getcmd() != OPENFILE && s == "ya") {
      //   if (n == 'w') a.command = YANKAW;
      //   else a.command = INVALID;
      //   a.isCommand = 1;
      // } else if (m.getcmd() != OPENFILE && s == "c") {
      //   a.isCommand = 1;
      //   switch (n) {
      //     case 'c': a.command = CHANGELINE; break;
      //     case 'w': a.command = CHANGEWORD; break;
      //     case 'e': a.command = CHANGE_ENDWORD; break;
      //     case '$': a.command = CHANGE_ENDLINE; break;
      //     default: a.command = INVALID;
      //   }
      // } else if (m.getcmd() != OPENFILE && s == "d") {
      //   a.isCommand = 1;
      //   switch (n) {
      //     case 'd': a.command = DELETELINE; break;
      //     case 'w': a.command = DELETEWORD; break;
      //     case 'e': a.command = DELETE_ENDWORD; break;
      //     case '$': a.command = DELETE_ENDLINE; break;
      //     default: a.command = INVALID;
      //   }
      // } 
      // else if (isPosInt(s)) {
      //   if (n >= '0' && n <= '9') {
      //     a.isCommand = 0;
      //     a.keystroke = n;
      //   } else {
      //     if (mapping.find(n) != mapping.end()) a.command = mapping[n];
      //     else a.command = INVALID;
      //     a.multiplier = std::stoi(s);
      //     a.isCommand = 1;
      //   }
      // } 
      else {
        a.isCommand = 0;
        a.keystroke = n;
      }
      a.multiplier = getmultiplier(m.getcmdstr());
      break;
    default:
      a.isCommand = 1;
      a.command = INVALID;
      break;
  }

  // if (m.getMode() == NORMAL || n == ESC) {
  //   if ((n >= '1' && n <= '9') || n == 'c' || n == 'd' || n == 'y') {
  //     a.isCommand = 0;
  //     a.keystroke = n;
  //   } else {
  //     a.isCommand = 1;
  //     if (mapping.find(n) != mapping.end()) a.command = mapping[n];
  //     else a.command = INVALID;
  //   }
  // } else {
  //   if (m.getMode() == COMMAND && mapping[n] == ENTER) {
  //     a.isCommand = 1;
  //     const std::string &s = m.getcmdstr();
  //     if (s == ":w") a.command = WRITE;
  //     else if (s == ":wq") a.command = WRITE_QUIT;
  //     else if (s == ":q") a.command = QUIT;
  //     else if (s == ":q!") a.command = QUITX;
  //     else if (s == ":0") a.command = CURSOR_BEG_FILE;
  //     else if (s == ":$") a.command = CURSOR_END_FILE;
  //     else a.command = INVALID;
  //   } else {
  //     a.isCommand = 0;
  //     a.keystroke = n;
  //   }
  // }
  return a;
}
