module;
#include <ncurses.h>
module mde;
import file;
int min(int x, int y) { return (x < y) ? x : y; }
// void EditorPane::moveCursorUp() {
//   if (cursory) {
//     --cursory;
//     cursorx = min(cursormaxx, rows[cursory].size());
//   } else {
//     cursorx = 0;
//   }
//   if (buftop && cursory - buftop < 5) --buftop;
// }
// void EditorPane::moveCursorDown() {
//   if (cursory < rows.size() - 1) {
//     ++cursory;
//     cursorx = min(cursormaxx, rows[cursory].size());
//   } else {
//     cursorx = rows[cursory].size();
//   }
//   if (rows.size() - buftop > getmaxy(w) &&
//       getmaxy(w) - cursory + buftop < 5) ++buftop;
// }
// void EditorPane::moveCursorLeft() {
//   if (cursorx) {
//     cursormaxx = --cursorx;
//   } else if (cursory) {
//     --cursory;
//     cursorx = rows[cursory].size();
//   }
//   if (buftop && cursory - buftop < 5) --buftop;
// }
// void EditorPane::moveCursorRight() {
//   if (cursorx < rows[cursory].size()) {
//     cursormaxx = ++cursorx;
//   } else if (cursory < rows.size() - 1) {
//     ++cursory;
//     cursorx = 0;
//   }
//   if (rows.size() - buftop > getmaxy(w) &&
//       getmaxy(w) - cursory + buftop < 5) ++buftop;  
// }
// void EditorPane::moveCursorw() {
//   if (cursorx == rows[cursory].size()) {
//     moveCursorRight(); return;
//   }
//   while (rows[cursory][cursorx] != ' ' && cursorx < rows[cursory].size()) {
//     moveCursorRight();
//   }
//   moveCursorRight();
// }
// void EditorPane::moveCursorb() {
//   while (rows[cursory][cursorx] != ' ' && cursorx) moveCursorLeft();
//   if (!cursorx) moveCursorLeft();
//   while (cursorx && rows[cursory][cursorx - 1] != ' ') moveCursorLeft();
// }
// void EditorPane::moveCursorf() {
//   for (int i = cursorx + 1; i < rows[cursory].size(); ++i)
//     if (rows[cursory][i] == searchc) { cursorx = i; return; }
// }
// void EditorPane::moveCursorF() {
//   for (int i = cursorx - 1; i >= 0; --i)
//     if (rows[cursory][i] == searchc) { cursorx = i; return; }
// }
// void EditorPane::moveCursorbegl() {
//   cursorx = 0;
// }
// void EditorPane::moveCursorendl() {
//   cursorx = rows[cursory].size();
// }
// void EditorPane::newlineblw() {
//   rows.insert(rows.begin() + cursory + 1, "");
//   moveCursorDown();
// }
// void EditorPane::newlineabv() {
//   rows.insert(rows.begin() + cursory, "");
//   cursorx = 0;
// }
// void EditorPane::write() {
//   std::ofstream f{filename};
//   for (const auto &r:rows) f << r << std::endl;
// }
// // void EditorPane::scrolltoview() {
// //   if (rows.size() - buftop > getmaxy(w) && 
// //       getmaxy(w) - cursory + buftop < 5) ++buftop;
// //   else if (buftop && cursory - buftop < 5) --buftop;
// // }

// void EditorPane::print() {
//   const File &f = m.getactivefile();
//   int yoffset = 0;
//   std::string s;
//   for (int i = 0; i < getmaxy(w); ++i) {
//     s = f.getdisplayline(i);
//     wprintw(w, "%s", s.c_str());
//     if (f.getviewstart() + i < f.getcursory() && getmaxx(w)) {
//       yoffset += s.size() / getmaxx(w);
//     }
//     if (getcury(w) < getmaxy(w)) wmove(w, getcury(w) + 1, 0);
//     else break;
//   }
//   wmove(w,
//         f.getcursory() - f.getviewstart() + yoffset +  
//           (getmaxx(w) ? f.getcursorx() / getmaxx(w) : 0),
//         (getmaxx(w)? f.getcursorx() % getmaxx(w)
//          : f.getcursorx()));
// }

