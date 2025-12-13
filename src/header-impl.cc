module;
#include <ncurses.h>
module mde;

Header::Header(const MarkdownEditor &m)
    : Component{m, 2, getmaxx(stdscr), 0, 0} {
  wborder(w, ' ', ' ', ' ', ACS_HLINE, ' ', ' ', ACS_HLINE , ACS_HLINE);

}
void Header::update() {
  if (m.getcmd() == WRESIZE) {
    wresize(w, 2, getmaxx(stdscr));
  }
  wclear(w);
  wborder(w, ' ', ' ', ' ', ACS_HLINE, ' ', ' ', ACS_HLINE , ACS_HLINE);
  wmove(w, 0, 0);
  for (const auto &n:m.getfiles()) {
    auto name = n.getname();
    if (name == m.getactivefile().getname()) {
      wattron(w, COLOR_PAIR(COLOR_MAGENTA));
      wprintw(w, "%s  ", name);
      wattroff(w, COLOR_PAIR(COLOR_MAGENTA));
    } else wprintw(w, "%s  ", name);
  }
  // wadd_wch(w, x);
  // mvwprintw(w, 0, 0, "header");
}
void Header::display() {
  wrefresh(w);
}
