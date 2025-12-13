module;
#include <ncurses.h>
module mde;

Footer::Footer(const MarkdownEditor &m)
    : Component{m, 2, getmaxx(stdscr), getmaxy(stdscr) - 2, 0} {
  wborder(w, ' ', ' ', ACS_HLINE, ' ', ACS_HLINE, ACS_HLINE, ' ', ' ');
}
void Footer::update() {
  wclear(w);
  if (m.getcmd() == WRESIZE) {
    mvwin(w, getmaxy(stdscr) - 2, 0);
    wresize(w, 2, getmaxx(stdscr));
  }
  wborder(w, ' ', ' ', ACS_HLINE, ' ', ACS_HLINE, ACS_HLINE, ' ', ' ');
  std::string s = m.getcmdstr();
  mvwprintw(w, 1, 0, "%s", s.c_str());
  const File &f = m.getactivefile();
  auto x = std::snprintf(nullptr, 0, 
    "line %ld, col %ld  |  c: %ld  w: %ld  l: %ld", 
    f.getcursory(), f.getcursorx(), f.getcc(), f.getwc(), f.getlc());
  mvwprintw(w, 1, getmaxx(w) - x, 
    "line %ld, col %ld  |  c: %ld  w: %ld  l: %ld", 
    f.getcursory(), f.getcursorx(), f.getcc(), f.getwc(), f.getlc());
}
void Footer::display() {
  wrefresh(w);
}
