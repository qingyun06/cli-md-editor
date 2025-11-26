module;
#include <ncurses.h>
module mde;
import <iostream>;

Component::Component(const MarkdownEditor &m, int h, int w, int startx,
                     int starty): m{m}, w{newwin(h, w, startx, starty)} {}
Component::~Component() {
  if (w) delwin(w);
}
