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
  auto o = m.getOut();
  if (m.getMode() == INSERT && o) {
    if (o == KEY_LEFT) moveCursorLeft();
    else if (o == KEY_RIGHT) moveCursorRight();
    else if (o == KEY_UP) moveCursorUp();
    else if (o == KEY_DOWN) moveCursorDown();
    else if (m.getOut() == KEY_BACKSPACE) {
      if (!cursorx) {
        if (cursory) {
          std::string s = rows[cursory];
          moveCursorLeft();
          rows.erase(rows.begin() + cursory + 1);
          rows[cursory] += s;
        }
      } else {
        moveCursorLeft();
        rows[cursory].erase(cursorx, 1);
      }
    } else {
      if (o == '\n' || o == '\r') {
        std::string s = rows[cursory];
        rows[cursory] = s.substr(0, cursorx);
        rows.insert(rows.begin() + cursory + 1, s.substr(cursorx));
        moveCursorDown();
        cursorx = 0;
      } else {
        rows[cursory].insert(cursorx, 1, o);
        moveCursorRight();
      }
    }
  } else {
    switch (m.getcmd()) {
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
      default:
        break;
    }
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
