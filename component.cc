module;
#include <ncurses.h>
export module mde:component;
import <string>;
export class MarkdownEditor;
export class Component {
 public:
  virtual void update() = 0;
  virtual void display() = 0;
  Component(const MarkdownEditor &m, int h, int w, int startx, int starty);
  virtual ~Component() = 0;

 protected:
  const MarkdownEditor &m;
  WINDOW *w;
};
