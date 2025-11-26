module;
#include <ncurses.h>
module mde;

PreviewPane::PreviewPane(const MarkdownEditor &m)
    : Component{m,
                getmaxy(stdscr) - 4, 
                getmaxx(stdscr) / 2,
                2,
                getmaxx(stdscr) / 2 + 1} {
  wborder(w, 0, 0, 0, 0, 0, 0, 0, 0);
}
void PreviewPane::update() {
  if (m.getcmd() == WRESIZE) {
    mvwin(w, 2, getmaxx(stdscr) / 2 + 1);
    wresize(w, getmaxy(stdscr) - 4, getmaxx(stdscr) / 2);
  }
}
void PreviewPane::display() {
  mvwprintw(w, 0, 0, "preview");
  wrefresh(w);
}
