module;
#include <ncurses.h>
module mde;

int min(int x, int y) { return (x < y) ? x : y; }
void EditorPane::moveCursorUp() {
  if (cursory) {
    --cursory;
    cursorx = min(cursormaxx, rows[cursory].size());
  } else {
    cursorx = 0;
  }
}
void EditorPane::moveCursorDown() {
  if (cursory < rows.size() - 1) {
    ++cursory;
    cursorx = min(cursormaxx, rows[cursory].size());
  } else {
    cursorx = rows[cursory].size();
  }
}
void EditorPane::moveCursorLeft() {
  if (cursorx) {
    cursormaxx = --cursorx;
  } else if (cursory) {
    --cursory;
    cursorx = rows[cursory].size();
  }
}
void EditorPane::moveCursorRight() {
  if (cursorx < rows[cursory].size()) {
    cursormaxx = ++cursorx;
  } else if (cursory < rows.size() - 1) {
    ++cursory;
    cursorx = 0;
  }
}
void EditorPane::moveCursorw() {
  if (cursorx == rows[cursory].size()) {
    moveCursorRight(); return;
  }
  while (rows[cursory][cursorx] != ' ' && cursorx < rows[cursory].size()) {
    moveCursorRight();
  }
  moveCursorRight();
}
void EditorPane::moveCursorb() {
  while (rows[cursory][cursorx] != ' ' && cursorx) moveCursorLeft();
  if (!cursorx) moveCursorLeft();
  while (cursorx && rows[cursory][cursorx - 1] != ' ') moveCursorLeft();
}
void EditorPane::moveCursorf() {
  for (int i = cursorx + 1; i < rows[cursory].size(); ++i)
    if (rows[cursory][i] == m.getfindstr()[0]) { cursorx = i; return; }
}
void EditorPane::moveCursorF() {
  for (int i = cursorx - 1; i >= 0; --i)
    if (rows[cursory][i] == m.getfindstr()[0]) { cursorx = i; return; }
}
void EditorPane::moveCursorbegl() {
  cursorx = 0;
}
void EditorPane::moveCursorendl() {
  cursorx = rows[cursory].size();
}
void EditorPane::newlineblw() {
  rows.insert(rows.begin() + cursory + 1, "");
  moveCursorDown();
}
void EditorPane::newlineabv() {
  rows.insert(rows.begin() + cursory, "");
  cursorx = 0;
}
EditorPane::EditorPane(const MarkdownEditor &m)
    : Component{m, getmaxy(stdscr) - 4, getmaxx(stdscr) / 2, 2, 0}, rows{},
      filename{m.getActiveFileE()} {
  std::fstream f{filename};
  std::string s;
  while (std::getline(f, s)) {
    // s.push_back('\n');
    rows.push_back(s);
  }
}
void EditorPane::write() {
  std::ofstream f{filename};
  for (const auto &r:rows) f << r << std::endl;
}
void EditorPane::update() {
  int o = m.getOut();
  if (m.getMode() == INSERT && o) {
    std::string s;
    switch (o) {
      case KEY_UP:
        moveCursorUp();
        break;
      case KEY_DOWN:
        moveCursorDown();
        break;
      case KEY_LEFT:
        moveCursorLeft();
        break;
      case KEY_RIGHT:
        moveCursorRight();
        break;
      case KEY_BACKSPACE:
        if (!cursorx) {
          if (cursory) {
            s = rows[cursory];
            moveCursorLeft();
            rows.erase(rows.begin() + cursory + 1);
            rows[cursory] += s;
          }
        } else {
          moveCursorLeft();
          rows[cursory].erase(cursorx, 1);
        }
        break;
      case '\n':
      case '\r':
      case KEY_ENTER:
        s = rows[cursory];
        rows[cursory] = s.substr(0, cursorx);
        rows.insert(rows.begin() + cursory + 1, s.substr(cursorx));
        moveCursorDown();
        cursorx = 0;
        break;
      default:
        rows[cursory].insert(cursorx, 1, o);
        moveCursorRight();
    }
  } else {
    switch (m.getcmd()) {
      case NORMAL_MODE:
        if (lastcmd == APPEND && cursorx) {
          moveCursorLeft();
          lastcmd == INVALID;
        }
        break;
      case APPEND:
        if (!rows[cursory].empty()) moveCursorRight();
        break;
      case CURSOR_BEG_LINE:
        moveCursorbegl();
        break;
      case CURSOR_END_LINE:
        moveCursorendl();
        break;
      case CURSOR_BEG_NEXT_W:
        moveCursorw();
        break;
      case CURSOR_BEG_PREV_W:
        moveCursorb();
        break;
      case WRESIZE:
        mvwin(w, 2, 0);
        wresize(w, getmaxy(stdscr) - 4, getmaxx(stdscr) / 2);
        break;
      case CURSOR_UP:
        moveCursorUp();
        break;
      case CURSOR_DOWN:
        moveCursorDown();
        break;
      case CURSOR_LEFT:
        moveCursorLeft();
        break;
      case CURSOR_RIGHT:
        moveCursorRight();
        break;
      case FINDF:
        moveCursorf();
        break;
      case FINDB:
        moveCursorF();
      case FINDR:
        if (lastcmd == FINDF) moveCursorf();
        else if (lastcmd == FINDB) moveCursorF();
        break;
      case FINDRO:
        if (lastcmd == FINDF) moveCursorF();
        else if (lastcmd == FINDB) moveCursorf();
        break;
      case NEWLINE:
        newlineblw();
        break;
      case NEWLINEABV:
        newlineabv();
        break;
      default:
        break;
    }
    if (m.getcmd() == FINDR || m.getcmd() == FINDRO);
    else lastcmd = m.getcmd();
  }
  if (rows.size() - buftop > getmaxy(w) && 
      getmaxy(w) - cursory + buftop < 5) ++buftop;
  else if (buftop && cursory - buftop < 5) --buftop;
  wclear(w);
  wmove(w, 0, 0);
  for (int i = 0; i < getmaxy(w); ++i) {
    wprintw(w, rows[buftop + i].c_str());
    if (getcury(w) < getmaxy(w)) wmove(w, getcury(w) + 1, 0);
  }
  wmove(w, cursory - buftop, cursorx);
  if (m.write()) write();
}
void EditorPane::display() {
  wrefresh(w);
}
