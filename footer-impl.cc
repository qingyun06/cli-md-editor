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
  mvwprintw(w, 1, 0, m.getcmdstr().c_str());
}
void Footer::display() {
  wrefresh(w);
}
