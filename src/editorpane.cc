export module mde:editorpane;
import <vector>;
import <string>;
import :component;
import action;
#include <ncurses.h>
export class MarkdownEditor;
export class EditorPane : public Component {
 public:
  EditorPane(const MarkdownEditor &m);
  void update() override;
  void display() override;
  const size_t &getpminy() const { return pminy; }
 private:
  int winmaxx;
  size_t pminx, pminy, lastcury;
  void print();
  void resizepad(int xoffset, int yoffset);
};
