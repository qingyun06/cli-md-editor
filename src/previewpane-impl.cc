module;
#include <ncurses.h>
module mde;
import <regex>;

// PreviewPane::PreviewPane(const MarkdownEditor &m)
//     : Component{m,
//                 getmaxy(stdscr) - 4, 
//                 getmaxx(stdscr) / 2,
//                 2,
//                 getmaxx(stdscr) / 2 + 1} {
//   scrollok(w, true);
// }

PreviewPane::PreviewPane(const MarkdownEditor &m, const EditorPane &e)
    : Component{m, newpad(getmaxy(stdscr) - 4, getmaxx(stdscr) / 2 - 2)},
      miny{0}, e{e}, winminx{getmaxx(stdscr) / 2 + 1} {}

// enum Format {PLAIN = 0, BOLD, ITALIC, BOLD_ITALIC, LIST};
// Format getFormat(Format f, const std::string &s) {
//   if (s.size() > 2) {
//     std::string t;
//     if (t == "<b>") {
//       if (f == ITALIC) return BOLD_ITALIC;
//       else return BOLD;
//     }
//     else if (t == "<i>") {
//       if (f == BOLD) return BOLD_ITALIC;
//       else return ITALIC;
//     }
//     else return PLAIN;
//   } else return PLAIN;
// }