void EditorPane::print() {
  const File &f = m.getactivefile();
  int yoffset = 0;
  if (f.getdata().size() >= getmaxy(w)) resizepad(0, getmaxy(w));
  for (const auto &n:f.getdata()) {
    waddstr(w, n.c_str());
    waddch(w, '\n');
  }
  // if (f.getdata().size() < getmaxy(stdscr) - 4) {
  wattron(w, COLOR_PAIR(COLOR_CYAN));
  for (int i = f.getdata().size(); i < getmaxy(w); ++i) {
    wprintw(w, "~\n");
  }
  wattroff(w, COLOR_PAIR(COLOR_CYAN));
  // }
  // printw("%d, %d", f.getcursory() + yoffset +
  //       (getmaxx(w) ? f.getcursorx() / getmaxx(w) : 0),
  //       (getmaxx(w) ? f.getcursorx() % getmaxx(w): f.getcursorx()));
  // if (f.getcursorx() > getmaxx(w)) {
  auto d = f.getdata();
  for (size_t i = 0; i < f.getcursory(); ++i) {
    if (d[i].size() > getmaxx(w)) yoffset += d[i].length() / getmaxx(w);
  }
    wmove(w, f.getcursory() + yoffset +
          (getmaxx(w) ? f.getcursorx() / getmaxx(w) : 0),
          (getmaxx(w) ? f.getcursorx() % getmaxx(w): f.getcursorx()));
  // } else wmove(w, f.getcursory(), f.getcursorx());
}

// void EditorPane::resizerows() {
//   unsigned int maxx = getmaxx(w);
//   std::string s;
//   for (size_t i = 0; i < rows.size(); ++i) {
//     if (rows[i].size() > maxx) {
//       s = rows[i].substr(maxx);
//       rows[i].erase(maxx);
//       rows.insert(rows.begin() + i, s);
//     }
//   }
// }
// EditorPane::EditorPane(const MarkdownEditor &m)
//     : Component{m, getmaxy(stdscr) - 4, getmaxx(stdscr) / 2, 2, 0},
//       pad{subpad(w, m.getactivefile().getlc(), getmaxx(stdscr), 0, 0)} {
//   wmove(pad, 0, 0);
//   scrollok(pad, true);
// }
void EditorPane::resizepad(int xoffset = 0, int yoffset = 0) {
  size_t oldx = getmaxx(w), oldy = getmaxy(w);
  delwin(w);
  w = newpad(oldy + yoffset, oldx + xoffset);
}
EditorPane::EditorPane(const MarkdownEditor &m)
    : Component{m, newpad(getmaxy(stdscr) - 4, getmaxx(stdscr) / 2 + 1)},
      winmaxx{getmaxx(stdscr) / 2}, pminy{0}, pminx{0} {}

void EditorPane::update() {
  const File &f = m.getactivefile();
  // size_t cury = f.getcursory();
  wclear(w);
  if (m.getcmd() == WRESIZEL) resizepad(-1, 0);
  else if (m.getcmd() == WRESIZER) resizepad(1, 0);
  print();

  int curx, cury, maxy = getmaxy(stdscr) - 4;
  getyx(w, cury, curx);

  if (cury - pminy < 5 || pminy > cury) {
    while (pminy && (cury - pminy < 5 || pminy > cury)) --pminy;
  } else if (pminy + maxy - cury <= 5 || pminy + maxy < cury)
    // printw("%d, %d, %d, %d", pminy, maxy, cury, f.getlc());
    while (pminy < f.getlc() - maxy &&
           (pminy + maxy - cury <= 5 || pminy + maxy < cury)) ++pminy;


  // if (lastcury < cury) {
  //   if (cury - lastcury == 1) {
  //     if (pminy + getmaxy(stdscr) - 4 - cury <= 5 &&
  //       f.getlc() > pminy + getmaxy(stdscr) - 3) ++pminy;
  //   }
  //   // else pminy = min(cury - (getmaxy(stdscr) - 4) / 2,
  //   //                  f.getlc() - getmaxy(stdscr) - 5);
  // }
  // else if (lastcury > cury) {
  //   if (lastcury - cury == 1)
  //     if (pminy && cury - pminy < 5) --pminy;
  //   // else pminy = min(cury - (getmaxy(stdscr) - 4) / 2,
  //   //                  f.getlc() - getmaxy(stdscr) - 5);
  // }

  // lastcury = cury;
}

// void EditorPane::update() {
  //   wclear(w);
  //   // wclear(pad);
  //   wborder(w, ' ', 0, ' ', ' ', ' ', '|', ' ', '|');
  //   wmove(pad, 0, 0);
  //   print();
  //   prefresh(pad, 0, 0, 0, 0, getmaxy(w) - 1, getmaxx(w) - 1);
  // }
void EditorPane::display() {
  if (winmaxx && m.getcmd() == WRESIZEL) --winmaxx;
  else if (winmaxx < getmaxx(stdscr) - 2 && m.getcmd() == WRESIZER) ++winmaxx;
  prefresh(w, pminy, pminx, 2, 0, getmaxy(stdscr) - 3, winmaxx);
  wrefresh(w);
} 
