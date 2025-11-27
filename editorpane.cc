export module mde:editorpane;
import <vector>;
import <string>;
import :component;
import action;
export class MarkdownEditor;
export class EditorPane : public Component {
 public:
  EditorPane(const MarkdownEditor &m);
  void update() override;
  void display() override;

 private:
  void moveCursorUp();
  void moveCursorDown();
  void moveCursorLeft();
  void moveCursorRight();
  void moveCursorw();
  void moveCursorb();
  void moveCursorf();
  void moveCursorF();
  void moveCursorbegl();
  void moveCursorendl();
  void newlineblw();
  void newlineabv();
  void write();
  int lastin = 0;
  Command lastcmd = INVALID;
  int buftop = 0, cursorx = 0, cursory = 0, cursormaxx = 0;
  std::vector<std::string> rows;
  std::string filename;
};