// std::string peekx(int n, std::string::iterator beg,
//                   const std::string::iterator &end) {
//   std::string r;
//   for (int i = 0; i < n && beg != end; ++i, ++beg) {
//     r += *beg;
//   }
//   return r;
// }
std::string gettag(const std::string &s, std::string::iterator i) {
  if (i == s.end()) return "";
  else if (*i == '<') {
    std::string r;
    int taglencutoff = 20;
    for (; i != s.end(); ++i) {
      if (!(taglencutoff--)) return "";
      r += *i;
      if (*i == '>') return r;
    }
    return "";
  } else return "";
} 
void PreviewPane::drawhorizontal(char c = '-') {
  for (int i = 0; i < getmaxx(w) - 4; ++i) wprintw(w, "%c", c);
}
void PreviewPane::resizepad(int xoffset = 0, int yoffset = 0) {
  size_t oldx = getmaxx(w), oldy = getmaxy(w);
  delwin(w);
  w = newpad(oldy + yoffset, oldx + xoffset);
}
void PreviewPane::update() {
  // if (m.getcmd() == WRESIZE) {
  //   mvwin(w, 2, getmaxx(stdscr) / 2 + 1);
  //   wresize(w, getmaxy(stdscr) - 4, getmaxx(stdscr) / 2);
  // }
  if (m.getcmd() == WRESIZEL) resizepad(1, 0);
  else if (m.getcmd() == WRESIZER) resizepad(-1, 0);
  wclear(w);
  wmove(w, 1, 2);
  std::string s = m.interpretActiveFile();
  std::string q;
  size_t y = e.getpminy();
  size_t lcount = 0;
  for (auto it = s.begin(); it != s.end(); ++it) {
    if (!getcurx(w)) wmove(w, getcury(w), getcurx(w) + 2);
    if (y == lcount) miny = lcount;
    q = gettag(s, it);
    if (q == "") {}
    else if (q == "<b>") {
      wattron(w, A_BOLD);
    }
    else if (q == "</b>") {
      wattroff(w, A_BOLD);
    }
    else if (q == "<i>") {
      wattron(w, A_ITALIC);
    }
    else if (q == "</i>") {
      wattroff(w, A_ITALIC);
    }
    else if (q == "<h>") {
      wattron(w, A_BOLD);
      wattron(w, COLOR_PAIR(COLOR_BLUE));
      wprintw(w, "\n --- ");
      ++lcount;
      ++y;
    }
    else if (q == "</h>") {
      wprintw(w, " ---\n");
      wattroff(w, COLOR_PAIR(COLOR_BLUE));
      wattroff(w, A_BOLD);
      ++lcount;
      ++y;
    }
    else if (q == "<bi>") {
      wattron(w, A_BOLD | A_ITALIC);
    }
    else if (q == "</bi>") {
      wattroff(w, A_BOLD | A_ITALIC);
    }
    else if (q == "<q>") {
      wprintw(w, " | ");
    }
    else if (q == "</q>") {}
    else if (q == "<c>") {
      wattron(w, COLOR_PAIR(COLOR_MAGENTA));
    }
    else if (q == "</c>") {
      wattroff(w, COLOR_PAIR(COLOR_MAGENTA));
    }
    else if (std::regex_match(q, std::regex{R"(<bc lang=\"([^\n]*)\">)"})) {
      drawhorizontal();
    }
    else if (q == "<bc>" || q == "</bc>") {
      drawhorizontal();
    }
    else if (q == "<hl>") {
      drawhorizontal('=');
    }
    else if (q == "<ol>" || q == "<ul>") {
      wprintw(w, "  ");
    }
    else if (q == "</ol>" || q == "</ul>") {}
    else if (q == "<BLACK>") wattron(w, COLOR_PAIR(COLOR_BLACK));
    else if (q == "</BLACK>") wattroff(w, COLOR_PAIR(COLOR_BLACK));
    else if (q == "<RED>") wattron(w, COLOR_PAIR(COLOR_RED));
    else if (q == "</RED>") wattroff(w, COLOR_PAIR(COLOR_RED));
    else if (q == "<GREEN>") wattron(w, COLOR_PAIR(COLOR_GREEN));
    else if (q == "</GREEN>") wattroff(w, COLOR_PAIR(COLOR_GREEN));
    else if (q == "<YELLOW>") wattron(w, COLOR_PAIR(COLOR_YELLOW));
    else if (q == "</YELLOW>") wattroff(w, COLOR_PAIR(COLOR_YELLOW));
    else if (q == "<BLUE>") wattron(w, COLOR_PAIR(COLOR_BLUE));
    else if (q == "</BLUE>") wattroff(w, COLOR_PAIR(COLOR_BLUE));
    else if (q == "<MAGENTA>") wattron(w, COLOR_PAIR(COLOR_MAGENTA));
    else if (q == "</MAGENTA>") wattroff(w, COLOR_PAIR(COLOR_MAGENTA));
    else if (q == "<CYAN>") wattron(w, COLOR_PAIR(COLOR_CYAN));
    else if (q == "</CYAN>") wattroff(w, COLOR_PAIR(COLOR_CYAN));
    else if (q == "<WHITE>") wattron(w, COLOR_PAIR(COLOR_WHITE));
    else if (q == "</WHITE>") wattroff(w, COLOR_PAIR(COLOR_WHITE));

    else if (q == "");
    else {
      wprintw(w, "%c", *it); continue;
    }

    it += q.length();

    if (lcount >= getmaxy(w) - 1) resizepad(0, getmaxy(w));
    wprintw(w, "%c", *it);
    if (*it == '\n') ++lcount;
  }
  wborder(w, 0, ' ', ' ', ' ', '|', ' ', '|', ' ');

}
void PreviewPane::display() {
  // wclear(w);
  // mvwprintw(w, 1, 1, "params");
  // mvwprintw(w, 2, 1, "cursorx: %d", m.getactivefile().getcursorx());
  // mvwprintw(w, 3, 1, "cursory: %d", m.getactivefile().getcursory());
  // mvwprintw(w, 4, 1, "viewstart: %ld", m.getactivefile().getviewstart());
  // mvwprintw(w, 5, 1, "viewend: %ld", m.getactivefile().getviewend());
  // wclear(w);

  // mvwprintw(w, 0, 0, "preview");
  if (winminx > 1 && m.getcmd() == WRESIZEL) --winminx;
  else if (winminx < getmaxx(stdscr) - 1 && m.getcmd() == WRESIZER) ++winminx;
  prefresh(w, miny + 1, 0, 2, winminx, 
          getmaxy(stdscr) - 3, getmaxx(stdscr) - 1);
  wrefresh(w);
}
